import os
import random
import time
import disktestcfg

def sizeof_fmt(num, suffix='B'):
    for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
        if abs(num) < 1024.0:
            return "%3.2f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.2f%s%s" % (num, 'Yi', suffix)



testDir = disktestcfg.testDir
    
testTotalWrite = disktestcfg.testTotalWrite # 10 * 1024 *1024*1024*1024 # T

# 总共100G
testFileSize = disktestcfg.testFileSize # 100*1024*1024 #100M
testFileNum = disktestcfg.testFileNum # 1000

dataFilePath = disktestcfg.dataFilePath # './data.7z'

testData = open(dataFilePath,'rb').read()

totalWriteSizePath = os.path.join( testDir, 'totalWriteSize' )

totalWrite = 0
try:
    totalWrite = int( open(totalWriteSizePath).readline() )
except :
    pass

lastSMARTInfo = ''
checkSMARTSize = 10 * 1024 * 1024 * 1024 # 每10G检测一次SMART
lastCheckSMARTSize = 0
def checkSMART( totalWrite, extLog ):
    global lastCheckSMARTSize
    global checkSMARTSize
    global lastSMARTInfo
    if totalWrite - lastCheckSMARTSize < checkSMARTSize:
        return
    lastCheckSMARTSize = totalWrite

    smartCmd = 'smartctl -a ' + disktestcfg.diskName
    smartInfo = os.popen( smartCmd ).read()
    if smartInfo is not lastSMARTInfo:
        lastSMARTInfo = smartInfo
        dateStr = time.strftime("%Y-%m-%d", time.localtime()) 
        dateTimeStr = time.strftime( "%Y-%m-%d %H:%M:%S", time.localtime() )
        os.system( 'echo "'+ dateTimeStr + '" >> smartinfo-' + dateStr + '.log')
        os.system( 'echo ' + 'Write data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) + ' >> smartinfo-' + dateStr + '.log' )
        os.system( 'echo "' + extLog + '" >> smartinfo-' + dateStr + '.log' )
        os.system( smartCmd + ' >> smartinfo-' + dateStr + '.log' )
    return smartInfo

#totalWrite = 0
dataDir = os.path.join( testDir, 'data')
while totalWrite < testTotalWrite:

    for fileName in os.listdir( dataDir ):
        filePath = os.path.join( dataDir, fileName )
        os.remove( filePath )

    #写一遍
    for fileIndex in range(0,testFileNum):
        fileName = str(fileIndex)
        filePath = os.path.join( dataDir, fileName)
        f = open( filePath, 'wb')
        
        data = testData[0:testFileSize]

        totalLen = 0
        beginWriteTime = time.time()
        while( totalLen < testFileSize ):
            totalLen += f.write( data )
        f.close()
        endWriteTime = time.time()
        totalWrite += totalLen
        open(totalWriteSizePath, 'w').write( str(totalWrite ))
        
        timeSpan = endWriteTime - beginWriteTime
        writeSpeed = totalLen / (1024*1024) / timeSpan
        print( time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())  + '\tWrite data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) + ' speed: ' + str(writeSpeed) + ' MB/S' )
        checkSMART( totalWrite, time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())  + '\tWrite data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) + ' speed: ' + str(writeSpeed) + ' MB/S' )

    # 读一遍
    readTotalLen = 0
    for fileIndex in range(0,testFileNum):
        fileName = str(fileIndex)
        filePath = os.path.join( dataDir, fileName)

        beginReadTime = time.time()

        f = open( filePath, 'rb')
        data1 = f.read()
        
        endReadTime = time.time()
        
        readTotalLen += len(data1)
        timeSpan = endReadTime - beginReadTime
        readSpeed = readTotalLen / (1024*1024) / timeSpan
        print( time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())  + '\tRead data:' + str(readTotalLen) + 'B / ' + sizeof_fmt(readTotalLen) + ' speed: ' + str(readSpeed) + ' MB/S' )
        
        f.close()
        

    print( time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())  + '\tWrite data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) )