#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os, logging, shutil, sys, time

# 移动文件夹, 同名覆盖.
# src: d:\abc
# dst: e:\abc
def moveTreeForce(src, dst):
    if not os.path.isdir( dst ):
        os.mkdir( dst )
        logging.info('mkdir %s', dst)

    errorOccur = False
    for name in os.listdir(src):
        srcPath = os.path.join(src, name)
        dstPath = os.path.join(dst, name )
        if os.path.isdir(srcPath):
            moveTreeForce( srcPath, dstPath )
            continue
        if os.path.exists( dstPath ):
            os.remove( dstPath )
        try:
            shutil.copy2( srcPath, dstPath )
        except:
            logging.exception( 'error' )
            logging.error( "copy file %s to %s fail!", srcPath, dstPath )
            errorOccur = True
        if os.path.isfile( dstPath ):
            os.remove( srcPath )    # 没有问题的话删除原文件.
        else:
            errorOccur = True
    if not errorOccur and os.path.isdir(src):
        os.removedirs(src)  # 移动完成, 删除原目录.

def initLogging( enableLogFile ):
    formatter   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s'    # 定义输出log的格式
    
    #fh.setFormatter(formatter)

    # 使用StreamHandler输出到屏幕
    ch = logging.StreamHandler()
    ch.setLevel(logging.INFO)
    #ch.setFormatter(formatter)
    #logging.addHandler( fh )
    #logging.addHandler( ch )
    loggingHandlers = [ ch ]
    if enableLogFile:
        logFileName = time.strftime('xmlylog-%Y%m%d-%H%M%S.log',time.localtime())
        fh = logging.FileHandler(logFileName)
        fh.setLevel(logging.DEBUG)
        loggingHandlers.append( fh )

    logging.basicConfig(level=logging.INFO,
        format   = '%(asctime)s  %(filename)s:%(lineno)d:%(funcName)s : %(levelname)s  %(message)s',    # 定义输出log的格式
        datefmt  = '%Y-%m-%d %A %H:%M:%S',                                     # 时间
        #filename = logFileName,                # log文件名
        #filemode = 'w',
        handlers = loggingHandlers
    )

if __name__=="__main__":
    initLogging(False)
    src = sys.argv[1]
    dst = sys.argv[2]
    dirName = os.path.basename( src )
    dst = os.path.join( dst, dirName )
    moveTreeForce( src, dst )
