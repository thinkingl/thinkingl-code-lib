import sys
import os
import sqlite3
from unrar import rarfile
import zipfileGBK
import collector


dbPath = 'files.db'
collectedDirs = [ '/share/disk-ssd/downloads' , '/share/disk-wd12t/books']

forceEncodingTable = {}
for line in open('forceEncoding.txt').readlines():
    line = line.strip()
    name,encoding = line.split('=')
    forceEncodingTable[name.strip()] = encoding.strip()

passwordTable = {}
for line in open('password.txt').readlines():
    line = line.strip()
    name,password = line.split('=')
    passwordTable[name.strip()] = password.strip()

def initDb():
    connect = sqlite3.connect( dbPath, 10*1000 )

    # 
    connect.execute( 'Create Table  IF NOT EXISTS files(path TEXT NOT NULL, isDir INTEGER NOT NULL, size INTEGER NOT NULL, mtime INTEGER NOT NULL, PRIMARY KEY(path));' )

    return connect

def unzip( path ):
    if not os.path.exists( path ):
        print( path, ' file not exist!')
        return
    print ( 'unzip ', path )
    dir = os.path.dirname( path )
    try:
        forceEncoding = None
        fileName = path.split('/')[-1]
        if fileName in forceEncodingTable:
            forceEncoding = forceEncodingTable[fileName]

        z = zipfileGBK.ZipFile( path, forceEncoding=forceEncoding )

        if fileName in passwordTable:
            z.setpassword( passwordTable.get(fileName).encode('utf-8') )

        for i in z.infolist():
            #print( i.filename )
            filePath = os.path.join( dir, i.filename )

            if os.path.exists( filePath ):
                continue
            z.extract(i.filename, dir)
            print( filePath )
    except zipfileGBK.BadZipFile:
        open( 'error.txt', 'a' ).write( path + '\n' )
        delEmptyFile( path )
    collector.update( dir )
    return

def unrar( path ):
    if not os.path.exists( path ):
        print( path, ' file not exist!')
        return
    print ( 'unrar ', path )

    dir = os.path.dirname( path )

    try:
        r = rarfile.RarFile( path )
        for i in r.infolist():
            #print( i.filename )
            filePath = os.path.join( dir, i.filename )

            if os.path.exists( filePath ):
                continue
            r.extract(i.filename, dir)
            print( filePath )
    except:
        open( 'error.txt', 'a' ).write( path + '\n' )
        delEmptyFile( path )
    collector.update( dir )
    return

def delEmptyFile(path):
    if not os.path.isfile( path ):
        print( 'path ', path, ' is not a file!')
        return
    with open(path, 'rb') as f:
        for b in f.read( 10000 ):
            if b != 0:
                print( path, ' is not empty' )
                return
        os.remove( path )
    return

if __name__=="__main__":
        
    db = initDb()

    zipList = list()
    for path, in db.execute( 'select path from files where path like "%disk-wd12t%.zip";' ):
        path = path.strip()
        
        if path.find ('/books/comic-漫画') != -1:
            print( 'ignore comic files! ', path )
            continue

        zipList.append( path )


    rarList = list()
    for path, in db.execute( 'select path from files where path like "%disk-wd12t%.rar";' ):
        path = path.strip()
        if path.find ('/books/comic-漫画') != -1:
            print( 'ignore comic files! ', path )
            continue
        rarList.append( path )

    for path in rarList:
        unrar( path )
    
        print( path )
        #break # test 1 file

    for path in zipList:
        unzip( path );
        #break # test 1 file.