import os
import sys
import struct
import json

def readAttr( boxObj, f, attrName, len, structFormat ):
    attrValue = f.read( len )
    attrValue = struct.unpack( structFormat, attrValue )[0]

    if boxObj.get( attrName ) != None:
        curValue = boxObj.get(attrName)
        if isinstance( curValue, list):
            curValue.append( attrValue )
        else:
            newValue = list()
            newValue.append( curValue )
            newValue.append( attrValue )
            boxObj[ attrName ] = newValue
    else:
        boxObj[ attrName ] = attrValue
    return attrValue


def readFileTypeBox( boxObj, f, boxBeginPos, boxLen ):

    readAttr( boxObj, f, 'major_brand', 4, '>L' )
    readAttr( boxObj, f, 'minor_version', 4, '>L' )

    while f.tell() < boxBeginPos + boxLen:
        readAttr( boxObj, f, 'compatible_brands', 4, '>L' )        
    return

# moov
def readMovieBox( boxObj, f, boxBeginPos, boxLen ):
    readBoxes( boxObj, f, boxBeginPos, boxLen)
    return

def readFullBox( boxObj, f ):
    version = readAttr( boxObj, f, 'version', 1, '=B' )
    flags = f.read(3)
    boxObj['flags'] = str(flags)
    return (version, flags )

# mvhd
def readMovieHeaderBox( boxObj, f, boxBeginPos, boxLen ):
    version, flags = readFullBox(boxObj, f )
    if version == 1:
        readAttr( boxObj, f, 'creation_time', 8, '>Q')
        readAttr( boxObj, f, 'modification_time', 8, '>Q')
        readAttr( boxObj, f, 'timescale', 4, '>L')
        readAttr( boxObj, f, 'duration', 8, '>Q')
    elif version == 0:
        readAttr( boxObj, f, 'creation_time', 4, '>L')
        readAttr( boxObj, f, 'modification_time', 4, '>L')
        readAttr( boxObj, f, 'timescale', 4, '>L')
        readAttr( boxObj, f, 'duration', 4, '>L')
    else:
        print( 'Invalid version:[', version, '] in mvhd!' )
    
    readAttr( boxObj, f, 'rate', 4, '>L')
    readAttr( boxObj, f, 'volum', 2, '>H')
    readAttr( boxObj, f, 'reserved-1', 2, '>H')
    readAttr( boxObj, f, 'reserved-2', 4, '>L')
    readAttr( boxObj, f, 'reserved-3', 4, '>L')

    for i in range (0,9):
        readAttr( boxObj, f, 'matrix', 4, '>L')

    readAttr( boxObj, f, 'next_track_ID', 4, '>L')
    return

def readTrackBox(boxObj, f, boxBeginPos, boxLen):
    readBoxes( boxObj, f, boxBeginPos, boxLen)
    return

# tkhd
def readTrackHeaderBox(boxObj, f, boxBeginPos, boxLen):
    version, flags = readFullBox(boxObj, f )
    if version == 1:
        readAttr( boxObj, f, 'creation_time', 8, '>Q')
        readAttr( boxObj, f, 'modification_time', 8, '>Q')
        readAttr( boxObj, f, 'track_ID', 4, '>L')
        readAttr( boxObj, f, 'reserved-1', 4, '>L')
        readAttr( boxObj, f, 'duration', 8, '>Q')
    elif version == 0:
        readAttr( boxObj, f, 'creation_time', 4, '>L')
        readAttr( boxObj, f, 'modification_time', 4, '>L')
        readAttr( boxObj, f, 'track_ID', 4, '>L')
        readAttr( boxObj, f, 'reserved-1', 4, '>L')
        readAttr( boxObj, f, 'duration', 4, '>L')
    else:
        print( 'Invalid version:[', version, '] in tkhd!' )
    readAttr( boxObj, f, 'reserved-2', 4, '>L')
    readAttr( boxObj, f, 'reserved-3', 4, '>L')
    readAttr( boxObj, f, 'layer', 2, '>H')
    readAttr( boxObj, f, 'alternate_group', 2, '>H')
    readAttr( boxObj, f, 'volume', 2, '>H')
    readAttr( boxObj, f, 'reserved-4', 2, '>H')

    for i in range (0,9):
        readAttr( boxObj, f, 'matrix', 4, '>L')
    
    readAttr( boxObj, f, 'width', 4, '>L')
    readAttr( boxObj, f, 'height', 4, '>L')

    return

