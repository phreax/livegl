#include "server.h" 
#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void *run_thread(void *arg) {
    LiveGLServer *server = (LiveGLServer *)arg;
    server->run();
}

LiveGLServer::LiveGLServer(int port) 
    :  _ctx(new zmq::context_t(1))
    ,  _socket(new zmq::socket_t(*_ctx, ZMQ_PULL) )
    ,  _shader(new Shader())
{
    
    char endpoint[64];
    sprintf(endpoint, "tcp://127.0.0.1:%d",port);
    cout << "Connecting to " << endpoint << endl;

    // setup connection
    _socket->bind(endpoint);

    cout << "connected" << endl;

    _pollitem[0].socket = *_socket;
    _pollitem[0].fd = 0;
    _pollitem[0].events = ZMQ_POLLIN;
    _pollitem[0].revents = 0;


}

LiveGLServer::~LiveGLServer() {
    pthread_cancel(_thread);
    delete _shader;
}

void LiveGLServer::start() {
    cout << "LiveGL server started" << endl;
    int err = pthread_create(&_thread, NULL, run_thread, (void *)this );
}

void LiveGLServer::run() {

    for(;;) {
        zmq::message_t msg;
        _socket->recv(&msg);
        process((const char*)msg.data());
        pthread_testcancel();
    }

}

// poll for events (non-blocking)
void LiveGLServer::poll() {

    zmq::message_t msg;
    int ret = zmq::poll(&_pollitem[0], 1,5);
    if(_pollitem[0].revents & ZMQ_POLLIN) {
        _socket->recv(&msg,0);
        process((const char*)msg.data());
    }

}

void LiveGLServer::process(const char *data) {

    cout << "data received" << endl;
    cout << data << endl;

    Json::Reader reader;
    Json::Value root;
    bool succ = reader.parse(data, root);

    if(!succ) {
        cout  << "Failed to parse message\n"
              << reader.getFormatedErrorMessages();
        return; 
    }

    const char *shader_type = 0, *shader_source = 0;

    if(root.isMember("shader")) 
        shader_type = root["shader"].asCString();
    if(root.isMember("source")) 
        shader_source = root["source"].asCString();

    cout << "parsing data successfuly" << endl;
    
    if(shader_type && shader_source) {
            
        try {
            _shader->replaceShader(shader_type,shader_source);
        } catch(const char *e) {
            cout << "new shader not loaded: " << e << endl;
        }

        cout << "shader replaced" << endl;
        return;
    }

    cout << "Invalid format" << endl;

}

// get id of current shader
int LiveGLServer::id() {
    return _shader->id();
}

// get id of current shader
void LiveGLServer::bind() {
    _shader->bind();
}

// block until thread is terminated
void LiveGLServer::join() {
    pthread_join(_thread, NULL);
}
