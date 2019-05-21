# -*- coding: utf-8 -*-
#! /usr/bin/env python3

import json
import os
import re
import urllib.request
from bs4 import BeautifulSoup
import urljoin
from math import log10
import socket
import logging
import shutil

LocalDir = "D:\\999-temp\\ximalaya\\"

urlSetFinishedPages = set()
urlSetUnfinishedPages = set()
urlSetErrorPages = set()
urlSetErrorFiles = set()

socket.setdefaulttimeout(100)

def bsObjForm(url):
    print("Request url:" + url)
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
    req = urllib.request.Request(url=url, headers=headers)
    try:
        html = urllib.request.urlopen(req).read().decode('utf-8','ignore')#, 'ignore'
        bsObj1 = BeautifulSoup(html, "html.parser")
        return bsObj1
    except:
        return


def getAlbumFolder(albumTitle, albumAnchor):
    albumTitle= re.sub('[\/:*?"<>|]','_',albumTitle)
    albumAnchor= re.sub('[\/:*?"<>|]','_',albumAnchor)
    folderPath = LocalDir+ albumTitle
    
    if( albumAnchor != None and albumAnchor != ""):
        folderPath = folderPath + '-' + albumAnchor
    folderPath = folderPath + '/'
    return folderPath

def getTrackLocalPath( albumFolderPath, trackTitle, trackIndex, fileExt ):
    albumFolder = albumFolderPath
    trackTitle= re.sub('[\/:*?"<>|]','_',trackTitle)
    if fileExt == "":
        fileExt = ".m4a"
    article_path= albumFolder+ trackIndex + "-" + trackTitle+ fileExt
    return  article_path

def isTrackExist( albumFolderPath, trackTitle, trackIndex, fileExt ):
    articalPath = getTrackLocalPath( albumFolderPath, trackTitle, trackIndex, fileExt )
    if  os.path.exists(articalPath):
        #print('%s 文件已经存在'%articalPath)
        return  True
    else:
        return False

def Schedule(a,b,c):
    '''''
    a:已经下载的数据块
    b:数据块的大小
    c:远程文件的大小
   '''
    per = 100.0 * a * b / c
    if per > 100 :
        per = 100
    #print ('%.2f%%' % per,'已下载的大小:',a*b,'文件大小:',c)
    #print '已经下载的数据块:',a#,'\n'
    #print '数据块的大小:',b#,'\n'
    #print '远程文件大小:',c,'\n'
    #print '已下载的大小:',a*b,'文件大小:',c

#获得专辑信息，分析音频文件名称，所属专辑，下载地址，并将文件保存在本地
def getM4a(url, trackIndex,albumFolderPath):
    print( "Get track file url:" + url + " index:" + trackIndex )
    bsObj = bsObjForm(url)
    soundInfo=bsObj.text
    try:
        jsonStr = json.loads(soundInfo)
    except Exception as e1:
        print( "load json fail! str:" + soundInfo + " url:" + url  )
        logging.exception("message")
        newJson = "{%s,%s,%s}"%( re.search( '"album_title":".*?"', soundInfo ).group(), re.search( '"play_path":".*?"', soundInfo ).group(), re.search( '"title":".*?"', soundInfo ).group() )
        jsonStr = json.loads(newJson)

    #album_title=jsonStr['album_title']
    play_path=jsonStr["play_path"]
    title=jsonStr["title"]

    if (play_path == 'null'):
        print( "Play path is null! title:" + title )
        return

    fileExt = os.path.splitext(play_path)[1]
    if( fileExt == "" ):
        fileExt = ".m4a"

    albumFolder = albumFolderPath  
    print(albumFolder,play_path,title)
    #将单曲写入文件
    #判断文件夹是否存在，如果不存在，则新建文件夹
    if os.path.exists( albumFolder)==False:
        os.makedirs(albumFolder)

    article_path= getTrackLocalPath( albumFolderPath, title, trackIndex, fileExt)

    print( "m4a url:" + play_path + "\tlocal path:" + article_path )

    #判断文件是否存在，如果存在，则不进行下载
    if  os.path.exists(article_path):
        print('%s 文件已经存在'%title)
    else:
        try:
            urllib.request.urlretrieve(play_path,article_path,Schedule)
        except:
            print( "request url fail! [%s] - [%s]"% (article_path,play_path))
            os.remove( article_path )
            urlSetErrorFiles.add( play_path )

