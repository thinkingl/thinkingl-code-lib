import os
import random
import time

def sizeof_fmt(num, suffix='B'):
    for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
        if abs(num) < 1024.0:
            return "%3.2f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.2f%s%s" % (num, 'Yi', suffix)



testDir = 'Z:/disk-ssd/tmp/disktest'
if not os.path.isdir( testDir ):
    testDir = '/share/disk-ssd/tmp/disktest'
if not os.path.isdir( testDir ):
    testDir = './data'
    
testTotalWrite = 10 * 1024 *1024*1024*1024 # T

# 总共100G
testFileSize = 100*1024*1024 #100M
testFileNum = 1000

dataFilePath = './data.7z'

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
def checkSMART( totalWrite ):
    global lastCheckSMARTSize
    global checkSMARTSize
    if totalWrite - lastCheckSMARTSize < checkSMARTSize:
        return
    lastCheckSMARTSize = totalWrite

    smartInfo = os.popen( 'smartctl -a /dev/sda' ).read()
    if smartInfo is not lastSMARTInfo:
        dateStr = time.strftime("%Y-%m-%d", time.localtime()) 
        os.system( 'echo `date` >> smartinfo-' + dateStr + '.log')
        os.system( 'echo ' + 'Write data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) + ' >> smartinfo-' + dateStr + '.log' )
        os.system( 'smartctl -a /dev/sda >> smartinfo-' + dateStr + '.log' )

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
        while( totalLen < testFileSize ):
            len = f.write( data )
            totalLen += len
        f.close()
        totalWrite += totalLen
        open(totalWriteSizePath, 'w').write( str(totalWrite ))
        print( 'Write data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) )
        checkSMART( totalWrite )

    # 读一遍
    for fileIndex in range(0,testFileNum):
        fileName = str(fileIndex)
        filePath = os.path.join( dataDir, fileName)
        f = open( filePath, 'rb')
        data = f.read()
        f.close()
        

    print( 'Write data:' + str(totalWrite) + 'B / ' + sizeof_fmt(totalWrite) )