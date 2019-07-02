import os

# 图片比较日志的存放目录.
compareLogDir = '/share/disk-320/'
if( not os.path.isdir( compareLogDir ) ):
    compareLogDir = 'D:/999-temp/javlib/'
compareLogPath = compareLogDir + 'avlibcomp.log'
# 数据库文件的存放目录.
DbDir = '/share/disk-320/'
if( not os.path.isdir( DbDir ) ):
    DbDir = 'c:/9-tmp/'
DbFileName = 'avlib.db'

# 图片文件的存放目录.
BaseDir = 'D:/999-temp/javlib/'
FileDir = 'datas2/'
