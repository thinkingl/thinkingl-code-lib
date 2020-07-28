# 图片详细信息
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.Qt import *
import requests
import labelAvPic
import avcmp


class AvPicInfoDlg():
    picDlg = None

    def __init__(self, fileName, data ):
        self.picDlg = QDialog()

        labelPic = labelAvPic.AvPicLabel()
        vBoxPicInfo = QVBoxLayout() # 图片右侧显示图片信息.

        hBoxLayout = QHBoxLayout()
        hBoxLayout.addWidget( labelPic )
        hBoxLayout.addLayout( vBoxPicInfo )


        vBox = QVBoxLayout()
        vBox.addLayout( hBoxLayout )
        self.picDlg.setLayout( vBox )

        self.picDlg.setWindowFlag( Qt.Dialog )
        self.picDlg.showFullScreen()

        labelPic.ShowPic( fileName, data )
        labelPic.setMaximumWidth( 3000 )
        self.ShowPicInfo( vBoxPicInfo, fileName )
        #picDlg.showMaximized()
        #picDlg.exec()
    def ShowPicInfo(self, vBoxPicInfo, fileName):
        # http://192.168.84.8:5000/imageInfoJson/2018-02-07%20DSD-722.jpg
        url = avcmp.BaseUrl + 'imageInfoJson/%s'%(fileName )
        rsp = requests.get(url)
        picInfo = rsp.json()
        print( picInfo )

        formLayout = QFormLayout()
        vBoxPicInfo.addLayout( formLayout )

        for attrName in picInfo:
            attrEdit = QLineEdit()
            attrEdit.setText( str(picInfo[attrName]))
            attrEdit.setMinimumWidth( 400 )
            formLayout.addRow( attrName, attrEdit )


        btnTest = QPushButton()
        btnTest.setText('test')
        btnTest.setMaximumWidth(200)
        vBoxPicInfo.addWidget( btnTest )