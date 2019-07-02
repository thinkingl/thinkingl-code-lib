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

javlibLocalDir = "d:/999-temp/javlib/"
waitingUrlFile = "waitting.txt"
finishedUrlFile = "finishedVideo.txt"
errorUrlFile = "error.txt"
dataDir = javlibLocalDir + "datas/"
dataDir2 = javlibLocalDir + "datas2/"

#fireFoxOptions = webdriver.FirefoxOptions()
#fireFoxOptions.set_headless()
#webdriver = Firefox(firefox_options=fireFoxOptions)
fp=webdriver.FirefoxProfile(r'C:\\Users\\thinkingl\\AppData\\Roaming\\Mozilla\\Firefox\\Profiles\\300ufwh8.default')  
browser = webdriver.Firefox()
#browser = webdriver.Chrome()

testUrl = "http://www.javlibrary.com/tw/"

try:
    f1 = open('cookie.txt','rb')
    #cookie = f1.read()
    cookie =pickle.load(f1)
    for c in cookie:
        browser.add_cookie(c)
    f1.close
except:
    print( "read cookie.txt fail!" )
   

browser.get(testUrl)
#for c in cookie:
#    webdriver.add_cookie(c)
#webdriver.refresh
WebDriverWait(browser, 10)


cookies = browser.get_cookies()
print( str(cookies) )

s = requests.session()
for cookie in cookies:
    s.cookies.set(cookie['name'],cookie['value'])

f1 = open('cookie.txt', 'wb')
f1.write(pickle.dumps(cookies))
f1.close

def GetPageSource( url ):
    #rep = s.get( url, timeout=100 )
    #pageSource = rep.text
    browser.get( url )
    pageSource = browser.page_source
    return pageSource

