import urllib
import os
import socket
import requests
#from HTMLParser import HTMLParser
import shutil
from bs4 import BeautifulSoup
from urllib.parse import urlparse, urljoin
from urllib.request import urlretrieve
import json
from selenium import webdriver
#from seleniumrequests import Firefox
from selenium.webdriver.support.wait import WebDriverWait
import pickle
import time
import proxy
import logging
from avdbClient import AvdbClient
import re
import sys
import queue
import threading

javlibLocalDir = "d:/999-temp/javlib/"
javlibLocalDir = "h:/javlib/"


waitingUrlFile = "waitting.txt"
finishedUrlFile = "finishedVideo.txt"
finishedVideoListUrlFile = "finishedVideoList.txt"
errorUrlFile = "error.txt"

# url的链接关系.
urlsMapFile = 'urlsMap.json'
urlsMap = {}
urlsMapLock = threading.RLock()

dirByDate = os.path.join( javlibLocalDir, 'byDate')
dirByActress = os.path.join( javlibLocalDir, 'byActress')

if not os.path.isdir( dirByDate ):
    os.makedirs( dirByDate )
if not os.path.isdir( dirByActress ):
    os.makedirs( dirByActress )


def initLogging():
    sys.stdout.reconfigure(encoding='utf-8')
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式

    if not os.path.isdir( 'logs' ):
        os.makedirs( 'logs' )

    logFileName = time.strftime('logs/avlib-%Y%m%d-%H%M%S.log',time.localtime())
    
    fh = logging.FileHandler(logFileName)
    fh.setLevel(logging.DEBUG)
    #fh.setFormatter(formatter)

    # 使用StreamHandler输出到屏幕
    ch = logging.StreamHandler()
    ch.setLevel(logging.INFO)
    #ch.setFormatter(formatter)
    #logging.addHandler( fh )
    #logging.addHandler( ch )

    logging.basicConfig(level=logging.INFO,
        format   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
        datefmt  = '%Y-%m-%d %A %H:%M:%S',                                     # 时间
        #filename = logFileName,                # log文件名
        #filemode = 'w',
        handlers = [fh, ch]
    )


def NormalizeName( name ):
    MaxNameLen = 128
    name = name[0:MaxNameLen] # 截断超长的部分. 要先截断,否则可能会在截断后在结尾出现空格.
    name = re.sub('[\\\/:*?"<>|\t\v\r\n]','-',name)#去掉非法字符
    name = re.sub('[\.\s]*$', '', name) # 去掉结尾的空白和.
    name = re.sub('^[\.\s]*', '', name) #去掉开始的空白和.
    name = name.replace( '\0', '' );    # 有的名称里面有0,必须去掉.
    return name

def saveUrlsMap():
    with urlsMapLock:
        global urlsMap
        urlsMapPath = os.path.join( javlibLocalDir, urlsMapFile )
        with open( urlsMapPath, 'w' ) as f:
            json.dump( urlsMap, f, indent=4 )
        urlsMapPathBk = urlsMapPath + '.bk'       # 防止程序在dump过程中出问题(关闭, 这里再多写一个备份文件)
        with open( urlsMapPathBk, 'wb' ) as fBk:
            with open( urlsMapPath, 'rb') as f:
                fBk.write( f.read() )
        return

def loadUrlsMap():
    with urlsMapLock:
        global urlsMap
        urlsMapPath = os.path.join( javlibLocalDir, urlsMapFile )
        try:
            with open( urlsMapPath ) as f:
                urlsMap = json.load( f )
        except:
            logging.exception( 'error' )
            urlsMap = {}
            
        if len( urlsMap ) > 0:
            return
        urlsMapPath = urlsMapPath + '.bk'       # 防止程序在dump过程中出问题(关闭, 从备份文件中读取)
        with open( urlsMapPath ) as f:
            urlsMap = json.load( f )
        return

def getUrls( url ):
    with urlsMapLock:
        global urlsMap
        return urlsMap.get( url )

def setUrls( url, releatedUrls ):
    with urlsMapLock:
        global urlsMap
        urlsMap[ url ] = releatedUrls
        return

