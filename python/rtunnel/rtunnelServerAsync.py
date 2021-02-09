# -*- coding: utf-8 -*- 
import json
import logging
import socket
import threading
import rtunnelUtils
import queue
import time
import base64
import asyncio

class RTunnelServer():

    clientTable = {}    # clientId -> data queue for client ( send to client ).
    appConnectionTable = {} # connectionId -> data queue for app ( send to app )
    recvBufSize = 1500

    def start( self, serverCfg ):
        logging.info( 'server start! cfg:[%s]', serverCfg )
        self.serverCfg = serverCfg

        if serverCfg.get( 'recvBufSize') != None:
            self.recvBufSize = serverCfg.get( 'recvBufSize')

        # 监听服务端口.
        threading.Thread( name='service', target=self.serviceThread, daemon=True ).start()

        return

    def serviceThread(self):
        logging.info( 'service thread run! cfg:[%s]', self.serverCfg )

        asyncio.run( self.serviceTask() )
        logging.info( 'service thread exit!' )

    async def serviceTask(self):
        tasks = []
        serviceClientTask = asyncio.create_task(self.serviceClientTask())
        tasks.append( serviceClientTask )
        mappingCfgs = self.serverCfg.get('map')
        for mapping in mappingCfgs:
            tunnelTask = asyncio.create_task(self.tunnelTask(mapping))
            tasks.append(tunnelTask)
        asyncio.gather( *tasks )
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
                        appQueue = self.appConnectionTable.get( connectionId )[0]
                        if appQueue == None:
                            logging.error( 'Can not find connection queue for %s', connectionId )
                        else:
                            await appQueue.put( msg )
            else:
                logging.error( 'First msg should be register, but recv [%s]', regMsg )

        except:
            logging.exception('error')

        writer.close()
        await writer.wait_closed()
        logging.info( 'serverClientReadTask exit!')

    async def clientWriteTask(self,rader, writer, clientId, clientDataQueue ):
        logging.info( 'clientWriteTask start! clientId: %s clientDataQueue:%s ', clientId, clientDataQueue)

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
                    logging.info( 'clientWriteTask get msg to send to client %s, msg: %s', clientId, str(sendMsg)[0:100] )

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
        logging.info( 'clientWriteTask exit! clientId: %s clientDataQueue:%s ', clientId, clientDataQueue)
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
