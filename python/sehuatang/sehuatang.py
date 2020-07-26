import os
import urllib.request
import logging
import time
import re
from bs4 import BeautifulSoup
import json
import socket
from concurrent.futures import ThreadPoolExecutor
import threading
from http.cookiejar import CookieJar

def mkdirs(path):
    if not os.path.isdir( path ):
        os.makedirs( path )

# 去掉不能做文件路径的非法字符.
def NormalizeName( name ):
    MaxNameLen = 128
    name = name[0:MaxNameLen] # 截断超长的部分. 要先截断,否则可能会在截断后在结尾出现空格.
    name = re.sub('[\\\/:*?"<>|\t\v\r\n]','-',name)#去掉非法字符
    name = re.sub('[\.\s]*$', '', name) # 去掉结尾的空白和.
    name = re.sub('^[\.\s]*', '', name) #去掉开始的空白和.
    name = name.replace( '\0', '' );    # 有的名称里面有0,必须去掉.
    return name

def isExcludeImg( url ):
    #'https://www.skeimg.com/u/20190728/10372746.gif'
    if url.find( 'https://www.skeimg.com/u/' ) == 0:
        return True
    imgName = url.split('/')[-1]
    excludeImgName = [
    'VZLGAs.gif',
    'ajPNS9.gif',
    '2018122101372595a.gif',
    'tzTS1P.gif',
    'bd7QzT.gif',
    'dD4BAAAAAAAA&bo=9AHIAPQByAACByM!&rf=viewer_4',
    '0hHxBnw.gif',
    '21873832.gif',
    'c9ad2b7860f27d71040780ca44b86efc.gif',
    'PZVXZL1.gif',
    '18e2e76df8d3223873167cbcfe9c51be.gif',
    '006fIiXMgw1f5a23xueecg30ao060qv5.gif',
    'ajPNS9.gif',
    '86.gif',
    'none.gif',
    'bd7QzT.gif',
    '16481395.jpg',
    '1586613712x1031866013.png'
    ]
    if imgName in excludeImgName:
        return True
    return False

        
