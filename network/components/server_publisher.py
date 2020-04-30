import zmq
import time
class ServerPublisher:
    def __init__(self,ip):
        print("Creating Server Publisher")
        self.ip = ip
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUSH) 
        pass

    def connect(self):
        self.socket.connect(self.ip+":1234")

    def send(self,command):
        self.socket.send(command)
        time.sleep(1)
        pass

