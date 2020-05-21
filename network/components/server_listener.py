import zmq
import time
from components.message import enginecmd_pb2 as pb 

class ServerListener:
    def __init__(self,ip,listener=None):
        print("Creating Server Listener " + str(ip))
        self.ip = ip
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB) 
        self.socket.setsockopt(zmq.SUBSCRIBE, b"")
        self.socket.setsockopt(zmq.CONFLATE,True)
        

    def connect(self):
        self.socket.connect(self.ip+":5555")
    
    def read(self):
        recvStart = time.time()
        data = self.socket.recv()
        recvEnd = time.time()
            
        pbStart = time.time()
        commands = pb.CommandList()
        commands.ParseFromString(data)
        pbEnd = time.time()

        print("Network: " + str( (recvEnd-recvStart)*1000) + "ms ProtoBuff: " + str((pbEnd - pbStart)*1000))

        return commands
