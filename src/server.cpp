#include "server.h" 
#include "midi_message.h"
#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

using namespace std;

void *run_thread(void *arg) {
    LiveGLServer *server = (LiveGLServer *)arg;
    server->run();
}

LiveGLServer::LiveGLServer(int port, bool blocking) 
    :  _ctx(new zmq::context_t(1))
    ,  _socket(new zmq::socket_t(*_ctx, ZMQ_PULL) )
    ,  _shader(new Shader())
    ,  _blocking(blocking)
    ,  _audiostream(new PASink())
    ,  _specta(new SpectralAnalyzer())
    ,  _midiin(new MidiInput(MIDI_PORT))
    ,  _time_uniform(Uniform("time"))
    ,  _time_step_size(0.01)
    ,  _is_paused(false)
{
    
    char endpoint[64];
    sprintf(endpoint, "tcp://127.0.0.1:%d",port);
    cout << "Connecting to " << endpoint << endl;

    // setup connection
    _socket->bind(endpoint);

    cout << "Successfull connected to server" << endl;

    _pollitem[0].socket = *_socket;
    _pollitem[0].fd = 0;
    _pollitem[0].events = ZMQ_POLLIN;
    _pollitem[0].revents = 0;

    // setup 1d texture for spectral data
    glGenTextures(1,&_soundtexture);
    glBindTexture(GL_TEXTURE_1D,_soundtexture);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    int width = _specta->nfreq() -1;

    float dummytex[4*width];
    memset(dummytex,0,sizeof(float)*width*4);

//    memset(dummytex,0,width);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA32F,width,0,GL_RGBA,GL_FLOAT,dummytex);

    init_midi_mapping();
}

void LiveGLServer::init_midi_mapping() {
    
    for( int i = MIDI_MAP_CC_START; i <= MIDI_MAP_CC_END; i++ ) {
        char *uniform_name = (char *)malloc(16);
        int midi_signature = make_midi_signature(i, MIDI_TYPE_CC);
        snprintf(uniform_name, 16, "midi_cc_%d", i);


        Uniform *uniform = new Uniform(uniform_name);
        _midi_uniform_map[midi_signature] = uniform;
        cout << "Map " << _midi_uniform_map[midi_signature]->name() << "( " << (int)midi_signature << " ) to uniform" << endl;
    }
        cout << _midi_uniform_map.size() << " value mapped" <<endl;
}

int LiveGLServer::make_midi_signature(int value, int type) {
    return (type << 8) | value;
}

LiveGLServer::~LiveGLServer() {
    pthread_cancel(_thread);
    delete _shader;
}

/* DEPRECATED */
void LiveGLServer::start() {
    cout << "LiveGL server started" << endl;
    int err = pthread_create(&_thread, NULL, run_thread, (void *)this );
}

/* DEPRECATED */
void LiveGLServer::run() {

    for(;;) {
        zmq::message_t msg;
        _socket->recv(&msg);
        handle_request((const char*)msg.data());
        pthread_testcancel();
    }

}

// poll for events (non-blocking)
void LiveGLServer::poll() {

    process_audio();
    process_midi();
    update_uniforms();

    zmq::message_t msg;
    
    if(_blocking) {
        _socket->recv(&msg);
        handle_request((const char*)msg.data());
        return;
    }
   
    // else no blocking
    int ret = zmq::poll(&_pollitem[0], 1,5);
    if(_pollitem[0].revents & ZMQ_POLLIN) {
        _socket->recv(&msg,0);
        handle_request((const char*)msg.data());
    }

}

void LiveGLServer::process_midi() {
  
    MidiMessage *midi_msg = _midiin->get_message();
    if(midi_msg) {
        cout <<  midi_msg->to_string() << endl;
        filter_for_midi_mapping(midi_msg);
        delete midi_msg;
    }
}

