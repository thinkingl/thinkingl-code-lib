#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os, logging, shutil, sys

# 移动文件夹, 同名覆盖.
# src: d:\abc
# dst: e:\abc
def moveTreeForce(src, dst, deleteSrc):
    if not os.path.isdir( dst ):
        os.mkdir( dst )
        logging.info('mkdir %s', dst)

    errorOccur = False
    for name in os.listdir(src):
        srcPath = os.path.join(src, name)
        dstPath = os.path.join(dst, name )
        if os.path.isdir(srcPath):
            moveTreeForce( srcPath, dstPath, True )
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
    if not errorOccur and os.path.isdir(src) and deleteSrc:
        os.removedirs(src)  # 移动完成, 删除原目录.


if __name__=="__main__":
    src = sys.argv[1]
    dst = sys.argv[2]
    moveTreeForce(src, dst, False)  # 不删除源根目录.
