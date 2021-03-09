import os
import time
import sys
import numpy
import random


def human_readable(plain_size):
    plain_size = float(plain_size)
    if plain_size <= 1024:
        return str( round(plain_size, 2)) + 'B'
    if plain_size <= 1024 * 1024:
        return str( round(plain_size / 1024, 2)) + 'K'
    if plain_size <= 1024 * 1024 * 1024:
        return str( round(plain_size / 1024 / 1024, 2)) + 'M'
    if plain_size <= 1024 * 1024 * 1024 *1024:
        return str( round(plain_size / 1024 / 1024 / 1024, 2)) + 'G'


def writeTest(testDir, dataList):

    total = 0
    startTime = time.time()

    try:
        os.makedirs( testDir )
    except:
        pass

    try:
        for root, dirs, files in os.walk( testDir ):
            for file in files:
                path = os.path.join( root, file )
                os.remove( path )
    except:
        pass

    try:
        while True:
            testFileName = 'diskfill-' + str(int(time.time())) + '.dat'
            testFilePath = os.path.join( testDir, testFileName )
            writeLen = 0
            with open( testFilePath, 'wb') as f:
                while writeLen < testFileSize:
                    randIndex = random.randint(0, len(dataList)-1)
                    data = dataList[randIndex]
                    at = time.time()
                    f.write(data)
                    bt = time.time()
                    writeLen = writeLen + len(data)
                    total = total + len(data)

                    curSpeed = human_readable( len(data)/(bt-at) )
                    totalSpeed = human_readable( total / (bt - startTime) )
                    totalSize = human_readable( total )
                    print( 'total write ', totalSize, 'cur write speed ', curSpeed, ' total write speed ', totalSpeed  )

    except:
        print( 'write full!' )

    endTime = time.time()
    totalSpeed = human_readable( total / (endTime - startTime) )
    print( 'total write speed ', totalSpeed  )


def readTest(testDir):
    # 读所有文件.
    startTime = time.time()
    total = 0
    for root, dirs, files in os.walk( testDir ):
        for file in files:
            path = os.path.join( root, file )
            with open( path, 'rb' ) as f:
                while True:
                    at = time.time()
                    data = f.read( 100*1024**2 )
                    bt = time.time()
                    if len( data ) == 0:
                        break

                    total = total + len(data)
                    curSpeed = human_readable( len(data)/(bt-at) )
                    totalSpeed = human_readable( total / (bt - startTime) )
                    print( 'cur read speed ', curSpeed, ' total read speed ', totalSpeed  )

    endTime = time.time()
    totalSpeed = human_readable( total / (endTime - startTime) )
    print( 'total read speed ', totalSpeed  )

random.seed( time.time()*1984 )
dataList = list()
for i in range(0,30):
    l= numpy.random.randint(low=-2147483648, high=0x7FFFFFFF, size=1024**2*10)
    dataList.append( l.tobytes() )

testDir = sys.argv[1]
testFileSize = 1024**3
testTimes = 1
if len( sys.argv ) > 2:
    testTimes = int(sys.argv[2])

print( 'test dir: ', testDir, ' test times: ', testTimes )

for i in range( 0, testTimes ):
    print( 'test - ' , i, ' start!' )
    writeTest(testDir, dataList)
    readTest(testDir)
    print( 'test - ', i, ' finished!')
