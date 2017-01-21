#ifndef PA_SINK_H
#define PA_SINK_H

#include <pulse/simple.h>
#include <pulse/error.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

class PASink {

    public:

        PASink(const char *device);
        ~PASink();

        int16_t *read_data();

    private:

        inline void get_error(int err, char *msg) {
            snprintf(msg, 512, "pa_simple_new() failed: %s\n", pa_strerror(err));
        }

        pa_sample_spec _sample_spec;
        pa_simple *_stream;
        int16_t *_buffer;
        const char *_device;

        // band cutoff frequencies
        int _hpf, _lpf;
};

#endif
