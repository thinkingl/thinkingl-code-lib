# 遍历目录, 读取所有文件, 检测哪些文件有损坏.

import os
import sys
import time

blockSize = os.stat('.').st_size
errorlog = open('errors.log', 'a')
errorlog.write( 'files check for ' + os.path.abspath('.') + ' start! time:' + time.ctime() + "\n" )
errorlog.flush()
for root,dirs,files in os.walk( '.' ):
    for fn in files:
        try:
            path = os.path.join( root, fn )
            print( path )
            #fd = os.open(path,os.O_DIRECT|os.O_RDONLY)
            fd = os.open(path,os.O_SYNC|os.O_RDONLY)
            while(os.read(fd,100*blockSize)):
                continue
            os.close(fd)
        except Exception as e:
            print( path + "\tread fail! e:", e )
            errorlog.write( path + '\t' + str(e) + '\n')
            errorlog.flush()

errorlog.write( 'files check for ' + os.path.abspath('.') + ' end! time:' + time.ctime() + "\n" )
errorlog.close()