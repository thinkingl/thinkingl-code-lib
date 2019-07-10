import sqlite3
import logging
import time


class XMLYDatabase:
    connect = None

    # 构造函数.


    # 初始化
    def init(self, dbPath):

        self.connect = sqlite3.connect( dbPath, 10*1000 )

        # album 属性表 - albumId | Attr name | attr value 
        self.connect.execute( 'Create Table  IF NOT EXISTS album(id TEXT NOT NULL, attrName TEXT NOT NULL, attrValue TEXT  NOT NULL, PRIMARY KEY(id,attrName));' )

        # track 属性表 - trackId | Attr name | attr value
        self.connect.execute( 'Create Table  IF NOT EXISTS track(id TEXT NOT NULL, attrName TEXT NOT NULL, attrValue TEXT  NOT NULL, PRIMARY KEY(id,attrName));' )

        # anchor 属性表 - anchorId | Attr name | attr value
        self.connect.execute( 'Create Table  IF NOT EXISTS anchor(id TEXT NOT NULL, attrName TEXT NOT NULL, attrValue TEXT  NOT NULL, PRIMARY KEY(id,attrName));' )

        return self.connect != None
    
    def close(self):
        if( self.connect ):
            self.connect.close()
    
    # 更新一个专辑.
    def updateAlbum(self, id, jsonAlbum ):
        return self.json2DB( 'album', id, jsonAlbum )

    # 更新主播信息
    def updateAnchor(self, id, jsonAnchor):
        return self.json2DB( 'anchor', id, jsonAnchor)

    # 下载完成一个音频.
    def finishTrack(self, trackId, jsonTrack):
        return self.json2DB( 'track', trackId, jsonTrack )


    # 获取专辑信息
    def getAlbum(self, id):
        return self.db2Json('album', id)

    # 获取音频信息.
    def getTrack(self, id):
        return self.db2Json('track',id)

    # 获取一个专辑的所有track
    def getTrackList(self, albumId):
        trackSet = set()
        # 有的音频是"转采"的, 它的album_id字段不是当前专辑, 而是原始出处的专辑. 这会造成当前专辑id查不到目标.
        # 改用自己加的属性名 albumId
        for row in self.connect.execute("select id from track where attrName='albumId' and attrValue = ? ;", (albumId,)):
            trackSet.add( row[0] )
        return list(trackSet)

    # 更新数据库
    def json2DB(self, tableName, id, jsonObj):
        for key in jsonObj :
            #for i in range(10):
            try:
                self.connect.execute('Replace into %s Values(?,?,?);'%tableName, (id,key, str(jsonObj[key])) )
            except:
                logging.error('Replace info db fail! id: %s key: %s value: %s', id, key, jsonObj[key])
                logging.exception('error')
                time.sleep(1)
                pass   
        self.connect.commit()
        return

    # 读取数据库
    def db2Json(self, tableName, id ):
        obj = {}
        for row in self.connect.execute('Select attrName, attrValue from %s where id = ?;'%tableName, (id,)):
            obj[row[0]] = row[1]
        return obj



