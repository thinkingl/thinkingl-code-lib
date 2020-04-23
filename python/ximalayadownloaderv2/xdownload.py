import requests
import os
import logging
import time



def download( url, file_path, customHeaders ):    
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
            logging.error( 'get content length is 0! url: ' + url )
            return False
        if total_size > 0 and total_size >= temp_size:
            logging.info( 'download already finished! url:' + url + ' total:%d - cur file size:%d'%(total_size, temp_size) )
            return True

    headers = {'Range': 'bytes=%d-' % temp_size}  
    if customHeaders != None:
        headers.update( customHeaders )

    logging.info( 'try download from ' + str( temp_size ) )
    r = requests.get(url, stream=True, verify=False, headers=headers)

    if r.status_code < 200 or r.status_code >=300:
        logging.error( "download http fail! url: %s r.status code: %d"%( url, r.status_code ) )
        r.close()
        return False

    total_size = int(r.headers['Content-Length'])
    logging.info( "content length:" + str( total_size ) )

    with open( file_path, 'ab' ) as f:
        for chunk in r.iter_content(chunk_size=1024):
            if chunk:
                temp_size += len(chunk)
                f.write(chunk)
    f.close()
    r.close()

    return os.path.exists(file_path) and os.path.getsize(file_path) >= total_size

def tryDownload(url, file_path, tryTimes, headers):
    if tryTimes == None or tryTimes <= 0:
        tryTimes = 100

    for i in range( 0,tryTimes ):
        result = download( url, file_path, headers )
        if result:
            logging.info( "Download success!" )
            return True
        time.sleep( 30 )
    os.remove( file_path )
    logging.error( "Download file fail! url: %s remove file %s !"%( url, file_path) )
    return False

if __name__ == "__main__":    
    url = 'http://aod.cos.tx.xmcdn.com/group15/M0B/4E/57/wKgDaFWx97_D8vWNAHba91YNMLw836.m4a'
    print( url )
    file_path = "e:/tmp/test.m4a"
    tryDownload(url, file_path, 0, None)