def getAlbumPages( soap, urlSetFinishedPages, urlSetPages ):
    pageArray = soap.select(".page-link")
    for pageObj in pageArray:
        pageUrl = pageObj.attrs['href']
        urlSetPages.add(pageUrl)
        #print( "add page url:" + pageUrl )
        urlSetPages -= urlSetFinishedPages

def getTrackSoundUrl( trackId, trackIndex, albumFolderPath ):
    trackInfoUrl = path_url='http://www.ximalaya.com/tracks/'+trackId+'.json' #"http://www.ximalaya.com/revision/play/tracks?trackIds=" + trackId;
    getM4a( trackInfoUrl, trackIndex,albumFolderPath )

#分析所属专辑内各文件地址
def getAlbumPageInfo( bsObj ):
    scriptList = bsObj.select('html > body > script')
    for scriptObj in scriptList:
        if( scriptObj.text.find("window.__INITIAL_STATE__ = ") != -1 ):
            albumJsonStr = re.findall("{.*}}", scriptObj.text)[0]
            albumJson = json.loads( albumJsonStr )
            return albumJson
    return

def saveAlbumCover( albumCoverImgUrl, albumFolderPath ):
    if( albumCoverImgUrl[0:5] != 'http:'):
        albumCoverImgUrl = "http:" + albumCoverImgUrl
    fileExt = os.path.splitext(albumCoverImgUrl)[1]
    if( fileExt == "" ):
        fileExt = ".jpg"
    folderPath = albumFolderPath
    coverImgPath = folderPath + "cover" + fileExt
    if os.path.exists( folderPath )==False:
        os.makedirs(folderPath)
    if( not os.path.exists(coverImgPath)):
        print( "save cover %s"%coverImgPath )
        urllib.request.urlretrieve( albumCoverImgUrl, coverImgPath )

# 解析专辑列表
def ParseZhubo( zhuboUrl ):
    albumUrlSet = set()
    bsObj = bsObjForm( zhuboUrl )
    pageArray = bsObj.select(".album-title");
    for pageObj in pageArray:
        pageUrl = pageObj.attrs['href']
        albumUrlSet.add(pageUrl)
    return albumUrlSet


baseUrl = "http://www.ximalaya.com"
beginUrl = "/xiangsheng/9723091/"


