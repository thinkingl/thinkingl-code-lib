import requests
import base64
import logging
import time
import json
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
        for i in range(0,100):
            try:
                rsp = requests.get( url, params=searchPicReq )
        
                fileName = rsp.json()['result']

                return fileName != None
            except Exception as e:
                logging.error( "isAVUrlExist exception! e: %s", str(e) )
                logging.exception( 'isAVUrlExist' )
                time.sleep(10)
                pass
        
        return False

    def dbIntegrityCheckStart(self):
        logging.info( "-------------------database integrity check start!!!-----------------" )
        for i in range( 0, 10 ):
            try:
                # 检测数据库是否损坏。
                url = self.serverBaseUrl + 'db/integritycheck'

                rsp = requests.post( url )
                logging.info( 'database integrity check start ret: %s', str(rsp.json()) )
                result = rsp.json()['result']
                if result != 'ok':
                    logging.error( "database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    time.sleep( 60 )
                else:
                    logging.info( "-------------------database integrity check  start OK!!!-----------------" )
                    return True
            except Exception as e:
                logging.error( 'Database IntegrityCheck start fail! e: %s', e )
                logging.exception( 'db IntegrityCheck start' )    
                time.sleep(60)
        logging.error( 'database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database IntegrityCheck start fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        time.sleep( 10000000)
        return False

    def getDBIntegrityCheckStatus(self):
        for i in range( 0, 10 ):
            try:
                # 检测数据库是否损坏。
                url = self.serverBaseUrl + 'db/integritycheck'

                rsp = requests.get( url )
                logging.info( 'database integrity check status ret: %s', str(rsp.json()) )
                result = rsp.json()['result']
                
                return result
            except Exception as e:
                logging.error( 'Database IntegrityCheck status fail! e: %s', e )
                logging.exception( 'db IntegrityCheck status' )    
                time.sleep(6*60)
        return 'fail'

    # 检查数据库是否损坏。
    def dbIntegrityCheck(self):
        logging.info( "-------------------begin database integrity check!!!-----------------" )

        self.dbIntegrityCheckStart()

        while( True ):
            result = self.getDBIntegrityCheckStatus()
            if result == 'ok':
                logging.info( 'Database integrity check pass!')
                return True
            elif result == 'running':
                logging.info( 'Database integrity check task running')
                time.sleep(10)
                continue
            else:
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                logging.info( 'Database integrity check fail!!!!!!!!!!!!!!! result: %s', result )
                time.sleep( 10000000)
                return False

        return False

    def dbBackupStart( self, backupToken ):
        for i in range( 0, 10 ):
            try:              
                backupUrl = self.serverBaseUrl + 'db/backup/<filename>'
                fileName = 'avlib-' + backupToken + '.db'
                backupUrl = backupUrl.replace( '<filename>', fileName )
                rsp = requests.post( backupUrl )
                logging.info( 'database backup ret: %s', str(rsp.json()))
                result = rsp.json()['error']
                if result == 'ok':
                    return True
            except Exception as e:
                logging.error( 'Database back up fail! e: %s', e )
                logging.exception( 'db backup' )    
                time.sleep(10)
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        time.sleep( 100000000)
        return False
    
    def dbBackupInfo( self, backupToken ):
        for i in range( 0, 10 ):
            try:              
                backupUrl = self.serverBaseUrl + 'db/backup/<filename>'
                fileName = 'avlib-' + backupToken + '.db'
                backupUrl = backupUrl.replace( '<filename>', fileName )
                rsp = requests.get( backupUrl )
                logging.info( 'database backup ret: %s', str(rsp.json()))
                result = rsp.json()
                return result
            except Exception as e:
                logging.error( 'Get Database backup info fail! e: %s', e )
                logging.exception( 'db backup' )    
                time.sleep(10)
        return None

    # 备份数据库
    def dbBackup(self, backupToken ):
        if not self.dbIntegrityCheck():
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            time.sleep( 100000000)
            return False

        isOk = self.dbBackupStart( backupToken )
        time.sleep(1)
        if isOk:
            while( True ):
                backupInfo = self.dbBackupInfo( backupToken )
                logging.info( 'backup info: %s', json.dumps(backupInfo) )
                if not backupInfo['threadAlive'] and backupInfo['exist']:
                    return True
                time.sleep( 2 )
            
        
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        logging.error( 'database backup fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
        time.sleep( 100000000)
        return False

if __name__ == "__main__":
    c = AvdbClient()
    #c.dbBackupInfo( 'bk-0' )
    #c.isAVUrlExist( "http://www.javlibrary.com/tw/?v=javlijb6si" )
    status = c.getDBIntegrityCheckStatus()
    c.dbIntegrityCheck()
    
    #c.dbBackup( 'testbackup' )