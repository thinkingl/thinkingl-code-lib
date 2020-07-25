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

javlibLocalDir = "d:/999-temp/javlib/"


waitingUrlFile = "waitting.txt"
finishedUrlFile = "finishedVideo.txt"
errorUrlFile = "error.txt"

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

def ParseJavlibVideoHtml( videoUrl, videoInfo, urlSet ):
    logging.info( "Parse video html:" + videoUrl)
    
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
        #print( "video director url:\t" + url )

    videoInfo["maker"] = ""
    videoMakerArray = soup.select("#video_maker > table > tr > td a[href]")
    if( len( videoMakerArray ) > 0 ):
        videoMaker = videoMakerArray[0]
        #print ( "video maker:\t" + videoMaker.text)
        videoInfo["maker"] = videoMaker.text
        url = urljoin( videoUrl, videoMaker.attrs["href"])
        urlSet.add( url )
        #print( "video maker url:\t" + url)

    videoLabelArray = soup.select("#video_label > table > tr > td a[href]")
    videoInfo["label"] = ""
    for videoLabel in videoLabelArray:
        #print ( "video label:\t" + videoLabel.text)
        videoInfo["label"] += videoLabel.text
        url = urljoin( videoUrl, videoLabel.attrs["href"])
        urlSet.add( url )
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
        #print( "video genres url:\t"+url)

    videoCastNameArray = soup.select("#video_cast > table > tr > td")
    videoInfo["cast"] = ""
    if( len( videoCastNameArray ) > 1 ):
        print ( "video cast:\t" + videoCastNameArray[1].text)
        videoInfo["cast"] += videoCastNameArray[1].text
        videoInfo["cast"] += "    "
    videoInfo["cast"] = videoInfo["cast"].strip()
    if( len(videoInfo["cast"]) == 0 ):
        videoInfo["cast"]   = "anonymous"

    videoCastUrlArray = soup.select( "#video_cast > table > tr > td a[href]")
    for videoCastUrl in videoCastUrlArray:
        url = urljoin( videoUrl, videoCastUrl.attrs["href"])
        urlSet.add( url )
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
    logging.info( json.dumps( videoInfo ) )
    return success

def ParseJavlibVideoListHtml( videoListUrl, newUrls ):
    logging.info( "parse video list url:\t" + videoListUrl )
    

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
    f = open( filePath, "w", encoding="utf-8")
    for url in urlSet:
        f.write( url + "\n" )
    f.close()
    bkFilePath = filePath + ".bk"
    shutil.copyfile( filePath, bkFilePath )

def SaveFinishedUrls( urlSet, fileDir ):
    filePathVideo = fileDir + "finishedVideo.txt"
    fVideo = open( filePathVideo, "w", encoding="utf-8")
    filePathList = fileDir + "finishedVideoList.txt"
    fVideoList = open( filePathList, "w", encoding="utf-8")
    for url in urlSet:
        if( url.startswith("http://www.javlibrary.com/tw/?v=") ):
            fVideo.write( url + "\n" )
        else:
            fVideoList.write( url+"\n" )
    fVideo.close()
    fVideoList.close()

def ReadUrls( filePath, urlSet ):
    try:
        with open( filePath, "r", encoding="utf-8") as f:
            urlSet |= set(f.read().splitlines())
        f.close()
    except FileNotFoundError:
        logging.info( "file not found" )

def downloadFileTry(url, localPath):
    for i in range(10):                
        try:
            urllib.request.urlretrieve( url, localPath)
            logging.info( 'downlaod file success! [%s] - [%s]', localPath, url)
            return True
        except Exception as e:
            logging.error( "download file except [%s], try again! [%s] - [%s]", e, url, localPath)
            if os.path.isfile( localPath ):
                os.remove( localPath )
            time.sleep(1)
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
    avdbClient.addPic( videoInfo, picData )
    return True

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
    castArry = attressName.split( '    ' )
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



waitingUrlSet = set()
waitingUrlFilePath = javlibLocalDir+waitingUrlFile
ReadUrls(waitingUrlFilePath, waitingUrlSet)
#print( waitingUrlSet)

finishedUrlSet = set()
finishedUrlFilePath = javlibLocalDir+finishedUrlFile
ReadUrls(finishedUrlFilePath, finishedUrlSet)

errorUrlSet = set()
errorUrlFilePath = javlibLocalDir + errorUrlFile
ReadUrls( errorUrlFilePath, errorUrlSet)

socket.setdefaulttimeout(100)
opener=urllib.request.build_opener()
opener.addheaders=[('User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1941.0 Safari/537.36')]
urllib.request.install_opener(opener)

urlBeginning = "http://www.javlibrary.com/tw/?v=javlijb6si"
#urlBeginning = "http://www.javlibrary.com/tw/?v=javli4yvsa"
#urlBeginning = "http://www.javlibrary.com/tw/?v=javli7yiym"

if( urlBeginning not in finishedUrlSet ):
    waitingUrlSet.add( urlBeginning )

while( len(waitingUrlSet ) > 0 ):
    waitingUrl = waitingUrlSet.pop()
    urlSet = set()
    success = True
    if( "/?v=" in waitingUrl ):
        videoInfo= {}
        success = ParseJavlibVideoHtml( waitingUrl, videoInfo, urlSet )
        if( success ):
            success = saveAV( videoInfo )
    else:
        success = ParseJavlibVideoListHtml( waitingUrl, urlSet )
    if( success ):
        finishedUrlSet.add( waitingUrl )
        urlSet -= finishedUrlSet
        waitingUrlSet |= urlSet
        # save urls.
        if( len(finishedUrlSet) % 100 == 0 ):
            SaveUrls( waitingUrlSet, waitingUrlFilePath)
            SaveFinishedUrls( finishedUrlSet, javlibLocalDir )
    else:
        errorUrlSet.add( waitingUrl )
        SaveUrls( errorUrlSet, errorUrlFilePath )
        time.sleep(30)
    time.sleep(1)   # 防止网站封ip。

SaveUrls( waitingUrlSet, waitingUrlFilePath)
SaveFinishedUrls( finishedUrlSet, javlibLocalDir )
