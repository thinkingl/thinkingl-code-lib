import requests
from bs4 import BeautifulSoup
import codecs

#export HTTP_PROXY="http://127.0.0.1::808"
#export HTTPS_PROXY="https://127.0.0.1::808"

proxies = {
  "http": "socks5://127.0.0.1:1080",
  "https": "socks5://127.0.0.1:1080",
}


proxies = {};

domain = 'http://m.23wx.cm'
url = '/0/767/15675647.html'
#http://m.23wx.cm/0/767/16892999.html
url = "/0/767/16892999.html"

f = open("c:/9-tmp/lgqm.txt", "w", encoding='utf-8');

while( url != "" ):
    fullUrl = domain+url;
    r = requests.get(fullUrl,  proxies=proxies)
    soup = BeautifulSoup(r.text, 'html.parser')

    title = soup.select('.content_title > h1')[0].get_text()
    print( title )

    f.write( title+"\r\n\r\n" );

    content = soup.select('#novelcontent > p')[0]
    contentText=content.text


    for tag in content.contents:
        if( "br" == tag.name ):
            f.write( "\r\n")
        else:
           f.write( tag )

    print( contentText )

    url = soup.select('#novelcontent > ul > li .p3 > a')[0]['href']
    print( url )

