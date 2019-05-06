import requests
import os
import json
from bs4 import BeautifulSoup
import socket
import urllib
from selenium import webdriver
import re
import time
import logging


songStorageDir = "X:/music/cachedownload"
songIdDir = "/`````songid"
cloudMusicCacheDir = os.environ['LOCALAPPDATA'] + "\\Netease\\CloudMusic\\Cache\\Cache"
cloudMusicLyricDir = os.environ['LOCALAPPDATA'] + "\\Netease\\CloudMusic\\webdata\\lyric"
MaxNameLen = 64 # 限制名称(歌手名称,专辑名称,歌曲名称)的最长长度, 超过长度的截断, 防止路径过长.

def NormalizeName( name ):
    name = name[0:MaxNameLen] # 截断超长的部分. 要先截断,否则可能会在截断后在结尾出现空格.
    name = re.sub('[\\\/:*?"<>|\t\v\r\n]','-',name)#去掉非法字符
    name = re.sub('\s*$', '', name) # 去掉结尾的空白
    name = re.sub('^\s*', '', name) #去掉开始的空白.
    name = name.replace( '\0', '' );    # 有的名称里面有0,必须去掉.
    return name

def install_proxy(PROXY_HOST, httpPort, ftpPort, sock5Port):
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

def GetSongInfo( songId, fileSize, format ):
    songIdDirPath = songStorageDir + songIdDir
    if not( os.path.exists(songIdDirPath) ):
            os.makedirs( songIdDirPath, 0o777, True )
    songInfoFilePath = songStorageDir + songIdDir + "/" + songId + ".json"
    if( os.path.isfile( songInfoFilePath ) ):
        try:
            songInfoFile = open( songInfoFilePath, "rb")
            songInfo = json.load( songInfoFile )
            songInfoFile.close()
            if( songInfo.get("songId") and songInfo.get( 'album'  ) and songInfo.get('artist') and songInfo.get('title')):
                return songInfo
        except:
            print( "Load song info fail! path:" + songInfoFilePath )
    fireFoxOptions = webdriver.FirefoxOptions()
    fireFoxOptions.set_headless()
    browser = webdriver.Firefox(firefox_options=fireFoxOptions)
    #browser = webdriver.Firefox()
    #browser = webdriver.Chrome()
    #browser = webdriver.PhantomJS()
    #browser.get( "http://music.163.com/#/song?id=" + songId )
    songUrl = "http://music.163.com/song?id=" + songId
    try:
        browser.get( songUrl )
    except:
        print( "Browser get url fail!" + songUrl )
        return

    try:
        browser.switch_to.frame('g_iframe')  # 从windows切换到frame，切换到真实信息所在的frame
    except:
        print( "switch to frame fail! song url:" + songUrl )
        return
    songInfo = {}
    try:
        songInfoJsonStr = browser.find_element_by_xpath( "/html/head/script[1]").get_attribute( "innerHTML" )
        songInfo = json.loads( songInfoJsonStr )
    except Exception as e:
        print( "Parse song info from html element /html/head/script[1] fail! songId:" + songId )
        logging.exception("message")
        try:
            songInfoJsonStr = browser.find_element_by_xpath( "/html/head/script[2]").get_attribute( "innerHTML" )
            songInfo = json.loads( songInfoJsonStr )
        except Exception as e:
            print( "Parse song info from html element /html/head/script[2] fail! songId:" + songId )
            logging.exception("message")
    
    try:
        infoArray = songInfo["description"].split('。')
        for subStr in infoArray:
            if( "歌手" in subStr ):
                songInfo["artist"] = subStr.split('：')[1]
            if( "专辑" in subStr ):
                songInfo["album"] = subStr.split('：')[1]
    except Exception as e:
        print( "Parse song info from json fail! songId:" + songId + " json:" + json.dumps(songInfo) )
        logging.exception("message")
    if( not songInfo ):
        songInfo = {}
    try:
        if( not songInfo.get( 'title' ) ):
            htmlTitle = browser.find_element_by_tag_name( "title" ).get_attribute("innerHTML")
            songInfo["title"] = htmlTitle.split(" - ")[0]
        if( not songInfo.get( 'album' ) ):
            songInfo["album"] = browser.find_elements_by_class_name( "s-fc4")[1].text.split('：')[1]
        songInfo['album']
        if( not songInfo.get( 'artist' ) ):
            songInfo["artist"] = browser.find_elements_by_class_name( "s-fc4")[0].text.split('：')[1]
        if( not songInfo.get( 'images' ) ):
            songInfo["images"] = browser.find_element_by_class_name( "j-img").get_attribute( 'src').split('?')
    except Exception as e:
        print( "Parse song info from html element fail 2! songId:" + songId )
        logging.exception("message")
        return
    songInfo["songId"] = songId
    songInfo["format"]= format
    songInfo["fileSize"] = fileSize
    # 保存.
    with open( songInfoFilePath, "w", encoding='utf-8' ) as f:
        json.dump( songInfo, f )
    f.close()
    #print( songInfo )
    browser.quit()
    return songInfo