def ParseJavlibVideoHtml( videoUrl, videoInfo, urlSet ):
    logging.info( "Parse video html:" + videoUrl)

    releatedUrls = []

    soup = proxy.bsObjForm( videoUrl )
    if soup == None:
        return False

    success = True
    #print (soup)
    videoInfo["url"] = videoUrl

    videoInfo["title"] = ""
    titleArray = soup.select("#video_title > h3 > a")
    #titleArray = browser.find_element_by_css_selector( "#video_title > h3 > a" )
    if( len( titleArray ) > 0 ):
        videoInfo["title"] = titleArray[0].text
        #print( videoInfo["title"] )

    #videoInfoArray = soup.select("#video_info")
    #if( len( videoInfoArray) > 0 ):
        #videoInfo = videoInfoArray[0]
    #videoInfoKeys= ["video_id", "video_date","video_length","video_director","video_maker","video_label","video_review","video_genres", "video_cast"]

    videoIdArray = soup.select("#video_id > table > tr > td")
    videoInfo["id"] = ''
    if( len( videoIdArray ) > 1 ):
        videoInfo["id"] = videoIdArray[1].text
        #print ( "videoId:\t" + videoInfo["videoId"])

    videoInfo["date"] =""
    videoDateArray = soup.select("#video_date > table > tr > td")
    if( len( videoDateArray ) > 1 ):
        videoInfo["date"] = videoDateArray[1].text
        #print ( "video date:\t" + videoDate)

    videoInfo["length"] = ""
    videoLengthArray = soup.select("#video_length > table > tr > td")
    if( len( videoLengthArray ) > 1 ):
        videoInfo["length"] = videoLengthArray[1].text
        #print ( "video length:\t" + videoLength)

    videoInfo["director"] = ""
    videoDirectorArray = soup.select("#video_director > table > tr > td")
    if( len( videoDirectorArray ) > 1 ):
        videoInfo["director"] = videoDirectorArray[1].text
        #print ( "video director:\t" + videoDirector.text)

    videoDirectorUrlArray = soup.select("#video_director > table > tr > td a[href]")
    for videoDirectorUrl in videoDirectorUrlArray:
        url = urljoin( videoUrl, videoDirectorUrl.attrs["href"])
        urlSet.add( url )
        releatedUrls.append( url )
        #print( "video director url:\t" + url )

    videoInfo["maker"] = ""
    videoMakerArray = soup.select("#video_maker > table > tr > td a[href]")
    if( len( videoMakerArray ) > 0 ):
        videoMaker = videoMakerArray[0]
        #print ( "video maker:\t" + videoMaker.text)
        videoInfo["maker"] = videoMaker.text
        url = urljoin( videoUrl, videoMaker.attrs["href"])
        urlSet.add( url )
        releatedUrls.append( url )
        #print( "video maker url:\t" + url)

    videoLabelArray = soup.select("#video_label > table > tr > td a[href]")
    videoInfo["label"] = ""
    for videoLabel in videoLabelArray:
        #print ( "video label:\t" + videoLabel.text)
        videoInfo["label"] += videoLabel.text
        url = urljoin( videoUrl, videoLabel.attrs["href"])
        urlSet.add( url )
        releatedUrls.append( url )
        #print( "video label url:\t" + url)

    videoReviewArray = soup.select("#video_review > table > tr > td > span")
    videoInfo["review"]=''
    for videoReview in videoReviewArray:
        #print ( "video review:\t" + videoReview.text)
        videoInfo["review"]=videoReview.text

    videoGenresArray = soup.select("#video_genres > table > tr > td a[href]")
    videoInfo["genres"] = ""
    for videoGenres in videoGenresArray:
        #print ( "video genres:\t" + videoGenres.text)
        videoInfo["genres"] += videoGenres.text
        videoInfo["genres"] += "    "
        url = urljoin( videoUrl, videoGenres.attrs["href"])
        urlSet.add( url )
        releatedUrls.append( url )
        #print( "video genres url:\t"+url)

    videoCastNameArray = soup.select("#video_cast > table > tr > td")
    videoInfo["cast"] = ""
    if( len( videoCastNameArray ) > 1 ):
        logging.debug ( "video cast:\t" + videoCastNameArray[1].text)
        videoInfo["cast"] += videoCastNameArray[1].text
        videoInfo["cast"] += "    "
    videoInfo["cast"] = videoInfo["cast"].strip()
    if( len(videoInfo["cast"]) == 0 ):
        videoInfo["cast"]   = "anonymous"

    videoCastUrlArray = soup.select( "#video_cast > table > tr > td a[href]")
    for videoCastUrl in videoCastUrlArray:
        url = urljoin( videoUrl, videoCastUrl.attrs["href"])
        urlSet.add( url )
        releatedUrls.append( url )
        #print( "video cast url:\t" + url)

    # image.
    videoInfo["imgFileName"] = ""
    imgArray = soup.select("#video_jacket_img")
    if( len( imgArray ) > 0 ):
        url = urljoin( videoUrl, imgArray[0].attrs["src"])    
        imgFileName = videoInfo["date"] + " " + videoInfo["id"] + ".jpg"
        videoInfo["imgFileName"] = imgFileName     
        videoInfo['imgUrl'] = url;   

    if( len( videoInfo["id"]) == 0 ):
         success = False
    logging.debug( json.dumps( videoInfo ) )

    # 保存url关系.
    setUrls( videoUrl, releatedUrls )

    return success

