#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import os
import sqlite3
import time
import json
#import thread
#这里我们提供必要的引用。基本控件位于pyqt5.qtwidgets模块中。
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QHBoxLayout, QDialog
from PyQt5.QtGui import QWindow, QPicture, QPixmap
from PyQt5.QtCore import Qt
import logging
from urllib import request
import avlibdb

db = avlibdb.CAvlibDb()
db.InitDbTable()
db.ConnectDb()
db.ThreadFile2Db()

 
if __name__ == '__main__':
    #每一pyqt5应用程序必须创建一个应用程序对象。sys.argv参数是一个列表，从命令行输入参数。
    app = QApplication(sys.argv)
    w = QDialog()


    hBox = QHBoxLayout(w)

    picOne = QLabel()
    hBox.addWidget( picOne )

    picTwo = QLabel()
    hBox.addWidget( picTwo )

    w.setLayout( hBox )

    w.resize( 3840,2160 )

    #move()方法移动窗口在屏幕上的位置到x = 300，y = 300坐标。
    #w.move(300, 300)
    #设置窗口的标题
    #w.setWindowTitle('AvCmp')
    w.setWindowTitle( 'AvCmp')

    

    #显示在屏幕上
    w.showMaximized()
    spaceSize = picOne.size()

    #picData1 = avlib.GetPicDataByName( '2019-02-07 SNTL-016.jpg' )
    #pix1 = QPixmap("D:/999-temp/javlib/datas2/栗衣みい/2019-02-07 SNTL-016.jpg")
    picData1 = request.urlopen('http://127.0.0.1:5000/image/2019-02-07 SNTL-016.jpg').read()
    pix1 = QPixmap()
    pix1.loadFromData(picData1)
    newPix1 = pix1.scaled( spaceSize, Qt.KeepAspectRatio, Qt.SmoothTransformation)
    picOne.setPixmap( newPix1 )

    #pix2 = QPixmap("D:/999-temp/javlib/datas2/有村のぞみ/2018-10-12 ABP-785.jpg")
    #pixData2 = avlib.GetPicData('143364')
    pixData2 = request.urlopen('http://127.0.0.1:5000/image/2018-10-12 ABP-785.jpg').read()
    pix2 = QPixmap()
    pix2.loadFromData(pixData2)
    newPix2 = pix2.scaled( spaceSize, Qt.KeepAspectRatio, Qt.SmoothTransformation)
    picTwo.setPixmap( newPix2 )
    
    #系统exit()方法确保应用程序干净的退出
    #的exec_()方法有下划线。因为执行是一个Python关键词。因此，exec_()代替
    sys.exit(app.exec_())