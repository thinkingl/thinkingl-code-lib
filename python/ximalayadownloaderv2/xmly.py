#!/usr/bin/python
# -*- coding: UTF-8 -*-

import db
import re
import logging
import urllib.request
from urllib.parse import quote
from bs4 import BeautifulSoup
import json
import os
import time
import socket
from queue import Queue
import threading
import xmSign
import psutil

class XMLYDownloader:
    #等待处理的url
    waittingUrls = list()

    #数据库
    database = db.XMLYDatabase()

    # xmSign.
    xmSign = xmSign.XMSign()


    baseDirs = ['D:/999-temp/xmly','/share/disk-160/xmly']
    baseDir = ''
    DB_FILE_NAME = 'xmly.db'
    threadNum = 5

    # 构造.
    def __init__(self):
        self.ReadUrl()
        logging.info( 'Read url num:%s'% len(self.waittingUrls) )

        for dir in self.baseDirs:
            if os.path.isdir( dir ):
                self.baseDir = dir
                logging.warning('Use base dir %s'%self.baseDir)
                break
        if( self.baseDir == '' ):
            logging.error("Can't find base dir!")
        self.database = self.newDatabase()

        self.xmSign.init()

    def getHeaders(self):
        headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
        headers['xm-Sign']  = self.xmSign.getXMSign()
        return headers      
    
    def newDatabase(self):
        database = db.XMLYDatabase()
        dbPath = os.path.join( self.baseDir, self.DB_FILE_NAME )
        try:
            database.init(dbPath)
        except:
            logging.exception("error")
        else:
            logging.debug('Data base path:' + dbPath)
        return database

    def run(self):
        totalDownloadNum = 0
        downloadTrackNum = 0
        while( len(self.waittingUrls)>0 and not os.path.isfile( 'stop.flag')):
            try:
                downloadNum = 0
                url = self.waittingUrls.pop()
                url = url.strip()
                
                albums = self.parseUrl( url )
                for album in albums:
                    downloadNum += self.downloadAlbum( album )
                    if os.path.isfile( 'stop.flag' ):
                        logging.error( 'Fand stop.flag, stop downloading!' )
                        break

                # 暂时先用这个逻辑....不完美.
                downloadTrackNum += downloadNum
                if( downloadTrackNum > 0 and len(self.waittingUrls) == 0 ):
                    self.ReadUrl() # 放开的话会再来一遍.
                    downloadTrackNum = 0

                totalDownloadNum += downloadNum
                logging.info( 'download over !Cur turn download track: %d total: %d url: %s' % (downloadTrackNum, totalDownloadNum,url) )
            except Exception as e:
                logging.error(e)
                logging.exception("message")
                pass
        print( "Donload over! download tracks total num:" , totalDownloadNum )

    # 打开文件读取url.
    def ReadUrl(self ):
        FileName = 'urls.txt'
        f = open(FileName, 'rb')
        while(True):
            url = f.readline().decode('utf-8')
            self.waittingUrls.append( url )
            if( len(url)==0):
                break
        f.close()

    # 现在音频线程.
    def threadDownloadTrack(self, trackTaskQueue, trackTaskResultQueue ):
        newDatabase = self.newDatabase()
        while( True ):
            if os.path.isfile( 'stop.flag' ):
                logging.error( 'Fand stop.flag, stop downloading!' )
                return
            try:
                
                task = trackTaskQueue.get_nowait()
            except:
                break
            result = self.downloadTrack(task[0], task[1], task[2], newDatabase)
            if result:
                trackTaskResultQueue.put_nowait( (result, task[0], task[1], task[2]) )
        newDatabase.close()
    
     # 下载一张专辑
    def downloadAlbum(self,id):
        logging.info( "download album %s", id )
        infos = self.getAlbumAndAnchorInfo(id)
        if( infos == None ):
            return 0
        albumInfo = infos[0]
        albumInfo['id'] = id    #没有id, 补充上.
        #if( albumInfo ):
            #self.database.updateAlbum(id, albumInfo)
        anchorInfo = infos[1]
        if( anchorInfo ):
            anchorId = anchorInfo['anchorId']
            self.database.updateAnchor(anchorId, anchorInfo)
        tracksInfo = infos[2]

        albumInfo['anchorId'] = anchorId # 专辑信息补充上主播id
        trackNum = tracksInfo['trackTotalCount']
        albumTitle = albumInfo['albumTitle']

        successDownloadTrackNum = 0
        if( self.isAlbumNeedDownload( id, albumInfo, tracksInfo ) ):
            trackIdList = self.getTrackList(id)
            trackTaskQueue = Queue()
            for trackInfo in trackIdList:
                # 任务放入队列.
                trackTaskQueue.put_nowait( (trackInfo, albumInfo, anchorInfo) )
                if os.path.isfile( 'stop.flag' ):
                    logging.error( 'Fand stop.flag, stop downloading!' )
                    break
                #if( self.downloadTrack( trackInfo, albumInfo, anchorInfo ) ):
                #    successDownloadTrackNum += 1
            trackTaskResultQueue = Queue()
            # 开启多个线程下载track.
            threads = []
            for i in range( self.threadNum ) :
                t = threading.Thread(target=self.threadDownloadTrack, args=(trackTaskQueue, trackTaskResultQueue))
                threads.append(t)
                t.start()
            for t in threads:
                t.join()
            successDownloadTrackNum = trackTaskResultQueue.qsize()

            if( successDownloadTrackNum >= trackNum ):
                logging.warning('Album %s finished! tracks num:%d', albumTitle, trackNum)
                self.database.updateAlbum(id, albumInfo)
            
            # 下载完一张专辑后, 调用命令设置目录的权限. 否则linux下无法剪切文件夹.
            if self.baseDir[0] == '/':
                chmodPath = os.path.join( self.baseDir, '*' )
                os.system('chmod 777 %s -R'%chmodPath)
        return successDownloadTrackNum

    # 解析专辑信息包括音频url
    def getAlbumAndAnchorInfo(self, id ):
        url = 'https://www.ximalaya.com/revision/album?albumId=%s'%id
        strAlbumInfo = self.urlGetContent( url )
        #logging.info( "Album id %s info: %s", id, strAlbumInfo)
        if len(strAlbumInfo) == 0:
            return None
        rawAlbumInfo = json.loads( strAlbumInfo )
        if( not 'data' in rawAlbumInfo ):
            logging.error( 'Get album info fail! id:%s url:%s return:%s', id, url, rawAlbumInfo )
            return None
        mainInfo = rawAlbumInfo['data']['mainInfo']
        anchorInfo = rawAlbumInfo['data']['anchorInfo']
        albumInfo = mainInfo
        trackInfo = rawAlbumInfo['data']['tracksInfo']
        return (albumInfo, anchorInfo,trackInfo)

    # 处理一个url
    def parseUrl(self, url ):
        if( re.search( '/zhubo/', url) ):
            return self.parseZhubo(url)
        
        albumId = self.parseId(url)
        if( albumId != '' ):
            return [albumId]

        reKey = re.search( '(?<=key=).*(?=&)', url )
        if( reKey ):
            key = reKey.group()
            return self.searchAlbumByKeyword(key)

        # https://www.ximalaya.com/revision/category/queryCategoryPageAlbums?category=youshengshu&subcategory=yingwenyuanban&meta=132_2722&sort=0&page=1&perPage=300
        if re.search( '/queryCategoryPageAlbums', url ):
            return self.parseAlbumByCategorySearchUrl(url)

        # https://www.ximalaya.com/youshengshu/shangye/mr132t2722/
        if re.search(  '(?<=www.ximalaya.com/)\D+?/\w+?(?=/)', url ):
            return self.parseAlbumByCategory(url)
            

        return []

    # 分类查找专辑.
    def parseAlbumByCategory(self, url):
        try:
            match = re.search(  '(?<=www.ximalaya.com/)\D+?/\w+?(?=/)', url ).group()
            categorys = match.split( '/' )
            category = categorys[0]
            subCategory = categorys[1]
            url = 'https://www.ximalaya.com/revision/category/queryCategoryPageAlbums?category=%s&subcategory=%s&meta=132_2722&sort=0&page=1&perPage=300'%(category,subCategory)
            return self.parseAlbumByCategorySearchUrl( url )
        except:
            logging.exception( 'error' )
            logging.error( 'parseAlbumByCategory fail! url:%s', url )
            return []

    # 通过分类查找专辑.
    def parseAlbumByCategorySearchUrl(self, url):
        pageNum = 1
        albums = []
        try:
            while( True ):
                curUrl = re.sub('(?<=page=)\d+(?=&perPage)', str(pageNum), url)
                pageNum += 1
                content = self.urlGetContent( curUrl )
                rsp = json.loads(content)
                if 'data' not in rsp:
                    break
                albumInfos = rsp['data']['albums']
                if len( albumInfos ) == 0 :
                    break
                for albumInfo in albumInfos:
                    id = albumInfo['albumId']
                    title = albumInfo[ 'title' ]
                    albums.append(id)
                    logging.info( 'Find album by category! %s - %s', id, title)
        except:
            logging.exception( 'error')
            logging.error( 'parse Category fail! url:%s pageNum:%d', url, pageNum)

        return albums
    
    # 解析关键字.
    def searchAlbumByKeyword(self, key):
        if( len(key) == 0 ):
            return []
        kw = urllib.parse.quote(key)
        page = 1
        albums = []
        while( True ):
            url = "https://www.ximalaya.com/revision/search?core=album&kw={}&page={}&spellchecker=false&rows=20&condition=relation&device=iPhone&fq=&paidFilter=true".format(kw,page)
            page += 1
            content = self.urlGetContent(url)
            try:
                searchRsp = json.loads(content)
                docs = searchRsp['data']['result']['response']['docs']
                if( len(docs) == 0 ):
                    break
                for doc in docs:
                    albumId = doc['id']
                    albumTitle = doc['title']
                    isPaid = doc['is_paid']
                    anchorId = doc['uid']
                    anchorName = doc['nickname']
                    if not isPaid:
                        albums.append( albumId )
                    logging.info( 'Result for %s : %s - %s anchor: %s - %s isPaid:%s', key, albumTitle, albumId, anchorName, anchorId, isPaid )
            except:
                logging.error( 'Search keyword fail! key:%s page:%d rsp:%s url:%s', key, page, content, url )
                break
        return albums


    # 处理一个主播页面.
    def parseZhubo(self, url):
        zhuboId = self.parseId(url)
        logging.info( "parse url:" + url + " zhubo id:" + zhuboId)
        if( zhuboId == None or len(zhuboId) == 0 ):
            logging.error( 'Invalid zhuboId url:%s', url )
            return []
        albumIds = []

        pageIndex = 1
        while( True ):
            albumListUrl = 'https://www.ximalaya.com/revision/user/pub?page=%d&pageSize=50&keyWord=&uid=%s'%(pageIndex,zhuboId)
            # https://www.ximalaya.com/zhubo/9677803/album/
            content = self.urlGetContent( albumListUrl )
            albumListRsp = json.loads( content )
            if not 'data' in albumListRsp:
                logging.error( "Can't find data in album list. url:%s ret:%s", albumListUrl, content )
                break
            data = albumListRsp['data']
            if not 'albumList' in data:
                logging.error( "Can't find albumList in album list. url:%s content:%s", albumListUrl, content)
                break
            albumList = data['albumList']
            if( len(albumList) == 0 ):
                logging.debug( "Anchor's album list query finished! album list:%s", albumIds )
                break
            for albumInfo in data['albumList']:
                albumId = albumInfo['id']
                albumTitle = albumInfo['title']
                anchorName = albumInfo['anchorNickName']
                albumIds.append(albumId)
                logging.info( "Find album %s - %s of anchor %s - %s", albumId, albumTitle, zhuboId, anchorName)
            pageIndex += 1
        return albumIds

    def parseZhuboV1(self, url):
        zhuboId = self.parseId(url)
        logging.info( "parse url:" + url + " zhubo id:" + zhuboId)

        zhuboUrl = 'http://www.ximalaya.com/zhubo/%s/'%zhuboId

        albums = []
        bsObj = self.bsObjForm( zhuboUrl )
        if( bsObj ):
            pageArray = bsObj.select(".album-title")
            for pageObj in pageArray:
                pageUrl = pageObj.attrs['href']
                albumId = self.parseId( pageUrl )
                albums.append(albumId)
        return albums

 
    # 下载一个音频
    def downloadTrack(self, trackInfo, albumInfo, anchorInfo, database):
        trackId = trackInfo['trackId']
        trackTitle = trackInfo['title']
        isPaid = trackInfo['isPaid']
        albumId = albumInfo['id']
        if database == None:
            database = self.database
        storeTrackInfo = database.getTrack(trackId)
        if( len(storeTrackInfo) != 0 ):
            logging.debug("track %s - %s has downloaded!"%(trackId,trackTitle) )
            if( 'albumId' not in storeTrackInfo ):
                storeTrackInfo[ 'albumId' ]  = albumId
                database.finishTrack( trackId, storeTrackInfo )                
            return True
        url = 'http://www.ximalaya.com/tracks/%s.json'%trackId
        content = self.urlGetContent(url)
        trackDetail = json.loads(content)
        trackInfo.update(trackDetail)
        trackInfo[ 'albumId' ] = albumId
        if albumId != trackInfo['album_id']:
            logging.error( 'Track %s - %s is 转采! owner albumId:%s true albumId:%s', trackId, trackTitle, albumId, trackInfo['album_id'] )
        trackM4aUrl = trackInfo['play_path']
        anchorName = anchorInfo['anchorName']
        trackIndex = trackInfo['index']

        trackPath = self.getLocalPath( anchorInfo, albumInfo, trackInfo, 'track')
        trackCoverPath = self.getLocalPath( anchorInfo, albumInfo, trackInfo, 'trackCover')
        albumCoverPath = self.getLocalPath(anchorInfo, albumInfo, trackInfo, 'albumCover')
        anchorCoverPath = self.getLocalPath(anchorInfo, albumInfo, trackInfo, 'anchorCover')

        result = True
        result &= self.downloadFile( trackInfo['play_path'], trackPath)
        if trackInfo['cover_url'] != None:
            result &= self.downloadFile( trackInfo['cover_url'], trackCoverPath)
        if not os.path.isfile( albumCoverPath ):
            result &= self.downloadFile( albumInfo['cover'], albumCoverPath)
        if not os.path.isfile( anchorCoverPath ):
            result &= self.downloadFile( anchorInfo['anchorCover'], anchorCoverPath)

        # 保存到数据库..
        if result:
            database.finishTrack( trackId, trackInfo )

        return result
    
    # 下载一个文件
    def downloadFile(self, url, localPath):
        if( url[0:2] == '//'):
            url = 'http:' + url
        pdir = os.path.dirname( localPath )
        os.makedirs( pdir, 0o777, True)


        while psutil.disk_usage( pdir ).free < 100*1024*1024 :  #100M
            logging.error( 'No enough space!' )
            time.sleep(1*60)

        for i in range(10):                
            try:
                urllib.request.urlretrieve( url, localPath)
                logging.info( 'downlaod file success! [%s] - [%s]', localPath, url)
                return True
            except:
                logging.error( "download file fail! [%s] - [%s]", url, localPath)
                if os.path.isfile( localPath ):
                    os.remove( localPath )
                time.sleep(1)
        return False


    # 获取本地路径.
    def getLocalPath(self, anchorInfo, albumInfo, trackInfo, fileType ):
        anchorName = self.normalizeName( anchorInfo['anchorName'] )
        albumTitle = self.normalizeName( albumInfo['albumTitle'] )
        trackTitle = self.normalizeName( trackInfo['title'])
        trackIndex = trackInfo['index']

        trackFileName = '%d-%s'%(trackIndex,trackTitle)

        filePath = os.path.join(self.baseDir, anchorName, albumTitle )
        if fileType == 'albumCover':
            coverUrl = albumInfo['cover']
            fileExt = coverUrl.split('.')[-1]
            filePath = os.path.join(self.baseDir, anchorName, albumTitle, albumTitle+'.'+fileExt )
        
        if fileType == 'trackCover':
            coverUrl = trackInfo['cover_url']
            if( coverUrl == None ):
                return ''
            fileExt = coverUrl.split('.')[-1]
            filePath = os.path.join(self.baseDir, anchorName, albumTitle, trackFileName+'.'+fileExt )
        if fileType == 'track':
            trackM4aUrl = trackInfo['play_path']
            fileExt = trackM4aUrl.split('.')[-1]
            filePath = os.path.join(self.baseDir, anchorName, albumTitle, trackFileName+'.'+fileExt )
        if fileType == 'anchorCover':
            coverUrl = anchorInfo['anchorCover']
            fileExt = coverUrl.split('.')[-1]
            filePath = os.path.join(self.baseDir, anchorName, anchorName+'.'+fileExt)
        return filePath

    
    # 去掉不能做文件路径的非法字符.
    def normalizeName(self, name ):
        name = re.sub('[\\\/:*?"<>|]','-',name)#去掉非法字符
        name = name[0:64] # 截断超长的部分
        return name

    # 专辑是否还需要下载.
    def isAlbumNeedDownload(self, albumId, curAlbumInfo, tracksInfo ):
        #付费专辑不下载.
        isPaid = curAlbumInfo['isPaid']
        if( isPaid == True ):
            logging.info( 'album %s %s need money!'%(albumId, curAlbumInfo['albumTitle']))
            return False
        #不公开的
        isPublic = curAlbumInfo['isPublic']
        if( isPublic == False ):
            logging.info( 'album %s %s is not public!'%(albumId, curAlbumInfo['albumTitle']))
            return False
        #下载完成的.
        storeAlbumInfo = self.database.getAlbum(albumId)
        if( 'updateDate' not in storeAlbumInfo ):
            logging.debug( 'album %s %s hs no update date!'%(albumId, curAlbumInfo['albumTitle']) )
            return True
        lastUpdateTime = curAlbumInfo['updateDate']
        storeUpdateTime = storeAlbumInfo['updateDate']
        if( lastUpdateTime != storeUpdateTime ):
            logging.info( 'album %s %s was updated at %s!'%(albumId, curAlbumInfo['albumTitle'], lastUpdateTime) )
            return True
        
        #Track是否完整.
        curTrackNum = tracksInfo['trackTotalCount']
        storeTrackList = self.database.getTrackList( albumId )
        if( len(storeTrackList) < curTrackNum ):
            logging.info( 'album %s %s has more tracks %d stored:%d!'%(albumId, curAlbumInfo['albumTitle'], curTrackNum, len(storeTrackList)) )
            return True

        return False

    # 获取专辑的Track列表
    def getTrackList(self, albumId):
        trackList = []
        pageNum = 1
        while( True ):
            url = 'https://www.ximalaya.com/revision/album/v1/getTracksList?albumId=%s&pageNum=%d'%(albumId,pageNum)
            content = self.urlGetContent( url )
            pageInfo = json.loads(content)
            trackInPage = pageInfo['data']['tracks']
            if( len(trackInPage) == 0 ):
                break
            trackList += trackInPage
            pageNum += 1
        return trackList

    # 解析Id出来.
    def parseId(self,url):
        id = ''
        try:
            id = re.search('(?<=/)\d+(?=/)',url).group()
        except:
            pass
        logging.debug("parse id %s from url: %s", id, url )
        return id

    def urlGetContent(self, url):
        headers = self.getHeaders() #{'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
        try:
            req = urllib.request.Request(url=url, headers=headers)
            content = urllib.request.urlopen(req).read().decode('utf-8','ignore')#, 'ignore'
            return content
        except:
            logging.exception("error")
            return ''
    
    def bsObjForm(self,url):
        try:
            html = self.urlGetContent(url)
            bsObj1 = BeautifulSoup(html, "html.parser")
            return bsObj1
        except:
            logging.exception("error")
            return 

def initLogging():
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式
    logFileName = time.strftime('xmlylog-%Y%m%d-%H%M%S.log',time.localtime())
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

if __name__=="__main__":
    socket.setdefaulttimeout(100)

    
    initLogging()
    logging.info( 'Start XMLY Downloader!' )
    downloader = XMLYDownloader() 
    downloader.run()
    if os.path.isfile( 'stop.flag'):
        os.rename( 'stop.flag', 'stop0.flag' )