def DownloadPages( urlSetUnfinishedPages, urlSetFinishedPages, urlSetErrorPages, urlSetErrorFiles ):
    while( len(urlSetUnfinishedPages) > 0 ):
        #print( "unfinished pages:" + str(urlSetUnfinishedPages) )
        unfinishedUrl = urlSetUnfinishedPages.pop()
        if unfinishedUrl == '':
            continue
        fullUrl = unfinishedUrl
        if( unfinishedUrl.find( "http://") != 0 ):
            fullUrl =  baseUrl + unfinishedUrl #urljoin.url_path_join( baseUrl, unfinishedUrl )

        if( re.search( '/zhubo/', fullUrl) ):
            albumUrlSet = ParseZhubo( fullUrl )
            print( "Parse zhubo url:" + fullUrl )
            print( str( albumUrlSet) )
            urlSetUnfinishedPages =  urlSetUnfinishedPages.union( albumUrlSet )
            urlSetUnfinishedPages -= urlSetFinishedPages
            continue

        bsObj = bsObjForm( fullUrl )
        if bsObj == None:
            urlSetErrorPages.add(unfinishedUrl)
            continue
        getAlbumPages( bsObj, urlSetFinishedPages, urlSetUnfinishedPages )
        albumJson = getAlbumPageInfo( bsObj )
        if( albumJson == None ):
            urlSetErrorPages.add(unfinishedUrl)
            continue

        albumInfoJson = albumJson["AlbumDetailPage"]["albumInfo"]
        albumId = albumInfoJson["albumId"]
        albumCoverImgUrl = albumInfoJson["mainInfo"]["cover"]
        albumTitle = albumInfoJson["mainInfo"]["albumTitle"]
        albumPlayCount = albumInfoJson["mainInfo"]["playCount"]
        albumIsFinished = albumInfoJson["mainInfo"]["isFinished"]
        albumAnchor = albumInfoJson['anchorInfo']['anchorName']
        print( "album id:" + str(albumId) + " Cover:" + albumCoverImgUrl + " Title:" + albumTitle + " anchor: " + albumAnchor + " PlayCount:" + str(albumPlayCount) + " IsFinished:" + str(albumIsFinished) )

        albumFolderPathOld = getAlbumFolder( albumTitle, "" )
        albumFolderPath = getAlbumFolder( albumTitle, albumAnchor )
        if( os.path.isdir( albumFolderPathOld ) ):
            shutil.move(albumFolderPathOld, albumFolderPath)

        allTrackNum = int(albumJson["AlbumDetailTrackList"]["tracksInfo"]["trackTotalCount"])
        print( "All track num:" + str( allTrackNum ) )
        tracksInfoArray = albumJson["AlbumDetailTrackList"]["tracksInfo"]["tracks"]

        if( allTrackNum <= 0 ):
            print( 'allTrackNum is 0!' )
            continue
        indexWidth = int( 0.5 + log10(allTrackNum) ) + 1
        indexFormatStr = "%0" + str(indexWidth) + "d"

        saveAlbumCover( albumCoverImgUrl, albumFolderPath )

        for tracksInfo in tracksInfoArray:
            trackId = str(tracksInfo["trackId"])
            trackIndex = indexFormatStr%int(tracksInfo["index"])
            trackName = tracksInfo["title"]
            isPaid = str(tracksInfo["isPaid"])
            if( isPaid == "True" ):
                print( "track %s-%s need money!"%(trackId,trackName) )
                continue
            #print( "track index:" + trackIndex + " id:" + trackId + " title:" + trackName + " isPaid:" + isPaid )
            if( isTrackExist( albumFolderPath, trackName, trackIndex, "") ):
                continue
            else:
                getTrackSoundUrl( trackId, trackIndex, albumFolderPath )
        urlSetFinishedPages.add( unfinishedUrl )
        

    print( "Finished urls:" + str(urlSetFinishedPages) )
    print( "Error urls:" + str( urlSetErrorPages ) )
    print ( "Error files:" + str( urlSetErrorFiles) )




