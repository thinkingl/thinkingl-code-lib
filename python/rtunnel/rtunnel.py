# -*- coding: utf-8 -*- 
import logging
import time
import json
import os
import sys
from logging.handlers import RotatingFileHandler
import rtunnelClient
import rtunnelServer



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
	ch.setLevel(logging.DEBUG)

	logging.basicConfig(level=logging.DEBUG,
		format   = '%(asctime)s.%(msecs)03d  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
		datefmt  = '%Y-%m-%d %H:%M:%S',                                     # 时间
		#filename = logFileName,                # log文件名
		#filemode = 'w',
		handlers = [fh, ch]
	)

if __name__=="__main__":
    mode = 'client'
    if sys.argv[-1] == '-c':
        mode = 'client'
    elif sys.argv[-1] == '-s':
        mode = 'server'

    initLogging( 'rtunnel-' + mode + '.log' )
    logging.info( '------------rtunnel ' + mode + ' start!---------------' )

    cfg = json.load( open( 'rtunnel.json', 'r') )

    if mode == 'client':
        client = rtunnelClient.RTunnelClient()
        client.startAsync( cfg.get( mode ))
    elif mode == 'server':
        serverCfg = cfg.get( mode )
        server = rtunnelServer.RTunnelServer()
        server.start(serverCfg)
        
    while( True ):
        time.sleep(1)
        