def ParseJavlibVideoListHtml( videoListUrl, newUrls ):
    logging.info( "parse video list url: %s " , videoListUrl )
    

    soup = proxy.bsObjForm( videoListUrl )
    #print( soup )

    videoInfoUrlArray = soup.select(".videos .video a[href]")
    for videoInfoUrl in videoInfoUrlArray:
        url = urljoin( videoListUrl, videoInfoUrl.attrs["href"] )
        newUrls.add( url )
        #print( "video url:\t" + url )

    videoListPageUrlArray = soup.select( ".page_selector a[href]")
    for videoListPageUrl in videoListPageUrlArray:
        url = urljoin( videoListUrl, videoListPageUrl.attrs["href"] )
        newUrls.add( url )
        #print( "video listt page url:\t" + url)
    return ( len(newUrls) > 0 )



def SaveUrls( urlSet, filePath ):
    urlList = list(urlSet)
    urlList.sort()
    f = open( filePath, "w", encoding="utf-8")
    for url in urlList:
        f.write( url + "\n" )
    f.close()
    bkFilePath = filePath + ".bk"
    shutil.copyfile( filePath, bkFilePath )

def SaveFinishedUrls( urlSet, fileDir ):
    urlList = list(urlSet)
    urlList.sort()

    filePathVideo = os.path.join(fileDir , "finishedVideo.txt")
    fVideo = open( filePathVideo, "w", encoding="utf-8")
    filePathList = os.path.join(fileDir , "finishedVideoList.txt")
    fVideoList = open( filePathList, "w", encoding="utf-8")
    for url in urlList:
        if( url.startswith("http://www.javlibrary.com/tw/?v=") ):
            fVideo.write( url + "\n" )
        else:
            fVideoList.write( url+"\n" )
    fVideo.close()
    fVideoList.close()
    with open( filePathVideo+'.bk', "wb") as fVideoBk:
        with open( filePathVideo, 'rb' ) as fVideo:
            fVideoBk.write( fVideo.read() )
    
    with open( filePathList+'.bk', "wb") as fVideoListBk:
        with open( filePathList, 'rb' ) as fVideoList:
            fVideoListBk.write( fVideoList.read() )

def ReadUrls( filePath, urlSet ):
    try:
        with open( filePath, "r", encoding="utf-8") as f:
            urlSet |= set(f.read().splitlines())
        f.close()
    except FileNotFoundError:
        logging.info( "file not found" )
    
    try:
        with open( filePath+'.bk', "r", encoding="utf-8") as f:
            urlSet |= set(f.read().splitlines())
        f.close()
    except FileNotFoundError:
        logging.info( "back file not found" )