urlSetUnfinishedPages.add( beginUrl )   #郭德纲相声
#urlSetUnfinishedPages.add( "/youshengshu/6062943/")     #临高启明
urlSetUnfinishedPages.add( "/renwen/6655240/" )         #红楼梦原著朗读和讲解
#urlSetUnfinishedPages.add( "/youshengshu/3757698/")  #冯仑 - 野蛮生长
urlSetUnfinishedPages.add( "/renwen/7651313/")      #晓说2017
urlSetUnfinishedPages.add( "/youshengshu/329041/") # 潘石屹 - 我的价值观
urlSetUnfinishedPages.add( "/youshengshu/3544633/") # 盗墓笔记 - 周建龙播讲
#urlSetUnfinishedPages.add( "/youshengshu/14195461/") # 临高启明 第一卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/14215560/") # 临高启明 第二卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/13527805/") # 临高启明 第三卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/14538024/") # 临高启明 第四卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/14999275/") # 临高启明 第五卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/15179260/") # 临高启明 第六卷 机器读的
#urlSetUnfinishedPages.add( "/youshengshu/15336581/") # 临高启明 第七卷 机器读的
urlSetUnfinishedPages.add( "/youshengshu/413068/") # 皮囊
urlSetUnfinishedPages.add( "/renwen/275154/") # 看见 - 柴静
urlSetUnfinishedPages.add( "/youshengshu/7564012/") # 送你一颗子弹 - 刘瑜 - 左脑禅播讲
urlSetUnfinishedPages.add( "/youshengshu/15172352/") # 中华人民共和国宪法 - 亮眼儿播讲
urlSetUnfinishedPages.add( "/youshengshu/15248653/") # 中华人民共和国监察法 - 亮眼儿播讲
urlSetUnfinishedPages.add( "/youshengshu/15273635/") # 民事诉讼法与民事诉讼法解释 - 亮眼儿播讲
urlSetUnfinishedPages.add( "/youshengshu/10404795/") # 社会心理学 ( 侯玉波 第三版 ) - 界世的美小 播讲
urlSetUnfinishedPages.add( "/youshengshu/12213434/") # 金瓶梅 - 安娜说播讲
urlSetUnfinishedPages.add( "/renwen/430667/") # 官场现形记 - 白云出岫
urlSetUnfinishedPages.add( "/youshengshu/290069/") #官场现形记 - 李说书
urlSetUnfinishedPages.add( "/youshengshu/3075245/") # 王小波文集 - 阿苏阿西
urlSetUnfinishedPages.add( "/youshengshu/3191173/") # 送你一颗子弹 - 刘瑜 - 胡糊糊亮亮
urlSetUnfinishedPages.add( "/youshengshu/18756196/") # 墨子（原文+译文） - 墨子 - 安娜说520
#urlSetUnfinishedPages.add( "/youshengshu/3664434/") # 肖申克的救赎 - 斯蒂芬·金 - 马西先生
urlSetUnfinishedPages.add( "/youshengshu/4820458/") # 一个人的朝圣 -   [英] 蕾秋·乔伊斯  - 小鱼有声书
urlSetUnfinishedPages.add( "/youshengshu/12167823/") # 霍乱时期的爱情 -   [哥伦比亚] 加西亚·马尔克斯  - 玛格麗特
urlSetUnfinishedPages.add( "/youshengshu/6588476/") # 杀死一只知更鸟 -   [美] 哈珀·李  - 玛格麗特

