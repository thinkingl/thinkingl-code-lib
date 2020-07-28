#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
import sqlite3
import time
import avlibcfg
import logging
import traceback
import json
import base64
import shutil
import threading

class CAvlibDb:
    dbFilePath=os.path.join( avlibcfg.DbDir, avlibcfg.DbFileName )
    dbConnect=None
    
    # ???????????
    dbWriteBackupLock = threading.Lock()

    def ConnectDb(self):
        self.dbConnect = sqlite3.connect(self.dbFilePath)
        return self.dbConnect
    
    def CloseDb(self):
        self.dbConnect.close()
        self.dbConnect = None

    def InitDbTable( self ):
        if self.dbConnect == None:
            self.ConnectDb()
        with self.dbWriteBackupLock:
            cur=self.dbConnect.cursor()
            sqlCreatePic='Create Table  IF NOT EXISTS Pic (FileName TEXT PRIMARY KEY NOT NULL);'
            cur.execute(sqlCreatePic)

            sqlCreateAttr='Create Table  IF NOT EXISTS Attr(Type TEXT NOT NULL, Name TEXT NOT NULL, Score BIGINT NOT NULL, PRIMARY KEY(Type,Name));'
            cur.execute(sqlCreateAttr)

            sqlCreatePicAttr='Create Table IF NOT EXISTS PicAttr(PicId BIGINT NOT NULL, AttrId BIGINT NOT NULL, PRIMARY KEY(PicId,AttrId));'
            cur.execute(sqlCreatePicAttr)
            cur.execute( 'Create Index IF NOT EXISTS indexAttr ON PicAttr(AttrId);')


            sqlCreatePicData='Create Table IF NOT EXISTS PicData(PicId BIGINT PRIMARY KEY NOT NULL, Data BLOB NOT NULL);'
            cur.execute(sqlCreatePicData)

            cur.close()

    def Pic2Db( self, fileName, filePath, dbCursor ):
        with self.dbWriteBackupLock:
            dbCursor.execute( 'Insert or ignore into Pic values( ? );', (fileName,) )   
            f=open(filePath,"rb")
            dbCursor.execute( 'Replace into PicData Select rowId, ? from Pic Where FileName = ?;', (f.read(),fileName) )
            f.close()

    def picBase642Db(self, fileName, picDataBase64, dbCursor ):
        with self.dbWriteBackupLock:
            dbCursor.execute( 'Insert or ignore into Pic values( ? );', (fileName,) )   
            picData = base64.decodebytes( picDataBase64.encode('utf-8') )
            dbCursor.execute( 'Replace into PicData Select rowId, ? from Pic Where FileName = ?;', (picData,fileName) )

    def PicAttr2Db( self, fileName, attrType, attrName, dbCursor ):
        with self.dbWriteBackupLock:
            dbCursor.execute( 'Insert or ignore into Attr values(?, ?, ? );', (attrType, attrName, 0))
            dbCursor.execute( 'Insert or ignore into PicAttr select Pic.rowId, Attr.rowId from Pic join Attr where Pic.Filename = ? and Attr.Type = ? and Attr.Name = ?;', (fileName, attrType, attrName) )

    def json2Db( self, obj, dbCursor ):
        #添加年.
        with self.dbWriteBackupLock:
            obj['year'] = obj['date'][0:4]
            fileName = obj['imgFileName']
            dbCursor.execute( 'Insert or ignore into Pic values( ? );', (fileName,) )   

            for key in obj:
                attrType = str(key)
                attrNameArray = obj[key].split()
                if len(attrNameArray) > 1:
                    attrNameArray.append( obj[key] )
                for attrName in attrNameArray:
                    dbCursor.execute( 'Insert or ignore into Attr values(?, ?, ? );', (attrType, attrName, 0))
                    dbCursor.execute( 'Insert or ignore into PicAttr select Pic.rowId, Attr.rowId from Pic join Attr where Pic.Filename = ? and Attr.Type = ? and Attr.Name = ?;', (fileName, attrType, attrName) )

    def jsonFile2Db( self, fileName, filePath, dbCursor ):
        with self.dbWriteBackupLock:
            jsonPath = filePath[0:-4] + '.json'
            if not os.path.isfile( jsonPath ):
                print( 'Json %s is not exist!'%jsonPath)
                return
            f=open(jsonPath,'rb')
            try:
                obj = json.load(f)
            except:
                print( 'Load json %s fail!'%jsonPath)
                return
            f.close()
            self.json2Db( obj, dbCursor )
        
    def QueryPic2PicDic(self):
        allPic = {}
        for row in self.dbConnect.execute( 'select rowId,* from Pic;' ):
            allPic[row[0]] = row[1]
        return allPic

    def QueryAttr2AttrDic(self):
        allAttr = {}
        for row in self.dbConnect.execute( 'select rowId,* from Attr;'):
            allAttr[row[0]] = list(row)
        return allAttr    
    
    def QueryPicAttr2Dic(self):
        picAttr = {}
        for row in self.dbConnect.execute( 'select * from PicAttr;'):
            if not row[0] in picAttr:
                picAttr[row[0]] = []
            picAttr[row[0]].append(row[1])
        return picAttr

    def GetPicData(self, picId):
        for row in self.dbConnect.execute( 'select Data from PicData where PicId = ?;', (picId,)):
            return row[0]
    
    def GetPicDataByName(self, fileName):
        for row in self.dbConnect.execute( 'select Data from PicData join Pic where PicData.PicId = Pic.rowId and Pic.fileName = ?;', (fileName,)):
            return row[0]

    def GetPicInfoJson(self, fileName):
        pic = {}
        score = 0
        for row in self.dbConnect.execute( 'select Attr.Type,Attr.Name,Attr.Score from Pic join PicAttr, Attr where PicAttr.picId = Pic.rowId and PicAttr.attrId = Attr.rowId and Pic.FileName = ?;', (fileName,)):
            type = row[0]
            name = row[1]
            score += int(row[2])
            if( type in pic ):
                if( len(name) > len(pic[type])):
                    pic[type] = name
            else:
                pic[type] = name
        pic['score'] = score
        return pic
    
    def GetRandomPic(self, num):
        picNameList = list()
        for row in self.dbConnect.execute( 'select * from Pic order by random() limit ?;', (num,)):
            picNameList.append( row[0] )
        return picNameList
    
    def UpdatePicAttrScore(self, fileName, scoreOffset, commit2Db):
        with self.dbWriteBackupLock:
            self.dbConnect.execute( 'update Attr set score = score+ ? where rowId in '
                '(select Attr.rowId from Attr join PicAttr,Pic where PicAttr.attrId = Attr.rowId and PicAttr.picId = Pic.rowId and Pic.FileName= ? );'
                , (scoreOffset, fileName ) )
            if(commit2Db):
                self.dbConnect.commit()
    
    def PicCompare(self, betterFileName, worseFileName):
        with self.dbWriteBackupLock:
            ret = {}
            ret['error'] = 'ok'
            try:
                self.UpdatePicAttrScore(betterFileName, 1, False)
                self.UpdatePicAttrScore(worseFileName,-1, False)
                self.dbConnect.commit()
            except:
                self.dbConnect.rollback()
                logging.exception("message")
                ret['error'] = 'Fail'
                ret['message'] = traceback.format_exc()
            return ret


    def TopPics(self, begin, num):
        picList = list()
        for row in self.dbConnect.execute('select Pic.FileName from Pic join Attr,PicAttr '
            'where Pic.rowId = PicAttr.picId and Attr.rowId=PicAttr.attrId  group by Pic.rowId order by sum(-Attr.score) limit ?,? ;', (begin,num)):
            picList.append(row[0])
        return picList

    # ????? ???????
    def searchPic( self, attrType, attrName ):
        # select fileName from Pic join PicAttr, Attr on Pic.rowId = PicAttr.PicId and PicAttr.AttrId = Attr.rowId  where Type = 'url' and Name = 'http://www.javlibrary.com/tw/?v=javli4324a';
        for row in self.dbConnect.execute( 'select fileName from Pic join PicAttr, Attr on Pic.rowId = PicAttr.PicId and PicAttr.AttrId = Attr.rowId  '
                'where Type = ? and Name = ?;', (attrType, attrName) ):
                return row[0]
        return None
    
    # ??????????
    def beginTransaction(self):
        cur = self.dbConnect.cursor()
        cur.execute( 'Begin transaction;' )
        return cur
    def commitTransacton(self, cur):
        with self.dbWriteBackupLock:
            cur.execute( 'Commit transaction;')
            cur.close()
    
    # ?????
    def backup( self, fileName ):
        with self.dbWriteBackupLock:
            backupPath = os.path.join( avlibcfg.DbDir, fileName )
            if os.path.isfile( backupPath ):
                os.remove( backupPath )
            shutil.copyfile( self.dbFilePath, backupPath )
            
    # ?????????
    def integrityCheck(self):
        with self.dbWriteBackupLock:
            msg = ''
            for row in self.dbConnect.execute( 'pragma integrity_check;' ):
                msg += row[0]
            return msg

    def ThreadFile2Db(self):
        self.InitDbTable( )
        cur = self.dbConnect.cursor()
        while True:
            cur.execute( 'Begin transaction;' )
            fileDirPath = avlibcfg.BaseDir+avlibcfg.FileDir
            curNum=0

            for root,dirs,files in os.walk(fileDirPath):
                for fileName in files:
                    if( curNum % 10000 == 0 ):
                        print( 'cur files num:%d'%curNum )
                    curNum += 1
                    ext = os.path.splitext(fileName)[1]
                    if( ext == '.jpg'):
                        self.Pic2Db(fileName, os.path.join(root,fileName), cur)
                        self.jsonFile2Db(fileName, os.path.join(root,fileName),cur)


            # fileList = os.listdir(fileDirPath)
            # totalNum = len(fileList)
            # percent = ''
            # for fileName in fileList:
            #     curNum += 1
            #     newPercent = '{:.0%}'.format( curNum / totalNum )
            #     if( newPercent != percent ):
            #         print( 'Cur progress: %s'%newPercent )
            #         percent = newPercent
            #         #conn.commit()
            #     ext = os.path.splitext(fileName)[1]
            #     if( ext == '.jpg'):
            #         Pic2Db(fileName, cur)
            #         Json2Db(fileName, cur)
            cur.execute( 'Commit transaction;')
            time.sleep(1*60)
            break
        cur.close()

if __name__ == "__main__":
    jsonFile = 'D:/999-temp/javlib/byDate/2011-06/2011-06-04 GAR-233.json';
    obj = json.load( open( jsonFile, 'rb') )
    db = CAvlibDb()
    db.ConnectDb()
    db.InitDbTable()
    cur = db.beginTransaction()
    db.json2Db( obj, cur )
    db.commitTransacton(cur)
    db.backup( "backuptest.db" )
    db.CloseDb()


    
