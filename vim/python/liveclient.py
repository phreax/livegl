import vim
import zmq
import time

class LiveClient:
    
    def __init__(self,endpoint="tcp://127.0.0.1:4223"):

        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUSH)

        self.socket.connect(endpoint)
        self.t = time.time()

    def send_shader(self,shader):

        now = time.time()
        if(now-self.t > 1):
            source = '\n'.join(vim.current.buffer)
            try:
                self.socket.send_json({"shader":shader,"source":source},zmq.NOBLOCK)
            except zmq.ZMQError:
                print "Server not ready"

        self.t = now
        
if __name__ == "__main__":

    liveclient = LiveClient()