def DecodeCloudMusicCacheFile( cacheFilePath, storageFilePath ):
    with open ( cacheFilePath,'rb') as f:
        btay = bytearray(f.read())
    with open(storageFilePath,'wb') as out:
        for i,j in enumerate(btay):
            btay[i] = j ^ 0xa3
        out.write(bytes(btay))
        out.close()
        f.close()

def GetSongDir( songInfo ):
    album = NormalizeName(songInfo[ "album" ])#去掉非法字符
    artist = NormalizeName(songInfo["artist"])#去掉非法字符
    songDir = songStorageDir + "/" + artist + "/" + album + "/"
    try:
        if not( os.path.exists(songDir) ):
            os.makedirs( songDir, 0o777, True )
    except:
        print( "make dir[" + songDir + "]fail!")
    return songDir

def GetSongFilePathWithoutExt( songInfo ):
    songTitle = songInfo["title"]
    songDir = GetSongDir( songInfo )
    fileName = NormalizeName(songTitle)#去掉非法字符
    return  songDir + fileName

def SaveSongFile( ucFilePath, songInfo ):
    songFilePath = GetSongFilePathWithoutExt( songInfo ) + "." + songInfo["format"]

    try:
        os.path.isfile( songFilePath )
    except:
        print( songFilePath + " Error!" )

    if( os.path.isfile( songFilePath ) ):
        if( os.path.getsize( songFilePath ) >= int( songInfo["fileSize"] ) ):
            #print( songFilePath + " already exist!")
            return songFilePath # 已经存在,并且文件大小可以.
    print( "song path: " + songFilePath );
    DecodeCloudMusicCacheFile( ucFilePath, songFilePath )
    print( "Save song to :" + songFilePath )
    return songFilePath

def SaveAlbumCover( songInfo ):
    album = NormalizeName(songInfo[ "album" ])#去掉非法字符
    coverFilePath = GetSongDir( songInfo ) + album + ".jpg"
    if( os.path.isfile( coverFilePath ) ):
        #print( "Cover " + coverFilePath + " already exist!" )
        return
    imgUrl = songInfo[ "images"][0]
    try:
        urllib.request.urlretrieve(imgUrl, coverFilePath)
    except urllib.error.HTTPError as err:
        print( "img get fail! url:\t"+ imgUrl  )
        print( err );
    except:
        print( "img get fail unknown except! url:\t"+ imgUrl  )
    print( "Save album cover to:" + coverFilePath )

def SaveLyric( songInfo ):
    lyricJsonFilePath = GetSongFilePathWithoutExt( songInfo ) + ".lyric.json"
    if( not os.path.exists( lyricJsonFilePath ) ):
        try:
            songId = songInfo["songId"]
            lyricUrl = "http://music.163.com/api/song/lyric?os=pc&id="+ songId +"&lv=-1&kv=-1&tv=-1"
            urllib.request.urlretrieve( lyricUrl, lyricJsonFilePath )
        except:
            print( "Save lyric fail! song info:" + json.dumps( songInfo ))

    try:
        lyricJsonFile = open( lyricJsonFilePath, "rb" )
        lyricJson = json.load( lyricJsonFile )
    except:
        print( "Load lyric json fail! json file path:" + lyricJsonFilePath )
        lyricJsonFile.close()
        os.remove( lyricJsonFilePath )
        return
    lyricJsonFile.close()
    if( not "lrc" in lyricJson.keys() ):
        return
    if( not "lyric" in lyricJson[ "lrc"].keys() ):
        return
    lyric = lyricJson[ "lrc"]["lyric"]
    lyric = lyric.replace( "\\n", "\n" )
    lrcFilePath = GetSongFilePathWithoutExt( songInfo ) + ".lrc"
    with open( lrcFilePath, "w", encoding='utf-8' ) as lrcFile:
        lrcFile.write( lyric )
        lrcFile.close()

