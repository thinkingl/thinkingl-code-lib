import execjs
import threading
import time
import queue
import os
import requests
import hashlib
import random

class XMSign:
    xmSignQueue = queue.Queue()
    headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36',
            'Accept': 'text/html,application/xhtml+ xml,application/xml;q = 0.9,image/webp,image/apng,*/*;q=0.8, application/signe-exchange;v = b3',
            'Host': 'www.ximalaya.com'
        }

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
    
    def getXMSignDirectly(self):
        serverTime = self.getxmtime()
        xmSign = self.getXMSignPy( serverTime )
        return xmSign
    
    def getXMSignPy(self, serverTime):
        # {himalaya-1571966953413}(67)1571966953413(10)1571970554308
        # MD5(himalaya-服务端时间)(小于100随机数)服务端时间(小于100随机数)本地时间
        nowTime = str(round(time.time()*1000))

        sign = str(hashlib.md5("ximalaya-{}".format(serverTime).encode()).hexdigest()) + "({})".format(str(round(random.random()*100))) + serverTime + "({})".format(str(round(random.random()*100))) + nowTime
        self.headers['xm-sign'] = sign
        return sign
    
    def getXMSignPyHimalaya(self, serverTime):
        # {himalaya-1571966953413}(67)1571966953413(10)1571970554308
        # MD5(himalaya-服务端时间)(小于100随机数)服务端时间(小于100随机数)本地时间
        nowTime = str(round(time.time()*1000))

        sign = str(hashlib.md5("himalaya-{}".format(serverTime).encode()).hexdigest()) + "({})".format(str(round(random.random()*100))) + serverTime + "({})".format(str(round(random.random()*100))) + nowTime
        self.headers['xm-sign'] = sign
        return sign
   
    def threadGenXMSign(self):
        os.environ["EXECJS_RUNTIME"] = "Node"
        jsCtx = execjs.compile( open('xmSign.js').read() )
        while( True ):
            xmlyTime = self.getxmtime()
            #xmSign1 = jsCtx.call( 'python', xmlyTime ) # int(time.time()*1000))
            xmSign2 = self.getXMSignPy( xmlyTime )
            self.putXMSign( xmSign2 )
            time.sleep(3)

    def getxmtime(self):
        url="https://www.ximalaya.com/revision/time"
        response = requests.get(url, headers=self.headers,verify=False)
        # print(response)
        html = response.text
        return html
