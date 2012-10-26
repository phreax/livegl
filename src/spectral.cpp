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

SpectralAnalyzer::SpectralAnalyzer(unsigned int _samplesize, unsigned int _rate)
    : samplesize(_samplesize), rate(_rate), lcf(LOW_CUT_F), hcf(HIGH_CUT_F),  nframe(0)
{

    // number of frequency bins
    unsigned int nfreq = samplesize/2 +1;
    
    spect      = new float[nfreq];
    spect_old  = new float[nfreq];
    spect_flux = new float[nfreq];
    bands= new float[3]; 
    bands_s= new float[3]; 
    bands_avg= new float[3]; 

    wave = new double[samplesize];
    freq = (fftw_complex *) fftw_malloc((sizeof(fftw_complex)) *samplesize);

    // create fftw plan for 1d dft processing
    plan  = fftw_plan_dft_r2c_1d(samplesize, wave, freq, FFTW_MEASURE);
    
    memset(wave,0,sizeof(double)*samplesize);
    memset(spect,0,sizeof(float)*nfreq);
    memset(spect_old,0,sizeof(float)*nfreq);

}

void SpectralAnalyzer::analyze_sample(int16_t *sample) {
    
    // normalize input to [-1,1]
    for(int i=0; i<samplesize; i++) {
        wave[i] = (double)sample[i]/(double)(SHRT_MAX+1);
    }

    // clear frequencies
    memset(freq,0,sizeof(fftw_complex)*samplesize);

    // clear bands
    memset(bands,0,sizeof(float)*3);

    fftw_execute(plan);

    // number of frequency bins
    unsigned int nfreq = samplesize/2 +1;

    // compute indices for cutoff
    unsigned int ilc, ihc;
    
    ilc = (unsigned int) ((float)nfreq * (float)lcf/(rate*0.5)) -1; 
    ihc = (unsigned int) ((float)nfreq * (float)hcf/(rate*0.5)) -1; 

    ilc = MAX(ilc,1); // should be at least one

    // number of bins for each band
    unsigned int nlow, nmid, nhigh;
    nlow = ilc;
    nmid = ihc-nlow;
    nhigh = nfreq-nmid;

    // compute engergy of each band && spectral flux
    for(int i=0; i<nfreq; i++) {
        spect[i] = (float)MAG(freq[i]) * log(i+2); 
        spect_flux[i] = spect[i] - spect_old[i]; // spectral difference
        spect_old[i] = spect[i];  // save old value
        
        spect_flux[i] = MAX(spect_flux[i],0.0);

        // compute average for each bin
        if(i<ilc && i < ihc) 
            bands[0] += spect_flux[i]/nlow;
        else if(i<ihc && i < nfreq)
            bands[1] += spect_flux[i]/nmid;
        else
            bands[2] += spect_flux[i]/nhigh;

    }

    float old_temp;
    float temp;

    for(int i=0;i<3;i++) {
        // save old values
        temp = MAX(bands[i],0.0);
        old_temp = bands_avg[i];

        bands[i] = MAX(temp - old_temp -AMP_THRES,0.0);

        if(nframe > 1 ) {

            // exclude outliers from average
            if(bands[i]==0.0) bands_avg[i] = temp*0.15 + bands_avg[i]*0.85;

            if(bands[i]>bands_s[i])
                bands_s[i] = bands[i];
            else
                bands_s[i] = bands_s[i]*0.85 +bands[i]*0.85;
        }
        else {
            bands_avg[i] = temp; 
            bands_s[i] = bands[i];
        }
    }

    nframe++;
    
} 

SpectralAnalyzer::~SpectralAnalyzer() {
    delete wave;
    delete spect;
    delete spect_old;
    delete spect_flux;
    delete bands;
    delete bands_s;
    delete bands_avg;
    free(freq);
}

