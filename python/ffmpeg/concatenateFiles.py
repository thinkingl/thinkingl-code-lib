import os


def concatenateWMVs( sourceDir:str, files:list, dstDir:str):
    listFile = open("list.txt", 'w', encoding='utf-8')
    for fileName in files:
        filePath = os.path.join(sourceDir, fileName)
        line = "file '%s'\n"%filePath
        listFile.write( line )
    listFile.close()
    dstFileName = os.path.splitext(files[0])[0][0:-2] +  os.path.splitext(files[0])[1]
    dstPath = os.path.join( dstDir, dstFileName )

    if os.path.exists( dstPath ):
        print( dstPath, "already exist!")
        return

    cmd = 'ffmpeg -f concat -safe 0 -i list.txt -c copy %s'%dstPath
    print( 'cmd:', cmd)
    p = os.popen( cmd )
    out = p.read()
    print( dstPath, "finished!")
    print( out )

if __name__=="__main__":
    inDir = "y:/Downloads"
    outDir = "y:/tmp"

    for root,dirs,files in os.walk( inDir ):
        for dir in dirs:
            dirPath = os.path.join( root, dir )
            files = os.listdir( dirPath )
            wmvFiles = list()
            for fileName in files:
                if fileName.endswith(".wmv" ):
                    wmvFiles.append( fileName )
                    #print( fileName, "in", dirPath )
            if len(wmvFiles) > 1:
                print( "concatenate files", wmvFiles )
                concatenateWMVs( dirPath, wmvFiles, outDir)