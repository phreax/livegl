#ifndef SPECTRAL_H
#define SPECTRAL_H

#include <stdint.h>
#include <fftw3.h>
#include "audioconfig.h"

class SpectralAnalyzer {
    
    public:

        SpectralAnalyzer(unsigned int samplesize=SAMPLESIZE, unsigned int rate=SAMPLERATE);
        ~SpectralAnalyzer();

        void analyze_sample(int16_t *sample);

        inline float *spectrum() {return _spect;}
        inline float *spectral_flux() {return _spect_flux;}
        inline float *bands_three() {return _bands_three;}
        inline unsigned int  samplesize() {return _samplesize;}
        inline unsigned int  nfreq() {return _samplesize/2 +1;}

        // set cutoff frequencies
        inline void set_low_freq(unsigned int lcf) {
            if(lcf > _rate || lcf > _hcf) throw "low cutoff frequency: invalid value";
            _lcf = lcf;
        }

        inline void set_high_freq(unsigned int hcf) {
            if(hcf > _rate || hcf < _lcf) throw "high cutoff frequency: invalid value";
            _hcf = hcf;
        }

    private:

        unsigned int _samplesize; // size of one samplepack
        unsigned int _rate;

        unsigned int _lcf, _hcf;

        double *_wave;           // sample vector in time domain
        fftw_complex *_freq;     // sample vector in frequency domain

        float *_spect;          // energy of the current spectrum
        float *_spect_old;      // energy of last spectrum

        float *_spect_flux;     // spectral difference
        float *_bands_three;    // amplitude of low,mid,high bands
        float *_bands_three_old;

        float _avgmin, _avgmax;

        int _nframe;

        fftw_plan _fftw_plan;    
};

#endif