def downloadFileTry(url, localPath):
    for i in range(5):                
        try:
            urllib.request.urlretrieve( url, localPath)
            logging.info( 'downlaod file success! [%s] - [%s]', localPath, url)
            return True
        except Exception as e:
            logging.error( "download file except [%s], try again! [%s] - [%s]", e, url, localPath)
            if os.path.isfile( localPath ):
                os.remove( localPath )
            time.sleep(10)
    logging.error( "download file fail! [%s] - [%s]", url, localPath)    
    return False

# 保存AV数据
#   1. 保存到数据库
#   2. 如果数据库中之前没有
#       a. 保存到本地月份目录下
#       b. 如果是单人作品，并且不是匿名演员，保存到本地演员目录下。
def saveAV( videoInfo ):
    avUrl = videoInfo['url']
    avId = videoInfo['id']
    avdbClient = AvdbClient()
    # 数据库是否存在
    if avdbClient.isAVUrlExist( avUrl ):
        logging.info( 'av %s already exist!', avId )
        return True
    # 保存本地文件

    # 图片
    picPath = saveAVImage( videoInfo )
    if picPath == None:
        logging.error( 'Save av image fail!' )
        return False

    # json和text文件
    SaveVideoInfo( videoInfo )

    # 保存到数据库
    picData = open(picPath, 'rb').read()
    for i in range(0,10):
        try:
            avdbClient.addPic( videoInfo, picData )
            return True
        except:
            logging.error( "addPic exception!")
            logging.exception( "addPic" )
            time.sleep(1)
            pass
    return False

def saveAVImage( videoInfo ):
    url = videoInfo['imgUrl']
    imgFileName = videoInfo['imgFileName']

    # 按月保存
    localPath = getPathByDate( videoInfo, imgFileName )
    if not downloadFileTry( url, localPath ):
        logging.error( 'Downlaod image %s-%s fail!', url, localPath )
        return None

    # 按演员保存。
    # 只有单人作品， 并且不是匿名演员，才保存。
    localPathActress = getPathByActress( videoInfo, imgFileName )
    if localPathActress != None :
        if not downloadFileTry( url, localPathActress ):
            logging.error( 'Downlaod image %s-%s fail!', url, localPathActress )
            return None
    return localPath

def getPathByDate( videoInfo, fileName ):
    yearMonth = videoInfo["date"][0:7]
    fileDir = os.path.join( dirByDate, yearMonth )
    if not os.path.isdir( fileDir ):
        os.makedirs( fileDir )
    filePath = os.path.join( dirByDate, yearMonth, fileName )
    return filePath

# 获取按演员保存的路径。 如果不需要保存返回None。、
# 只有单人作品， 并且不是匿名演员，才保存。
def getPathByActress( videoInfo, fileName ):
    attressName = videoInfo['cast']
    castArry = attressName.split( ' ' )
    anonymousCastSet = set()
    anonymousCastSet.add( 'anonymous' )
    if len( castArry ) == 1 and not attressName in anonymousCastSet :
        attressName = NormalizeName( attressName )
        localPath = os.path.join( dirByActress, attressName, fileName )

        fileDir = os.path.join( dirByActress, attressName )
        if not os.path.isdir( fileDir ):
            os.makedirs( fileDir )
        return localPath
    return None

def SaveVideoInfo( videoInfo ):
    wikiStr = ""
    wikiStr += videoInfo["title"]
    wikiStr += "\n\n"
    wikiStr += ("番号:  \t" + videoInfo["id"] + "\\\\\n")
    wikiStr += ("发行日期:  \t" + videoInfo["date"] + "\\\\\n")
    wikiStr += ("长度:  \t" + videoInfo["length"] + "\\\\\n")
    wikiStr += ("导演:  \t" + videoInfo["director"] + "\\\\\n")
    wikiStr += ("制作商:  \t" + videoInfo["maker"] + "\\\\\n")
    wikiStr += ("发行商:  \t" + videoInfo["label"] + "\\\\\n")
    wikiStr += ("网站评分:  \t" + videoInfo["review"] + "\\\\\n")
    wikiStr += ("类别:  \t" + videoInfo["genres"] + "\\\\\n")
    wikiStr += ("演员:  \t" + videoInfo["cast"] )
    wikiStr += "\r\n\r\n"
    wikiStr += ("{{:wiki:" + videoInfo["imgFileName"] + "|}}")
    wikiStr += "\r\n\r\n"
    wikiStr += ("[[" + videoInfo["url"] + "|" + videoInfo["title"] + "]]")

    fileName = videoInfo["date"] + " " + videoInfo["id"] + ".txt"

    # 保存到月份目录
    filePath = getPathByDate( videoInfo, fileName )
    with open( filePath, "w", encoding='utf-8' ) as f:
        f.write( wikiStr )
    f.close()

    localPathActress = getPathByActress( videoInfo, fileName )
    if localPathActress != None:
        shutil.copyfile( filePath, localPathActress )

    jsonFileName = videoInfo["date"] + " " + videoInfo["id"] + ".json"
    filePath = getPathByDate( videoInfo, jsonFileName )
    with open( filePath, "w", encoding='utf-8' ) as f:
        json.dump( videoInfo, f )
    f.close()

    path2 = getPathByActress( videoInfo, jsonFileName )
    if path2 != None:
        shutil.copyfile( filePath, path2 )

