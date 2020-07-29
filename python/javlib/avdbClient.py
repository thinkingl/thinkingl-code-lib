import requests
import base64
import logging
import time
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

    # 检查数据库是否损坏。
    def dbIntegrityCheck(self):
        logging.info( "-------------------begin database integrity check!!!-----------------" )
        for i in range( 0, 10 ):
            try:
                # 检测数据库是否损坏。
                url = self.serverBaseUrl + 'db/integritycheck'

                rsp = requests.get( url )
                logging.info( 'database integrity check ret: %s', str(rsp.json()) )
                result = rsp.json()['result']
                if result != 'ok':
                    logging.error( "database is corrupted!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    logging.error( "database is corrupted!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    logging.error( "database is corrupted!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    logging.error( "database is corrupted!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    logging.error( "database is corrupted!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
                    time.sleep(100000000)               
                else:
                    logging.info( "-------------------database integrity check OK!!!-----------------" )
                    return True
            except Exception as e:
                logging.error( 'Database IntegrityCheck fail! e: %s', e )
                logging.exception( 'db IntegrityCheck' )    
                time.sleep(10)
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            time.sleep( 100000000)
            return False
    
    # 备份数据库
    def dbBackup(self, backupToken ):
        if not self.dbIntegrityCheck():
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            logging.error( 'database IntegrityCheck fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            time.sleep( 100000000)
            return False

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

if __name__ == "__main__":
    c = AvdbClient()
    c.isAVUrlExist( "http://www.javlibrary.com/tw/?v=javlijb6si" )
    c.dbBackup( 'testbackup' )