#include "spectral.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h> 
#include <math.h> 
#include <float.h> 

#define ENERGY(X) (X[0]*X[0]+X[1]*X[1])
#define MAG(X) sqrt(X[0]*X[0]+X[1]*X[1])
#define MAX(a,b) (a<b?b:a)
#define MIN(a,b) (a<b?a:b)

SpectralAnalyzer::SpectralAnalyzer(unsigned int samplesize, unsigned int rate)
    : _samplesize(samplesize), _rate(rate), _lcf(LOW_CUT_F), _hcf(HIGH_CUT_F), _avgmin(0.0), _avgmax(1.0), _nframe(0)
{

    // number of frequency bins
    unsigned int nfreq = _samplesize/2 +1;
    
    _spect      = new float[nfreq];
    _spect_old  = new float[nfreq];
    _spect_flux = new float[nfreq];
    _bands_three= new float[3]; 
    _bands_three_old= new float[3]; 

    _wave = new double[_samplesize];
    _freq = (fftw_complex *) fftw_malloc((sizeof(fftw_complex)) *_samplesize);

    // create fftw plan for 1d dft processing
    _fftw_plan  = fftw_plan_dft_r2c_1d(_samplesize, _wave, _freq, FFTW_MEASURE);
    
    memset(_wave,0,sizeof(double)*_samplesize);
    memset(_spect,0,sizeof(float)*nfreq);
    memset(_spect_old,0,sizeof(float)*nfreq);

}

void SpectralAnalyzer::analyze_sample(int16_t *sample) {
    
    // normalize input to [-1,1]
    for(int i=0; i<_samplesize; i++) {
        _wave[i] = (double)sample[i]/(double)(SHRT_MAX+1);
    }

    // clear frequencies
    memset(_freq,0,sizeof(fftw_complex)*_samplesize);

    // clear bands
    memset(_bands_three,0,sizeof(float)*3);

    fftw_execute(_fftw_plan);

    // number of frequency bins
    unsigned int nfreq = _samplesize/2 +1;

    // compute indices for cutoff
    unsigned int ilc, ihc;
    
    ilc = (unsigned int) ((float)nfreq * (float)_lcf/(_rate*0.5)) -1; 
    ihc = (unsigned int) ((float)nfreq * (float)_hcf/(_rate*0.5)) -1; 

    ilc = MAX(ilc,1); // should be at least one

    // number of bins for each band
    unsigned int nlow, nmid, nhigh;
    nlow = ilc;
    nmid = ihc-nlow;
    nhigh = nfreq-nmid;


/*printf("ilc = %d\n",ilc);
printf("ihc = %d\n",ihc);
printf("nfreq = %d\n",nfreq);
printf("nlow = %d\n",nlow);
printf("nmid = %d\n",nmid);
printf("nhigh = %d\n",nhigh);
*/

    // compute engergy of each band && spectral flux
    for(int i=0; i<nfreq; i++) {
        _spect[i] = (float)MAG(_freq[i]) * log(i+2); 
        _spect_flux[i] = _spect[i] - _spect_old[i]; // spectral difference
        _spect_old[i] = _spect[i];  // save old value
        
        // compute average for each bin
        if(i<ilc && i < ihc) 
            _bands_three[0] += _spect_flux[i]/nlow;
        else if(i<ihc && i < nfreq)
            _bands_three[1] += _spect_flux[i]/nmid;
        else
            _bands_three[2] += _spect_flux[i]/nhigh;

    }

    for(int i=0;i<3;i++) {
        _bands_three[i] = MAX(_bands_three[i],0.0);
        if(_nframe > 1) {
            _bands_three[i] = _bands_three[i]*0.1 + _bands_three_old[i]*0.9;
        }
        _bands_three_old[i] = _bands_three[i];
    }

    _nframe++;
    
} 

SpectralAnalyzer::~SpectralAnalyzer() {
    delete _wave;
    delete _spect;
    delete _spect_old;
    delete _spect_flux;
    delete _bands_three;
    free(_freq);
}

