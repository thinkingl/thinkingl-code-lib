import sys
import os
import time
import hashlib
import hashlink
import requests

def isFileExclude(filePath:str) -> bool:
    extName = filePath.split('.')[-1]
    if extName == 'cfg':
        #print( "file ", filePath, " is exclude by ext name cfg!" )
        return True
    if extName.startswith( '115chrome' ):
        #print( "file ", filePath, " is exclude by ext name 115chrome ! " )
        return True
    filename = os.path.basename( filePath )
    if os.path.getsize(filePath) == 0 and filename.startswith( '.' ):
        #print( "file ", filePath, " is exclude by 0 size and base name start with .! " )
        return True
    return False

def getHashlink(localFilePath :str) -> str:
    try:
        with open(localFilePath,'rb') as f:
            sha = hashlib.sha1()
            sha.update(f.read(1024*128))
            BlockHASH = sha.hexdigest()
            f.seek(0,0)
            sha = hashlib.sha1()

            readsize = 1000*1024*1024
            while( True ):
                b = f.read(readsize)
                if len(b) == 0:
                    break
                sha.update(b)
            TotalHASH=sha.hexdigest()

            TotalHASH = TotalHASH.upper()
            BlockHASH = BlockHASH.upper()

            dirname = os.path.basename( os.path.dirname(localFilePath) )
            filename = os.path.basename(localFilePath)
            filesize = os.path.getsize( localFilePath )
            hash = { 'fileName':filename, 'dirName':dirname, 'fileSize':int(filesize), 'totalHash':TotalHASH, 'blockHash':BlockHASH}
            hash['hashlink'] = hashlink.Hashlink(hash).getHashlink()
            return hash
    except Exception as e:
        print(e)
        return None
    return None



def autoScanHash(rootDirs:list):
    for rootDir in rootDirs:
        print( 'start scan ', rootDir )
        for root, dirs, filenames in os.walk( rootDir ):
            for filename in filenames:
                filePath = os.path.join( root, filename )
                print( 'filePath: ', filePath)
                if isFileExclude( filePath ):
                    continue
                dirname = os.path.basename( root )
                url = 'http://localhost:5003/hashlink/' + dirname + '/' + filename
                rsp = requests.get(url).json()
                print( 'get rsp from server: ', rsp)
                if not rsp.get('error') == 0:
                    hash = getHashlink( filePath )
                    print( 'getHash: ', hash)
                    url = 'http://localhost:5003/hashlink'
                    requests.put( url=url, json=hash)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print( 'rootdir as arg!' )
        exit(1)

    rootDir = sys.argv[1]
    print( 'rootDir: ', rootDir)
    autoScanHash([rootDir,])