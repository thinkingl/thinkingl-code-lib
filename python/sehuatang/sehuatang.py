import os
import urllib.request
import logging
import time
from bs4 import BeautifulSoup


class SeHuaTang:
    baseUrl = 'https://www.sehuatang.net'
    gqzwzmUrl = 'forum-103-<page>.html'
    finishedUrl = set()

    def getHeaders(self):
        headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
        return headers  
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

    def threadCrawl(self):
        pageNum = 1
        while( True ):
            pageUrl = self.gqzwzmUrl.replace('<page>', str(pageNum))
            url = self.baseUrl + '/' + pageUrl
            pageNum += 1
            bs = self.bsObjForm(url)
            lastUrl = bs.select('a.last')[0]['href']
            for a in bs.select('a.xst'):
                contentUrl = self.baseUrl+'/'+a['href']
                title = a.text
                logging.info( 'Title %s url %s', title, contentUrl )
                self.saveThread( contentUrl, title )
            if( lastUrl == pageUrl ):
                logging.info( 'Last page url: %s', lastUrl )
                break
            a = 10 

    def saveThread(self, threadUrl, title):
        bs = self.bsObjForm(threadUrl)
        threadInfo = {}
        logging.info( '' )
        # 帖子文本和下面的评论内容.
        for td in bs.select('td.t_f'):
            logging.info( td.text )
            if 'text' not in threadInfo:
                threadInfo['text'] = []
            threadInfo['text'].append( td.text )
        # 图片,封面和预览等.
        for img in bs.select('img.zoom'):
            if 'img' not in threadInfo:
                threadInfo['img'] = []
            if 'file' in img.attrs:
                logging.info( img['file'] )
                threadInfo['img'].append( img['file'] )
                continue
            if 'src' in img.attrs:
                logging.info( img['src'] )
                threadInfo['img'].append( img['src'] )
                continue
            logging.error('invalid img! %s', img )
        # 下载的磁力链接
        for li in bs.select('div.blockcode > div > ol > li'):
            if 'magnet' not in threadInfo:
                threadInfo['magnet'] = []
            code = li.text
            threadInfo['magnet'].append( code )
            logging.info( code )
        # 种子文件链接(附件)
        for attach in bs.select( 'p.attnm > a'):
            if 'attach' not in threadInfo:
                threadInfo['attach'] = []
            attachUrl = attach['href']
            if attachUrl.find( 'http' ) == -1:
                attachUrl = self.baseUrl + '/' + attachUrl
            threadInfo['attach'].append( attachUrl )
            logging.info( attachUrl )
        # 帖子发表时间.
        for span in bs.select( 'div.authi > em > span'):
            createTime = span['title']
            if len(createTime) > 0:
                logging.info( 'time:%s', createTime )
                threadInfo['createTime'] = createTime
                break
        # 保存论坛到本地文件.
        
        b = 10

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

if __name__=="__main__":
    initLogging()
    sht = SeHuaTang()
    sht.threadCrawl()
