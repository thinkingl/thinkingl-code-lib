import execjs
import threading
import time
import queue
import os

class XMSign:
    xmSignQueue = queue.Queue()
    

    def init(self):
        threading.Thread( target=self.threadGenXMSign, daemon=True ).start()

    def putXMSign(self,xmSign ):
        try:
            while( True ):
                self.xmSignQueue.get(False)
        except:
            pass
        self.xmSignQueue.put( xmSign )
    
    def getXMSign( self ):
        xmSign = self.xmSignQueue.get()
        self.xmSignQueue.put( xmSign )
        return xmSign

    def threadGenXMSign(self):
        os.environ["EXECJS_RUNTIME"] = "Node"
        jsCtx = execjs.compile( open('xmSign.js').read() )
        while( True ):
            xmSign = jsCtx.call( 'python', int(time.time()*1000))
            self.putXMSign( xmSign )
            time.sleep(3)

