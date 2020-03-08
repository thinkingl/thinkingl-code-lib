import os

testDir = 'h:/disktest'
if not os.path.isdir( testDir ):
    testDir = '/share/disk-ssd/tmp/disktest'
if not os.path.isdir( testDir ):
    testDir = './data'

diskName = 'h:'
if not os.path.isdir( diskName ):
    diskName = '/dev/sda'
    
testTotalWrite = 10 * 1024 *1024*1024*1024 # T

# 总共100G
testFileSize = 1*1024*1024*1024 #1g
testFileNum = 1500

dataFilePath = './data.7z'