void LiveGLServer::update_uniforms() {
    // update time
    if( !_is_paused ) {
        _time_uniform.incr(_time_step_size);
        _time_uniform.update_shader(_shader->id());
    }

    // update midi mapped uniforms
    for (MidiUniformMap::iterator it=_midi_uniform_map.begin(); it!=_midi_uniform_map.end(); ++it) {
        Uniform *uniform = (Uniform *)it->second;
        uniform->update_shader(_shader->id());
    }
}

void LiveGLServer::filter_for_midi_mapping(MidiMessage *midi_msg) {
    if(midi_msg->is_cc() && midi_msg->cc_chan() == MIDI_CC_CHAN) {

       cout << midi_msg << endl;
       int midi_signature = make_midi_signature(midi_msg->cc_number(), MIDI_TYPE_CC);
       MidiUniformMap::const_iterator it = _midi_uniform_map.find(midi_signature);
       if( it != _midi_uniform_map.end() ) {
         Uniform *uniform = it->second;
         uniform->set( midi_msg->cc_value_f() );
       } else {
        cout << "No uniform mapped to CC#" << midi_msg->cc_number() << endl;
       }
    }
    // system exlusive
    else if(midi_msg->is_system_exclusive()) {
        if(midi_msg->is_stop()) {
            _is_paused = true;
        }
        else if(midi_msg->is_start()) {
            _is_paused = !_is_paused;
        }
    }
}

void LiveGLServer::process_audio() {
    try {
        // update sound texture
        int16_t *sample = _audiostream->read_data();
        _specta->analyze_sample(sample); 
        float *spect = _specta->spectrum();
        float *spect_flux = _specta->spectral_flux();
        float *bands_three = _specta->bands_three();
        float *bands_smooth = _specta->bands_smooth();

        unsigned int width = _specta->nfreq()-1;
        glBindTexture(GL_TEXTURE_1D,_soundtexture);
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, width, GL_RED, GL_FLOAT,spect);
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, width, GL_GREEN, GL_FLOAT,spect_flux);

        int uloc = glGetUniformLocation(_shader->id(), "bands_three");
        glUniform1fv(uloc, 3, bands_three);

        uloc = glGetUniformLocation(_shader->id(), "sound");
        glUniform3fv(uloc, 1, bands_smooth);

    } catch(const char *e) {
        cout << "error while processing audio: " << e << endl;
    }
}

void LiveGLServer::handle_request(const char *data) {

    Json::Reader reader;
    Json::Value root;
    bool succ = reader.parse(data, root);

    if(!succ) {
        cout  << "Failed to parse message\n"
              << reader.getFormatedErrorMessages();
        return; 
    }

    string action;

    if(!root.isMember("action")) {
        cout << "Invalid message format" << endl;
    }
        
    action = root["action"].asString();

    if(action == "pause") {
    
        cout << "Pause shader" << endl;
        _shader->unbind();
        return;
    }

    if(action == "resume") {
    
        cout << "Resume shader" << endl;
        _shader->bind();
        return;
    }

    if(action == "send_shader") {

        if(!_shader->active()) {
            cout << "Shaders a currently deactivated." << endl;
            return;
        }

        const char *shader_type = 0, *shader_source = 0;

        if(root.isMember("shader")) 
            shader_type = root["shader"].asCString();
        if(root.isMember("source")) 
            shader_source = root["source"].asCString();

        if(shader_type && shader_source) {
                
            try {
                _shader->replace_shader(shader_type,shader_source);
            } catch(const char *e) {
                cout << "Shader not loaded: " << e << endl;
            }

            cout << "New "<< shader_type << " programm loaded" << endl;
            return;
        }
    
        cout << "Missing parameters for: send_shader" << endl;
    }

}

// get id of current shader
int LiveGLServer::id() {
    return _shader->id();
}

void LiveGLServer::bind() {
    _shader->bind();
}

// block until thread is terminated
void LiveGLServer::join() {
    pthread_join(_thread, NULL);
}
