#ifndef __SERVER_H
#define __SERVER_H

#include "shader.h" 
#include "spectral.h"
#include "pasink.h"
#include "zmq.hpp"

class LiveGLServer {

    public:
        
        LiveGLServer(bool mute=false, int port=4223, bool blocking=false);
        ~LiveGLServer();

        // bind current shader
        void bind();
        int id();

        void poll();
        void start();
        void join();
        void run();

    protected:

        void update_sound_texture();
        void handle_request(const char* data);

    private:
        
        zmq::context_t *_ctx;
        zmq::socket_t  *_socket;
        zmq::pollitem_t  _pollitem[1];
        Shader         *_shader;
        bool            _blocking;
        pthread_t      _thread;
        GLuint         _soundtexture;
        PASink         *_audiostream;
        SpectralAnalyzer *_specta;

};


#endif
