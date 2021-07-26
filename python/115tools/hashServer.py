from hashlinkDB import HashlinkDB
from flask import Flask
from flask import request
import json
from flask import Response
import queue
from threading import Thread
import logging
import time
import os
import re
from hashlink import Hashlink
from hashtools import *
import requests

app = Flask(__name__)


def initLogging():
    #sys.stdout.reconfigure(encoding='utf-8')
    # 使用FileHandler输出到文件
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式

    if not os.path.isdir( 'logs' ):
        os.makedirs( 'logs' )

    logFileName = time.strftime('logs/hashserver-%Y%m%d-%H%M%S.log',time.localtime())
    
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
initLogging()

def appendHashlinkLogFile(hashlink:str):
    with open( 'hashlinklog.txt', 'a', encoding='utf-8' ) as f:
        f.write( hashlink + '\n' )
        f.close()

@app.route("/")
def hello():
    return "Hello, I'm HashServer!"

@app.route("/hashlink", methods=['PUT'])
def addHashlink():
    hash = request.get_json()
    if not "updateTime" in hash:
        hash['updateTime'] = time.time()
    logging.info( 'Add hash link: %s', hash )
    db = HashlinkDB()
    db.addHashlink(**hash)

    appendHashlinkLogFile( Hashlink(hash).getHashlink() )
    return {"error":0}

@app.route("/hashlink/<dirName>/<fileName>", methods=['GET'])
def getHashlink(dirName:str, fileName:str):
    #searchParam = request.args
    db = HashlinkDB()
    hash = db.getHashlink(dirName, fileName)
    if hash == None:
        return {'error':-1, 'msg':'file not exist!'}
    hash['hashlink'] = Hashlink(hash).getHashlink()
    return {"error":0, 'hash':hash}


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5003)