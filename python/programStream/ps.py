from re import A
from scapy.all import *
import json
import struct

filePath = 'programStream.pcapng'
filePath = 'ps2.pcapng'
filePath = 'kd-pure.pcap'
filePath = 'sample.pcapng'

packList = rdpcap( filePath )
print( "open file " + filePath + " obj: " + str(packList) )

sessions= packList.sessions()

packetsInfo = {}

rawDataFile = open(filePath + '.raw.data', 'wb')
esDataFile = open( filePath + '.es.data', 'wb')
psDataFile = open( filePath + '.ps.data', 'wb')


def readPES( pesData, packetInfo ):
    esDataFile.write( pesData )
    startCode = pesData[0:3]
    assert( startCode == b'\x00\x00\x01' )
    streamId = pesData[3]
    packetInfo['streamId'] = streamId

    if streamId != 224:
        print( 'stream id is not 224, it is ', streamId )

    pesLen = pesData[4:6]
    pesLen = struct.unpack('>H', pesLen)[0]
    flag1 = pesData[6]
    flag2 = pesData[7]
    flag2 = format( flag2, '08b' )
    ptsFlag = flag2[0] == '1'
    dtsFlag = flag2[1] == '1'
    escrFlag = flag2[2] == '1'
    esrateFlag = flag2[3] == '1'
    dsmTrickModeflag = flag2[4] == '1'


    curPos = 8

    pesLoadPos = curPos
    if flag2 != b'\x00':
        pesHeaderLen = pesData[curPos]
        curPos += 1
        pesLoadPos = curPos + pesHeaderLen

    if ptsFlag:
        pts = pesData[curPos:curPos+4]
        curPos += 4

    if dtsFlag:
        dts = pesData[curPos:curPos+4]
        curPos += 4

    pesLoad = pesData[pesLoadPos:]
    rawDataFile.write( pesLoad )

    #print( 'here' )

    return
def readPS(psData, packetInfo):
    psDataFile.write( psData )
    # PACK identifier -- 0xBA
    syncBytes = psData[0:4]

    if syncBytes == b'\x00\x00\x01\xe0':
        readPES( psData, packetInfo )
        return

    if syncBytes != b'\x00\x00\x01\xba':
        print( 'invalid ps sync bytes!' )
        return
    assert( syncBytes == b'\x00\x00\x01\xba' )
    # System Clock Reference
    scr = psData[4:10]
    # Program_Mux_Rate
    programMuxRate = psData[10:14]

    curPos = 14
    if psData[curPos:curPos+4] == b'\x00\x00\x01\xbb':
        partialSystemHeaderStartCode = psData[curPos:curPos+4]
        curPos += 4
        partialSystemHeaderLen = psData[curPos:curPos+2]
        partialSystemHeaderLen = struct.unpack( '>H', partialSystemHeaderLen )[0]
        curPos += 2
        curPos += partialSystemHeaderLen
    
    if psData[ curPos: curPos+4] == b'\x00\x00\x01\xbc':
        partialSystemMapStartCode = psData[curPos:curPos+4]
        curPos += 4
        partialSystemMapLen = psData[curPos:curPos+2]
        partialSystemMapLen = struct.unpack( '>H', partialSystemMapLen )[0]
        curPos += 2
        curPos += partialSystemMapLen

    # pes
    pes = psData[curPos:]
    readPES( pes, packetInfo )

    return

# 应该先把rtp组帧
# 然后再把每一个ps帧解析.
# 不需要解析ps流.
# 组帧过程中有可能有乱序/重复的/丢失的帧.

for session in sessions:
    print( "start parse session ", session )
    sessionInfo = {}
    packetsInfo[ session ] = sessionInfo
    packets = sessions[session]

    packetsInfo = list()
    sessionInfo['packets'] = packetsInfo
    psStartFlag = False
    for i,p in enumerate( packets ):
        #packetInfo = {}
        #packetsInfo.append( packetInfo )
        payload = p.payload
        rtpData = payload.load
        rtpPacket = scapy.layers.rtp.RTP( _pkt=rtpData )
        timestamp = rtpPacket.timestamp
        sourcesync = rtpPacket.sourcesync
        sequence = rtpPacket.sequence
        payloadType = rtpPacket.payload_type
        marker = rtpPacket.marker
        psData = rtpPacket.load

        syncBytes = psData[0:4]
        if not psStartFlag:
            if syncBytes == b'\x00\x00\x01\xba':
                psStartFlag = True
            else:
                continue
        psDataFile.write( psData )

psDataFile.close()

psStream = open( psDataFile, 'b' )

        #readPS( psData, packetInfo )

packetsInfoText = json.dumps( packetsInfo, indent=4)
with open( filePath + '.txt', 'w') as f:
    f.write( packetsInfoText )

