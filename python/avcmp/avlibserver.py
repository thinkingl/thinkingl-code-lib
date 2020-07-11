from flask import Flask
from flask import request
from avlibdb import CAvlibDb
import json
from flask import Response
from queue import Queue,Empty
import queue
from threading import Thread
import threading
import avlibcfg
import logging
import time

# 存放待处理的用户图片比较操作的队列
# Web收到用户的请求后插入队列
# 实际处理线程在其中取出, 进行处理.
compareOperationQueue = Queue()

# 记录所有操作的日志文件, 每一行记录一个比较操作.
compareLogfile = open( avlibcfg.compareLogPath, 'ab')

# top image的列表缓存.
# begin         开始序号
# fileNameList
topImageFileCache = { "begin":0, "fileNameList":[] }

# 想要的top image列表的序号
# begin
# num
topImageFileIndexWant = {"begin":0, "num":100}

# 线程互斥锁.
topImageLock = threading.Lock()

def ThreadImageScoreManager(compareOperationQueue):
    avlib = CAvlibDb()
    avlib.ConnectDb()
    needRescore = True
    while( True ):
        try:
            op = compareOperationQueue.get( block=False )
            better = op['better']
            worse = op['worse']
            ret = avlib.PicCompare( better, worse )
            if( ret['error'] == 'ok'):
                compareLogfile.write( (json.dumps(op)+"\n").encode('utf-8'))
                compareLogfile.flush()
                needRescore = True
        except queue.Empty:
            with topImageLock:
                begin = topImageFileIndexWant['begin']
                num = topImageFileIndexWant['num']
                cacheBegin = topImageFileCache['begin']
                cacheNum = len( topImageFileCache['fileNameList'])

                # 对需求的前后放大100进行查询.
                begin -= 100
                begin = max(0, begin)
                num += 200

                if( begin < cacheBegin or begin+num > cacheBegin + cacheNum ):
                    needRescore = True
            if( needRescore ):
                
                topFileList = avlib.TopPics(begin, num)
                with topImageLock:
                    topImageFileCache['begin'] = begin
                    topImageFileCache[ 'fileNameList'] = topFileList
                needRescore = False
        time.sleep(1)

#threading._start_new_thread( ThreadImageScoreManager, compareOperationQueue )
t = Thread( target=ThreadImageScoreManager, args=(compareOperationQueue,))
t.start()

app = Flask(__name__)


@app.route("/")
def hello():
    return "Hello World!"

# 获取图片数据。
@app.route("/image/<fileName>", methods=['GET'])
def getImg(fileName):
    avlib = CAvlibDb()
    avlib.ConnectDb()
    picData = avlib.GetPicDataByName(fileName)
    if( picData == None ):
        return 'Image %s not exist!'%fileName
    rsp = Response(picData, mimetype='image/jpeg')
    #rsp.headers["Content-Disposition"] = "attachment; filename={};".format(fileName)
    return rsp

# 获取图片json信息。
@app.route("/imageInfoJson/<fileName>", methods=['GET'])
def getImgInfoJson(fileName):
    avlib = CAvlibDb()
    avlib.ConnectDb()
    imgInfo = avlib.GetPicInfoJson( fileName )
    return (imgInfo)
    #return "image info json" + fileName

# 添加一个图片
@app.route("/image/<fileName>", methods=['POST'])
def addImg(fileName):
    addImgReq = request.get_json()
    
    return ""



@app.route("/imageInfoText/<fileName>", methods=['GET'])
def getImgInfoTxt(fileName):
    return "image info text" + fileName

@app.route("/random")
def randomImg():
    num = request.args.get('num','2')
    avlib = CAvlibDb()
    avlib.ConnectDb()
    imageNameList = avlib.GetRandomPic(num)
    avlib.CloseDb()

    output = { "imageList": imageNameList }
    return output

@app.route("/compare", methods=['POST'])
def compare():
    print(request.method)
    op = request.get_json()
    ret = {'error':'ok'}
    if( op != None and op['better'] != None and op['worse'] != None):
        compareOperationQueue.put( op )
    else:
        ret['error'] = 'Invalid operation!'

    return ( ret )

@app.route("/top", methods=['POST','GET'])
def topImg():
    want = None
    if( request.method == 'GET' ):
        want = request.args
    if( request.method == 'POST'):        
        want = request.get_json()
    ret = {'error':'ok'}
    if( want != None and want['begin'] != None and want['num'] != None):
        with topImageLock:
            begin = int( want['begin'] )
            num = int( want['num'] )
            topImageFileIndexWant['begin'] = begin
            topImageFileIndexWant['num'] = num
            if( begin >= topImageFileCache['begin'] and begin + num <= ( topImageFileCache['begin'] + len(topImageFileCache['fileNameList']) )):
                fileList = topImageFileCache['fileNameList'][ begin - topImageFileCache['begin'] : begin - topImageFileCache['begin'] + num ]
                ret['fileList'] = fileList
                ret['begin'] = begin
                ret['num'] = num
            else:
                ret['error'] = 'Top image cache have not prepared! Please try again latter!'
    else:
        ret['error'] = 'Invalid operation!'
        
    return ( ret )

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5001)