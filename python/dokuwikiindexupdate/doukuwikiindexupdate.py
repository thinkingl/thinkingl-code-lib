import requests
import os
import time

pageDir = '/var/www/html/dokuwiki/data/pages/av'
fileList = os.listdir( pageDir )

namespace = 'av'

for fileName in fileList:
    if fileName.split('.')[-1] != 'txt':
        print( 'invalid file ', fileName )
        continue
    wikiName = fileName.replace( '.txt', '' )
    indexUrl = 'http://192.168.84.8/dokuwiki/lib/exe/indexer.php?id=%s%%3A%s&%d'%( namespace, wikiName, int(time.time()) )
    print( 'update ', wikiName, ' by url: ', indexUrl )
    requests.get( indexUrl )