class SeHuaTang:
    baseUrl = 'https://www.sehuatang.org'

    localBaseDir = 'H:/data/sehuatang'
    if not os.path.isdir( localBaseDir ):
        localBaseDir = '/share/disk-ssd/tmp/sehuatangtmp'

    # 华人自拍
    gqzwzmUrl = 'forum-98-<page>.html'
    localDir = os.path.join( localBaseDir, 'huarenzipai' )

    # 国产原创
    gqzwzmUrl = 'forum-2-<page>.html'
    localDir = os.path.join( localBaseDir, 'guochanyuanchuang' )
    
    # 高清字幕
    gqzwzmUrl = 'forum-103-<page>.html'
    localDir = os.path.join( localBaseDir, 'gqzm' )

    maxFilePerStoreDir = 5000   # 每个存储文件夹下最多的文件数目.

    #finishedUrl = set()
    finishedThreadId = set()
    threadPoolSaveThread = ThreadPoolExecutor(20)
    writeFileLock = threading.Lock()

    # 色花堂帖子url的格式是 thread-165716-1-10.html
    # 其中 165716 是帖子id
    # -1 是第一页
    # -10 是帖子位于列表的第十页.
    def getThreadId( self, url ):
        threadName = url.split('/')[-1]
        return threadName.split('-')[1]


    def getHeaders(self):
        headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36'}
        return headers  
    def urlGetContent(self, url):
        headers = self.getHeaders() #{'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
        
        req = urllib.request.Request(url=url, headers=headers)
        
        content = urllib.request.urlopen(req).read().decode('utf-8','ignore')#, 'ignore'
        return content

    
    def bsObjForm(self,url):
        for i in range (0,10):
            try:
                html = self.urlGetContent(url)
                bsObj1 = BeautifulSoup(html, "html.parser")
                return bsObj1
            except:
                logging.error( 'bsObjForm %s fail!', url )
                logging.exception("error")
                time.sleep(60)
        return None 

    def threadCrawl(self):
        pageNum = 1
        lastUrl = ''
        while( True ):
            pageUrl = self.gqzwzmUrl.replace('<page>', str(pageNum))
            url = self.baseUrl + '/' + pageUrl
            logging.info( 'Begin form page %d url: %s ', pageNum, url )
            pageNum += 1
            bs = self.bsObjForm(url)
            if bs.select('a.last'):
                lastUrl = bs.select('a.last')[0]['href']
            for a in bs.select('a.xst'):
                contentUrl = self.baseUrl+'/'+a['href']
                title = a.text
                if title == '想得到邀请码进':
                    continue
                if contentUrl == self.baseUrl + '/thread-164767-1-1.html':
                    continue
                if not self.isThreadStored( contentUrl ):
                    logging.info( 'Title %s url %s', title, contentUrl )
                    self.saveThread( contentUrl, title )
            if( lastUrl == pageUrl ):
                logging.info( 'Last page url: %s', lastUrl )
                break

    def downloadFileTry(self, url, localPath):
        for i in range(100):                
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

    def initFinishedUrls(self):
        viewBaseDir = os.path.join( self.localDir, 'view' )
        mkdirs( viewBaseDir )
        for dName in os.listdir( viewBaseDir ):
            urlFileName = 'urls-' + dName + '.txt'
            urlFilePath = os.path.join( viewBaseDir, dName, urlFileName)
            if os.path.isfile( urlFilePath ):
                urlFile = open( urlFilePath, "r")
                for line in urlFile.readlines():
                    #self.finishedUrl.add( line.strip() )
                    self.finishedThreadId.add( self.getThreadId(line) )
    
    def save2DirEasyView(self, threadInfo):
        title = threadInfo['title']
        title = normalizeName(title)
        logging.info( 'Save %s to dir easy to view'%(title,) )
        viewBaseDir = os.path.join( self.localDir, 'view' )
        mkdirs( viewBaseDir )
        nameList = os.listdir( viewBaseDir )
        
        # 当前文件夹.
        maxName = '1'
        if len(nameList) > 0:
            maxName = max(nameList)
        curDirIndex = int(maxName)
        curStoreDir = os.path.join( viewBaseDir, maxName )
        if( os.path.isdir(curStoreDir) and len(os.listdir(curStoreDir) ) > self.maxFilePerStoreDir ):
            curDirIndex += 1
            maxName = str(curDirIndex)
            curStoreDir = os.path.join( viewBaseDir, maxName )

        # 创建当前文件夹. 
        mkdirs( curStoreDir )

        # 图片.
        for imgSrc in threadInfo['img'] or []:
            imgOldName = imgSrc.split('/')[-1]
            imgFileName = title + '-' + imgOldName
            self.downloadFileTry( imgSrc, os.path.join(curStoreDir, imgFileName))
        
        self.writeFileLock.acquire()

        # url列表文件.
        urlListFilePath = os.path.join( curStoreDir, 'urls-' + maxName + '.txt' )
        urlFile = open( urlListFilePath, "a" )
        urlFile.write( threadInfo['url'] + '\n')
        urlFile.close()

        # 帖子正文.
        bodyTextFilePath = os.path.join( curStoreDir, 'info-' + maxName + '.txt')
        txtFile = open( bodyTextFilePath, "a", encoding='utf-8' )

        txtFile.write('\n--------------------------------------------------------------------------------\n')

        # 标题
        txtFile.write( title + '\n' )

        # 日期
        if( 'createTime' in threadInfo):
            txtFile.write( threadInfo['createTime'] + '\n')

        # url
        txtFile.write( threadInfo['url'] + '\n')

        # 正文和评论
        for txt in threadInfo['text'] or []:
            txtFile.write( txt )
            txtFile.write( '\n' )

        # 磁力链接
        if( 'magnet' in threadInfo ):
            for magnet in threadInfo['magnet']:
                txtFile.write( magnet )
                txtFile.write('\n')

        txtFile.write('\n--------------------------------------------------------------------------------\n')

        txtFile.close()

        self.writeFileLock.release()


    def save2DirStore(self, threadInfo):
        title = threadInfo['title']
        title = normalizeName( title )
        logging.info( 'Save %s to dir storage.'%(title,) )

        if( 'createTime' not in threadInfo):
            logging.error( '%s has no create time!'%(title,))
            return
        

        createTime = threadInfo['createTime']
        storeBaseDir = os.path.join( self.localDir, 'store', createTime[:7], title )

        mkdirs( storeBaseDir )
        
        #帖子名称.json 帖子信息
        jsonPath = os.path.join( storeBaseDir, title + '.json')
        jsonFile = open( jsonPath, 'w')
        json.dump( threadInfo, jsonFile )
        jsonFile.close()

        #            图片, 原名.
        for imgSrc in threadInfo['img'] or []:
            imgFileName = imgSrc.split('/')[-1]
            self.downloadFileTry( imgSrc, os.path.join(storeBaseDir, imgFileName))

        #            帖子名称.txt 帖子原文+评论
        txtPath = os.path.join( storeBaseDir, title + '.txt')
        txtFile = open( txtPath, 'w', encoding='utf-8')
        for txt in threadInfo['text'] or []:
            txtFile.write( txt )
            txtFile.write( '\n' )
        for magnet in threadInfo['magnet'] or []:
            txtFile.write( magnet )
            txtFile.write( '\n' )
        txtFile.close()

        #            种子文件 原名.
        for attachObj in threadInfo['attach'] or []:
            attachFileName = attachObj['name']
            attachUrl = attachObj['url']
            self.downloadFileTry( attachUrl, os.path.join(storeBaseDir, attachFileName))

        #            magnet.txt 磁链
        magnetPath = os.path.join( storeBaseDir, 'magnet.txt')
        magnetFile = open( magnetPath, 'w')
        for magnet in threadInfo['magnet'] or []:
            magnetFile.write( magnet )
            magnetFile.write( '\n' )
        magnetFile.close()


    


    def isThreadStored(self, url):
        threadId = self.getThreadId( url )
        return threadId in self.finishedThreadId
    
    def saveThread2LocalAsync(self, threadInfo ):
        self.threadPoolSaveThread.submit( self.saveThread2Local, threadInfo )
        logging.info( '%s submited to threadpool!', threadInfo['title'] )


    def saveThread2Local(self, threadInfo ):
        self.save2DirStore(threadInfo)
        self.save2DirEasyView(threadInfo)
        self.finishedThreadId.add( self.getThreadId(threadInfo['url']))
        logging.info( '%s work done!', threadInfo['title'] )


    def saveThread(self, threadUrl, title):
        bs = self.bsObjForm(threadUrl)
        threadInfo = {}
        #logging.info( '' )
        threadInfo['url'] = threadUrl
        threadInfo['title'] = title
        # 帖子文本和下面的评论内容.
        threadInfo['text'] = []
        for td in bs.select('td.t_f'):
            #logging.info( td.text )
            threadInfo['text'].append( td.text )
        # 图片,封面和预览等.
        if 'img' not in threadInfo:
                threadInfo['img'] = []
        for img in bs.select('img.zoom'):
            imgUrl = ''
            if 'file' in img.attrs:
                #logging.info( img['file'] )
                imgUrl = ( img['file'] )
                if not isExcludeImg( imgUrl ):
                    threadInfo['img'].append( imgUrl )

            if 'src' in img.attrs:
                #logging.info( img['src'] )
                imgUrl = ( img['src'] )
            if imgUrl != '':
                if not isExcludeImg( imgUrl ):
                    threadInfo['img'].append( imgUrl )
                continue
            logging.error('invalid img! %s', img )
        # 下载的磁力链接
        if 'magnet' not in threadInfo:
            threadInfo['magnet'] = []
        for li in bs.select('div.blockcode > div > ol > li'):
            code = li.text
            threadInfo['magnet'].append( code )
            #logging.info( code )
        # 种子文件链接(附件)
        threadInfo['attach'] = []
        for attach in bs.select( 'p.attnm > a'):
            attachObj = {}
            attachUrl = attach['href']
            attachObj['name'] = attach.text
            if attachUrl.find( 'http' ) == -1:
                attachUrl = self.baseUrl + '/' + attachUrl
            attachObj['url'] = attachUrl
            threadInfo['attach'].append( attachObj )
            #logging.info( attachUrl )
        # 帖子发表时间.
        for span in bs.select( 'div.authi > em > span'):
            createTime = span['title']
            if len(createTime) > 0:
                #logging.info( 'time:%s', createTime )
                threadInfo['createTime'] = createTime
                break
        if 'createTime' not in threadInfo:
            for em in bs.select( 'div.authi > em' ):
                # 发表于 2019-09-28 11:02:54
                emText = em.text
                if( emText.find( '发表于 ' ) == 0 ):
                    createTime = emText[len('发表于 '):]
                    threadInfo['createTime'] = createTime
                    break
        # 保存论坛到本地文件.
        #self.saveThread2Local( threadInfo )
        self.saveThread2LocalAsync( threadInfo )

def initLogging():
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式
    logFileName = time.strftime('./log/sht-%Y%m%d-%H%M%S.log',time.localtime())
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

def initRequest():
    proxyhandler = urllib.request.ProxyHandler({'http':'http://192.168.84.162:8118', 'https':'http://192.168.84.162:8118'})

    cj = CookieJar()
    #opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
    cookieHandler = urllib.request.HTTPCookieProcessor(cj)

    opener = urllib.request.build_opener( proxyhandler, cookieHandler )
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
    opener.addheaders = [('User-agent', 'Mozilla/5.0')]
    urllib.request.install_opener(opener)

if __name__=="__main__":
    initLogging()
    socket.setdefaulttimeout(100)
    initRequest()
    sht = SeHuaTang()
    sht.initFinishedUrls()
    sht.threadCrawl()
    logging.error( 'waitting file download finished!' )
    time.sleep(1*60*60)