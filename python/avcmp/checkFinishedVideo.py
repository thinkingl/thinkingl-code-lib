import os
import avlibdb
import time

avlibdb = avlibdb.CAvlibDb()
avlibdb.ConnectDb()

urlFilePath = './finishedVideo.txt'

newFilePath = './newFinishedVideo.txt'
newFile = open(newFilePath, 'w' )

with open( urlFilePath, 'r' ) as urlFile:
    urls = urlFile.read().splitlines()
    for url in urls:
        picName = avlibdb.searchPic( "url", url )
        if picName != None:
            newFile.write( url + '\n' )
            #print( picName + ' - ' + url )
        else:
            print( "url " + url + " not exist!" )
        #time.sleep(5)
        

    