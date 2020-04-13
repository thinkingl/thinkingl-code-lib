#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
import sqlite3
import time
import json
#import thread
#这里我们提供必要的引用。基本控件位于pyqt5.qtwidgets模块中。
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QHBoxLayout, QDialog, QVBoxLayout, QPushButton, QSizePolicy
from PyQt5.QtGui import QWindow, QPicture, QPixmap
from PyQt5.QtCore import Qt
import logging
#from urllib import request # 速度非常慢..
import urllib
import requests
import threading
from queue import Queue,Empty
import queue
import labelAvPic  


#BaseUrl = 'http://192.168.84.8:5000/'
BaseUrl = 'http://127.0.0.1:5001/'

topPicNum = 4
picOne = None
picTwo = None
topPicLabels = None

class AVCmpClient():
    randomPics = None

    topPicBegin = 0
    #topPicList = []

    topCmdQueue = Queue()
    topPicQueue = Queue()
    randomQueue = Queue(maxsize=100)

    def __init__(self):
        print( 'AVCmpClient inint!')
        topThread = threading.Thread(target=self.ThreadTopPic, args=(self.topCmdQueue, self.topPicQueue))
        topThread.start()
        randomThread = threading.Thread(target=self.ThreadRandom, args=(self.randomQueue,))
        randomThread.start()

    def ShowRandom(self):
        item = None
        try:
            item = self.randomQueue.get(block=False)
        except queue.Empty:
            pass
        if( item != None ):
            fileName1 = item[0]['name']
            fileName2 = item[1]['name']
            data1 = item[0]['data']
            data2 = item[1]['data']
            self.randomPics = [fileName1, fileName2]
            picOne.ShowPic( fileName1, data1 )
            picTwo.ShowPic( fileName2, data2 )
        # urlRandom = BaseUrl + 'random'
        # randomPics = json.loads( request.urlopen( urlRandom ).read() )
        # if( len(randomPics) < 2):
        #     print( "error random pics : " + str(randomPics) )
        # self.ShowPic( picOne, randomPics[0] )
        # self.ShowPic( picTwo, randomPics[1])

    def CmpPic(self,betterPic, worsePic):
        cmpReq = { 'better': betterPic, 'worse':worsePic}
        url = BaseUrl + 'compare'
        rsp = requests.post( url, json = cmpReq )
        print( rsp.text )
        self.ShowRandom()
        return rsp.text

    def ShowTop(self):
        topQueueItem = None
        while True:
            try:
                topQueueItem = self.topPicQueue.get( block=False )
            except queue.Empty:
                break
        if topQueueItem != None:
            topFileList = topQueueItem['fileList']
            topPicIndex = 0
            for item in topFileList:
                fileName = item['name']
                data = item['data']
                if topPicIndex < len( topPicLabels ):
                    topPicLabels[topPicIndex].ShowPic( fileName, data )
                topPicIndex += 1

        # url = BaseUrl + 'top?begin=%s&num=%s'%(self.topPicBegin, topPicNum)
        # topPics = json.loads( request.urlopen( url ).read() )
        # curTopPicList = topPics['fileList']
        # if( curTopPicList != self.topPicList ):
        #     self.topPicList = curTopPicList
        #     topPicIndex = 0
        #     for fileName in self.topPicList:
        #         if topPicIndex < len( topPicLabels ):
        #             self.ShowPic( topPicLabels[topPicIndex], fileName )
        #         topPicIndex+=1


    def OnLeftBetter(self):
        print( "Left better!")
        self.CmpPic( self.randomPics[0], self.randomPics[1])
    def OnRightBetter(self):
        print( "Right better!")
        self.CmpPic( self.randomPics[1], self.randomPics[0])

    def OnPageUp(self):
        print( 'page up')
        self.topPicBegin -= (topPicNum-1)
        self.topPicBegin = max( 0, self.topPicBegin )
        cmd = {'begin':self.topPicBegin, 'num':topPicNum}
        self.topCmdQueue.put( cmd )

    def OnPageDown(self):
        print( 'page down')
        self.topPicBegin += (topPicNum - 1)
        cmd = {'begin':self.topPicBegin, 'num':topPicNum}
        self.topCmdQueue.put( cmd )

    def OnTimerUpdateTop(self):
        self.ShowTop()
        timer = threading.Timer(1, self.OnTimerUpdateTop)
        timer.start()
    
    def GetPicData(self, fileName):
        url = BaseUrl + 'image/%s'%(urllib.parse.quote( fileName ))
        #picData = request.urlopen( url ).read()
        rsp = requests.get( url )
        picData = rsp.content
        return picData

    # 获取top 图片的线程.
    # 从 topCmdQueue 中获取 { 'begin':? 'num':?}
    # 向 topPicQueue 中放置 { 'begin':? fileList[{ 'name':?, 'data':? },....]}
    def ThreadTopPicWork(self, topCmdQueue, topPicQueue, begin, num, topPicList, topPicCache):
        try:
            try:
                cmd = topCmdQueue.get( timeout = 5 )
                begin = cmd['begin']
                num = cmd['num']
                #continue    # 一直取空为止, 只关心最后的命令.
            except queue.Empty:
                pass
            url = BaseUrl + 'top?begin=%s&num=%s'%(begin, num)
            #topPics = json.loads( request.urlopen( url ).read() )
            rsp = requests.get(url)
            topPics = rsp.json()
            if( topPics['error'] != 'ok'):
                print( 'Get top fail! rsp:' + str(topPics) )
                return
            curTopPicList = topPics['fileList']
            if( curTopPicList != topPicList ):
                topPicQueueItem = {}
                topPicQueueItem['begin'] = begin
                topPicQueueItemPicList = list()
                topPicList = curTopPicList
                for fileName in topPicList:
                    if( fileName in topPicCache ):
                        picData = topPicCache[ fileName ]
                    else:
                        picData = self.GetPicData(fileName)
                        topPicCache[fileName] = picData
                    
                    picFile = {}
                    picFile['name'] = fileName
                    picFile['data'] = picData
                    topPicQueueItemPicList.append( picFile )
                topPicQueueItem['fileList'] = topPicQueueItemPicList

                topPicQueue.put( topPicQueueItem )
        except:
            print( 'ThreadTopPicWork fail!')
            logging.exception( 'message' )
            time.sleep( 5 )
        
    def ThreadTopPic(self, topCmdQueue, topPicQueue):
        num = topPicNum
        topPicList = None
        topPicCache = {}
        while True:
            self.ThreadTopPicWork(topCmdQueue, topPicQueue, self.topPicBegin, topPicNum, topPicList, topPicCache)
            #time.sleep(1)


    # 获取Random图片, 放入randomPicQueue
    # [{ 'name':?, 'data':? }, {'name':?'data':?}]
    def ThreadRandom(self, randomPicQueue ):
        while True:
            try:
                urlRandom = BaseUrl + 'random'
                rsp = requests.get(urlRandom)
                randomPics = rsp.json()['imageList']
                #randomPics = json.loads( request.urlopen( urlRandom ).read() )
                if( len(randomPics) < 2):
                    print( "error random pics : " + str(randomPics) )
                    continue
                fileName1 = randomPics[0]
                fileName2 = randomPics[1]

                item = [{'name':fileName1, 'data':self.GetPicData(fileName1)},{'name':randomPics[1], 'data':self.GetPicData(fileName2)}]
                randomPicQueue.put( item )
            except:
                print( 'Thread Random work fail!' )
                logging.exception( 'message' )
                time.sleep(5)

