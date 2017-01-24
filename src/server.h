#ifndef __SERVER_H
#define __SERVER_H

#define MIDI_PORT 1
#define MIDI_CC_CHAN 1
#define MIDI_MAP_CC_START 21
#define MIDI_MAP_CC_END 47 

#define MIDI_TYPE_CC      0xb
#define MIDI_TYPE_NOTE_ON 0x9
#define MAX_MIDI_MASSAGES 20

#include "shader.h" 
#include "spectral.h"
#include "pasink.h"
#include "uniform.h"
#include "midi_input.h"
#include "zmq.hpp"
#include <map>

typedef std::map<int, Uniform *> MidiUniformMap;

class LiveGLServer {

    zmq::context_t             *_ctx;
    zmq::socket_t              *_socket;
    zmq::pollitem_t             _pollitem[1];
    Shader                     *_shader;
    bool                        _blocking;
    pthread_t                   _thread;
    GLuint                      _soundtexture;
    PASink                     *_audiostream;
    SpectralAnalyzer           *_specta;
    MidiInput                  *_midiin;
    MidiUniformMap              _midi_uniform_map;
    Uniform                     _time_uniform;
    float                       _time_step_size;
    bool                        _is_paused;
    UniformIv                  *_midi_notes_uniform; // state for each note 0-11

public:

    LiveGLServer(char *audio_device=NULL, int port=4223, bool blocking=false);
    ~LiveGLServer();

    // bind current shader
    void bind();
    int id();

    void poll();
    void start();
    void join();
    void run();

protected:

    void handle_request(const char* data);
    void process_audio();
    void process_midi();
    void update_uniforms();

    void filter_for_midi_mapping(MidiMessage *midi_msg);
    int make_midi_signature(int value, int type);
    void init_midi_mapping();

};

#endif
