#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import struct
import io
import mutagen

class CacheFile:
    def __init__(self, path : str ) -> None:
        self._path = path
        self._data = None

    def getFileType(self) -> str:
        tmpPath = 'e:/tmp/tmp-music.unknown'
        self.exportDecodedFile( tmpPath )
        info = mutagen.File( tmpPath )
        os.remove( tmpPath )
        for m in info.mime:
            if m.endswith( 'flac' ):
                return 'flac'
            if m.endswith( 'mp3' ):
                return 'mp3'
        assert( False )
        return ""

        if self._data == None :
            self._data = self.decodeFile()
        offset = 0
        id3_tag = struct.unpack_from( '3s', self._data, offset)[0]
        if id3_tag == b'ID3':
            print( 'find ID3 header!' )
            '''
                ID3v2/file identifier      "ID3"
                ID3v2 version              $04 00
                ID3v2 flags                %abcd0000
                ID3v2 size             4 * %0xxxxxxx
            '''

            offset += 3
            version = struct.unpack_from( '2s', self._data, offset)
            offset += 2
            flags = struct.unpack_from( 'c', self._data, offset)
            offset += 1
            size = struct.unpack_from( '!L', self._data, offset )
            offset += 4



        if self._data[offset:4] == b'fLaC':
            print( 'flac format!!' )
            return 'flac'
        

        return "mp3"
    
    def decodeFile(self) -> bytearray:
        with open ( self._path,'rb') as f:
            btay = bytearray(f.read())
            for i,j in enumerate(btay):
                btay[i] = j ^ 0xa3
            return btay
    
    def readFileRaw(self) -> None:
        with open ( self._path,'rb') as f:
            self._data = bytearray(f.read())
        
    def exportDecodedFile(self, path : str ) -> None:
        if self._data == None :
            self._data = self.decodeFile()
        with open( path, "wb" ) as f:
            f.write( self._data )
            f.close()


if __name__=="__main__":
    # test
    path = "F:/0-music/0-163music/李健/李健/李健 - 美若黎明.flac"
    cf = CacheFile( path )
    cf.readFileRaw()
    type = cf.getFileType()
    assert( type == 'flac' )

    path = 'F:/0-music/1-ipod/王若琳 - I Love You.mp3'
    cf = CacheFile( path )
    cf.readFileRaw()
    type = cf.getFileType()
    assert( type == 'mp3' )

    path = 'e:/Cache/240515-999-320b9df4cd6da4d2717e0af12cf14673.uc'
    cf = CacheFile( path )
    type = cf.getFileType()
    assert( type == 'flac' )

    for f in os.listdir( "e:/Cache" ):
        path = os.path.join( "e:/Cache", f )
        if f.endswith( ".uc" ):
            cf = CacheFile( path )
            type = cf.getFileType()
            print( f, " type is ", type, " size: ", os.path.getsize( path ) )
            assert( type == 'flac' )


