import socket
import logging
import time
import select
import threading
import sys

def treehole( port = 1333 ):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    while True:
        try:
            print( 'treehole start to liten on ', port)
            sock.bind(('0.0.0.0', port))  #配置soket，绑定IP地址和端口号
            sock.listen() #
            print( 'treehole litenning on ', port, ' success')
            break
        except:
            print( 'bind port fail! try again!' )
            time.sleep(1)
    while True:
        connection,address = sock.accept()  
        print( 'recv connection from ', str( address) )
        threading.Thread(name='treehole for ' + str(address), target=treeholeRecvThread, args=(connection, address)).start()


def treeholeRecvThread(connection, address):
    try:
        while True:
            data = connection.recv(10*1024)
            if len( data ) == 0:
                print( 'recv empty data from :', str(address) )
                break
            print( 'recv data from address: ', str(address))
            print( data )
            # echo
            connection.send(data)
    except:
        pass
    print( 'treeholeRecvThread for ', str(address), ' exit!' )
    connection.close()
    return


if len(sys.argv) > 1:
    treehole( sys.argv[1] )
else:
    treehole()