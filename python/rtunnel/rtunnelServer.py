# -*- coding: utf-8 -*- 
import json
import logging
import socket
import threading
import rtunnelUtils
import queue
import time
import base64

class RTunnelServer():

    clientTable = {}    # clientId -> data queue for client ( send to client ).
    appConnectionTable = {} # connectionId -> data queue for app ( send to app )

    def start( self, serverCfg ):
        logging.info( 'server start! cfg:[%s]', serverCfg )
        self.serverCfg = serverCfg

        # 监听服务端口.
        threading.Thread( name='service', target=self.serviceThread ).start()

        # 监听通道端口.
        '''
        "map":[
            {
                "name":"remote desktop control for 90",
                "clientId":"1",
                "localPort":3389,
                "remoteAddr":"192.168.84.90",
                "remotePort":3389,
                "protocol":"tcp"
            }
        ]
        '''
        mappingCfgs = serverCfg.get('map')
        for mapping in mappingCfgs:
            threading.Thread( name='tunnel-'+mapping.get('name'), target=self.tunnelThread, args=(mapping,) ).start()

        return

    def serviceThread(self):
        logging.info( 'service thread run! cfg:[%s]', self.serverCfg )

        port = self.serverCfg.get('port')
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  

        while True:
            try:
                logging.info( 'start to bind to %d', port )
                sock.bind(('0.0.0.0', port))  #配置soket，绑定IP地址和端口号
                sock.listen() #
                logging.info( 'server litenning on %d', port)
                break
            except:
                logging.exception( 'error' )
                time.sleep(1)

        while True:  #循环轮询socket状态，等待访问
                
            connection,address = sock.accept()  
            logging.info( 'Accept connection from %s', address )
            threading.Thread(name='serviceConnection-recv-'+str(address), target=self.serviceConnectionRecvThread, args=(connection,address)).start()
            
        return

    # 服务的数据发送线程.
    def serviceConnectionSendThread(self, connection, clientId, address, dataQueue):
        logging.info( 'serviceConnectionSendThread for %s@%s start! dataQueue:[%s]', clientId, address, dataQueue )

        lastAllConnections = None
        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = dataQueue.get( block=True, timeout=1)
                except:
                    #logging.debug( 'serviceConnectionSendThread get from dataQueue fail!' )
                    pass
                if sendMsg != None:
                    logging.debug( 'serviceConnectionSendThread get msg to send, msg: %s', sendMsg )
                    rtunnelUtils.sendJsonMsg( connection, sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    #connection.send(b'')
                    # 发一个连接校验包.
                    allConnections = list(self.appConnectionTable.keys())
                    if allConnections != lastAllConnections:
                        lastAllConnections = allConnections
                        syncMsg = { 'cmd':'sync', 'connections':allConnections}
                        rtunnelUtils.sendJsonMsg( connection, syncMsg )
                    else:
                        #发一个空包,试试socket是不是有效.
                        connection.send(b'')
                
        except:
            logging.exception( 'error' )
        logging.info( 'serviceConnectionSendThread for %s@%s exit! dataQueue:[%s]', clientId, address, dataQueue )
        return

    def serviceConnectionRecvThread(self, connection, address):
        logging.info( 'serviceConnectionRecvThread for %s start!', address )
        sendThread = None
        clientId = None
        try:  
            # 先收注册, 初始化.
            regMsg = rtunnelUtils.recvMsg( connection )
            if regMsg.get('cmd') == 'register':
                clientId = regMsg.get('clientId')
                logging.info( 'recv client [%s] register!', clientId )
                
                queueSize = self.serverCfg.get( 'queueSize' )
                sendQueue = queue.Queue( queueSize )
                self.clientTable[ clientId ] = sendQueue
                logging.info( 'bind clientId - sendQueue : [%s - %s]', clientId, sendQueue )

                sendThread = threading.Thread(name='serviceConnection-send-'+ clientId + '@' + str(address), target=self.serviceConnectionSendThread, args=(connection, clientId, address, sendQueue))
                sendThread.start()


                while True:
                    msg = rtunnelUtils.recvMsg( connection )
                    logging.debug( 'serviceConnectionRecvThread recv msg from client. msg: %s', msg )
                    cmd = msg.get('cmd')
                    if cmd == 'sync':
                        allConnections = msg.get('connections')
                        for connect in allConnections:
                            if not connect in self.appConnectionTable:
                                logging.error( 'connection %s is invalid!', connect )
                                # 发个信令把这个连接关掉.
                                disconnectMsg = {'cmd':'disconnect', 'id':connect}
                                sendQueue.put( disconnectMsg )
                                logging.info( 'put msg to server2Client queue, msg: %s', disconnectMsg )
                            else:
                                logging.debug( 'sync, connection %s is ok!', connect)
                        pass
                    else:
                        connectionId = msg.get( 'id' )
                        appQueue = self.appConnectionTable.get( connectionId )
                        if appQueue == None:
                            logging.error( 'Can not find connection queue for %s', connectionId )
                        else:
                            appQueue.put( msg )
            else:
                logging( 'First msg should be register, but recv [%s]', regMsg )
                
        except:
            logging.exception( 'error' )
        #
        logging.info( 'Threads for %s@%s start to exit!', clientId, address )
        connection.close()
        sendThread.join()
        if clientId:
            del self.clientTable[clientId]
        logging.info( 'Threads for %s@%s exit!', clientId, address )
        return

    def tunnelThread(self, mappingCfg):
        logging.info( 'tunnel thread run! tunnel cfg:[%s]', mappingCfg)

        port = mappingCfg.get( 'localPort' )
        clientId = mappingCfg.get( 'clientId' )

        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  

        while True:
            try:
                logging.info( 'tunnel start to liten on %d', port)
                sock.bind(('0.0.0.0', port))  #配置soket，绑定IP地址和端口号
                sock.listen() #
                logging.info( 'tunnel litenning on %d success', port)
                break
            except:
                logging.exception( 'error' )
                time.sleep(1)

        while True:  #循环轮询socket状态，等待访问
            connection,address = sock.accept()  

            logging.info( 'recv connection from %s', address )
            send2ClientQueue = self.clientTable.get( clientId )
            if send2ClientQueue == None:
                logging.error( 'Client %s has not connected yet.', clientId )
                connection.close()
                continue
            connectionId = str(address)
            queueSize = self.serverCfg.get( 'queueSize' )
            appQueue = queue.Queue( queueSize )
            self.appConnectionTable[ connectionId ] = appQueue

            clientDataQueue = self.clientTable[ clientId ]

            threading.Thread( name='tunnel-send-' + str(address), target=self.tunnelSendThread, args=(connection,mappingCfg, connectionId, appQueue) ).start()
            threading.Thread( name='tunnel-recv-' + str(address), target=self.tunnelRecvThread, args=(connection,mappingCfg, connectionId, clientDataQueue) ).start()

        return
    
    # 从连接到通道上的app接收数据.
    def tunnelRecvThread(self, connection, mappingCfg, connectionId, clientQueue):
        logging.info( 'tunnelRecvThread for %s start!', connectionId )

        # 先给客户端发一个连接命令.
        addr = mappingCfg.get( 'remoteAddr' )
        port= mappingCfg.get( 'remotePort' )
        connectMsg = { 'cmd':'connect', 'id':connectionId, 'addr':addr,'port':port}
        clientQueue.put( connectMsg )
        logging.debug( 'tunnelRecvThread put msg to client queue. msg: %s', connectMsg)
        try:
            while True:
                data = connection.recv( 1500 )
                if len( data ) == 0:
                    logging.error( 'recv empty data. connectionId: %s addr: %s:%d', connectionId, addr, port )
                    break
                # 数据转给client.
                dataMsg = {'cmd':'data', 'id':connectionId,'data':base64.encodestring(data).decode()}
                clientQueue.put( dataMsg )
                logging.debug( 'tunnelRecvThread put msg to client queue. msg: %s', dataMsg)
        except:
            logging.error( 'tunnelRecvThread recv data except!' )
            logging.exception('error')

        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        clientQueue.put( disconnectMsg )
        logging.debug( 'tunnelRecvThread put msg to client queue. msg: %s', disconnectMsg)

        logging.info( 'tunnelRecvThread for %s exit!', connectionId )
        return

    # 数据发到app.
    def tunnelSendThread(self,connection, mappingCfg, connectionId, appQueue):
        logging.info( 'tunnelSendThread for %s start!', connectionId )

        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = appQueue.get( block=True, timeout=1)
                except:
                    pass
                if sendMsg != None:
                    logging.debug( 'tunnelSendThread has msg to send. msg: %s', sendMsg )
                    cmd = sendMsg.get('cmd')
                    if cmd == 'data':
                        # 将data中的数据发给connection的app
                        data = sendMsg.get( 'data' )
                        data = base64.decodebytes( data.encode() )
                        connection.send( data )
                        
                    elif cmd == 'disconnect':
                        # 和app断开连接.
                        logging.info( 'client cmd server to disconnect with app! id:%s', connectionId )
                        connection.close()
                        break
                        pass
                    else:
                        logging.error( 'Unsupported msg: %s', sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    connection.send(b'')
                
        except:
            logging.exception( 'error' )

        del self.appConnectionTable[ connectionId ]
        logging.info( 'del app queue from appConnectionTable for %s', connectionId )

        logging.info( 'tunnelSendThread for %s exit!', connectionId )
        return