#include "spectral.h"
#include "pasink.h"
#include <stdio.h>

#define LEN 50

int main(int argc, const char *argv[])
{

    try {

    PASink pa_sink;
    SpectralAnalyzer specta;

    int16_t *sample;
    float *bands;

    for(;;) {
        sample = pa_sink.read_data();
        specta.analyze_sample(sample);
        bands = specta.bands_three();

        int l = bands[0] * LEN;
        int m = bands[1] * LEN;
        int h = bands[2] * LEN;

        printf("\n\n");
        for(int i = 0; i<l; i++) {
            printf("|");
        }

        printf("\n\n");
        for(int i= 0; i<m; i++) {
            printf("|");
        }

        printf("\n\n");
        for(int i= 0; i<h; i++) {
            printf("|");
        }
        system("clear");

    }

    } catch(const char * e) {
        std::cout << "Caught exception: " << e << std::endl;
    }

    return 0;
}
