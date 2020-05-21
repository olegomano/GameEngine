from PyQt5 import QtWidgets,QtCore,QtGui
from ui.mainwindow import Scene
#import message.engproto.enginecmd_pb2 as Message
import components.server_publisher as server_publisher
import components.server_listener as server_listener
import sys
import time
import threading


class GameContext():
    def __init__(self):
        self.app = None
        self.scene = None
        self.ip = None
        pass

    def start(self,ip):
        self.ip = ip
        self.app = QtWidgets.QApplication(sys.argv)
        w = QtWidgets.QMainWindow()
        self.scene = Scene()
        w.setCentralWidget(self.scene)
        w.show()
        threading.Thread(target=self.read_server).start()
        sys.exit(self.app.exec_())
     
    def read_server(self):
        reader = server_listener.ServerListener(self.ip)
        reader.connect()
        updateMap = {}
        prev = time.time()
        while(True):
            now = time.time()
            delta = now - prev
            print("FrameTime: " + str(delta*1000) + "ms")
            prev = now

            msg = reader.read()
            
            for command in msg.Cmd:
                objId = command.Opa
                name = command.Name
                value = command.Opb
                if objId not in updateMap:
                    updateMap[objId] = {}
                updateMap[objId][name]=value       
            
            for objId in updateMap:
                self.scene.setPos(objId,updateMap[objId]['x'],updateMap[objId]['y'])
            #except Exception as e:
            #    print("Loop Exception : " + str(e))

def main():
    ip = "tcp://192.168.0.130"
    context = GameContext()
    context.start(ip)
    #threading.Thread(target=write_server).start()
    
if __name__ == "__main__":
    main()