#urlSetUnfinishedPages.add( "/youshengshu/5697029/") # 月亮和六便士 -  [英] 威廉·萨默塞特·毛姆 - 玛格麗特
urlSetUnfinishedPages.add( "/youshengshu/7494116/") # 贫民窟的百万富翁 -   [印度] 维卡斯·斯瓦鲁普 - 小鱼有声书
urlSetUnfinishedPages.add( "/youshengshu/3376153/") # 基督山伯爵 - 大仲马 - 秦笙故事
urlSetUnfinishedPages.add( "/youshengshu/11542809/") # 阿甘正传 -   [美] 温斯顿·葛鲁姆 -无话不讲
urlSetUnfinishedPages.add( "/youshengshu/312611/")  # 百年孤独 - 哥伦比亚 加西亚·马尔克斯 - 主播罗宁
urlSetUnfinishedPages.add( "/youshengshu/5115679/") # 人生的枷锁 - [英] 威廉·萨默塞特·毛姆 - 悦简
#urlSetUnfinishedPages.add( "/youshengshu/14661785/") # 教父2 - 西西里人 - [美] 马里奥·普佐 - 主播罗宁
urlSetUnfinishedPages.add( "/youshengshu/15356614/") # 阿甘正传 -   [美] 温斯顿·葛鲁姆 - 弘然
urlSetUnfinishedPages.add( "/youshengshu/9875838/")  # 《三个火枪手》 - 大仲马 - 博雅人文
urlSetUnfinishedPages.add( "/youshengshu/11217375/") # 麦田里的守望者 -  J·D·塞林格 - 思空语堂
urlSetUnfinishedPages.add( "/youshengshu/10364890/") # 战争与和平 - 俄国 列夫·尼古拉耶维奇·托尔斯泰 - 主播罗宁
urlSetUnfinishedPages.add( "/youshengshu/3387034/") # 双城记 - 英国 查尔斯·狄更斯 - 高乐高WHY伯爵
urlSetUnfinishedPages.add( "/youshengshu/10829068/") # 基督山伯爵 - 大仲马 - 主播罗宁
#urlSetUnfinishedPages.add( "/youshengshu/6090103/")     # 教父三部曲：教父、西西里人、最后的教父 － 马里奥普佐 - GGWizard
urlSetUnfinishedPages.add( "/youshengshu/11735478/") # 毛姆 面纱 （悦简）
urlSetUnfinishedPages.add( "/renwen/10398132/" ) # 《艺术史导论》清华大学 - 博雅人文
urlSetUnfinishedPages.add( "/shangye/269179/") # 吴晓波频道
urlSetUnfinishedPages.add( "/youshengshu/16496298/" ) # 《余欢》刘瑜 孤独得像一个星球 - 缦丽
urlSetUnfinishedPages.add( "/youshengshu/3747452/" ) # 500废大作战(临高启明 第一卷) - 陆大胖子
urlSetUnfinishedPages.add( "/youshengshu/3784849/") # 500废大作战 第二卷(临高启明 第二卷) - 陆大胖子
urlSetUnfinishedPages.add( "/youshengshu/4137073/") # 500废大作战 第3卷(临高启明 第3卷) - 陆大胖子
urlSetUnfinishedPages.add( "/youshengshu/4774110/") # 500废大作战 第4卷(临高启明 第4,5卷) - 陆大胖子
urlSetUnfinishedPages.add( "/qinggan/6163096/") # 夜色温柔-柴静 - 流浪天使1
urlSetUnfinishedPages.add( "/youshengshu/257280/") # 柴静《用我一辈子去忘记》- 卡卡radio
urlSetUnfinishedPages.add( "/youshengshu/4894844/") #红与黑 - 主播罗宁
urlSetUnfinishedPages.add( "/youshengshu/18859040/") # 临高启明 第五卷 - 婉婉的冒险
urlSetUnfinishedPages.add( "/youshengshu/16415285/" ) # 500废大作战 第6卷(临高启明 第6卷) - 陆大胖子
urlSetUnfinishedPages.add( "/waiyu/3531660/") # 世界名人英文演讲 - 英文原版读物
urlSetUnfinishedPages.add( "/renwen/5354438/") # 文艺复兴在意大利 - 碎片先生
urlSetUnfinishedPages.add( "/renwen/6414376/") # 艺术世界漫游指南 - 孤山聊艺术
urlSetUnfinishedPages.add( "/youshengshu/2857488/") # 红与黑 - 气象先生平安
#urlSetUnfinishedPages.add( "/youshengshu/4819310/") # 战争与和平 - gentle杰克
urlSetUnfinishedPages.add( "/youshengshu/3492211/" ) # 呼啸山庄 - 演播蜘蛛
urlSetUnfinishedPages.add( "/youshengshu/6805053/" ) # 远大前程 - 高乐高WHY伯爵
urlSetUnfinishedPages.add( "/youshengshu/10026000/" ) # 巴黎圣母院 - 雨果 - 博雅人文
urlSetUnfinishedPages.add( "/youshengshu/4100688/" ) # 大卫·科波菲尔（中文版）- 知北游z
urlSetUnfinishedPages.add( "/youshengshu/3419108/" ) # 复活 - 有声的金明
urlSetUnfinishedPages.add( "/youshengshu/3765198/" ) # 包法利夫人 - 且听倾城
urlSetUnfinishedPages.add( "/youshengshu/5242438/" ) # 追忆似水年华 - 主播罗宁
urlSetUnfinishedPages.add( "/youshengshu/10024798/" ) # 《复活》托尔斯泰 - 博雅人文
urlSetUnfinishedPages.add( "/youshengshu/3584718/" ) # 羊脂球 - nick的声音世界
urlSetUnfinishedPages.add( "/youshengshu/2869815/" ) # 高老头 - 熙城chuanqi
urlSetUnfinishedPages.add( "/renwen/15273276/" ) # 晓说2018 - 超级制作
urlSetUnfinishedPages.add( "/jiaoyu/7516087/" ) # 个案说法 - 我是方弘
urlSetUnfinishedPages.add( "/renwen/4291180/" ) # 鲁迅全集-选读 - 白云出岫
urlSetUnfinishedPages.add( "/xiangsheng/9742296/" ) # 郭麒麟相声全集 - 郭德纲相声
urlSetUnfinishedPages.add( "/renwen/10422957/" ) # 《理想国》导读 复旦大学 - 博雅人文
urlSetUnfinishedPages.add( "/ertong/277077/" ) # 格林童话故事全集 - 儿童故事机
urlSetUnfinishedPages.add( "/youshengshu/12225530/" ) # 安徒生童话故事全集 - 岁月音域
urlSetUnfinishedPages.add( "/lishi/13626147/" ) # 第二次世界大战全史(二战史) - ae_wn
urlSetUnfinishedPages.add( "/renwen/5088879/" ) # 读者 - 读者
urlSetUnfinishedPages.add( "/lvyou/3501680/" ) # 不傻在欧洲 - 李不傻
urlSetUnfinishedPages.add( "/ertong/6233693/" ) # 米小圈上学记：一二三年级 -米小圈
# 加利福尼亚慕容 已经直接添加了主播.
#urlSetUnfinishedPages.add( "/renwen/15882009/" ) # 月亮和六便士·毛姆|慕容读经典 - 加利福尼亚慕容
#urlSetUnfinishedPages.add( "/youshengshu/11523230/" ) # 《简爱》| 慕容读经典系列 - 加利福尼亚慕容
#urlSetUnfinishedPages.add( "/lvyou/5721812/" ) # 行者无疆（主播：慕容） - 加利福尼亚慕容
#urlSetUnfinishedPages.add( "/youshengshu/12488323/" ) # 《爱的艺术》·弗洛姆|慕容读经典 - 加利福尼亚慕容
urlSetUnfinishedPages.add( "/youshengshu/21750972/" ) # 海子诗选：你来人间一趟，你要看看太阳 - 喜马有声剧
urlSetUnfinishedPages.add( "/youshengshu/2819988/" ) # 一个陌生女人的来信：茨威格小说集 - 静播频道
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #
urlSetUnfinishedPages.add( "" ) #

