import requests
import time


class HashClient():

    def __init__(self, serverBaseUrl) -> None:
        self.serverBaseUrl = serverBaseUrl
    
    
    def addHashlink(self, **kw):
        updateTime = time.time()
        hash = {}
        hash.update( kw )
        hash['updateTime'] = updateTime
        print( 'add hash: ', hash)
        url = self.serverBaseUrl + '/hashlink'
        rsp = requests.put( url=url, json=hash)
        print( 'add hash rsp: ', rsp )
        if( 200 != rsp.status_code ):
            return False
        if( rsp.json().get('error') != 0 ):
            return False
        return True

    def gethashlink(self, dirName:str, fileName:str)->dict:
        url = self.serverBaseUrl + '/hashlink/' + dirName + '/' + fileName
        rsp = requests.get(url)
        print( 'get rsp from server: ', rsp)
        if rsp.status_code != 200:
            return {"error":500}
        return rsp.json()