from selenium import webdriver
import os
import urllib.request
import logging
import time
import re
from bs4 import BeautifulSoup
from http.cookiejar import CookieJar
import requests
from selenium.webdriver.support.wait import WebDriverWait
import json


def GetFirefox(PROXY_HOST, httpPort, ftpPort, sock5Port):
    fp = webdriver.FirefoxProfile( profile_directory='e:/firefoxprofile')
    fp.set_preference("network.proxy.type", 1)
    fp.set_preference("network.proxy.http",PROXY_HOST)
    fp.set_preference("network.proxy.http_port",int( httpPort ))
    fp.set_preference("network.proxy.https",PROXY_HOST)
    fp.set_preference("network.proxy.https_port",int( httpPort ))
    fp.set_preference("network.proxy.ssl",PROXY_HOST)
    fp.set_preference("network.proxy.ssl_port",int( httpPort ))
    if httpPort != None and httpPort != 0:
        fp.set_preference("network.proxy.ftp", ftpPort)
        fp.set_preference("network.proxy.ftp_port",int(ftpPort))
    if sock5Port != None and sock5Port != 0:
        fp.set_preference("network.proxy.socks",PROXY_HOST)
        fp.set_preference("network.proxy.socks_port",int(sock5Port))
    userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) Gecko/20100101 Firefox/94.0"
    fp.set_preference("general.useragent.override", userAgent)
    fp.update_preferences()

    

    wb = webdriver.Firefox(firefox_profile=fp)

    try:
        if os.path.isfile( "cookies.json" ):
            cookies = json.load( open("cookies.json") )
            for cookie in cookies:
                wb.add_cookie( cookie )
    except:
        logging.exception( 'error' )

    return wb



def initUrllib():
    proxyhandler = urllib.request.ProxyHandler({'http':'http://192.168.84.162:8118', 'https':'http://192.168.84.162:8118'})

    cj = CookieJar()
    #opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
    cookieHandler = urllib.request.HTTPCookieProcessor(cj)

    opener = urllib.request.build_opener( proxyhandler, cookieHandler )
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) ' 'Chrome/51.0.2704.63 Safari/537.36'}
    opener.addheaders = [('User-agent', 'Mozilla/5.0')]
    urllib.request.install_opener(opener)

firefox = None
def initFirefox():
    global firefox
    if firefox == None:
        firefox = GetFirefox( "172.16.64.90", 8118, 0, 0 )
        testUrl = "http://www.javlibrary.com/tw/"
        firefox.get(testUrl)
        time.sleep(30)
        #WebDriverWait(firefox, 60)
    return firefox

def urlGetContent( url):
        #req = urllib.request.Request(url=url)
        
        #content = urllib.request.urlopen(req).read().decode('utf-8','ignore')#, 'ignore'

        mode = "browser"
        mode = "requests"

        if mode == "requests":
            # requests的方式
            r = requests.get( url, timeout=30)
            content = r.text
        else:
            # 浏览器的方式
            
            browser = initFirefox()
            browser.get( url )
            content = browser.page_source

            cookies = browser.get_cookies()

            with open( 'cookies.json', mode='w' ) as f:
                json.dump( cookies, f )
                f.close()
        

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
        time.sleep(1*60)
    return None 


if __name__=="__main__":
    urlTest = "http://www.javlibrary.com/tw/?v=javlijb6si"
    initUrllib()
    bs = bsObjForm( urlTest )
    bs
