import os
import sys


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print( 'Usage: cmd sourceFile dstFile' )
        exit(1)
    sourceFilePath = sys.argv[1]
    dstFilePath = sys.argv[2]

    sourceLines = []
    with open(sourceFilePath, encoding='utf-8') as sf:
        sourceLines = sf.readlines()
        sf.close()

    dstSet = set()
    with open(dstFilePath, encoding='utf-8') as df:
        destLines = df.readlines()
        dstSet = set(destLines)
    curDstlines = open()