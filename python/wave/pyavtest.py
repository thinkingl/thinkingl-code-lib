import av
from fractions import Fraction
import wave_ex
import numpy

def test8kulawTo8k16bitpcm():
    inFile = '8kulaw.wav'
    ulawAudio = av.open( inFile )

    outFile = '8kulawTo8k16bitpcm.wav'
    pcmAudio = av.open( outFile, 'w' )

    s1 = pcmAudio.add_stream( 'pcm_s16le', rate= 8000 )
    ctx1 = s1.codec_context
    ctx1.layout = 'mono'
    ctx1.channels = 1

    for frame in ulawAudio.decode():
        print( 'in frame:', frame )
        for packet in s1.encode(frame):
            pcmAudio.mux( packet )

    for packet in s1.encode():
        pcmAudio.mux( packet )    
    pcmAudio.close()

def test8k16bitpcmStero():
    inFile = '8kulaw.wav'
    ulawAudio = av.open( inFile )

    outFile = '8kulawTo8k16bitpcm-2ch.wav'
    pcmAudio = av.open( outFile, 'w' )

    s1 = pcmAudio.add_stream( 'pcm_s16le', rate= 8000 )
    ctx1 = s1.codec_context
    ctx1.layout = 'stereo'
    ctx1.channels = 2

    for frame in ulawAudio.decode():
        print( 'in frame:', frame )
        for packet in s1.encode(frame):
            pcmAudio.mux( packet )

    for packet in s1.encode():
        pcmAudio.mux( packet )    
    pcmAudio.close()


from av import AudioFormat, Codec, VideoFormat, codecs_available
from av.container.streams import StreamContainer


def testG722toWav():
    
    # Read g722 data. 8k g722.
    testFile = 'g722.raw'
    g722Data = open(testFile, 'rb').read()

    dec_codec_ctx = Codec('g722', 'r').create()

    if dec_codec_ctx.codec.audio_formats != None:
        sample_fmt = dec_codec_ctx.codec.audio_formats[-1].name
        dec_codec_ctx.format = sample_fmt

    sample_rate = 8000
    channel_layout = "mono"
    channels = 1

    dec_codec_ctx.time_base = Fraction(1) / sample_rate
    dec_codec_ctx.sample_rate = sample_rate
    dec_codec_ctx.layout = channel_layout
    dec_codec_ctx.channels = channels
    dec_codec_ctx.open()

    codec_name = 'pcm_mulaw'
    
    enc_codec = Codec(codec_name, 'w')

    enc_codec_ctx = enc_codec.create()

    sample_fmt = enc_codec_ctx.codec.audio_formats[-1].name
    sample_rate = 8000
    channel_layout = "mono"
    channels = 1

    enc_codec_ctx.time_base = Fraction(1) / sample_rate
    enc_codec_ctx.sample_rate = sample_rate
    enc_codec_ctx.format = sample_fmt
    enc_codec_ctx.layout = channel_layout
    enc_codec_ctx.channels = channels

    enc_codec_ctx.open()

    packet = av.packet.Packet(g722Data)
    decoded_data = None
    packetBytes = None
    for frame in dec_codec_ctx.decode(packet):
        print( frame )
        frame.sample_rate = 8000
        frame.rate = 8000
        for p in enc_codec_ctx.encode( frame ):
            packetBytes = p.to_bytes()
            print( p )
        decoded_data = frame.planes[0].to_bytes()
        open( testFile + '.test', 'wb').write( decoded_data )
        a = 1
    
    w = wave_ex.Wave_write( 'g722TomulawWav.wav' )
    w.setsampwidth( 2 )
    w.setcomptype( wave_ex.WAVE_FORMAT_MULAW, None )
    w.setframerate( 8000 )
    w.setnchannels( 1 )
    w.setnframes( 0 )
    w.writeframesraw( packetBytes );
    w.close()

    w = wave_ex.Wave_write( 'g722Towav.wav' )
    w.setsampwidth( 2 )
    w.setcomptype( wave_ex.WAVE_FORMAT_PCM, 'PCM' )
    w.setframerate( 8000 )
    w.setnchannels( 1 )
    w.setnframes( 0 )
    w.writeframesraw( decoded_data );
    w.close()

    w = wave_ex.Wave_write( 'g722Towav-2ch.wav' )
    w.setsampwidth( 2 )
    w.setcomptype( wave_ex.WAVE_FORMAT_PCM, 'PCM' )
    w.setframerate( 8000 )
    w.setnchannels( 2 )
    w.setnframes( 0 )

    # 组织2声道数据.
    dataLen = len( decoded_data )
    waveData = bytearray( b'\0' )* dataLen * 2
    waveData[::4] = decoded_data[0:dataLen:2]
    waveData[1::4] = decoded_data[1:dataLen:2]
    waveData[2::4] = decoded_data[0:dataLen:2]
    waveData[3::4] = decoded_data[1:dataLen:2]
    w.writeframesraw( waveData );
    w.close()


if __name__ == '__main__':
    test8kulawTo8k16bitpcm()
    #test8k16bitpcmStero()
    testG722toWav()