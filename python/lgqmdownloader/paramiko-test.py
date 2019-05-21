#!/usr/bin/python
# coding=utf-8

import paramiko
import os

def sftp_upload(host,port,username,password,local,remote):
    sf = paramiko.Transport((host,port))
    sf.connect(username = username,password = password)
    sftp = paramiko.SFTPClient.from_transport(sf)
    try:
        if os.path.isdir(local):#判断本地参数是目录还是文件
            for f in os.listdir(local):#遍历本地目录
                sftp.put(os.path.join(local+f),os.path.join(remote+f))#上传目录中的文件
        else:
            sftp.put(local,remote)#上传文件
    except Exception as e:
        print('upload exception:',e)
    sf.close()

host = '172.16.251.1'#主机
port = 2222 #端口
username = 'kedacom' #用户名
password = 'kedacom#123' #密码
local = 'F:\\sftptest\\'#本地文件或目录，与远程一致，当前为windows目录格式，window目录中间需要使用双斜线
remote = '/tmp'#远程文件或目录，与本地一致，当前为linux目录格式
sftp_upload(host,port,username,password,local,remote)#上传
