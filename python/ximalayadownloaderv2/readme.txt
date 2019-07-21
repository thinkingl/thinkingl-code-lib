待开发功能:
1. 完善日志, 主模块和附属模块共用日志需要处理.
3. 出现了空的主播/专辑目录.

已开发:
2019-07-12:
2. 专辑封面文件的后缀不一定是jpg, 还可能是jpeg, png
4. 总是有报错, 删除专辑文件夹的时候文件夹不存在. (删除前增加了判断, 待观察)

2019-07-11: 
1. 将配置独立到xmlycfg.py
2. 当前下载目录为临时目录, 磁盘剩余空间不足后自动移动文件到最终目录.
3. 去掉下载完专辑后的chmod过程.

2019-07-10: 优化专辑是否需要下载的判断,优化策略, 对于已完结的专辑尽量利用数据库中存储的专辑信息, 减少从网络获取专辑信息的消耗.
对于数据库中记录1月之前最后更新的专辑, 1周内不再重复检查是否需要下载.

2019-07-09: 下载文件前检测剩余磁盘空间, 100M以下等待空间释放再继续.

支持url格式:
    2级分类url:
        # 历史 - 世界史
        https://www.ximalaya.com/lishi/reci91/
    按分类搜索url:
        # 有声书 - 英文原版 - 免费
        https://www.ximalaya.com/revision/category/queryCategoryPageAlbums?category=youshengshu&subcategory=yingwenyuanban&meta=132_2722&sort=0&page=1&perPage=300
    主播Url:
        # 关键字格式是 /zhubo/主播id/
        urlSetUnfinishedPages.add( "/zhubo/45945908/" ) # 中国日报

    关键字搜索:
        # 关键格式是 key=关键字&
        搜索专辑关键字key=诺贝尔文学&

    专辑:
        # 关键格式是 /专辑id/
        #urlSetUnfinishedPages.add( "/renwen/15882009/" ) # 月亮和六便士·毛姆|慕容读经典 - 加利福尼亚慕容

