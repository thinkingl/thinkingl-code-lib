import os
import logging
from logging.handlers import RotatingFileHandler


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

if __name__=="__main__":
	initLogging('rotatinglog.log',maxLogFileBytes=20, backupCount=12)
	logging.info( '----rotating log test start!----' )
	for i in range(0,100):
		logging.info( 'write log %d -----------------------------------------------------------', i )
	for logFile in os.listdir( './log' ):
		print( logFile )
	