import os

anchorParentDir = '/share/disk-wd12t/1-audiobook/xmly'

anchorMap = {}
lines = open( 'anchortable.txt', 'r', encoding='utf-8' ).readlines()
for line in  lines:
    items = line.split( '|' )
    anchorId = items[0].strip()
    name = items[1].strip()
    #print( 'anchor ' + anchorId + "-" + name )
    anchorMap[ name ] = anchorId

for dirName in os.listdir( anchorParentDir ):
    dirPath = os.path.join( anchorParentDir,dirName)
    if os.path.isdir( dirPath ):
        if dirName in anchorMap:
            anchorId = anchorMap[ dirName ]
            newName = anchorId + "-" + dirName
            newPath = os.path.join( anchorParentDir, newName )
            os.rename( dirPath, newPath )
            print( 'Rename dir ' + dirPath + ' to ' + newPath )
            
            #break # 暂时测试, 只改一个.
        else:
            print( 'dir ' + dirName + ' is not in anchorMap!' )
    else:
        print( 'dir ' + dirPath + " is not a dir!")