def ParseJavlibVideoHtml( videoUrl, videoInfo, urlSet ):
    print( "Parse video html:" + videoUrl)
    try:
        #headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
        #rep = requests.get( videoUrl, timeout=100, headers=headers )
        #fireFoxOptions = webdriver.FirefoxOptions()
        #fireFoxOptions.set_headless()
        #browser = webdriver.Firefox(firefox_options=fireFoxOptions)
        
        #webdriver.get( videoUrl )
        #pageSource = webdriver.page_source

        pageSource = GetPageSource( videoUrl )
        #rep = webdriver.request('GET', videoUrl)
    except Exception as err:
        print( "request url fail!:" + videoUrl )
        print( err )
        return False
    #print( rep )
    soup = BeautifulSoup(pageSource, 'html.parser')

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

    videoIdArray = soup.select("#video_id > table > tbody > tr > td")
    videoInfo["id"] = ''
    if( len( videoIdArray ) > 1 ):
        videoInfo["id"] = videoIdArray[1].text
        #print ( "videoId:\t" + videoInfo["videoId"])

    videoInfo["date"] =""
    videoDateArray = soup.select("#video_date > table > tbody > tr > td")
    if( len( videoDateArray ) > 1 ):
        videoInfo["date"] = videoDateArray[1].text
        #print ( "video date:\t" + videoDate)

    videoInfo["length"] = ""
    videoLengthArray = soup.select("#video_length > table > tbody > tr > td")
    if( len( videoLengthArray ) > 1 ):
        videoInfo["length"] = videoLengthArray[1].text
        #print ( "video length:\t" + videoLength)

    videoInfo["director"] = ""
    videoDirectorArray = soup.select("#video_director > table > tbody > tr > td")
    if( len( videoDirectorArray ) > 1 ):
        videoInfo["director"] = videoDirectorArray[1].text
        #print ( "video director:\t" + videoDirector.text)

    videoDirectorUrlArray = soup.select("#video_director > table > tbody > tr > td a[href]")
    for videoDirectorUrl in videoDirectorUrlArray:
        url = urljoin( videoUrl, videoDirectorUrl.attrs["href"])
        urlSet.add( url )
        #print( "video director url:\t" + url )

    videoInfo["maker"] = ""
    videoMakerArray = soup.select("#video_maker > table > tbody > tr > td a[href]")
    if( len( videoMakerArray ) > 0 ):
        videoMaker = videoMakerArray[0]
        #print ( "video maker:\t" + videoMaker.text)
        videoInfo["maker"] = videoMaker.text
        url = urljoin( videoUrl, videoMaker.attrs["href"])
        urlSet.add( url )
        #print( "video maker url:\t" + url)

    videoLabelArray = soup.select("#video_label > table > tbody > tr > td a[href]")
    videoInfo["label"] = ""
    for videoLabel in videoLabelArray:
        #print ( "video label:\t" + videoLabel.text)
        videoInfo["label"] += videoLabel.text
        url = urljoin( videoUrl, videoLabel.attrs["href"])
        urlSet.add( url )
        #print( "video label url:\t" + url)

    videoReviewArray = soup.select("#video_review > table > tbody > tr > td > span")
    videoInfo["review"]=''
    for videoReview in videoReviewArray:
        #print ( "video review:\t" + videoReview.text)
        videoInfo["review"]=videoReview.text

    videoGenresArray = soup.select("#video_genres > table > tbody > tr > td a[href]")
    videoInfo["genres"] = ""
    for videoGenres in videoGenresArray:
        #print ( "video genres:\t" + videoGenres.text)
        videoInfo["genres"] += videoGenres.text
        videoInfo["genres"] += "    "
        url = urljoin( videoUrl, videoGenres.attrs["href"])
        urlSet.add( url )
        #print( "video genres url:\t"+url)

    videoCastNameArray = soup.select("#video_cast > table > tbody > tr > td")
    videoInfo["cast"] = ""
    if( len( videoCastNameArray ) > 1 ):
        print ( "video cast:\t" + videoCastNameArray[1].text)
        videoInfo["cast"] += videoCastNameArray[1].text
        videoInfo["cast"] += "    "
    videoInfo["cast"] = videoInfo["cast"].strip()
    if( len(videoInfo["cast"]) == 0 ):
        videoInfo["cast"]   = "anonymous"

    videoCastUrlArray = soup.select( "#video_cast > table > tbody > tr > td a[href]")
    for videoCastUrl in videoCastUrlArray:
        url = urljoin( videoUrl, videoCastUrl.attrs["href"])
        urlSet.add( url )
        #print( "video cast url:\t" + url)

    # image.
    videoInfo["imgFileName"] = ""
    imgArray = soup.select("#video_jacket_img")
    if( len( imgArray ) > 0 ):
        url = urljoin( videoUrl, imgArray[0].attrs["src"])
        print( "img url 1:\t"+ url )
        imgFileName = videoInfo["date"] + " " + videoInfo["id"] + ".jpg"
        imgFilePath = dataDir + imgFileName
        videoInfo["imgFileName"] = imgFileName
        try:
            urllib.request.urlretrieve(url, imgFilePath)
        except urllib.error.HTTPError as err:
            if os.path.isfile( imgFilePath ):
                os.remove( imgFilePath )
            print( "img get fail! url:\t"+url  )
            print( err )
            success = False
            return success
        except:
            if os.path.isfile( imgFilePath ):
                os.remove( imgFilePath )
            return False

        imgFileDir2 = dataDir2 + videoInfo["cast"] + "/"
        if( len( imgFileDir2) > 200 ):
                imgFileDir2 = imgFileDir2[:200] + "/"
        try:
            os.mkdir( imgFileDir2 )
        except FileExistsError :
            #print( imgFileDir2 + "already exist!" )
            pass

        try:
            imgFilePath2 = imgFileDir2 + imgFileName
            urllib.request.urlretrieve(url, imgFilePath2)
        except FileNotFoundError:
            print( "invalid dir:\t"+imgFileDir2)
        except urllib.error.HTTPError:
            print( "img get fail! url:\t"+url )
            if os.path.isfile( imgFilePath2 ):
                os.remove( imgFilePath2 )
            success = False
            return success
        except:
            if os.path.isfile( imgFilePath2 ):
                os.remove( imgFilePath2 )
            return False

    if( len( videoInfo["id"]) == 0 ):
         success = False
    print( json.dumps( videoInfo ) )
    return success

def ParseJavlibVideoListHtml( videoListUrl, newUrls ):
    print( "parse video list url:\t" + videoListUrl )
    try:
        #rep = requests.get( videoListUrl, timeout=10 )
        #rep = webdriver.request( 'GET', videoListUrl, timeout=10)
        pageSource = GetPageSource( videoListUrl )
    except:
        print( "request url fail!:" + videoListUrl )
        return False

    soup = BeautifulSoup(pageSource, "html.parser")
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
        print( "file not found" )

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
    filePath = dataDir + fileName
    with open( filePath, "w", encoding='utf-8' ) as f:
        f.write( wikiStr )
    f.close()

    dir2 = dataDir2 + videoInfo["cast"] + "/"
    if( len( dir2) > 200 ):
            dir2 = dir2[:200] + "/"
    path2 = dir2 + fileName

    try:
        os.mkdir( dir2 )
    except FileExistsError:
        #print( "dir [" + dir2 + "already exist" )
        pass

    shutil.copyfile( filePath, path2 )

    fileName = videoInfo["date"] + " " + videoInfo["id"] + ".json"
    filePath = dataDir + fileName
    with open( filePath, "w", encoding='utf-8' ) as f:
        json.dump( videoInfo, f )
    f.close()

    path2 = dir2 + fileName
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
urlBeginning = "http://www.javlibrary.com/tw/?v=javli7yiym"

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
            SaveVideoInfo( videoInfo )
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

SaveUrls( waitingUrlSet, waitingUrlFilePath)
SaveFinishedUrls( finishedUrlSet, javlibLocalDir )
