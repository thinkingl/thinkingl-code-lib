from selenium import webdriver
import sys
import logging
import time
import os
import re
import urllib
import requests


MaxNameLen = 128
yiquanchaorenUrl = 'https://www.manhuabei.com/manhua/yiquanchaoren/'

commicDir = 'X:/commic'


def GetBrowser(PROXY_HOST, httpPort, ftpPort, sock5Port):
    fp = webdriver.FirefoxProfile()
    fp.set_preference("network.proxy.type", 1)
    fp.set_preference("network.proxy.http",PROXY_HOST)
    fp.set_preference("network.proxy.http_port",int( httpPort ))
    fp.set_preference("network.proxy.https",PROXY_HOST)
    fp.set_preference("network.proxy.https_port",int( httpPort ))
    fp.set_preference("network.proxy.ssl",PROXY_HOST)
    fp.set_preference("network.proxy.ssl_port",int( httpPort ))
    fp.set_preference("network.proxy.ftp", ftpPort)
    fp.set_preference("network.proxy.ftp_port",int(ftpPort))
    fp.set_preference("network.proxy.socks",PROXY_HOST)
    fp.set_preference("network.proxy.socks_port",int(sock5Port))
    fp.set_preference("general.useragent.override","Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194A")
    fp.update_preferences()
    return webdriver.Firefox(firefox_profile=fp)


def NormalizeName( name ):
    name = name[0:MaxNameLen] # 截断超长的部分. 要先截断,否则可能会在截断后在结尾出现空格.
    name = re.sub('[\\\/:*?"<>|\t\v\r\n]','-',name)#去掉非法字符
    name = re.sub('[\.\s]*$', '', name) # 去掉结尾的空白和.
    name = re.sub('^[\.\s]*', '', name) #去掉开始的空白和.
    name = name.replace( '\0', '' );    # 有的名称里面有0,必须去掉.
    return name


def initLogging():
    sys.stdout.reconfigure(encoding='utf-8')
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式

    if not os.path.isdir( 'logs' ):
        os.makedirs( 'logs' )

    logFileName = time.strftime('logs/manhualog-%Y%m%d-%H%M%S.log',time.localtime())
    
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

def downloadCommit( commitTitle, sectionName, url  ):
    logging.info( 'start download %s - %s - %s', commitTitle, sectionName, url)
    curPage = 1
    maxPage = 1000

    while( curPage<=maxPage ):
        # https://www.manhuabei.com/manhua/yiquanchaoren/483830.html?p=2  p=n为第n张图片网页的url
        pageUrl = url + '?p='+ str(curPage)
        browser.get( pageUrl )
        time.sleep(1)
        if maxPage == 1000:
            maxPageInfo = browser.find_element_by_css_selector( 'div#images p').text #'(1/26)'
            maxPageInfo = maxPageInfo.split( '/' )[-1]
            maxPage = (int)(maxPageInfo.split(')')[0])
            logging.info( 'max page: %d', maxPage )

        img = browser.find_element_by_css_selector( 'div#images img')
        imgUrl = img.get_attribute('src') # url
        imgIndex = img.get_attribute('data-index') # index
        fileName = imgIndex + '.jpg'

        fileDir = os.path.join( commicDir, NormalizeName(commitTitle), NormalizeName(sectionName) )
        if not os.path.isdir( fileDir ):
            os.makedirs( fileDir )

        filePath = os.path.join( commicDir, NormalizeName(commitTitle), NormalizeName(sectionName), fileName )
        #try:
        urllib.request.urlretrieve(imgUrl, filePath)        
        #except:
        #    logging.exception('error')
        #    logging.error( "img get fail unknown except! url: %s path: %s", imgUrl, filePath  )
        #    os.remove( filePath )
        curPage += 1

    logging.info( 'finished download %s - %s - %s', commitTitle, sectionName, url)


def parseCommicUrlList( browser, url ):
    logging.info( "url:" + url )
    browser.get( url )
    time.sleep(5)
    
    manhuaSectionList = browser.find_elements_by_css_selector('.zj_list')

    comicInfoList = list()

    for section in manhuaSectionList:
        try:
            title = section.find_element_by_css_selector('div h2').text
            logging.info( 'Find Section :' + title )

            eleAList = section.find_elements_by_css_selector('ul li a')
            sectionIndex = len(eleAList)
            for eleA in eleAList:
                url = eleA.get_attribute('href')
                name = eleA.text
                commic = {}
                commic['url'] = url
                commic['commicTitle'] = title
                commic['sectionName'] = name
                commic['index'] = sectionIndex
                sectionIndex -= 1
                logging.info( 'Find commit %s', commic )      
                comicInfoList.append( commic )      
        except:
            pass

    for commic in comicInfoList:
        url = commic['url']
        commicTitle = commic['commicTitle']
        sectionName = str(commic['index']) + '-' + commic['sectionName']
        fileDir = os.path.join( commicDir, NormalizeName(commicTitle), NormalizeName(sectionName) )
        if os.path.isdir(fileDir):
            logging.info( "commic %s - %s already exist!", commicTitle, sectionName )
            continue
        try:
            downloadCommit( commicTitle, sectionName, url )
        except:
            logging.exception( 'Download comic fail!' )
            os.rename( fileDir, fileDir+'-error' )
            logging.error( 'Download %s fail!', fileDir )

onePieceUrl = 'https://www.manhuabei.com/manhua/haizeiwang/'
if __name__=="__main__":
    initLogging()
    browser = GetBrowser("127.0.0.1", 8118, 2121, 1080)
    parseCommicUrlList( browser, yiquanchaorenUrl )
    parseCommicUrlList( browser, onePieceUrl )
