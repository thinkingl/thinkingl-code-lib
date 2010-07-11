# Copyright(C) 2008 SupDo.com
# Licensed under the GUN License, Version 3.0 (the "License");
# 
# File:        ReadMe.txt
# Author:      KuKei
# Create Date: 2008-07-16
# Description: readme
# Modify Date: 2008-12-31

使用须知：
1.欢迎使用SDBlog1.0
2首先您必须对GAE有一定的了解，比如如何上传文件，登录后台控制面板等等。
3此程序为安装程序，支持SDBlog1.0 Beta版本升级。
4SDBlog1.0 Beta版本中的备份文件在正式版中无法使用，因此建议使用升级功能。

安装使用说明：
1.在上传文件之前先做配置；
   a.先将app.yaml中第一行的app-id改为您自己的id.
   b.如果你是Apps帐户或者想添加多个管理员，请将帐户的email在Settings中做配置；
     将这句代码ADMINLIST =['quke.cn@gmail.com']中的Email改为自己的帐户名称；如果有多个管理员可用逗号分开；

2.上传文件；
   a.如果你对python及gae有一定的了解可以用Google提供的python脚本上传文件，说明详见gae官方文件。
   b.你也可以使用我们开发的第三方上传工具，此工具不依赖python+gae的环境;见http://www.quke.cn/log-150.html；

3.安装；输入http://app-id.appspot.com/install/开始安装；共4步，必须安装到最后一步，否者不保证安全。安装完毕开始使用；

4.管理；点击页面的右上角图标进入管理页面；

5.卸载：请先登录GAE后台，将Status表中'step'为'5'的数据删除，然后输入http://app-id.appspot.com/install/?ActionDo=Rebuild执行；

升级使用说明
1.在上传文件之前先做配置；
   a.先将app.yaml中第一行的app-id改为您自己的id.
   b.如果你是Apps帐户或者想添加多个管理员，请将帐户的email在Settings中做配置；
     将这句代码ADMINLIST =['quke.cn@gmail.com']中的Email改为自己的帐户名称；如果有多个管理员可用逗号分开；

2.上传文件；
   a.如果你对python及gae有一定的了解可以用Google提供的python脚本上传文件，说明详见gae官方文件。
   b.你也可以使用我们开发的第三方上传工具，此工具不依赖python+gae的环境;见http://www.quke.cn/log-150.html；
   
3.升级：输入http://app-id.appspot.com/upgrade/，点击升级按钮。

如有任何疑问可到论坛：http://groups.google.com/group/sdblog/或者博客http://www.quke.cn提出；