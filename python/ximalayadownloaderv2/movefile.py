import shutil
import psutil
import os
import xmlycfg
import time
import logging
import threading

class MoveFile:
    def startCheck(self):
        threading.Thread(target=self.threadCheckDiskSpace, daemon=True).start()

    def threadCheckDiskSpace(self):
        while( not os.path.isfile( xmlycfg.StopFlagFileName ) ):
            freeSpace = psutil.disk_usage( xmlycfg.getCurTempDir() ).free
            if( freeSpace < xmlycfg.MinDiskSpace ):
                self.moveXMLYFolders()
            time.sleep(1*60)

    def moveXMLYFolders(self):
        nameList = os.listdir( xmlycfg.getCurTempDir() )
        for name in nameList:
            path = os.path.join( xmlycfg.getCurTempDir(), name )
            if self.isXMLYAnchorDir( path ):
                finalPath = os.path.join( xmlycfg.getCurFinalDir(), name)
                try:
                    logging.warning( 'begin to move anchor dir %s to %s', path, finalPath)
                    self.moveTreeForce(path, finalPath)
                    #shutil.move(path, finalPath)
                    logging.warning( 'move anchor dir %s to %s', path, finalPath)
                except:
                    logging.exception( 'error' )
                    logging.error( 'move anchor dir %s to %s fail!!', path, finalPath )

    # 移动文件夹, 同名覆盖.
    # src: d:\abc
    # dst: e:\abc
    def moveTreeForce(self, src, dst):
        if not os.path.isdir( dst ):
            os.mkdir( dst )
            logging.info( 'mkdir %s', dst )
        errorOccur = False
        for name in os.listdir(src):
            srcPath = os.path.join(src, name)
            dstPath = os.path.join(dst, name )
            if os.path.isdir(srcPath):
                self.moveTreeForce( srcPath, dstPath )
                continue
            if os.path.exists( dstPath ):
                os.remove( dstPath )
            try:
                shutil.copy2( srcPath, dstPath )
            except:
                logging.exception( 'error' )
                logging.error( "copy file %s to %s fail!", srcPath, dstPath )
                errorOccur = True
            if os.path.isfile( dstPath ):
                os.remove( srcPath )    # 没有问题的话删除原文件.
            else:
                errorOccur = True
        if not errorOccur and os.path.isdir(src):
            os.rmdir(src)  # 移动完成, 删除原目录.
            logging.info( 'rmdir %s', src)
    
    def isXMLYAnchorDir(self, anchorDirPath ):
        if not os.path.isdir( anchorDirPath):
            return False
        anchorName = os.path.basename( anchorDirPath )
        hasCover = False
        for fileName in os.listdir( anchorDirPath ):
            baseName = fileName.split('.')[0]
            ext = fileName.split('.')[-1]
            if baseName == anchorName and ( ext == 'jpg' or ext == 'jpeg' or ext == 'png'): # 封面是 专辑名.图片文件后缀
                hasCover = True
                break
        if not hasCover:
            return False
        return True

if __name__=="__main__":
    m= MoveFile()
    m.threadCheckDiskSpace()
    #m.startCheck()