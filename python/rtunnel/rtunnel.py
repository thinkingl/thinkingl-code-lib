# -*- coding: utf-8 -*- 
import logging
import time
import json
import os
import sys
from logging.handlers import RotatingFileHandler
import rtunnelClient
import rtunnelServer
import rtunnelServerAsync
import xcell
import argparse


def initLogging( logFileName, logDir='./log', maxLogFileBytes=10*1024*1024, backupCount = 10, logLev = logging.INFO ):
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

	logging.basicConfig(level=logLev,
		format   = '%(asctime)s.%(msecs)03d  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
		datefmt  = '%Y-%m-%d %H:%M:%S',                                     # 时间
		#filename = logFileName,                # log文件名
		#filemode = 'w',
		handlers = [fh, ch]
	)

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='XCell')
	parser.add_argument('--mode', nargs='?', default='cell', help='work mode')
	parser.add_argument('--cfg', nargs='?', default='xcell.json', help='configure file')

	args = parser.parse_args( )

	mode = args.mode

	cfgFile = args.cfg

	lev = logging.INFO
	cfg = json.load( open( cfgFile, 'r') )
	logCfg = cfg.get('log')
	if  logCfg != None:
		if logCfg.get( 'logLev') != None:
			lev = logCfg.get( 'logLev')
		

	initLogging( 'rtunnel-' + cfgFile + '.log', logLev=lev )
	logging.info( '------------rtunnel ' + mode + ' start!---------------' )


	if mode == 'client':
		client = rtunnelClient.RTunnelClient()
		client.start( cfg.get( mode ))
	elif mode == 'server':
		serverCfg = cfg.get( mode )
		server = rtunnelServerAsync.RTunnelServer()
		server.start(serverCfg)
	elif mode == 'cell':
		cell = xcell.XCell()
		cell.start( cfg )
		
	while( True ):
		time.sleep(1)
		