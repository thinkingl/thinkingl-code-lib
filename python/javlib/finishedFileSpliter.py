

def SaveUrls( urlSet, fileDir ):
    filePathVideo = fileDir + "finishedVideo.txt"
    fVideo = open( filePathVideo, "w", encoding="utf-8")
    filePathList = fileDir + "finishedVideoList.txt"
    fVideoList = open( filePathList, "w", encoding="utf-8")
    for url in urlSet:
        if( url.startswith("http://www.javlibrary.com/tw/?v=") ):
            fVideo.write( url + "\n" )
        else:
            fVideoList.write( url+"\n" );
    fVideo.close()
    fVideoList.close()


def ReadUrls( filePath, urlSet ):
    try:
        with open( filePath, "r", encoding="utf-8") as f:
            urlSet |= set(f.read().splitlines())
        f.close()
    except FileNotFoundError:
        print( "file not found" );

oldFinishedPath = "D:\\999-temp\\javlib\\finished.txt";
urlSet = set()
ReadUrls(oldFinishedPath, urlSet)
SaveUrls( urlSet, "D:\\999-temp\\javlib\\" )
