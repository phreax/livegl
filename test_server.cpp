#include "server.h"
#include "iostream"
#include "zmq.hpp"

using namespace std;
using namespace zmq;

int main(int argc, const char *argv[])
{
   
    cout << "creating server" << endl; 
    LiveGLServer *server = new LiveGLServer();
    cout << "starting server" << endl; 
    server->start();
    server->join();
    
    
    return 0;

}
