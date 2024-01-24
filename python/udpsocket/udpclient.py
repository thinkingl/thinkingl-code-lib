import argparse
import socket
import random



if __name__=="__main__":
    parser = argparse.ArgumentParser(description='UDP socket server')
    #parser.add_argument( "startPort", type=int, help="UDP start port.")
    #parser.add_argument( "endPort", type=int, help="UDP end port.")
    parser.add_argument( "serverIP", help='Server IP Addr' )
    parser.add_argument( "serverPort", type=int, help="Server port")
    parser.add_argument( "len", type=int, help="send data len")

    args = parser.parse_args()

    #print( "port:", args.startPort, "-", args.endPort )


    s = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )

    #data = random.randbytes( args.len )
    data = bytearray(args.len)
    for i in range(args.len):
        data[i] = i%255
    s.sendto( data, (args.serverIP, args.serverPort ) )
