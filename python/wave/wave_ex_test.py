
import wave_ex


def testUlawRead():
    ulawFilePath = '8kulaw.wav'

    r = wave_ex.Wave_read(ulawFilePath)
    assert( wave_ex.WAVE_FORMAT_MULAW == r.getcomptype() )

    f = r.readframes(1)
    while( len(f) > 0 ):
        f = r.readframes(1)


def testReadWrite():
    ulawFilePath = '8kulaw.wav'

    r = wave_ex.Wave_read(ulawFilePath)
    assert( wave_ex.WAVE_FORMAT_MULAW == r.getcomptype() )

    w = wave_ex.Wave_write('8kulaw_rewrite.wav')
    w.setsampwidth( r.getsampwidth() )
    w.setcomptype( r.getcomptype(), None )
    w.setframerate( r.getframerate() )
    w.setnchannels( r.getnchannels() )
    w.setnframes( r.getnframes() )
    w.setparams( r.getparams() )

    f = r.readframes(1)
    while( len(f) > 0 ):
        w.writeframesraw(f)
        f = r.readframes(1)
    w.close()
    #with open( ulawFilePath ) as f:

def testTrans2TwoChannels():
    ulawFilePath = '8kulaw.wav'

    r = wave_ex.Wave_read(ulawFilePath)
    assert( wave_ex.WAVE_FORMAT_MULAW == r.getcomptype() )

    w = wave_ex.Wave_write('8kulaw_rewrite_2chn.wav')
    w.setparams( r.getparams() )
    w.setnchannels( 2 )

    f = r.readframes(1)
    while( len(f) > 0 ):
        f = f+f
        w.writeframesraw(f)
        f = r.readframes(1)
    w.close()

if __name__ == '__main__':
    testUlawRead()
    testTrans2TwoChannels()
    testReadWrite()