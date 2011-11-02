#include "pasink.h"
#include <stdint.h> 
#include <iostream> 

using namespace std;

PASink::PASink(const char *device) :
  _device(device)
{

    // init pulse sample specification
    _sample_spec.format = PA_SAMPLE_S16LE;
    _sample_spec.rate = SAMPLERATE;
    _sample_spec.channels = 1;


    // low latency settings
    pa_buffer_attr ba;
    ba.tlength = pa_usec_to_bytes(10*1000,&_sample_spec);
    ba.fragsize = pa_usec_to_bytes(10*1000,&_sample_spec);
    ba.minreq = pa_usec_to_bytes(0,&_sample_spec);
    ba.maxlength = pa_usec_to_bytes(20*1000,&_sample_spec); 

    int error;
    
    cout << "connecting to device " << endl;
    cout << "\tdevice: " << _device << endl;
    cout << "\tformat: " << _sample_spec.format << endl;
    cout << "\trate: " << _sample_spec.rate << endl;
    printf("\tchannels %d\n", _sample_spec.channels);

    // create new record stream
    _stream = pa_simple_new(NULL, "livegl", PA_STREAM_RECORD, _device, "record",&_sample_spec, NULL, &ba, &error);
    if(!_stream) {
      cout << "failed creating pulseaudio stream" << endl;
      char msg[512];
      get_error(error,msg);
      throw msg;
    }
    
    // alloc buffer
    _buffer = new int16_t[SAMPLESIZE];

}
    
PASink::~PASink() {
    if(_buffer) free(_buffer);
    if(_stream) pa_simple_free(_stream);
}

/* read one samplepack from pulse audio stream */
int16_t *PASink::read_data() {

    int error;
    if(pa_simple_read(_stream,_buffer,SAMPLESIZE*sizeof(int16_t),&error)<0) {
      cout << "failed reading pulse audio stream" << endl;
      char msg[512];
      get_error(error,msg);
      throw msg;
    }

    return _buffer;
}
