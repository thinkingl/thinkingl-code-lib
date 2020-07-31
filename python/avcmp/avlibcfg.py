import os

# 图片比较日志的存放目录.
compareLogDir = '/share/disk-ssd/avlib'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = 'H:/data/javlib'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = 'D:/999-temp/javlib'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = '/share/disk-160/avlib'
compareLogPath = compareLogDir + 'avlibcomp.log'
# 数据库文件的存放目录.
DbDir = '/share/disk-ssd/avlib/'
if( not os.path.isdir( DbDir ) ):
    DbDir = 'H:/data/javlib'
if( not os.path.isdir( DbDir ) ):
    DbDir = 'D:/999-temp/javlib'
if( not os.path.isdir( DbDir ) ):
    DbDir = '/share/disk-160/avlib'
DbFileName = 'avlib.db'

# 图片文件的存放目录.
BaseDir = 'H:/data/javlib/'
FileDir = 'datas2/'
