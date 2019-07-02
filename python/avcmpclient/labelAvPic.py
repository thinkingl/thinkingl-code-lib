#from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.Qt import *
import avpicinfo 

class AvPicLabel(QLabel):
    picFileName = ''
    picData = None
    picDlg = None

    def __init__(self,parent=None):
        super( AvPicLabel,self).__init__(parent)
        self.setSizePolicy( QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding) )
    
    def ShowPic( self, fileName, data ):
        self.picFileName = fileName
        self.picData = data
        pix1 = QPixmap()
        pix1.loadFromData(data)
        spaceSize = self.size()
        newPix1 = pix1.scaled( spaceSize, Qt.KeepAspectRatio, Qt.SmoothTransformation)
        self.setPixmap( newPix1 )
    def mouseDoubleClickEvent(self,e):
        print ( self.picFileName + ' mouse double clicked' + str(e)  )
    def mousePressEvent(self,e):
        if( Qt.RightButton == e.button() ):
            self.picDlg = avpicinfo.AvPicInfoDlg( self.picFileName, self.picData )
            
        #print ('mousePressEvent ' + str( e.button() ) )
    # def focusInEvent(self,e):
    #     #print ('focusInEvent')
    # def focusOutEvent(self,e):
    #     #print ('focusOutEvent')
    # def moveEvent(self,e):
    #     #print ('moveEvent')
    # def leaveEvent(self,e): #鼠标离开label
    #     #print ('leaveEvent')
    # def enterEvent(self,e): #鼠标移入label
    #     #print ('enterEvent')
    # def mouseMoveEvent(self,e):
    #     #print ('mouseMoveEvent')