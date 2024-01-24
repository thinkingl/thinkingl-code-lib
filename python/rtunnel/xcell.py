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

# 整合RTunnel的Client和Server的所有功能.
# 只要两个cell之间的链路建立起来, 就可以实现所有的共享功能.
# 1. 建立ConveyorBelt连接其他的cell
# 2. 监听端口, 接收其它cell的连接
# 3. 

class XCell():

    cellWriteQueueTable = {}    # 向Cell发送消息的队列 表

    def start(self, cellCfg):
        self.cellCfg = cellCfg
        logging.info( 'XCell start! celllCfg: %s', cellCfg )
        if cellCfg.get( 'recvBufSize') != None:
            self.recvBufSize = cellCfg.get( 'recvBufSize')
        self.conveyorCfg = self.cellCfg.get('conveyor')
        self.queueSize = self.conveyorCfg.get( 'queueSize' )
        threading.Thread(name='cellThread', target=self.cellThread, daemon=True).start()
        return

    def cellThread(self):
        logging.info( 'cellThread start!' )
        asyncio.run( self.cellMain() )
        logging.info( 'cellThread exit!' )
        return

    async def cellMain(self):
        logging.info( 'cellMain start!' )

        tasks = list()
        # 创建主动去连别人的conveyor Belt.
        nodes = self.cellCfg.get( 'nodes' )
        for node in nodes:
            conveyorTask = asyncio.create_task( self.conveyorClientServerTask(node) )
            tasks.append( conveyorTask )

        # 创建等待别人来注册的监听.
        serviceClientTask = asyncio.create_task(self.serviceClientTask())
        tasks.append( serviceClientTask )

        # 本地的端口映射任务.
        portTunnelCfgs = self.cellCfg.get('portTunnels')
        for portTunnelCfg in portTunnelCfgs:
            tunnelTask = asyncio.create_task(self.tunnelTask(portTunnelCfg))
            tasks.append(tunnelTask)

        await asyncio.gather(*tasks)
        logging.info( 'cellMain start!' )
    
    async def conveyorClientServerTask(self, node):
        logging.info( 'conveyorClientServerTask start! node:[%s]', node)

        while True:
            try:
                serverAddr = node.get('addr')
                serverPort = node.get('port')
                logging.info( 'client start to connect to %s:%d!', serverAddr, serverPort )
                reader, writer = await asyncio.open_connection( serverAddr, serverPort )
                logging.info( 'client connect to %s:%d success!', serverAddr, serverPort )

                # 发送自己的注册.
                me = self.cellCfg.get('me')
                await sendMsgAsync( writer, 'hello', me=me)

                # 创建接收task.
                readTask = asyncio.create_task( self.conveyorReadTask(reader, writer), name='conveyorReadTask')

                #

                await asyncio.gather(*[readTask])
                logging.info( 'client-server conveyor read&write task exit, reconnect!' )

            except:
                logging.exception( 'error' )

            logging.error( 'conveyorClientServerTask going to next loop' )
            await asyncio.sleep(1)
        logging.info( 'conveyorClientServerTask exit! node:[%s]', node)
        return

    async def conveyorReadTask(self, reader, writer):
        logging.info( 'conveyorReadTask start!')

        try:
            # 先收注册, 初始化.
            regMsg = await rtunnelUtils.recvMsgAsync( reader )
            if regMsg.get('cmd') == 'hello':
                he = regMsg.get( 'me' )
                user = he.get('user')
                cell = he.get('cell')
                cellId = cell.get('id')

                cellWriteQueue = asyncio.Queue( self.queueSize )
                self.cellWriteQueueTable[ cellId ] = cellWriteQueue

                # 创建发送任务, 可以向这个cell发送正常消息了.
                writeTask = asyncio.create_task( self.clientWriteTask(writer, cellWriteQueue), name='clientWriteTask')

                while True:
                    logging.debug( 'conveyorReadTask start to recv msg from server.' )
                    msg = await rtunnelUtils.recvMsgAsync( reader )
                    logging.debug( 'conveyorReadTask recv msg from server. msg: %s', msg )

                    continue
                
                    if msg['cmd'] == 'connect':
                        addr = msg.get('addr')
                        port = msg.get('port')
                        connectionId = msg.get('id')
                        # 创建Task处理这个连接.
                        queueSize = self.clientCfg.get( 'queueSize' )

                        client2AppQueue = asyncio.Queue( queueSize )
                        self.client2AppQueueTable[ connectionId ] = client2AppQueue

                        logging.info( 'start to run app task for %s - %s:%d', connectionId, addr, port )
                        asyncio.create_task( self.clientAppTaskRun( addr, port, connectionId, client2AppQueue, client2ServerQueue))
                        #await self.clientAppTaskRun( addr, port, connectionId, client2AppQueue, client2ServerQueue)
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

                pass
            else:
                logging.error( 'First msg should be register, but recv [%s]', regMsg )
        except:
            logging.exception('error')

        try:
            pass
        except:
            logging.exception( 'error' )
        
        writer.close()
        await writer.wait_closed()
        logging.info( 'conveyorReadTask exit!')
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
            await asyncio.gather(*[appReadTask, appWriteTask] )

        except:
            logging.exception( 'error' )
        # 发送断链给服务端.
        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        await client2ServerQueue.put( disconnectMsg )

        logging.info( 'clientAppTaskRun exit! addr: %s port: %d connectionId: %s client2AppQueue: %s client2ServerQueue: %s',addr, port, connectionId, client2AppQueue, client2ServerQueue )
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


    # 监听服务端口， 等待client连接。
    async def serviceClientTask(self):
        logging.info( "serviceClientTask run!")
        port = self.serverCfg.get('port')

        while True:
            try:
                logging.info( 'start_server port %d', port )
                server = await asyncio.start_server( self.handleClient, '0.0.0.0', port )
                logging.info( 'start_server port %d success!', port )

                async with server:
                    await server.serve_forever()
            except:
                logging.exception( 'error' )
                await asyncio.sleep(1)

        logging.info( 'serviceClientTask exit!')
        return

    async def handleClient(self, reader, writer):
        logging.info( 'handleClient start.' )
        asyncio.create_task( self.serverClientReadTask(reader, writer ) )
        logging.info( 'handleClient exit')
        return

    async def serverClientReadTask(self,reader,writer):
        logging.info( 'serverClientReadTask start!' )

        try:
            # 先收注册, 初始化.
            regMsg = await rtunnelUtils.recvMsgAsync( reader )
            if regMsg.get('cmd') == 'register':
                clientId = regMsg.get('clientId')
                logging.info( 'recv client [%s] register!', clientId )
                
                queueSize = self.serverCfg.get( 'queueSize' )
                sendQueue = asyncio.Queue( queueSize )
                self.clientTable[ clientId ] = sendQueue
                logging.info( 'bind clientId - sendQueue : [%s - %s]', clientId, sendQueue )
                asyncio.create_task( self.clientWriteTask(reader, writer, clientId, sendQueue ) )

                while True:
                    msg = await rtunnelUtils.recvMsgAsync(reader)
                    logging.debug( 'serverClientReadTask recv msg from client. msg: %s', msg )
                    cmd = msg.get('cmd')
                    if cmd == 'sync':
                        logging.info( 'recv sync msg %s', msg )
                        allConnections = msg.get('connections')
                        for connect in allConnections:
                            if not connect in self.appConnectionTable:
                                logging.error( 'connection %s is invalid!', connect )
                                # 发个信令把这个连接关掉.
                                disconnectMsg = {'cmd':'disconnect', 'id':connect}
                                await sendQueue.put( disconnectMsg )
                                logging.info( 'put msg to server2Client queue, msg: %s', disconnectMsg )
                            else:
                                logging.debug( 'sync, connection %s is ok!', connect)
                        pass
                    else:
                        connectionId = msg.get( 'id' )
                        appQueueTuple = self.appConnectionTable.get( connectionId )
                        if appQueueTuple == None:
                            logging.error( 'Can not find connection queue for %s', connectionId )
                        else:
                            appQueue = appQueueTuple[0]
                            await appQueue.put( msg )
            else:
                logging.error( 'First msg should be register, but recv [%s]', regMsg )

        except:
            logging.exception('error')

        writer.close()
        await writer.wait_closed()
        logging.info( 'serverClientReadTask exit!')

    async def clientWriteTask(self,rader, writer, clientId, clientDataQueue ):
        logging.info( 'clientWriteTask start! clientId: %s ', clientId)

        lastAllConnections = None
        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = await asyncio.wait_for( clientDataQueue.get(), timeout=1)
                except:
                    #logging.debug( 'serviceConnectionSendThread get from dataQueue fail!' )
                    pass
                if sendMsg != None:
                    logging.debug( 'clientWriteTask get msg to send, msg: %s', sendMsg )
                    #logging.info( 'clientWriteTask get msg to send to client %s, msg: %s', clientId, str(sendMsg)[0:100] )

                    await rtunnelUtils.sendJsonMsgAsync( writer, sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    #connection.send(b'')
                    # 发一个连接校验包.
                    allConnections = list()
                    for c in self.appConnectionTable:
                        if clientId == self.appConnectionTable[c][1]:
                            allConnections.append( c )
                    if allConnections != lastAllConnections:
                        lastAllConnections = allConnections
                        syncMsg = { 'cmd':'sync', 'clientId':clientId, 'connections':allConnections}
                        logging.info( 'send sync msg: %s', syncMsg )
                        await rtunnelUtils.sendJsonMsgAsync( writer, syncMsg )
                    else:
                        #发一个空包,试试socket是不是有效.
                        writer.write(b'')
                        await writer.drain()
                        # 判断连接是否有效.
                        if writer.is_closing():
                            logging.error( 'server-client connection %s is closing.' )
                            break
                
        except:
            logging.exception( 'error' )
        logging.info( 'clientWriteTask exit! clientId: %s ', clientId)
        return

    # 通道task。
    async def tunnelTask(self, tunnelMappingCfg):
        logging.info( 'tunnelTask start! cfg: %s', tunnelMappingCfg )
        port = tunnelMappingCfg.get( 'localPort' )
        clientId = tunnelMappingCfg.get( 'clientId' )

        while True:
            try:
                logging.info( 'start tunnel service on port %d', port )
                server = await asyncio.start_server( lambda reader, writer : self.handleTunnel(reader, writer, tunnelMappingCfg), '0.0.0.0', port )
                logging.info( 'start tunnel service on port %d success!', port )

                async with server:
                    await server.serve_forever()
            except:
                logging.exception( 'error' )
                await asyncio.sleep(1)


        logging.info( 'tunnelTask exit!')
        return

    # tunnel 收发。
    async def handleTunnel( self, reader, writer, tunnelMappingCfg ):
        logging.info( 'handleTunnel for start! cfg: %s', tunnelMappingCfg )
        clientId = tunnelMappingCfg.get( 'clientId' )

        peer = writer.get_extra_info('peername')
        logging.info( 'Tunnel connected by peer %s', peer )

        send2ClientQueue = self.clientTable.get( clientId )
        if send2ClientQueue == None:
            logging.error( 'Client %s has not connected yet.', clientId )
            writer.close()
        else:
            connectionId = str( peer )
            queueSize = self.serverCfg.get( 'queueSize' )
            appQueue = asyncio.Queue( queueSize )
            self.appConnectionTable[ connectionId ] = (appQueue,clientId)
            logging.info( 'new connectionId: %s', connectionId )


        tunnelReadTask = asyncio.create_task( self.tunnelRead(reader,writer, connectionId, tunnelMappingCfg) )
        tunnelWriteTask = asyncio.create_task( self.tunnelWrite(reader,writer, connectionId, appQueue) )

        await asyncio.gather( *[tunnelReadTask, tunnelWriteTask] )

        logging.info( 'handleTunnel for exit! cfg: %s', tunnelMappingCfg )
        return

    # tunnel 收， 发给client
    async def tunnelRead(self, reader, writer, connectionId, tunnelMappingCfg):
        logging.info( 'tunnelRead start!' )
        clientId = tunnelMappingCfg.get( 'clientId' )
        # 先给客户端发一个连接命令.
        addr = tunnelMappingCfg.get( 'remoteAddr' )
        port= tunnelMappingCfg.get( 'remotePort' )
        connectMsg = { 'cmd':'connect', 'id':connectionId, 'addr':addr,'port':port}
        clientDataQueue = self.clientTable[ clientId ]
        await clientDataQueue.put( connectMsg )
        logging.debug( 'tunnelRead put msg to client queue. msg: %s', connectMsg)
        try:
            while True:
                data = await reader.read( self.recvBufSize )
                if len( data ) == 0:
                    logging.error( 'recv empty data. connectionId: %s addr: %s:%d', connectionId, addr, port )
                    break
                # 数据转给client.
                dataMsg = {'cmd':'data', 'id':connectionId,'data':base64.encodestring(data).decode()}
                await clientDataQueue.put( dataMsg )
                logging.debug( 'tunnelRead put msg to client queue. msg: %s', dataMsg)
        except:
            logging.error( 'tunnelRead recv data except!' )
            logging.exception('error')

        disconnectMsg = { 'cmd':'disconnect', 'id':connectionId }
        await clientDataQueue.put( disconnectMsg )
        logging.info( 'tunnelRead put msg to client queue. msg: %s', disconnectMsg)

        writer.close()
        await writer.wait_closed()
        logging.info( 'tunnelRead for %s exit!', connectionId )

        return
    # tunnel 发， 把client发过来的和这个connectionId的数据送给tunnel对端的app
    async def tunnelWrite(self, reader, writer, connectionId, appQueue):
        logging.info( 'tunnelWrite start! connectionId: %s' , connectionId )
        try:
            while True:
                sendMsg = None
                try:
                    sendMsg = await asyncio.wait_for( appQueue.get(), timeout=1 )
                except:
                    pass
                if sendMsg != None:
                    logging.debug( 'tunnelWrite has msg to send. msg: %s', sendMsg )
                    cmd = sendMsg.get('cmd')
                    if cmd == 'data':
                        # 将data中的数据发给connection的app
                        data = sendMsg.get( 'data' )
                        data = base64.decodebytes( data.encode() )
                        writer.write( data )
                        await writer.drain()
                        
                    elif cmd == 'disconnect':
                        # 和app断开连接.
                        logging.info( 'client cmd server to disconnect with app! id:%s', connectionId )
                        writer.close()
                        await writer.wait_closed()
                        break
                        pass
                    else:
                        logging.error( 'Unsupported msg: %s', sendMsg )
                else:
                    # 发一个空包,试试socket是不是有效.
                    writer.write(b'')
                    await writer.drain()
                
        except:
            logging.exception( 'error' )

        del self.appConnectionTable[ connectionId ]
        logging.info( 'del app queue from appConnectionTable for %s', connectionId )

        logging.info( 'tunnelWrite exit! connnectionId: %s', connectionId )
        return