import requests
import base64
# avdb 客户端

class AvdbClient:
    serverBaseUrl = 'http://127.0.0.1:5001/'
    serverBaseUrl = 'http://192.168.84.8:5001/'

    # 添加一个图片
    def addPic( self, avInfo, picData ):
        fileName = avInfo['imgFileName']
        url = self.serverBaseUrl + 'image/' + fileName
        picBase64 = base64.b64encode( picData ).decode('utf-8')

        addPicReq = {}
        addPicReq['picJson'] = avInfo
        addPicReq['picData'] = picBase64

        rsp = requests.put( url, json=addPicReq )
        return rsp 

    # 图片是否存在
    def isAVUrlExist( self, avUrl ):
        url = self.serverBaseUrl + 'image/search'

        searchPicReq = {}
        searchPicReq['url'] = avUrl
        for i in range(0,10):
            try:
                rsp = requests.get( url, params=searchPicReq )
        
                fileName = rsp.json()['result']

                return fileName != None
            except:
                print( "isAVUrlExist exception!" )
                pass
        
        return False
        

if __name__ == "__main__":
    c = AvdbClient()
    c.isAVUrlExist( "http://www.javlibrary.com/tw/?v=javlijb6si" )