# 从等待集合中获取一个等待的url
# 新的算法加入排序，不再散乱的从waittingSet中获取。
# 这样可以有规律的下载。
# 可以防止因为分页问题错过一些。
def getWaittingUrl( waittingSet ):
    if len( waittingSet ) == 0:
        return None
    waitingList = list(waittingSet)
    waitingList.sort(reverse=True)
    url = waitingList.pop()

    waitingList.sort()
    # 如果需要一个列表，优先使用演员。
    if url.find( 'http://www.javlibrary.com/tw/?v=' ) == -1:
        for u in waitingList:
            if u.find( 'http://www.javlibrary.com/tw/vl_star.php' ) != -1:
                url = u
                break

    waittingSet.remove( url )
    return url

# 数据库是否存在
def isExistInDb( avUrl ):
    # 数据库是否存在
    avdbClient = AvdbClient()
    if avdbClient.isAVUrlExist( avUrl ):
        logging.info( 'av %s already exist!', avUrl )
        return True
    return False

def parseSaveAv( waitingUrl, urlSet):
    if isExistInDb( waitingUrl ):
        # 先从本地获取url关联的url.
        # 如果本地没有记录, 就解析html.
        releatedUrls = getUrls( waitingUrl )
        if releatedUrls == None:
            videoInfo= {}
            success = ParseJavlibVideoHtml( waitingUrl, videoInfo, urlSet )
        else:
            urlSet.update( releatedUrls )
        return True  # 已经存在的url直接完成。
    else:
        videoInfo= {}
        success = ParseJavlibVideoHtml( waitingUrl, videoInfo, urlSet )
        if( success ):
            success = saveAV( videoInfo )
        return success

# 备份所有数据
# 数据库只保留2份备份。
# 本地每天保存2份备份。
def backupAllData(backupIndex, waittingSet, finishedSet, errorUrlSet):
    logging.warn( "!!!!!!!!!The backup start!!!!!!!!!!!!!!!!!!!!" )
    dbBackToken = 'bk-' + str(backupIndex)
    avdbClient = AvdbClient()
    if not avdbClient.dbBackup( dbBackToken ):
        logging.error( 'db backup fail!!!!' )
        return
    
    cur_time = time.time()
    strtoday = time.strftime('%Y%m%d',time.localtime(cur_time))
    backDir = os.path.join( javlibLocalDir, 'bk', strtoday, str(backupIndex) )
    if os.path.isdir( backDir ):
        shutil.rmtree( backDir )
    os.makedirs( backDir )

    backWaittingPath = os.path.join( backDir, waitingUrlFile )
    SaveUrls( waittingSet, backWaittingPath )

    SaveFinishedUrls( finishedSet, backDir )

    backupErrorPath = os.path.join( backDir, errorUrlFile )
    SaveUrls( errorUrlSet, backupErrorPath )
    logging.warn( "!!!!!!!!!The backup end!!!!!!!!!!!!!!!!!!!!" )

