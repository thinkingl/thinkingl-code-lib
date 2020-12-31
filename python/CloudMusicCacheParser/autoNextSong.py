import os
import sys
import time
import keyboard
import logging
import random


songStorageDir = "X:/music/cachedownload"




def initLogging():
    sys.stdout.reconfigure(encoding='utf-8')
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式
    logFileName = time.strftime('autoNextSong-%Y%m%d.log',time.localtime())
    
    fh = logging.FileHandler(logFileName)
    fh.setLevel(logging.DEBUG)
    #fh.setFormatter(formatter)

    # 使用StreamHandler输出到屏幕
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    #ch.setFormatter(formatter)
    #logging.addHandler( fh )
    #logging.addHandler( ch )

    logging.basicConfig(level=logging.INFO,
        format   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
        datefmt  = '%Y-%m-%d %A %H:%M:%S',                                     # 时间
        #filename = logFileName,                # log文件名
        #filemode = 'w',
        handlers = [fh, ch]
    )

def getCacheDir():
    cloudMusicCacheDirFile = os.environ['LOCALAPPDATA'] + "\\Netease\\CloudMusic\\cache_path"
    cachePathFile = open( cloudMusicCacheDirFile, "rb")
    cloudMusicCacheDir = cachePathFile.read().decode( "utf-16")
    cachePathFile.close()
    return cloudMusicCacheDir

def getDirSize(dir):
    size = 0
    for root, dirs, files in os.walk(dir):
        try:
            size += sum([os.path.getsize(os.path.join(root, name)) for name in files])
        except:
            pass
    return size

if __name__=="__main__":
    initLogging()

    minWaitTime = 5;
    maxWaitTime = 5*60
    if len( sys.argv ) > 2:
        minWaitTime = int(sys.argv[1])
        maxWaitTime = int(sys.argv[2])
    if minWaitTime < 2:
        minWaitTime = 2
    if maxWaitTime < 6:
        maxWaitTime = 6

    # 种子,否则随机是固定的.
    random.seed( time.time() )

    logging.info( 'wait time is %d - %d', minWaitTime, maxWaitTime )

    cacheDir = getCacheDir()
    sizeNotChangeTime = 0
    lastSize = 0
    startTime = time.time()
    songCount = 0
    sizeChangedFromLastSong = False
    while( True ):
        time.sleep(1)

        curSize = getDirSize( cacheDir )
        if lastSize == curSize:
            sizeNotChangeTime += 1
        else:
            sizeNotChangeTime = 0
            lastSize = curSize
            sizeChangedFromLastSong = True
            print( 'size changed to ' + str(curSize))
        
        if sizeNotChangeTime > random.randrange(minWaitTime, maxWaitTime):
            logging.info( 'dir size [%d] not change for [%d] times, next song!', curSize, sizeNotChangeTime )
            keyboard.press_and_release( 'ctrl+shift+num add' )
            sizeNotChangeTime = 0
            
            if sizeChangedFromLastSong :
                sizeChangedFromLastSong = False
                songCount += 1
                curTime = time.time()
                costTime = curTime - startTime
                hour = int(costTime / 3600)
                min = int((costTime % 3600) / 60 )
                sec = int((costTime % 60))
                averageMin = int(songCount * 60 / costTime)
                averageHour = int(songCount * 3600 / costTime)
                logging.info( '%d songs for %d hour %d min %d seconds(%d second total). average: %d / min;  %d / hour', int(songCount), hour, min, sec, int(costTime), averageMin, averageHour )
            else:
                logging.error( 'may cloud music client error or hot key fail!' )


    