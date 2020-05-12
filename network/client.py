from PyQt5 import QtWidgets,QtCore,QtGui
from ui.mainwindow import Scene
import message.engproto.enginecmd_pb2 as Message
import components.server_publisher as server_publisher
import components.server_listener as server_listener
import sys
import time
import threading

def main_ui():
    app = QtWidgets.QApplication(sys.argv)
    w = QtWidgets.QMainWindow()
    s = Scene()
    w.setCentralWidget(s)
    w.show()
    sys.exit(app.exec_())

ip = "tcp://192.168.0.130"
    
def read_server():
    global ip
    reader = server_listener.ServerListener(ip)
    reader.connect()
    while(True):
        msg = reader.read()
        print(str(msg))
        

def write_server():
    global ip
    publisher = server_publisher.ServerPublisher(ip)    
    publisher.connect()
    count = 0
    
    while(True):
        count = count + 1

        msgList = Message.CommandList()	
        msg = Message.Command()
        msg.Name = "Hello"
        msg.Opa = count
        msgList.Cmd.append(msg)    
        #msgList.Cmd.append(msg)
        #msgList.Cmd.append(msg)
        #msgList.Cmd.append(msg)
        #msgList.Cmd.append(msg)

        publisher.send(msgList.SerializeToString())

        if count % 10 == 0:
            print(str(count))



def main():
    threading.Thread(target=read_server).start()
    #threading.Thread(target=write_server).start()

    while(True):
        time.sleep(1)

if __name__ == "__main__":
    main()
