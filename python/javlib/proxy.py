from selenium import webdriver
import os
import urllib.request
import logging
import time
import re
from bs4 import BeautifulSoup
from http.cookiejar import CookieJar


def GetFirefox(PROXY_HOST, httpPort, ftpPort, sock5Port):
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



def initUrllib():
    proxyhandler = urllib.request.ProxyHandler({'http':'http://192.168.84.162:8118', 'https':'http://192.168.84.162:8118'})

    cj = CookieJar()
    #opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
    cookieHandler = urllib.request.HTTPCookieProcessor(cj)

    opener = urllib.request.build_opener( proxyhandler, cookieHandler )
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
    opener.addheaders = [('User-agent', 'Mozilla/5.0')]
    urllib.request.install_opener(opener)


def urlGetContent( url):
        req = urllib.request.Request(url=url)
        
        content = urllib.request.urlopen(req).read().decode('utf-8','ignore')#, 'ignore'
        return content

    
def bsObjForm(url):
    for i in range (0,10):
        try:
            html = urlGetContent(url)
            bsObj1 = BeautifulSoup(html, "html.parser")
            if not bsObj1 == None:
                return bsObj1
        except:
            logging.error( 'bsObjForm %s fail!', url )
            logging.exception("error")
        time.sleep(5*60)
    return None 


if __name__=="__main__":
    urlTest = "http://www.javlibrary.com/tw/?v=javlijb6si"
    initUrllib()
    bs = bsObjForm( urlTest )
    bs
