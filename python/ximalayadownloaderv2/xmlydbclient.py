import xmlycfg
import requests

class XMLYDatabaseClient:
    xmlyDatabaseServiceUrl = xmlycfg.XMLYDatabaseServiceUrl

    # 初始化
    def init(self, dbPath):
        return True
    
    def close(self):
        return
    
    # 更新一个专辑.
    def updateAlbum(self, id, jsonAlbum ):
        return self.json2DB( 'album', id, jsonAlbum )

    # 更新主播信息
    def updateAnchor(self, id, jsonAnchor):
        return self.json2DB( 'anchor', id, jsonAnchor)

    # 下载完成一个音频.
    def finishTrack(self, trackId, jsonTrack):
        return self.json2DB( 'track', trackId, jsonTrack )

    def getAnchor(self, id):
        return self.db2Json('anchor', id)

    # 获取专辑信息
    def getAlbum(self, id):
        return self.db2Json('album', id)

    # 获取音频信息.
    def getTrack(self, id):
        return self.db2Json('track',id)

    # 获取一个专辑的所有track
    def getTrackList(self, albumId):
        url = self.xmlyDatabaseServiceUrl + '/tracklist/' + str(albumId)
        response = requests.get( url=url )
        trackListJson = response.json()
        return trackListJson["trackList"]

    # 更新数据库
    def json2DB(self, tableName, id, jsonObj):
        url = self.xmlyDatabaseServiceUrl + '/' + tableName + '/' + str(id)
        requests.put( url=url, json=jsonObj )
        return

    # 读取数据库
    def db2Json(self, tableName, id ):
        url = self.xmlyDatabaseServiceUrl + '/' + tableName + '/' + str(id)
        response = requests.get( url )
        return response.json()

if __name__ == '__main__':
    clientTest = XMLYDatabaseClient()
    trackList = clientTest.getTrackList( '3488393' )
    print( "track list:" + str(trackList) )
    albumInfo = clientTest.getAlbum( '3488393' )
    print( 'album info:' + str(albumInfo) )
    albumInfo['id'] = '1'
    clientTest.updateAlbum( '1', albumInfo )
    albumInfo = clientTest.getAlbum( '1' )
    print( 'album info of 1:' + str(albumInfo) )