def HackCloudMusicCache():
    cacheFiles = os.listdir( cloudMusicCacheDir );
    for fileName in cacheFiles: #遍历文件夹
        ucFilePath = cloudMusicCacheDir + "\\" + fileName
        if(os.path.isfile(ucFilePath)): #判断是否是文件夹，不是文件夹才打开
              #f = open( cloudMusicCacheDir+"/"+file); #打开文件
              isUC= fileName.endswith('.uc')
              if( isUC ):
                  strArray = fileName.split("-")
                  songId = strArray[0];
                  #print( "song id is :" + songId )  #文件名的第一部分是歌曲ID
                  idxFilePath = ucFilePath.replace('.uc', '.idx')
                  infoFilePath = ucFilePath.replace('.uc', '.info')
                  idxFileExist = os.path.exists( idxFilePath )
                  infoFileExist = os.path.exists( infoFilePath )
                  if( idxFileExist and infoFileExist ):     # info 或 idx 文件不存在的时候,歌曲还没有播放完.
                      idxFile = open( idxFilePath, 'rb')
                      try:
                        idxJson = json.load( idxFile )
                      except:
                          print( "Json parse idxFile fail! file:" + idxFilePath )
                          idxFile.close()
                          continue
                      #print( idxJson )
                      fileSize = int(idxJson['size'])
                      zoneEnd = int(idxJson['zone'][0].split(' ')[1])
                      cacheFinished = fileSize <= zoneEnd+1
                      idxFile.close()
                      if( not cacheFinished ):
                          print( "File " + ucFilePath + " has not finished!")
                          continue
                      infoFile = open( infoFilePath, 'rb' )
                      try:
                        infoJson = json.load( infoFile )
                      except:
                          print( "Load song info json fail path:" + infoFilePath )
                          continue
                      musicFileFormat = infoJson['format']
                      #print( "file format is :" + musicFileFormat)

                      songInfo = GetSongInfo( songId, fileSize, musicFileFormat )
                      try:
                        if( not songInfo["songId"] ):
                            continue
                      except:
                          print( "songInfo invalid!" )
                          continue
                      storageSongPath = SaveSongFile( ucFilePath, songInfo )
                      SaveAlbumCover( songInfo )  # 保存封面
                      SaveLyric( songInfo )         # 保存歌词.


def getdirsize(dir):
   size = 0
   for root, dirs, files in os.walk(dir):
      size += sum([os.path.getsize(os.path.join(root, name)) for name in files])
   return size

def human_readable(plain_size):
    plain_size = float(plain_size)
    if plain_size <= 1024:
        return str( round(plain_size, 2)) + 'B'
    if plain_size <= 1024 * 1024:
        return str( round(plain_size / 1024, 2)) + 'K'
    if plain_size <= 1024 * 1024 * 1024:
        return str( round(plain_size / 1024 / 1024, 2)) + 'M'
    if plain_size <= 1024 * 1024 * 1024 *1024:
        return str( round(plain_size / 1024 / 1024 / 1024, 2)) + 'G'

#install_proxy( "127.0.0.1", 808, 2121, 1080 )
while( True ):
    print( "--------------------Start hack cloud music cache!------------------" )
    dirSize = 0
    try:
        HackCloudMusicCache();
        dirSize = getdirsize(songStorageDir)
    except Exception as e:
        print( "HackCloudMusicCache except!" )
        logging.exception("message")
    print( "End hack cloud music cache!------------------Cur dir size:%s"%human_readable(dirSize) )

    time.sleep( 60 * 5 )
