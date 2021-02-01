# -*- coding: utf-8 -*- 
import json
import threading
import logging
import socket
from rtunnelUtils import *
import time
import queue
import rtunnelUtils
import base64

class RTunnelClient():

    client2AppQueueTable = {}   # connectionId -> client2AppQueue

    def start(self, clientCfg ):
        threading.Thread(name='clientThread', target=self.clientThread, args=(clientCfg,)).start()
        return


    def clientThread(self, clientCfg):
        logging.info( 'clientThread start! cfg:[%s]', clientCfg)
        queueSize = clientCfg.get( 'queueSize' )
        client2ServerQueue = queue.Queue( queueSize )

        while True:
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
                '''
                "client":{
                    "id":"1",
                    "serverAddr":"lizhixing.top",
                    "serverPort":2080
                }
                '''
                address = (clientCfg.get('serverAddr'), clientCfg.get('serverPort'))
                sock.connect( address )

                logging.info( 'client connect to %s success!', address )

                # 发送自己的注册.
                sendMsg(sock, 'register', clientId=clientCfg.get('id') )

                sendThread = threading.Thread( name='clientSendThread', target=self.clientSendThread, args=(sock,client2ServerQueue))
                sendThread.start()

                while True:
                    msg = recvMsg( sock )
                    logging.debug( 'clientThread recv msg from server. msg: %s', msg )
                    if msg['cmd'] == 'connect':
                        address = (msg.get('addr'), msg.get('port'))
                        connectionId = msg.get('id')
                        # 创建线程处理这个连接请求.
                        client2AppQueue = queue.Queue( queueSize )
                        self.client2AppQueueTable[ connectionId ] = client2AppQueue
                        threading.Thread(name='ClientAppRecvThread-' + connectionId + '-' + str(address), target=self.clientAppRecvThread, args=(connectionId,address, client2ServerQueue, client2AppQueue)).start()
                    elif msg['cmd'] == 'sync':
                        allConnections = msg.get('connections')
                        for connect in allConnections:
                            if not connect in self.client2AppQueueTable:
                                logging.error( 'connection %s is invalid!', connect )
                                # 发个信令把这个连接关掉.
                                disconnectMsg = {'cmd':'disconnect', 'id':connect}
                                client2ServerQueue.put( disconnectMsg )
                                logging.info( 'put msg to client2Server queue, msg: %s', disconnectMsg )
                            else:
                                logging.debug( 'sync, connection %s is ok!', connect)
                        pass
                    else:
                        connectionId = msg.get('id')
                        client2AppQueue = self.client2AppQueueTable.get( connectionId )
                        if client2AppQueue != None:
                            client2AppQueue.put( msg )
                            logging.debug( 'put msg to client2App queue, msg: %s', msg )
                        else:
                            logging.error( 'Cannot find client2AppQueue for msg! msg:%s', msg )
            except:
                logging.exception( 'error' )
                time.sleep(1)
        return
    

    def clientSendThread(self, connection, dataQueue):
        logging.info( 'clientSendThread start!')
        # 将要发给服务的数据发出去.
        try:
            lastAllConnections = None
            while True:
                sendMsg = None
                try:
                    sendMsg = dataQueue.get( block=True, timeout=1)
                except:
                    pass
                if sendMsg != None:
                    rtunnelUtils.sendJsonMsg( connection, sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    # connection.send(b'')
                    # 发一个连接校验包.
                    allConnections = list(self.client2AppQueueTable.keys())
                    if allConnections != lastAllConnections:
                        lastAllConnections = allConnections
                        syncMsg = { 'cmd':'sync', 'connections':allConnections}
                        rtunnelUtils.sendJsonMsg( connection, syncMsg )
                    else:
                        #发一个空包,试试socket是不是有效.
                        connection.send(b'')
        except:
            logging.exception( 'error' )

        logging.info( 'clientSendThread exit!')
        return

    def clientAppRecvThread(self, connectionId, address, client2ServerQueue, client2AppQueue):
        logging.info( 'clientAppRecvThread start! connectionId: %s address: %s', connectionId, address)
        # 连接App.
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
            sock.connect( address )

            # 创建发送线程.
            threading.Thread(name='clientAppSendThread-' + connectionId + '-' + str(address), target=self.clientAppSendThread, args=(sock, connectionId, address, client2AppQueue)).start()

            # 接收数据. 
            while True:
                data = sock.recv(1500)
                if len( data ) == 0:
                    logging.error( 'recv empty data. connectionId: %s addr: %s', connectionId, address )
                    break
                # 将数据转给server.
                dataMsg = {'cmd':'data', 'id':connectionId,'data':base64.encodestring(data).decode()}
                client2ServerQueue.put( dataMsg )
                logging.debug( 'put msg to client2Server queue. msg: %s', dataMsg )

        except:
            logging.exception( 'error' )

        # 发送断链给服务端.
        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        client2ServerQueue.put( disconnectMsg )

        logging.info( 'clientAppRecvThread exit! connectionId: %s address: %s', connectionId, address)
        return

    def clientAppSendThread(self, connection, connectionId, address, client2AppQueue ):
        logging.info( 'clientAppSendThread start! connectionId: %s addr: %s', connectionId, address)
        # 将要发给服务的数据发出去.
        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = client2AppQueue.get( block=True, timeout=1)
                except:
                    pass
                if sendMsg != None:
                    logging.debug( 'clientAppSendThread get msg from queue, msg: %s', sendMsg )
                    cmd = sendMsg.get('cmd')
                    if cmd == 'disconnect':
                        # 和app断开连接.
                        logging.info( 'server cmd client to disconnect with app! id:%s', connectionId )
                        connection.close()
                        break
                    elif cmd == 'data':
                        data = sendMsg.get( 'data' )
                        data = base64.decodebytes( data.encode() )
                        connection.send( data )
                        logging.debug( 'Client send data to app, data:[%s]', data )
                        pass
                    else:
                        logging.error( "Unsupported cmd ! msg:%s", sendMsg )
                    pass
                else:
                    # 发一个空包,试试socket是不是有效.
                    connection.send(b'')
                
        except:
            logging.exception( 'error' )

        del self.client2AppQueueTable[ connectionId ]
        logging.info( 'del app queue from client2AppQueueTable for %s', connectionId )

        logging.info( 'clientAppSendThread exit! %s addr: %s', connectionId, address)
        return