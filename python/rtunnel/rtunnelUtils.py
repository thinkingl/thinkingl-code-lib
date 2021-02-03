import json
import struct
import logging
import asyncio

def sendJsonMsg(sock, msg ):
    msgBytes = json.dumps(msg).encode('utf-8')
    sendData = struct.pack('!L', len(msgBytes))
    sendData = sendData + msgBytes
    sock.send( sendData )
    logging.debug( 'msg:[%s] send:[%s]', msg, sendData )
    return

def sendMsg(sock, cmd, **kwargs ):
    msg = kwargs
    msg['cmd'] = cmd

    sendJsonMsg(sock, msg)
    return

async def sendJsonMsgAsync(writer, msg):
    msgBytes = json.dumps(msg).encode('utf-8')
    sendData = struct.pack('!L', len(msgBytes))
    sendData = sendData + msgBytes
    writer.write( sendData )
    await writer.drain()
    logging.debug( 'sendMsgAsync msg:[%s] send:[%s]', msg, sendData )
    return

async def sendMsgAsync(writer, cmd, **kwargs ):
    msg = kwargs
    msg['cmd'] = cmd

    await sendJsonMsgAsync(writer, msg)
    return

def recvData(sock, dataLen):
    recvLen = 0
    recvData = bytes()
    while recvLen < dataLen:
        leftLen = dataLen - recvLen
        curData = sock.recv(leftLen)
        if len( curData ) == 0:
            logging.error( 'recv 0 bytes data!' )
            raise Exception( "Empty data!" )
        recvLen = recvLen + len(curData)
        recvData = recvData + curData
    return recvData

    

def recvMsg(sock):
    lenData = recvData(sock,4)
    len = struct.unpack('!L', lenData)[0]
    logging.debug( 'recv msg len: [%d]', len)
    msgData = recvData(sock, len)
    logging.debug( 'recv msg data:[%s]', msgData)
    msg = json.loads(msgData)
    logging.debug( 'recv [%s]', msg)
    return msg;

async def recvMsgAsync(reader):
    len = await reader.readexactly(4)
    len = struct.unpack('!L', len)[0]
    logging.debug( 'recv msg len: [%d]', len)
    msgData = await reader.readexactly(len)
    logging.debug( 'recv msg data:[%s]', msgData)
    msg = json.loads(msgData)
    logging.debug( 'recv [%s]', msg)
    return msg;