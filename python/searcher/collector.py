import os
import sqlite3
import time
import sys

dbPath = 'files.db'
collectedDirs = [ '/share/disk-ssd/downloads' , '/share/disk-wd12t/books']

excludeFiles = set()
for line in open( 'exclude.txt').readlines():
    excludeFiles.add( line.strip() )

def initDb():
    connect = sqlite3.connect( dbPath, 10*1000 )

    # 
    connect.execute( 'Create Table  IF NOT EXISTS files(path TEXT NOT NULL, isDir INTEGER NOT NULL, size INTEGER NOT NULL, mtime INTEGER NOT NULL, PRIMARY KEY(path));' )

    return connect

db = initDb()

def collect( path ):
    for excludeFile in excludeFiles:
        if path.find( excludeFile ) != -1:
            print( 'exclude [', path, ']' )
            return
    #print( 'path [', path, ']' )
    mtime = os.path.getmtime( path )
    #print( 'mtime ', mtime )
    stat = os.stat( path )
    size = os.path.getsize( path )
    #print( 'stat ', stat )

    #print( path, os.path.isdir(path) )
    db.execute('replace into files Values(?,?,?,?);', (path, int(os.path.isdir(path)), int(size), int(mtime)) )
    #print( 'db execute ok.' )
    return

lastTime = time.time()

def update( collectedDir ):
    print( 'update [', collectedDir, ']')
    db.execute( 'delete from files where path like ?;', (os.path.join(collectedDir,collectedDir)+'%',) )
    for root, dirs, files in os.walk( collectedDir ):
        for dir in dirs:
            collect(os.path.join(root,dir));
            
        for file in files:
            collect(os.path.join(root,file));

        curTime = time.time()
        global lastTime
        if curTime - lastTime > 60:
            db.commit()
            lastTime = curTime
    print( 'before commit ', collectedDir)
    db.commit()
    print( 'after commit', collectedDir)


if __name__=="__main__":
    method = sys.argv[1]

    if method == 'rebuild':
        for collectedDir in collectedDirs:
            update( collectedDir )
        db.commit()
    elif method == 'update':
        updateDir = sys.argv[2]
        update( updateDir )
    else:
        print( 'Unknown method!' )