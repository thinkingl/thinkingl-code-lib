
import sys
import struct
import io
import wave_ex

fileName = '8kulaw.wav'
fileName = '8kmp316.wav'
if len( sys.argv ) < 2:
    print( 'no file name param!' )
    #exit(0)
else:
    fileName = sys.argv[1]
with open( fileName, 'rb' ) as f:
    f.seek(0,io.SEEK_END)
    fileSize = f.tell()
    print( 'File size:\t', fileSize)
    f.seek(0,0)
    chunkID = f.read(4).decode()
    print( 'chunkID:\t', str(chunkID) )
    chunkSize = f.read(4)
    chunkSize = struct.unpack('<L', chunkSize)[0]
    print( 'Chunksize:\t', chunkSize)
    format = f.read(4).decode()
    print( 'Format:\t\t', format)
    while( f.tell() < chunkSize +8 ):
        subChunkID = f.read(4).decode()
        if subChunkID == '\x00':
            continue
        print( '\tSubChunkID:\t', subChunkID)
        subChunkSize = struct.unpack('<L', f.read(4))[0]
        print( '\tSubChunkSize:\t', subChunkSize)

        subChunkDataStartPos = f.tell()
        subChunkEndPos = subChunkDataStartPos + subChunkSize

        if subChunkID == 'fmt ':
            audioFormat = struct.unpack('<H', f.read(2))[0]
            audioFormatName = wave_ex.getCompressionName( audioFormat )
            print( '\tAudioFormat:\t', audioFormat, '\t', audioFormatName)
            numChannels = struct.unpack('<H', f.read(2))[0]
            print( '\tNumChannels:\t', numChannels)
            sampleRate = struct.unpack('<L', f.read(4))[0]
            print( '\tSampleRate:\t', sampleRate )
            byteRate = struct.unpack('<L', f.read(4))[0]
            print( '\tbyteRate:\t', byteRate )
            blockAlign = struct.unpack('<H', f.read(2))[0]
            print( '\tBlockAlign:\t', blockAlign)
            bitsPerSample = struct.unpack('<H', f.read(2))[0]
            print( '\tbitPerSample:\t', bitsPerSample)

            # 是否有扩展字段.
            if f.tell() < subChunkEndPos:
                cbSize = struct.unpack('<H', f.read(2))[0]
                print( '\tcbSize:\t', cbSize)
            
            if f.tell() < subChunkEndPos:
                wValidBitsPerSample = struct.unpack('<H', f.read(2))[0]
                print( '\twValidBitsPerSample:\t', wValidBitsPerSample)
            
            if f.tell() < subChunkEndPos:
                dwChannelMask = str( f.read(4) )
                print( '\tdwChannelMask:\t', dwChannelMask)

            if f.tell() < subChunkEndPos:
                SubFormat = str(f.read(16))
                print( '\tSubFormat:\t', SubFormat)

            pass
        elif subChunkID == 'fact':
            dwSampleLength = struct.unpack('<L', f.read(4))[0]
            print( '\tSampleLength:\t', dwSampleLength)
            pass
        elif subChunkID == 'data':
            dataSize = struct.unpack('<L', f.read(4))[0]
            print( '\tDatasize:\t', dataSize)
            # 打印64字节数据.
            pLen = min( dataSize, 64 )
            while( pLen >= 16 ):
                print( '\t', f.read(16))
                pLen = pLen - 16
            pass
        elif subChunkID == 'LIST':
            # https://www.recordingblogs.com/wiki/list-chunk-of-a-wave-file
            listType = str(f.read(4)) #struct.unpack('<L', f.read(4))[0]
            print( '\tListtype:\t', listType)
            listDataLen = subChunkSize - 4 ;
            print( '\tListdata:\t', f.read(listDataLen))
        else:
            # 打印64字节数据
            dataSize = subChunkSize - 2
            pLen = min( dataSize, 64 )
            while( pLen >= 16 ):
                print( '\t', f.read(16))
                pLen = pLen - 16

        print( '\n' )
        f.seek( subChunkEndPos, 0)        