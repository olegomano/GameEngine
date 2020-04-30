import message.engproto.enginecmd_pb2 as Message
import components.server_publisher as server_publisher

def main():
    ip = "tcp://192.168.0.130"
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
        msgList.Cmd.append(msg)
        msgList.Cmd.append(msg)
        msgList.Cmd.append(msg)
        msgList.Cmd.append(msg)

        publisher.send(msgList.SerializeToString())

        if count % 1000 == 0:
            print(str(count))

if __name__ == "__main__":
    main()
