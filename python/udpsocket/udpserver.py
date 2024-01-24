import select
import socket
import sys
import queue
import argparse



if __name__=="__main__":
    parser = argparse.ArgumentParser(description='UDP socket server')
    parser.add_argument( "startPort", type=int, help="UDP start port.")
    parser.add_argument( "endPort", type=int, help="UDP end port.")

    args = parser.parse_args()

    print( "port:", args.startPort, "-", args.endPort )

    if not args.endPort > args.startPort:
        print( "End port must > start port!" )
        exit(0)

    sockets = list()
    filenoMap = {}
    for port in range(args.startPort, args.endPort ):
        #print( "Create socket for port ", port )
        s = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )  # SOCK_STREAM
        s.bind( ('0.0.0.0', port) )
        print( 'fileno %d for port %d'%( s.fileno(), port) )
        filenoMap[s.fileno() ] = s
        sockets.append(s)

    inputs = sockets

    outputs = []

    mode = "poll"

    if mode == "poll":
        poll = select.poll()
        for s in sockets:
            poll.register(s, select.POLLIN)
        
        while inputs:
            for fd, event in poll.poll():
                print( fd, event )
                if event == select.POLLIN:
                    s = filenoMap.get(fd)
                    data = s.recv(1024*1024)
                    print( "recv data [%d]"% (len(data),) )
    else:
        while inputs:
            readable, writable, exceptional = select.select( inputs, outputs, inputs, 10 )

            for s in readable:
                data = s.recv(1024*1024)
                print( "recv data [%d]"% (len(data),) )
