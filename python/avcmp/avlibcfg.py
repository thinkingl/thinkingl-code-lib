import os

# 图片比较日志的存放目录.
compareLogDir = '/share/disk-ssd/avlib'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = 'H:/data/javlib'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = 'D:/999-temp/javlib'
compareLogPath = compareLogDir + 'avlibcomp.log'
# 数据库文件的存放目录.
DbDir = '/share/disk-320/'
if( not os.path.isdir( DbDir ) ):
    DbDir = 'H:/data/javlib'
if( not os.path.isdir( DbDir ) ):
    DbDir = 'D:/999-temp/javlib'
DbFileName = 'avlib.db'

# 图片文件的存放目录.
BaseDir = 'H:/data/javlib/'
FileDir = 'datas2/'