if __name__ == '__main__':
    #每一pyqt5应用程序必须创建一个应用程序对象。sys.argv参数是一个列表，从命令行输入参数。
    app = QApplication(sys.argv)
    w = QDialog()

    client = AVCmpClient()

    hBoxPicCmp = QHBoxLayout()

    picOne = labelAvPic.AvPicLabel()
    hBoxPicCmp.addWidget( picOne )
    picTwo = labelAvPic.AvPicLabel()
    hBoxPicCmp.addWidget( picTwo )

    hBoxButtons = QHBoxLayout()
    btnLeftBetter = QPushButton()
    btnLeftBetter.setText( "Left" )
    btnLeftBetter.clicked.connect( client.OnLeftBetter )

    btnRandom = QPushButton()
    btnRandom.setText('刷新')
    btnRandom.clicked.connect( client.ShowRandom )

    # btnLeftBetter.show()
    btnRightBetter = QPushButton()
    btnRightBetter.setText( "Right")
    btnRightBetter.clicked.connect( client.OnRightBetter )
    #btnRightBetter.show()
    hBoxButtons.addWidget( btnLeftBetter )
    hBoxButtons.addWidget( btnRandom )
    hBoxButtons.addWidget( btnRightBetter )
    #hBoxPicCmp.addWidget( btnLeftBetter )

    hBoxTopPics = QHBoxLayout()
    btnPageUp = QPushButton()
    btnPageUp.setText('<')
    btnPageUp.setMaximumSize(50,600)

    btnPageUp.clicked.connect( client.OnPageUp )
    btnPageDown = QPushButton(w)
    btnPageDown.setText('>')
    btnPageDown.setMaximumSize(50,600)
    btnPageDown.clicked.connect( client.OnPageDown )

    # vBoxPageDown = QVBoxLayout()
    #vBoxPageDown.addStretch()
    #vBoxPageDown.addWidget(btnPageDown)
    #vBoxPageDown.addStretch()
    topPicLabels = list()
    for i in range(0 , topPicNum):
        topPic = labelAvPic.AvPicLabel()
        topPic.setMaximumHeight(600)
        topPicLabels.append( topPic )


    hBoxTopPics.addWidget(btnPageUp)
    for topPic in topPicLabels:
        hBoxTopPics.addWidget( topPic )
    #hBoxTopPics.addLayout( vBoxPageDown )
    hBoxTopPics.addWidget( btnPageDown )

    vBox = QVBoxLayout(w)
    vBox.addLayout( hBoxTopPics )
    vBox.addLayout( hBoxPicCmp )
    vBox.addLayout( hBoxButtons )

    w.setLayout( vBox )

    w.resize( 3840,2160 )

    #move()方法移动窗口在屏幕上的位置到x = 300，y = 300坐标。
    #w.move(300, 300)
    #设置窗口的标题
    #w.setWindowTitle('AvCmp')
    w.setWindowTitle( 'AvCmp')
    flags = w.windowFlags()
    flags |=Qt.WindowMinimizeButtonHint
    flags |=Qt.WindowMaximizeButtonHint
    w.setWindowFlags(flags)

    #显示在屏幕上
    w.showMaximized()
    #w.showFullScreen()
    client.ShowRandom()
    client.ShowTop()
    client.OnTimerUpdateTop()
    #系统exit()方法确保应用程序干净的退出
    #的exec_()方法有下划线。因为执行是一个Python关键词。因此，exec_()代替
    sys.exit(app.exec_())