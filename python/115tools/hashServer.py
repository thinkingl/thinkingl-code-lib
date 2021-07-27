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
import hashcfg
from logging.handlers import RotatingFileHandler


app = Flask(__name__)


def initLogging( logFileName, logDir='./log', maxLogFileBytes=10*1024*1024, backupCount = 10 ):
	if logDir == None:
		logDir = ''
	if not os.path.isdir( logDir ):
		os.makedirs( logDir )

	# 循环覆盖的日志文件. 
	logFileName = os.path.join( logDir, logFileName )
	fh = RotatingFileHandler(logFileName, maxBytes=maxLogFileBytes, backupCount=backupCount,encoding="utf-8")

	fh.setLevel(logging.DEBUG)

	# 使用StreamHandler输出到屏幕
	ch = logging.StreamHandler()
	ch.setLevel(logging.INFO)

	logging.basicConfig(level=logging.INFO,
		format   = '%(asctime)s.%(msecs)03d  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
		datefmt  = '%Y-%m-%d %H:%M:%S',                                     # 时间
		#filename = logFileName,                # log文件名
		#filemode = 'w',
		handlers = [fh, ch]
	)

initLogging('hashserver.log', logDir=hashcfg.LogDir)

def appendHashlinkLogFile(hashlink:str):
    hashlinklogpath = hashcfg.HashLogPath
    hashlogdir = os.path.dirname( hashlinklogpath )
    if not os.path.isdir( hashlogdir ):
        os.makedirs( hashlogdir )
    with open( hashlinklogpath, 'a', encoding='utf-8' ) as f:
        f.write( hashlink + '\n' )
        f.close()

@app.route("/hashlink")
def hello():
    return "Hello, I'm HashServer!"

@app.route("/hashlink", methods=['PUT'])
def addHashlink():
    hash = request.get_json()
    if not "updateTime" in hash:
        hash['updateTime'] = time.time()
    logging.info( 'Add hash link: %s', hash )
    db = HashlinkDB(hashcfg.HashDBPath)
    db.addHashlink(**hash)

    appendHashlinkLogFile( Hashlink(hash).getHashlink() )
    return {"error":0}

@app.route("/hashlink/<dirName>/<fileName>", methods=['GET'])
def getHashlink(dirName:str, fileName:str):
    #searchParam = request.args
    db = HashlinkDB( hashcfg.HashDBPath )
    hash = db.getHashlink(dirName, fileName)
    if hash == None:
        return {'error':-1, 'msg':'file not exist!'}
    hash['hashlink'] = Hashlink(hash).getHashlink()
    return {"error":0, 'hash':hash}


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5003)