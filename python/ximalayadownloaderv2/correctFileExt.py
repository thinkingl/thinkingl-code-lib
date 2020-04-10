import os

fileDirPath = '.'
#fileDirPath = 'e:'

def correctPicFile():
    for root,dirs,files in os.walk(fileDirPath):
        for fileName in files:
            oldPath = os.path.join( root, fileName )

            fileExt = os.path.splitext( fileName )[-1]
            if len(fileExt) > 5:
                fileExt2 = fileExt.split( '!' )[0]
                if fileExt != fileExt2:                
                    newName = os.path.splitext( fileName )[0] + fileExt2
                    newPath = os.path.join( root, newName )
                    os.rename( oldPath, newPath )
                    print( oldPath + " -> " + newPath )
                    continue

            parts = fileName.split( '..' )
            if len(parts) ==2:
                newPath = os.path.join( root, parts[0] + '.' + parts[1] )
                os.rename( oldPath, newPath )
                print( oldPath + " -> " + newPath )

                
# 倒序的.
def resort():
    indexSet = set()
    fileList = os.listdir('.')
    for fileName in fileList:
        if '-' in fileName:
            try:
                fileIndex = int(fileName.split( '-' )[0])
            except:
                print( "file " + fileName + " is invalid" )
                continue
            indexSet.add( fileIndex )
        else:
            print( "file " + fileName + " is invalid" )
    maxIndex = max( indexSet )
    for fileName in fileList:
        if '-' in fileName:
            try:
                fileIndex = int(fileName.split( '-' )[0])
            except:
                print( "file " + fileName + " is invalid" )
                continue
            newIndex = maxIndex - fileIndex + 1
            fileNameParts = fileName.split('-')
            fileNameParts[0] = str(newIndex)
            newName = ''
            for fileNamePart in fileNameParts:
                newName += fileNamePart
                newName += '-'
            newName = newName[0:-1]
            print( fileName + " -> " + newName )
            os.rename( fileName, newName )

resort()