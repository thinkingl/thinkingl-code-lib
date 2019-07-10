待开发功能:
2019-07-09: 优化专辑是否需要下载的判断,优化策略, 对于已完结的专辑尽量利用数据库中存储的专辑信息, 减少从网络获取专辑信息的消耗.

已开发:
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

