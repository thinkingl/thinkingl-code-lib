import hashlib
import os
import time
from shutil import *

class Hashtools(object):
    hashLinkTable = {}


    def __init__(self, allHashInfoFile):
        self.allHashInfoFile = allHashInfoFile
        #self.rootdir = rootdir
        self.loadAllHashlinks()

        return

    def getHashlink(self, localFilePath :str) -> str:
        try:
            with open(localFilePath,'rb') as f:
                sha = hashlib.sha1()
                sha.update(f.read(1024*128))
                BlockHASH = sha.hexdigest()
                f.seek(0,0)
                sha = hashlib.sha1()

                readsize = 1000*1024*1024
                while( True ):
                    b = f.read(readsize)
                    if len(b) == 0:
                        break
                    sha.update(b)
                TotalHASH=sha.hexdigest()

                TotalHASH = TotalHASH.upper()
                BlockHASH = BlockHASH.upper()

                dirname = os.path.basename( os.path.dirname(localFilePath) )
                filename = os.path.basename(localFilePath)
                
                return '115://' + filename+'|'+str(os.path.getsize(localFilePath))+'|'+TotalHASH+'|'+BlockHASH+'|' + dirname
        except Exception as e:
            print(e)
            return
        return ''

    def getFileKeyFromHashlink(self, hashLink:str) ->str:
        # 文件的唯一标识:  目录 + 文件名 + 文件大小
        items = hashLink.split( '|' )
        dirname = items[4]
        filename = items[0].replace('115://','')
        filesize = items[1]
        return dirname + '-' + filename +'-'+filesize

    def getFileKeyFromLocal(self, filePath:str) -> str:
        # 文件的唯一标识:  目录 + 文件名 + 文件大小
        fullpath = os.path.abspath( filePath )
        filename = os.path.basename( fullpath )
        dirname = os.path.basename( os.path.dirname(fullpath) )
        filesize = str(os.path.getsize(filePath))
        return dirname + '-' + filename +'-'+filesize

    def loadAllHashlinks(self):
        if not os.path.isfile( self.allHashInfoFile ):
            print( 'all hashlink file ', self.allHashInfoFile, " not exist!" )
            return
        for line in open(self.allHashInfoFile, encoding='utf-8').readlines():
            hashLInk = line.strip()
            fileKey = self.getFileKeyFromHashlink( hashLInk )
            self.hashLinkTable[ fileKey ] = hashLInk


    def saveHashlink(self, hashLink:str):
        fileKey = self.getFileKeyFromHashlink( hashLink )
        if not fileKey in self.hashLinkTable:
            print( "save hashlink: ", hashLink )
            with open( self.allHashInfoFile, 'a', encoding='utf-8' ) as f:
                f.write( hashLink + '\n' )
                f.close()
            self.hashLinkTable[fileKey] = hashLink
        return

    def isFileExclude(self, filePath:str) -> bool:
        extName = filePath.split('.')[-1]
        if extName == 'cfg':
            #print( "file ", filePath, " is exclude by ext name cfg!" )
            return True
        if extName.startswith( '115chrome' ):
            #print( "file ", filePath, " is exclude by ext name 115chrome ! " )
            return True
        filename = os.path.basename( filePath )
        if os.path.getsize(filePath) == 0 and filename.startswith( '.' ):
            #print( "file ", filePath, " is exclude by 0 size and base name start with .! " )
            return True
        return False
    
    def hashDir(self,dirPath):
        for root, dirs, filenames in os.walk( dirPath ):
            for filename in filenames:
                filePath = os.path.join( root, filename )
                if self.isFileExclude( filePath ):
                    continue
                fileKey = self.getFileKeyFromLocal( filePath )
                if fileKey in self.hashLinkTable:
                    continue
                else:
                    hashlink = self.getHashlink(filePath)
                    self.saveHashlink( hashlink )
        return


    def hashDirContinued(self, rootDir, backupDir):
        while( True ):
            oldSize = os.path.getsize( self.allHashInfoFile )
            self.hashDir( rootDir )
            newSize = os.path.getsize( self.allHashInfoFile )
            if oldSize != newSize:
                self.backupFile(self.allHashInfoFile, backupDir, 100)
            time.sleep(60)

    def backupFile(self, filePath, backDir, maxNum):
        baseName = os.path.basename( filePath )
        newName = str(int(time.time())) + '-' + baseName
        newPath = os.path.join( backDir, newName )
        copy2(filePath, newPath)
        fileList = os.listdir(backDir)
        if len(fileList) > maxNum:
            os.remove(fileList[0])
        return


if __name__ == '__main__':
    rootDir = 'Z:/Downloads'
    rootDir = 'x:/Downloads'
    allHashInfoFile = 'E:/all115hashlinks.txt'
    backupDir = 'f:/all115hashlinksbackup'
    hashTool = Hashtools(allHashInfoFile)
    hashTool.hashDirContinued( rootDir, backupDir )
