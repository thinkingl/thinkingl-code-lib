import os

pageDir = '/var/www/html/dokuwiki/data/pages'

mvToDir = '/var/www/html/dokuwiki/data/pages/av'

fileList = os.listdir( pageDir )

avPageListFile = open( 'avlist.txt', 'w', encoding='utf-8')

for fileName in fileList:
    if fileName.split('.')[-1] != 'txt':
        print( 'invalid file ', fileName )
        continue
    filePath = os.path.join( pageDir, fileName )


    with open( filePath, 'r' ) as f:
        content = f.read()
        f.close()
        if content.find( '[[http://www.javlibrary.com/tw/?v=jav' ) != -1:
            #print( 'file ', fileName, ' is av page file!!!!!!!!!!!!' )
            avPageListFile.write( fileName )
            avPageListFile.write( '\n' )

            mvToPath = os.path.join( mvToDir, fileName )
            
            os.rename( filePath, mvToPath )
            print( 'mv to ', mvToPath )
        else:
            #print( 'file ', fileName, ' is not av page file!')
            pass
    
avPageListFile.close()