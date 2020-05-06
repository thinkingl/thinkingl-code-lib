import requests
from bs4 import BeautifulSoup
import codecs
import re

mainPage = 'https://www.fpzw.com/xiaoshuo/0/63/'

proxies = {
  "http": "socks5://127.0.0.1:1080",
  "https": "socks5://127.0.0.1:1080",
}

#proxies = {}

localDir = 'c:/2-tmp/'
#localDir = '/share/disk-160/tmp/'

#如果超过万，则分为两部分以节约代码和运行速度
def turn(x):
    i=str(x)
    if len(i) >4:
        i=tran(i[0:-4])+'万'+tran(i[-4:])
    else:
        i=tran(i[-4:])
    return i
#转换数字并插入对应单位，单位为‘零’则再插入一个‘零’以方便正则表达式替换
def tran(x):
    num=['零','一','二','三','四','五','六','七','八','九']
    kin=['零','十','百','千']
    x=list(reversed(x))
    for i in x:
        x[(x.index(i))]=num[int(i)]
    if len(x) >= 2:
        if x[1]==num[0]:
            x.insert(1,kin[0])
        else:
            x.insert(1,kin[1])
        if len(x) >= 4:
            if x[3]==num[0]:
                x.insert(3,kin[0])
            else:
                x.insert(3,kin[2])
            if len(x) >= 6:
                if x[5]==num[0]:
                    x.insert(5,kin[0])
                else:
                    x.insert(5,kin[3])
    x=delz(x)
    x=del10(x)  # 删除一十x的一
    return x
#进行多余‘零’的删除
#reversed()函数真是可以用在列表和字符串。
#加上 if 语句 防止对不必要的数据进行正则表达式检测
def delz(x):
    x=''.join(x)
    if '零零'in x:
        x=re.sub('零+','零',x)
    if x.startswith('零'):
        x=list(x)
        x.remove('零')
    x=reversed(x)
    x=''.join(x)
    return x

def del10(x):
   x= re.sub( '^一十', '十', x)
   return x


def getSectionText(sectionUrl):
    sectionUrl = mainPage + sectionUrl
    obj = requests.get( sectionUrl,  proxies=proxies )
    obj.encoding = 'gbk'
    #print( obj.text )
    ss = BeautifulSoup(obj.text, 'html.parser')

    text = ss.select( '.Text' )[0].text
    text = text.replace('\xa0', '\n')
    beginPos = text.find( '\n\n\n\n') + 4
    text = text[beginPos:]
    text = text.replace('2k小说阅读网', '' )
    text = text.replace( '\n\n\n\n', '\n')
    return text


r = requests.get(mainPage,  proxies=proxies)
r.encoding ='gbk'
soup = BeautifulSoup(r.text, 'html.parser')

curParseIndex = 3   # 当前序号
maxSearchSectionOffset = 20 # 找当前章节时最多找的个数.

array = soup.select( 'dd > a' )

allFirstSection = []
for i in range(0,len(array)):
    if array[i].text.find( '第一节' ) == 0:
        allFirstSection.append(i)
        print( 'Find first section: ' + array[i].text )

partIndex = 0
for firstSectionIndex in allFirstSection:
    partIndex += 1
    print( 'Begin part %d' % partIndex )

    f = open( localDir + "lgqm-" + str(partIndex) +".txt", "w", encoding='utf-8')

    curSection = 1  # 当前章节
    curParseIndex = firstSectionIndex
    while curParseIndex < len(array):
        curSecChn = turn( curSection )

        findSection = False
        for i in range(curParseIndex, min(curParseIndex+maxSearchSectionOffset, len(array)) ):
            title = array[i].text

            # 修复网页上的错误.
            if title == '第二百二十三节 余波':
                title = '第三百二十三节 余波'
            if title == '三百四十一节 调整':
                title = '三百四十二节 调整'
            if title == '四百四十一节 有技巧的善后':
                title = '四百四十二节 有技巧的善后'
            if title == '第六十五节 蝴蝶效应':
                title = '第六十六节 蝴蝶效应'
            if title == '第八十五节 救援徐岗':
                title = '第八十六节 救援徐岗'
            if title == '第一百五十三节 突围逃命':
                title = '第一百五十四节 突围逃命'
            if not title.find( curSecChn ) == -1:
                url = array[i]['href']    
                print( "Find section " + str(curSection) + " - " + array[i].text + " url:" + url )
                f.write( '%s\r\n' % title )
                text = getSectionText( url )
                f.write( text )
                f.write( '\r\n\r\n' )

                curSection += 1
                curParseIndex = i+1
                findSection = True

                # 特殊处理.
                if title .find( '第' + turn(curSection) + '节' ) != -1:
                    print( title + ' contains 2 section!' )
                    curSection += 1
                if title == '第四百一十六节 开价问题':
                    print( '第四百一十七节 missed!' )
                    curSection += 1
                if title == '第一百零六节 杨举人的野望':
                    print( '第一百零七节 missed!' )
                    curSection += 1
                break

        if not findSection:
            print( "Can't find secion " + str(curSection) + " New part!")
            break

    f.close()

print( soup )
