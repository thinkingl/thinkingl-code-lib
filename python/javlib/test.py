from threading import local
import urllib
import os
import time
from urllib.request import urlretrieve
import logging
import requests



def download( url, file_path, customHeaders ):    

    # 发现续传会导致文件内容损坏, 这里禁掉续传功能.
    try:
        if os.path.isfile( file_path ):
            os.remove( file_path )
            logging.info( 'remove unfinished file %s before download.', file_path )
    except:
        pass

    temp_size = 0
    if os.path.exists(file_path):
        temp_size = os.path.getsize(file_path)  # 本地已经下载的文件大小
    else:
        temp_size = 0
    
    if( temp_size > 0 ):
        r = requests.get( url,stream=True, verify=False, headers=customHeaders )
        total_size = int(r.headers['Content-Length'])
        statusCode = r.status_code
        r.close()
        if not statusCode == 200:
            logging.error( 'get content length fail! url: '+ url + ' status code: ' + str(statusCode) )
            return False
        if total_size == 0:
            logging.error( 'get content length is 0! url: %s file:%s'% (url, file_path) )
            return False
        if total_size > 0 and total_size <= temp_size:
            logging.info( 'download already finished! url:' + url + ' total:%d - cur file size:%d'%(total_size, temp_size) )
            return True

    headers = {'Range': 'bytes=%d-' % temp_size}  
    if customHeaders != None:
        headers.update( customHeaders )

    if temp_size > 0:
        logging.info( 'try download from %d file:%s url:%s'%( temp_size, file_path, url ) )
    r = requests.get(url, stream=True, verify=False, headers=headers)

    if r.status_code < 200 or r.status_code >=300:
        logging.error( "download http fail! url: %s r.status code: %d file:%s"%( url, r.status_code, file_path ) )
        r.close()
        return False

    total_size = int(r.headers['Content-Length'])
    logging.info( "content length: %d file:%s"%( total_size, file_path ) )

    with open( file_path, 'ab' ) as f:
        for chunk in r.iter_content(chunk_size=1024):
            if chunk:
                temp_size += len(chunk)
                f.write(chunk)
    f.close()
    r.close()

    return os.path.exists(file_path) and os.path.getsize(file_path) == total_size

def downloadFileTry(url, localPath):
    for i in range(5):                
        try:
            #urllib.request.urlretrieve( url, localPath)
            download( url, localPath, None )
            #logging.info( 'downlaod file success! [%s] - [%s]', localPath, url)
            return True
        except Exception as e:
            #logging.error( "download file except [%s], try again! [%s] - [%s]", e, url, localPath)
            if os.path.isfile( localPath ):
                os.remove( localPath )
            time.sleep(10)
    #logging.error( "download file fail! [%s] - [%s]", url, localPath)    
    return False

downloadFileTry( 'https://img11.pixhost.to/images/141/46725394_i39015.jpg', 'e:/test.jpg')