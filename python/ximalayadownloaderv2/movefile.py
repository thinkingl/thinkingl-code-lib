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

    def removeEmptyDirs(self):
        for root, dirs, files in os.walk(  xmlycfg.getCurTempDir() ):
            if not os.listdir(root):
                os.rmdir(root)
                logging.warning( 'remove empty dir:' + root )


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
        self.removeEmptyDirs()

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

            # 如果文件是最近修改的, 忽略. 因为可能文件正在写入中.
            # 暂定1分钟.
            if (time.time() - os.stat( srcPath ).st_mtime) < 60:
                logging.info( 'file ' + srcPath + ' modified recently!' )
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
            ext = ext.lower()
            if baseName == anchorName and ( ext == 'jpg' or ext == 'jpeg' or ext == 'png'): # 封面是 专辑名.图片文件后缀
                hasCover = True
                break
        if hasCover:
            return True

        # 是不是有音频文件 .m4a
        for root, dirs, files in os.walk(  anchorDirPath ):
            for f in files:
                if os.path.splitext( f )[1] == '.m4a':
                    return True
        
        return False

if __name__=="__main__":
    m= MoveFile()
    #m.threadCheckDiskSpace()
    #m.startCheck()
    m.moveXMLYFolders()