import os
import sys

dir = './'
if len( sys.argv ) > 1:
    dir = sys.argv[1]

fileList = os.listdir( dir )

indexedFiles = list()
for fileName in fileList:
    filePath = os.path.join( dir, fileName )
    if os.path.isfile( filePath ):
        try:
            curIndex = int( fileName.split('-')[0] )
            indexedFiles.append( (curIndex, fileName) )
        except:
            pass

indexedFiles.sort()

minIndex = indexedFiles[0][0]
maxIndex = indexedFiles[-1][0]

for index, fileName in indexedFiles:
    reversedIndex = maxIndex - index + 1
    print( 'change index ', index, ' to ', reversedIndex )
    newFileName = fileName.replace( '%d-'%index, '%d-'%reversedIndex )
    print( 'rename file ', fileName, ' to ', newFileName )
    filePath = os.path.join( dir, fileName )
    newFilePath = os.path.join( dir, newFileName )
    os.rename( filePath, newFilePath )