# edts
def readEditBox(boxObj, f, boxBeginPos, boxLen):
    readBoxes( boxObj, f, boxBeginPos, boxLen)
    return

# elst
def readEditListBox(boxObj, f, boxBeginPos, boxLen):
    version, flags = readFullBox(boxObj, f )
    entry_count = readAttr( boxObj, f, 'entry_count', 4, '>L' )
    for i in range(0, entry_count):
        if version == 1:
            readAttr(boxObj, f, 'segment_duration', 8, '>Q')
            readAttr(boxObj, f, 'media_time', 8, '>Q')
        else:
            readAttr(boxObj, f, 'segment_duration', 4, '>L')
            readAttr(boxObj, f, 'media_time', 4, '>L')
        readAttr( boxObj, f, 'media_rate_integer', 2, '>H' )
        readAttr( boxObj, f, 'media_rate_fraction', 2, '>H' )
    return

# mdia
def readMediaBox(boxObj, f, boxBeginPos, boxLen):
    readBoxes( boxObj, f, boxBeginPos, boxLen)
    return
def readNextBox(boxObj, f, boxBeginPos, boxLen):
    return 

readFunctionMap = {}
readFunctionMap['ftyp'] = readFileTypeBox
readFunctionMap['moov'] = readMovieBox
readFunctionMap['mvhd'] = readMovieHeaderBox
readFunctionMap['trak'] = readTrackBox
readFunctionMap['tkhd'] = readTrackHeaderBox
readFunctionMap['edts'] = readEditBox
readFunctionMap['elst'] = readEditListBox
readFunctionMap['mdia'] = readMediaBox

def readBoxes(parent, f, parentBoxBeginPos, parentBoxLen ):
    while f.tell() < parentBoxBeginPos + parentBoxLen:
        readBox( parent, f )

def readBox(parent, f ):
    boxBeginPos = f.tell()
    boxLen = f.read(4)
    if len( boxLen ) == 0:
        print( 'file end at ', boxBeginPos )
        return False
    boxLen = struct.unpack( ">L", boxLen )[0]
        
    boxType = f.read(4)
    boxType = boxType.decode('utf-8')
    if boxLen == 1:
        boxLen = f.read(8)
        boxLen = struct.unpack( ">Q", boxLen )[0]
    
    if boxType == 'uuid':
        boxType = f.read(16)    # user type

    boxObj = {}
    if parent.get( boxType ) != None:
        curValue = parent.get(boxType)
        if isinstance( curValue, list):
            curValue.append( boxObj )
        else:
            newValue = list()
            newValue.append( curValue )
            newValue.append( boxObj )
            parent[ boxType ] = newValue
    else:
        parent[ boxType ] = boxObj

    boxObj['type'] = str(boxType)
    boxObj['len'] = boxLen
    readBoxFunction = readFunctionMap.get( boxType )
    if readBoxFunction != None:
        readBoxFunction( boxObj, f, boxBeginPos, boxLen )
    else:
        print( 'Unknown box:\t', boxType )

    if boxLen == 0:
        print( 'box extends to end of file.' )
        return False
    else:
        f.seek( boxBeginPos + boxLen )
        return True


filePath = "D:\\6-video\\WeChat_20200930103550.mp4"
if len( sys.argv ) > 1:
    filePath = sys.argv[1]

with open( filePath, 'rb' ) as f:
    bOk = True
    mp4 = {}
    while bOk:
        bOk = readBox(mp4, f )

    msg = json.dumps( mp4, indent=4 )
    print( msg )