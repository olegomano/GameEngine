import zmq

class ServerListener:
    def __init__(self,ip,listener=None):
        print("Creating Server ServerListener")
        self.ip = ip
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB) 
        self.listener = listener
        pass

    def connect(self):
        self.socket.connect(self.ip+":1234")
    
    def start(self):
        pass

    def stop(self):
        pass

