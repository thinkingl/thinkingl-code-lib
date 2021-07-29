import os
import sys
import argparse

def getHashlinkKey(hashlink:str) ->str:
    hashlink = hashlink.strip()
    ar = hashlink.split('|')
    # 115://mdvr00024_A.mp4|2047301498|07718F4549EEB9EFBD6B301353E635A3DA0728FC|31CB7D0CB548DBCB3483FE66EC91800D5B84C093|MDVR-024
    # 文件名部分可能是修改过的,所以忽略文件名, 比较其余的部分.
    key = str( ar[1:4] )
    return key


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Union hash link files.')
    
    parser.add_argument('srcFiles', metavar='SrcFile', type=str, nargs='+',
                    help='source files')

    parser.add_argument('dstFile', metavar='DstFile', type=str,
                    help='source files')

    parser.add_argument('--test', 
                    help='Only test, do not union.', action="store_true")

  

    args = parser.parse_args()
    
    sourceFiles = args.srcFiles
    destFile = args.dstFile
    onlyTest = args.test

    sourceLines = []
    for srcFile in sourceFiles:
        with open( srcFile, encoding='utf-8') as sf:
            sourceLines += sf.readlines()
            sf.close()

    dstSet = set()
    with open(destFile, encoding='utf-8') as df:
        destLines = df.readlines()
        for l in destLines:
            dstSet.add( getHashlinkKey(l) )
        df.close()

    df = None
    if not onlyTest:
        df = open(destFile, 'a', encoding='utf-8')
    for sl in sourceLines:
        k = getHashlinkKey(sl)
        if not k in dstSet:
            if df:
                df.write( sl )
            print( sl )
    if df:
        df.close()

            