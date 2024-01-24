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
    if plain_size <= 1024 * 1024 * 1024 *1024 * 1024:
        return str( round(plain_size / 1024 / 1024 / 1024 / 1024, 2)) + 'T'
    return str( round(plain_size / 1024 / 1024 / 1024 / 1024, 2)) + 'T'


def writeTest(testDir, dataList):

    total = 0
    startTime = time.time()

    try:
        os.makedirs( testDir )
    except:
        pass

    try:
        while True:
            testFileName = 'diskfill-' + str(int(time.time())) + '.dat'
            testFilePath = os.path.join( testDir, testFileName )
            writeLen = 0

            at = time.time()

            with open( testFilePath, 'wb') as f:
                while writeLen < testFileSize:
                    randIndex = random.randint(0, len(dataList)-1)
                    data = dataList[randIndex]
                    f.write(data)
                    writeLen = writeLen + len(data)
                    total = total + len(data)
            bt = time.time()
            curSpeed = human_readable( writeLen/(bt-at) )
            totalSpeed = human_readable( total / (bt - startTime) )
            totalSize = human_readable( total )
            print( 'total write ', totalSize, ' cur write speed ', curSpeed, 'B/s total write speed ', totalSpeed, 'B/s'  )

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
            readLen = 0
            at = time.time()

            with open( path, 'rb' ) as f:
                while True:
                    data = f.read( 100*1024**2 )
                    if len( data ) == 0:
                        break

                    total = total + len(data)
                    readLen = readLen + len(data)
            bt = time.time()
            curSpeed = human_readable( readLen/(bt-at) )
            totalSpeed = human_readable( total / (bt - startTime) )
            totalLen = human_readable( total )
            print( 'total read ', totalLen, ' cur read speed ', curSpeed, 'B/s total read speed ', totalSpeed, 'B/s'  )

    endTime = time.time()
    totalSpeed = human_readable( total / (endTime - startTime) )
    print( 'total read speed ', totalSpeed  )

random.seed( time.time()*1984 )
dataList = list()
for i in range(0,20):
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
    try:
        for root, dirs, files in os.walk( testDir ):
            for file in files:
                path = os.path.join( root, file )
                os.remove( path )
    except:
        pass
    print( 'test - ', i, ' finished!')