# 添加主播
urlSetUnfinishedPages.add( "/zhubo/48210967/" ) # 加利福尼亚慕容
urlSetUnfinishedPages.add( "/zhubo/11175476/" ) # 英文名著畅听
urlSetUnfinishedPages.add( "/zhubo/7694761/" ) # 国际获奖儿童故事
urlSetUnfinishedPages.add( "/zhubo/41010336/" ) # 江涛声音工作室
urlSetUnfinishedPages.add( "/zhubo/25572713/" ) # 夜歌而行
urlSetUnfinishedPages.add( "/zhubo/5634126/" ) # 马聪STA   
urlSetUnfinishedPages.add( "/zhubo/65419887/" ) # 瀚章播读
urlSetUnfinishedPages.add( "/zhubo/67540468/" ) # ae_wn  历史人文
urlSetUnfinishedPages.add( "/zhubo/70721567/" ) # 孤山聊艺术 音乐历史
urlSetUnfinishedPages.add( "/zhubo/35302492/" ) # 一语癔语
urlSetUnfinishedPages.add( "/zhubo/27553392/" ) # 纪保超的分享
urlSetUnfinishedPages.add( "/zhubo/47821280/" ) # 王廷志_4o
urlSetUnfinishedPages.add( "" ) #

# for test...
urlSetUnfinishedPages.clear()
urlSetUnfinishedPages.add( "/youshengshu/4820458/" )


while( True ):
    try:
        DownloadPages( urlSetUnfinishedPages, urlSetFinishedPages, urlSetErrorPages, urlSetErrorFiles )
    except:
        logging.exception("message")
        urlSetFinishedPages -= urlSetErrorPages
        urlSetUnfinishedPages = urlSetUnfinishedPages.union(urlSetErrorPages)
    if( len(urlSetUnfinishedPages) == 0 ):
        break