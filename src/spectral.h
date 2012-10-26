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

        inline float *spectrum() {return spect;}
        inline float *spectral_flux() {return spect_flux;}
        inline float *bands_three() {return bands;}
        inline float *bands_smooth() {return bands_s;}
        inline unsigned int  nfreq() {return samplesize/2 +1;}

        // set cutoff frequencies
        inline void set_lowfreq(unsigned int lcf) {
            if(lcf > rate || lcf > hcf) throw "low cutoff frequency: invalid value";
            lcf = lcf;
        }

        inline void set_highfreq(unsigned int hcf) {
            if(hcf > rate || hcf < lcf) throw "high cutoff frequency: invalid value";
            hcf = hcf;
        }

    private:

        unsigned int samplesize; // size of one samplepack
        unsigned int rate;

        unsigned int lcf, hcf;

        double *wave;           // sample vector in time domain
        fftw_complex *freq;     // sample vector in frequency domain

        float *spect;          // energy of the current spectrum
        float *spect_old;      // energy of last spectrum

        float *spect_flux;     // spectral difference
        float *bands;    // amplitude of low,mid,high bands
        float *bands_s;  // smooth bands
        float *bands_avg;

        int nframe;

        fftw_plan plan;    
};

#endif
