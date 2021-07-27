import os
import sys

def getHashlinkKey(hashlink:str) ->str:
    hashlink = hashlink.strip()
    ar = hashlink.split('|')
    # 115://mdvr00024_A.mp4|2047301498|07718F4549EEB9EFBD6B301353E635A3DA0728FC|31CB7D0CB548DBCB3483FE66EC91800D5B84C093|MDVR-024
    # 文件名部分可能是修改过的,所以忽略文件名, 比较其余的部分.
    key = str( ar[1:] )
    return key


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
        for l in destLines:
            dstSet.add( getHashlinkKey(l) )
        df.close()

    with open(dstFilePath, 'a', encoding='utf-8') as df:
        for sl in sourceLines:
            k = getHashlinkKey(sl)
            if not k in dstSet:
                df.write( sl )
                print( sl )
        df.close()

            