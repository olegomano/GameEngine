import zmq

class ServerListener:
    def __init__(self,ip,listener=None):
        print("Creating Server Listener " + str(ip))
        self.ip = ip
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB) 
        self.socket.setsockopt(zmq.SUBSCRIBE, b"")
        

    def connect(self):
        self.socket.connect(self.ip+":5555")
    
    def read(self):
        print("Read")
        data = self.socket.recv()
        return data