def threadParseUrl( queueWaittingUrl, queueResult ):
    while( True ):
        waitingUrl = queueWaittingUrl.get()

        urlSet = set()
        success = True
        if( "/?v=" in waitingUrl ):
            success = parseSaveAv(waitingUrl, urlSet )
        else:
            success = ParseJavlibVideoListHtml( waitingUrl, urlSet )
        
        result = (waitingUrl, success, urlSet )
        queueResult.put( result )
    

# 等待线程处理的url队列
queueWaittingUrl = queue.Queue(maxsize=2)
# url处理结果，内部处理结果是（string url, bool result, set urlSet）的元组。
queueResult = queue.SimpleQueue()

threadNum = 2
for i in range(0, threadNum):
    threading.Thread(target=threadParseUrl,args=(queueWaittingUrl,queueResult)).start()


initLogging()

waitingUrlSet = set()
waitingUrlFilePath = javlibLocalDir+waitingUrlFile
ReadUrls(waitingUrlFilePath, waitingUrlSet)
#print( waitingUrlSet)

finishedUrlSet = set()
finishedUrlFilePath = javlibLocalDir+finishedUrlFile
ReadUrls(finishedUrlFilePath, finishedUrlSet)
finishedUrlVideoListPath = javlibLocalDir+finishedVideoListUrlFile
ReadUrls( finishedUrlVideoListPath, finishedUrlSet)

errorUrlSet = set()
errorUrlFilePath = javlibLocalDir + errorUrlFile
ReadUrls( errorUrlFilePath, errorUrlSet)

# 关联url.
loadUrlsMap()

socket.setdefaulttimeout(100)
opener=urllib.request.build_opener()
opener.addheaders=[('User-Agent','Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36')]
urllib.request.install_opener(opener)

urlBeginning = "http://www.javlibrary.com/tw/?v=javlijb6si"
#urlBeginning = "http://www.javlibrary.com/tw/?v=javli4yvsa"
#urlBeginning = "http://www.javlibrary.com/tw/?v=javli7yiym"

if( urlBeginning not in finishedUrlSet ):
    waitingUrlSet.add( urlBeginning )

workingUrlSet = set()
while( len(waitingUrlSet ) > 0 or len(workingUrlSet) > 0 ):
    waitingUrl = getWaittingUrl( waitingUrlSet ) # waitingUrlSet.pop()

    if waitingUrl == None:
        time.sleep(1)
    else:
        # 放入线程处理队列。
        queueWaittingUrl.put( waitingUrl )
        workingUrlSet.add( waitingUrl )

    while( not queueResult.empty() ):
        result = queueResult.get()
        url = result[0]
        success = result[1]
        urlSet = result[2]

        workingUrlSet.remove( url ) # 删除有结果的。
        if( success ):
            finishedUrlSet.add( url )
            urlSet -= finishedUrlSet
            urlSet -= workingUrlSet
            waitingUrlSet |= urlSet
            
            
            # 备份。
            numberBetweenBackup = 50000
            if( len(finishedUrlSet ) % numberBetweenBackup == 0 ):
                
                # 等待当前任务完成。
                while not queueWaittingUrl.empty():
                    time.sleep(1)
                time.sleep(60)      # 先简单的sleep等待完成，后续考虑更好的方式。

                backupIndex = int(len(finishedUrlSet) / numberBetweenBackup) % 2
                backupAllData( backupIndex, waitingUrlSet, finishedUrlSet, errorUrlSet )
            #elif len(finishedUrlSet) % 500 == 0 :
            #    avdbClient = AvdbClient()
            #    avdbClient.dbIntegrityCheck()
                
            # save urls. save after backup, do not save if backup fail!.
            if( len(finishedUrlSet) % 100 == 0 ):
                SaveUrls( waitingUrlSet | workingUrlSet, waitingUrlFilePath)
                SaveFinishedUrls( finishedUrlSet, javlibLocalDir )
                saveUrlsMap()
                logging.info( 'finished: %d waitting: %d err: %d', len(finishedUrlSet), len( waitingUrlSet ), len(errorUrlSet) )
        else:
            errorUrlSet.add( url )
            SaveUrls( errorUrlSet, errorUrlFilePath )
            time.sleep(30)

SaveUrls( waitingUrlSet, waitingUrlFilePath)
SaveFinishedUrls( finishedUrlSet, javlibLocalDir )
