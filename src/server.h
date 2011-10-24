#ifndef __SERVER_H
#define __SERVER_H

#include "shader.h" 
#include "zmq.hpp"

class LiveGLServer {

    public:
        
        LiveGLServer(int port=4223);
        ~LiveGLServer();

        // bind current shader
        void bind();
        int id();


        void poll();
        void start();
        void join();
        void run();

    protected:
        void process(const char* data);

    private:
        
        zmq::context_t *_ctx;
        zmq::socket_t  *_socket;
        zmq::pollitem_t  _pollitem[1];
        Shader         *_shader;

        pthread_t      _thread;


};


#endif
