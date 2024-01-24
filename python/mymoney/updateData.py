from stockDataStore import StockDataStore
import akshare as ak
from stockDataStoreFile import StockDataStoreFile
import datetime
import time
from stockExchangeListsParser import *

#STOCK_TYPE_AH = "AH"
STOCK_TYPE_A = "A"
STOCK_TYPE_H = "H"

EXPIRE_TIME = 24*3600

def getStockNameDict(type:str) -> dict[str,str]:
    stockSpot = None
    if type == STOCK_TYPE_A:
        stockSpot = ak.stock_zh_a_spot_em()
        pass
    elif type == STOCK_TYPE_H:
        stockSpot = ak.stock_hk_spot()
        pass
    else:
        raise
    
    stockDict = {}
    for i, row in stockSpot.iterrows():
        symbol = row[0]
        name = row[1]
        stockDict[symbol] = name
    return stockDict

def updateStockDictType( stockDataStore:StockDataStore, type:str ) -> None:
    curStockDict = stockDataStore.getStockDict() 
    stockDictAH = curStockDict.typeDict.get( type )
    if stockDictAH != None:
        updatedTime = stockDictAH.updatedTime
        if time.time() - updatedTime < EXPIRE_TIME:
            print( 'stock dict ' + type + ' is fresh, do not need to update!' )
            return
    stockNameDict = getStockNameDict(type)
    stockDataStore.updateStockDict( type, time.time(), stockNameDict )


def updateStockDict( stockDataStore:StockDataStore ):
    updateStockDictType( stockDataStore, STOCK_TYPE_A )
    updateStockDictType( stockDataStore, STOCK_TYPE_H )

def updateStockExchangeLists() -> StockExchangeHistory:
    stockExchangeListDir = 'C:/Users/thinkingl/AppData/Local/Kingsoft/WPS Cloud Files/userdata/qing/filecache/李之兴的云文档/股票交割单'
    parser = StockExchangeListsParser(stockExchangeListDir)
    history = parser.parseAll()
    return history

def getStockInHistory(history:StockExchangeHistory, stockDataStore:StockDataStore) ->set[str]:
    stockSet = set()

    recordList = history.getAllRecordList()
    for record in recordList:
        if not record.symbol in stockSet:
            if stockDataStore.isStock( record.symbol ):
                stockSet.add( record.symbol )
                print( 'stock in history: ', record.symbol, ' - ', record.name )
    return stockSet

def updateStockHistory( symbol:str, stockDataStore:StockDataStore):
    pass

if __name__=="__main__":
    stockDataStore = StockDataStoreFile( 'E:\\stockdata' )
    updateStockDict( stockDataStore )

    history = updateStockExchangeLists()
    stockInHistory = getStockInHistory( history, stockDataStore )

    print( stockInHistory )