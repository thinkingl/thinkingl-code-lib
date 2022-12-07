import os
import sys

db_path = '/share/disk-ssdgw2T/avlib/avlib.db'
db_cmd_prefix = 'sqlite3 %s '%(db_path)
pic_dir = '/share/disk-ssd/tmp/avpics'

# 生成dokuwiki的文本
def genDokuwikiTxt( picId ):
    # 把属性都查出来.
    cmd = "%s 'select * from Attr where rowId in ( select attrId from PicAttr where PicId = %s ); '"%(db_cmd_prefix, picId)
    rsp = os.popen( cmd ).read().strip()
    print( "result:[%s] for cmd [%s]"%(rsp, cmd ))
    attrlines = rsp.split('\n')

    attrDic = {}
    for attr_line in attrlines:
        attr = attr_line.split('|')
        attrName = attr[0]
        attrValue = attr[1]
        #print( 'attr %s - %s'%(attrName, attrValue) )
        # 取长度最长的那个属性.
        if attrName in attrDic:
            oldValue = attrDic.get( attrName )
            if len(attrValue) > len(oldValue):
                attrDic[ attrName ] = attrValue
        else:
            attrDic[ attrName ] = attrValue

    # 按格式生成:
    # IPTD-884 前田かおりの濃厚な接吻とSEX\\
    # 识别码: 	IPTD-884 \\
    # 发行日期: 	2012-05-01\\
    # 长度: 	170 分钟\\
    # 导演: 	宇佐美忠則  \\
    # 制作商: 	IDEA POCKET  \\
    # 发行商: 	ティッシュ  \\
    # 使用者评价: 	(8.50) 	\\
    # 类别: 	单体作品数位马赛克 	\\
    # 演员: 	前田かおり 	\\

    # {{:wiki:iptd884pl.jpg|}}

    # 参考:
    # * [[http://www.javlibrary.com/cn/?v=javlioap44|IPTD-884 前田かおりの濃厚な接吻とSEX]]
    format = '{title} \\\\\n'\
        '识别码:\t{id}\\\\\n'\
        '发行日期:\t{date}\\\\\n'\
        '长度:\t{length}\\\\\n'\
        '导演:\t{director}\\\\\n'\
        '制作商:\t{maker}\\\\\n'\
        '发行商:\t{label}\\\\\n'\
        '使用者评价:\t{review}\\\\\n'\
        '类别:\t{genres}\\\\\n'\
        '演员:\t{cast}\n'\
        '\n{{:av:{imgFileName}|}}\n'\
        '\n参考:\n'\
        '  * [[{url}|{title}]]'

    txt = format
    for k in ['title', 'id', 'date', 'length', 'director', 'maker', 'label', 'review', 'genres', 'cast', 'imgFileName', 'url']:
        value = ''
        if k in attrDic:
            value = attrDic.get(k)
        txt = txt.replace( '{%s}'%k, value )
    
    print( '\n\n%s\n\n'%txt )

    

if __name__ == '__main__':
    keywords = sys.argv[1:]
    print("keywords: %s"%(keywords) )

    searchPicCmd = ''

    if len(keywords) == 1:
        keyword = keywords[0]
        if keyword.startswith( 'id='):
            id=keyword.split('=',1)[1]
            searchPicCmd = "%s \"select rowId,FileName from Pic where rowId='%s'\""%(db_cmd_prefix, id)
        

    if len(searchPicCmd) == 0:
        searchKeyword = ''
        for k in keywords :
            searchKeyword = searchKeyword + '%' + k
        searchKeyword += '%'
        
        searchKeyword = searchKeyword.replace( ' ', '%' )

        searchPicCmd = "%s \"select rowId,FileName from Pic where FileName like '%s' limit 100;\""%(db_cmd_prefix, searchKeyword)
    rsp = os.popen( searchPicCmd ).read().strip()
    print( "result:[%s] for cmd [%s]"%(rsp, searchPicCmd ))
    if( len(rsp) == 0 ):
        print( 'result is empty!' )
        exit(0)
    resultLines = rsp.split('\n')
    # 得到唯一的文件时, 打印出文件的详细信息
    if( len( resultLines ) == 1 ):
        line = resultLines[0]
        picId = line.split( '|' )[0].strip()
        picName = line.split( '|' )[1].strip()
        
        genDokuwikiTxt( picId )

        # 图片文件生成到一个目录.
        if not os.path.exists( pic_dir ) :
            os.makedirs( pic_dir )
        pic_path = os.path.join( pic_dir, picName )
        
        importPicCmd = "%s \"select writefile('%s', data) from PicData where PicId =%s\""%(db_cmd_prefix, pic_path, picId)
        rsp = os.popen( importPicCmd ).read().strip()
        print( "result:[%s] for cmd [%s]"%(rsp, importPicCmd ))
    else:
        out = ''
        for line in resultLines:
            picId = line.split( '|' )[0].strip()
            picName = line.split( '|' )[1].strip()
            title = ''
            getTitleCmd = "%s \"select name from Attr where type='title' and rowId in ( select attrId from PicAttr where PicId = %s ); \""%(db_cmd_prefix, picId)
            rsp = os.popen( getTitleCmd ).read().strip()
            print( "result:[%s] for cmd [%s]"%(rsp, getTitleCmd ))
            title = max( rsp.strip().splitlines(), key=len )
            print( "title is [%s]"%(title) )
            out = out + picId + '\t' + picName + '\t' + title + '\n'
        print( "result:\n\n" )
        print(out)
        print( "\n" )
    