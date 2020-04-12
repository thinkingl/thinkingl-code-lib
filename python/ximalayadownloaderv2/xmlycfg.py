import os
import logging

MinDiskSpace = 1*1024*1024*1024 # 1G 最小的磁盘空间, 剩余磁盘空间不足后停止下载, 开始移动文件.
TempDir = ['D:/999-temp/xmly','/share/disk-ssd/tmp/xmlytmp','/share/disk-ssd/xmlydownload']
FinalDir = ['D:/999-temp/finalxmly','/share/disk-wd12t/1-audiobook/xmly']
DBPath = ['F:/tmp/xmly/xmly.db', '/share/disk-ssd/xmly/xmly.db']
DownloadThreadNum = 5
StopFlagFileName = 'stop.flag'
CurTempDir = ''
CurFinalDir = ''
CurDBPath = ''
UseRemobeDatabase = True
XMLYDatabaseServiceUrl = "http://192.168.84.8:5000"


def getCurTempDir():
    global CurTempDir
    global TempDir
    if len(CurTempDir) > 0:
        return CurTempDir
    for dir in TempDir:
        if os.path.isdir( dir ):
            CurTempDir = dir
            logging.warning('Use Temp dir %s',CurTempDir)
            break
    return CurTempDir

def getCurFinalDir():
    global CurFinalDir
    global FinalDir
    if len(CurFinalDir) > 0:
        return CurFinalDir
    for dir in FinalDir:
        if os.path.isdir( dir ):
            CurFinalDir = dir
            logging.warning('Use final dir %s',CurFinalDir)
            return CurFinalDir
    logging.error("Can't find valid final dir!!")
    return CurFinalDir

def getCurDBPath():
    global CurDBPath
    if len(CurDBPath) > 0:
        return CurDBPath
    for path in DBPath:
        if os.path.isfile( path ):
            CurDBPath = path
            logging.warning('Use db path %s', CurDBPath )
            return CurDBPath
    logging.error("Can't find db file path!")
    return CurDBPath


if __name__=="__main__":
    tmpDir = getCurTempDir()
    finalDir = getCurFinalDir()
