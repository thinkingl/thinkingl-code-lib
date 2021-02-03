# -*- coding: utf-8 -*- 
import json
from os import read
import threading
import logging
import socket
from rtunnelUtils import *
import time
import queue
import rtunnelUtils
import base64
import asyncio

class RTunnelClient():

    client2AppQueueTable = {}   # connectionId -> client2AppQueue
    clientCfg = {}
    recvBufSize = 1500

    def start(self, clientCfg ):
        self.clientCfg = clientCfg
        if clientCfg.get( 'recvBufSize') != None:
            self.recvBufSize = clientCfg.get( 'recvBufSize')
        threading.Thread(name='clientThread', target=self.clientThread, args=(clientCfg,), daemon=True).start()
        return

    def startAsync(self, clientCfg):
        self.clientCfg = clientCfg
        if clientCfg.get( 'recvBufSize') != None:
            self.recvBufSize = clientCfg.get( 'recvBufSize')
        threading.Thread(name='clientThreadAsync', target=self.clientThreadAsync, args=(clientCfg,), daemon=True).start()
        return

    async def clientMain(self):
        logging.info( 'clientMain start!' )
        clientServerTask = asyncio.create_task( self.clientServerTask(self.clientCfg) )
        await asyncio.gather(*[clientServerTask])
        logging.info( 'clientMain start!' )

    def clientThreadAsync(self, clientCfg):
        logging.info( 'clientThreadAsync start!' )
        asyncio.run( self.clientMain() )
        logging.info( 'clientThreadAsync exit!' )
        return

    async def clientAppReadTask(self, connectionId, reader, client2ServerQueue):
        logging.info( 'clientAppReadTask start! connectionId: %s', connectionId )
        try:
            # 接收数据. 
            while True:
                data = await reader.read(self.recvBufSize)
                if len( data ) == 0:
                    logging.error( 'clientAppReadTask read empty data. connectionId: %s', connectionId )
                    break
                # 将数据转给server.
                dataMsg = {'cmd':'data', 'id':connectionId,'data':base64.encodestring(data).decode()}
                await client2ServerQueue.put( dataMsg )
                logging.debug( 'put msg to client2Server queue. msg: %s', dataMsg )

        except:
            logging.exception( 'error' )

        # 发送断链给服务端.
        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        await client2ServerQueue.put( disconnectMsg )
        logging.info( 'clientAppReadTask exit!' )
        return

    async def clientAppWriteTask(self, connectionId, writer, client2AppQueue):
        logging.info( 'clientAppWriteTask start!' )
        # 将要发给服务的数据发出去.
        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = await asyncio.wait_for( client2AppQueue.get(), timeout=1)
                except:
                    pass
                if sendMsg != None:
                    logging.debug( 'clientAppSendThread get msg from queue, msg: %s', sendMsg )
                    cmd = sendMsg.get('cmd')
                    if cmd == 'disconnect':
                        # 和app断开连接.
                        logging.info( 'server cmd client to disconnect with app! id:%s', connectionId )
                        break
                    elif cmd == 'data':
                        data = sendMsg.get( 'data' )
                        data = base64.decodebytes( data.encode() )
                        writer.write(data)
                        await writer.drain()
                        logging.debug( 'Client send data to app, data:[%s]', data )
                        pass
                    else:
                        logging.error( "Unsupported cmd ! msg:%s", sendMsg )
                    pass
                else:
                    # 发一个空包,试试socket是不是有效.
                    writer.write(b'')
                    await writer.drain()
                
        except:
            logging.exception( 'error' )

        del self.client2AppQueueTable[ connectionId ]
        logging.info( 'del app queue from client2AppQueueTable for %s', connectionId )

        logging.info( 'clientAppSendThread exit! %s', connectionId)
        writer.close()
        await writer.wait_closed()
        logging.info( 'clientAppWriteTask exit!' )
        return

    async def clientAppTaskRun(self, addr, port, connectionId, client2AppQueue, client2ServerQueue):
        logging.info( 'clientAppTaskRun start! addr: %s port: %d connectionId: %s client2AppQueue: %s client2ServerQueue: %s',addr, port, connectionId, client2AppQueue, client2ServerQueue )
        
        try:
            logging.info( 'client start to connect to app %s:%d!', addr, port )
            reader, writer = await asyncio.open_connection( addr, port )
            logging.info( 'client connect to app %s:%d success!', addr, port )

            appReadTask = asyncio.create_task( self.clientAppReadTask(connectionId, reader, client2ServerQueue ) )
            appWriteTask = asyncio.create_task( self.clientAppWriteTask(connectionId, writer, client2AppQueue ) )
            #await asyncio.gather(*[appReadTask, appWriteTask] )

        except:
            logging.exception( 'error' )
        # 发送断链给服务端.
        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        await client2ServerQueue.put( disconnectMsg )

        logging.info( 'clientAppTaskRun exit! addr: %s port: %d connectionId: %s client2AppQueue: %s client2ServerQueue: %s',addr, port, connectionId, client2AppQueue, client2ServerQueue )
        return

    async def clientReadTask(self, reader, client2ServerQueue, writer):
        logging.info( 'clientReadTask start!')
        clientId = self.clientCfg.get('id')

        try:
            while True:
                logging.debug( 'clientThread start to recv msg from server.' )
                msg = await rtunnelUtils.recvMsgAsync( reader )
                logging.debug( 'clientThread recv msg from server. msg: %s', msg )
            
                if msg['cmd'] == 'connect':
                    addr = msg.get('addr')
                    port = msg.get('port')
                    connectionId = msg.get('id')
                    # 创建Task处理这个连接.
                    queueSize = self.clientCfg.get( 'queueSize' )

                    client2AppQueue = asyncio.Queue( queueSize )
                    self.client2AppQueueTable[ connectionId ] = client2AppQueue

                    logging.info( 'start to run app task for %s - %s:%d', connectionId, addr, port )
                    await self.clientAppTaskRun( addr, port, connectionId, client2AppQueue, client2ServerQueue)
                    #asyncio.get_event_loop().call_soon(self.clientAppTaskRun, (addr, port, connectionId, client2AppQueue, client2ServerQueue) )

                elif msg['cmd'] == 'sync':
                    logging.info( 'recv sync: %s', msg)
                    syncClientId = msg.get('clientId')
                    if syncClientId != clientId:
                        logging.error( 'sync msg with wrong client id! sync client id: %s, my id: %s', syncClientId, clientId )
                        continue
                    allConnections = msg.get('connections')
                    for connect in allConnections:
                        if not connect in self.client2AppQueueTable:
                            logging.error( 'connection %s is invalid!', connect )
                            # 发个信令把这个连接关掉.
                            disconnectMsg = {'cmd':'disconnect', 'id':connect}
                            await client2ServerQueue.put( disconnectMsg )
                            logging.info( 'put msg to client2Server queue, msg: %s', disconnectMsg )
                        else:
                            logging.debug( 'sync, connection %s is ok!', connect)
                    pass
                else:
                    connectionId = msg.get('id')
                    client2AppQueue = self.client2AppQueueTable.get( connectionId )
                    if client2AppQueue != None:
                        await client2AppQueue.put( msg )
                        logging.debug( 'put msg to client2App queue, msg: %s', msg )
                    else:
                        logging.error( 'Cannot find client2AppQueue for msg! msg:%s', str(msg)[0:100] )
        except:
            logging.exception( 'error' )
        
        writer.close()
        await writer.wait_closed()
        logging.info( 'clientReadTask exit!')
        return

    async def clientWriteTask(self, writer, msgQueue):
        logging.info( 'clientWriteTask start!')
        # 将要发给服务的数据发出去.
        try:
            lastAllConnections = None
            idleNum= 0
            while True:
                sendMsg = None
                try:
                    sendMsg = await asyncio.wait_for( msgQueue.get(), timeout=1)
                except:
                    pass
                if sendMsg != None:
                    await rtunnelUtils.sendJsonMsgAsync( writer, sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    # connection.send(b'')
                    # 发一个连接校验包.
                    allConnections = list(self.client2AppQueueTable.keys())
                    if allConnections != lastAllConnections or idleNum > 60:
                        idleNum = 0
                        lastAllConnections = allConnections
                        clientId = self.clientCfg.get('id')
                        syncMsg = { 'cmd':'sync', 'clientId':clientId, 'connections':allConnections}
                        await rtunnelUtils.sendJsonMsgAsync( writer, syncMsg )
                    else:
                        # asyncio对writer的连接判断完全不可靠, 所以需要reader那边帮这边close.....
                        #发一个空包,试试socket是不是有效.
                        writer.write(b'')
                        await writer.drain()
                        # 判断连接是否有效.
                        if writer.is_closing():
                            logging.error( 'client-server connection %s is closing.' )
                            break
                        idleNum = idleNum + 1
        except:
            logging.exception( 'error' )

        writer.close()
        await writer.wait_closed()
        logging.info( 'clientWriteTask exit!')
        return

    async def clientServerTask(self, clientCfg):
        logging.info( 'clientServerTask start! cfg:[%s]', clientCfg)
        queueSize = clientCfg.get( 'queueSize' )

        while True:
            try:
                serverAddr = clientCfg.get('serverAddr')
                serverPort = clientCfg.get('serverPort')
                logging.info( 'client start to connect to %s:%d!', serverAddr, serverPort )
                reader, writer = await asyncio.open_connection( serverAddr, serverPort )
                logging.info( 'client connect to %s:%d success!', serverAddr, serverPort )

                # 发送自己的注册.
                clientId = clientCfg.get('id')
                await sendMsgAsync( writer, 'register', clientId= clientId)

                # 创建2个task.
                client2ServerQueue = asyncio.Queue(queueSize)
                readTask = asyncio.create_task( self.clientReadTask(reader,client2ServerQueue, writer), name='clientReadTask')

                writeTask = asyncio.create_task( self.clientWriteTask(writer, client2ServerQueue), name='clientWriteTask')

                await asyncio.gather(*[readTask, writeTask])

            except:
                logging.exception( 'error' )

            logging.error( 'clientServerTask going to next loop' )
            await asyncio.sleep(1)

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
                clientId = clientCfg.get('id')
                sendMsg(sock, 'register', clientId= clientId)

                sendThread = threading.Thread( name='clientSendThread', target=self.clientSendThread, args=(sock,client2ServerQueue), daemon=True)
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
                        logging.info( 'recv sync: %s', msg)
                        syncClientId = msg.get('clientId')
                        if syncClientId != clientId:
                            logging.error( 'sync msg with wrong client id! sync client id: %s, my id: %s', syncClientId, clientId )
                            continue
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
                            logging.error( 'Cannot find client2AppQueue for msg! msg:%s', str(msg)[0:100] )
            except:
                logging.exception( 'error' )
                time.sleep(1)
        return
    

    def clientSendThread(self, connection, dataQueue):
        logging.info( 'clientSendThread start!')
        # 将要发给服务的数据发出去.
        try:
            lastAllConnections = None
            idleNum= 0
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
                    if allConnections != lastAllConnections or idleNum > 60:
                        idleNum = 0
                        lastAllConnections = allConnections
                        clientId = self.clientCfg.get('id')
                        syncMsg = { 'cmd':'sync', 'clientId':clientId, 'connections':allConnections}
                        rtunnelUtils.sendJsonMsg( connection, syncMsg )
                    else:
                        #发一个空包,试试socket是不是有效.
                        connection.send(b'')
                        idleNum = idleNum + 1
        except:
            logging.exception( 'error' )

        connection.close()
        logging.info( 'clientSendThread exit!')
        return

    def clientAppRecvThread(self, connectionId, address, client2ServerQueue, client2AppQueue):
        logging.info( 'clientAppRecvThread start! connectionId: %s address: %s', connectionId, address)
        # 连接App.
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
            sock.connect( address )

            # 创建发送线程.
            threading.Thread(name='clientAppSendThread-' + connectionId + '-' + str(address), target=self.clientAppSendThread, args=(sock, connectionId, address, client2AppQueue),daemon=True).start()

            # 接收数据. 
            while True:
                data = sock.recv(self.recvBufSize)
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
        connection.close()
        return