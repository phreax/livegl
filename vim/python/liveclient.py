import vim
import zmq
import time
import re

class LiveClient:
    
    def __init__(self,endpoint="tcp://127.0.0.1:4223"):

        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUSH)

        self.socket.connect(endpoint)
        self.t = time.time()
        self.active = True

        self.requireRegex = re.compile('^//\s*require\s+"([\w,./]+)"\s*$',re.MULTILINE)

    def send_shader(self,shader_type):

        msg = {"action":"send_shader"}

        now = time.time()
        if(now-self.t > 1):
            source = '\n'.join(vim.current.buffer)
            source = self.include_requirements(source)
            msg.update({"shader":shader_type,"source":source})
            try:
                self.socket.send_json(msg,zmq.NOBLOCK)
            except zmq.ZMQError:
                print "Server not ready"

        self.t = now
     
    def pause_shader(self):

        msg = {"action":"pause"}

        now = time.time()
        if(now-self.t > 1):
            try:
                self.socket.send_json(msg,zmq.NOBLOCK)
            except zmq.ZMQError:
                print "Server not ready"

        
        self.active = False
        self.t = now


    def resume_shader(self):

        msg = {"action":"resume"}

        now = time.time()
        if(now-self.t > 1):
            try:
                self.socket.send_json(msg,zmq.NOBLOCK)
            except zmq.ZMQError:
                print "Server not ready"

        self.active = True
        self.t = now

    def toggle_shader(self):
        if(self.active):
            self.pause_shader()
        else:
            self.resume_shader()


    def include_requirements(self,src):

        def match_function(match):
            filename = match.group(1)
            try:
                f = file(filename)
                return f.read()
            except:
                print 'Could not open file: '+filename
                return ""

        return self.requireRegex.sub(match_function,src)

        
if __name__ == "__main__":

    liveclient = LiveClient()

