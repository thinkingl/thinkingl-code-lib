import akshare as ak
from stockDataStoreFile import StockDataStoreFile
import datetime
import time
from stockExchangeListsParser import *

STOCK_TYPE_AH = "AH"
EXPIRE_TIME = 24*3600

def updateStockDict( stockDataStore ):
    curStockDict = stockDataStore.getStockDict() 
    stockDictAH = curStockDict.typeDict.get( STOCK_TYPE_AH )
    if stockDictAH != None:
        updatedTime = stockDictAH.updatedTime
        if time.time() - updatedTime < EXPIRE_TIME:
            print( 'stock dict ' + STOCK_TYPE_AH + ' is fresh, do not need to update!' )
            return
    stock_zh_ah_name_dict = ak.stock_zh_ah_name()
    stockDataStore.updateStockDict( STOCK_TYPE_AH, time.time(), stock_zh_ah_name_dict )

def updateStockExchangeLists():
    stockExchangeListDir = 'C:/Users/thinkingl/AppData/Local/Kingsoft/WPS Cloud Files/userdata/qing/filecache/李之兴的云文档/股票交割单'
    parser = StockExchangeListsParser(stockExchangeListDir)
    history = parser.parseAll()


if __name__=="__main__":
    updateStockExchangeLists()

    stockDataStore = StockDataStoreFile( 'H:\\stockdata' )
    updateStockDict( stockDataStore )

    