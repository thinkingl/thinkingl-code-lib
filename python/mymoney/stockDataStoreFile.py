from json import encoder
from stockDictInfo import StockDictInfo, StockDictInfoOneType
from stockDataStore import StockDataStore
import datetime
from stockInfo import *
import json
import os
from dataclasses_json import dataclass_json
import logging

FILENAME_STOCK_DICT = 'stock_dict.json'

class StockDataStoreFile( StockDataStore ):

    rootPath:str

    def __init__(self, rootPath:str) -> None:
        super().__init__()
        self.rootPath = rootPath
        pass

    def getStockDict(self) -> StockDictInfo:
        try:
            allStockDict = StockDictInfo()
            filePath = os.path.join( self.rootPath, FILENAME_STOCK_DICT )
            if os.path.isfile( filePath ):
                allStockDict = StockDictInfo.from_json( open(filePath).read() )
                #allStockDict = json.loads( open(filePath).read() )
            return allStockDict
        except:
            logging.exception( 'error' )
            return StockDictInfo()
    
    # 股票是否存在
    def isStock(self, symbol:str) -> bool:
        allStockDict = self.getStockDict()
        for type, oneType in allStockDict.typeDict.items():
            if symbol in oneType.stockDict:
                return True
        return False

    # 更新股票字典
    def updateStockDict(self, type:str, endTime:float, stockDict:dict ):
        curStockDictInfo = self.getStockDict()
        stockDictInfoOneType = curStockDictInfo.typeDict.get( type )
        if stockDictInfoOneType == None:
            stockDictInfoOneType = StockDictInfoOneType( type )
            curStockDictInfo.typeDict[ type ] = stockDictInfoOneType
        
        stockDictInfoOneType.type = type
        stockDictInfoOneType.updatedTime = endTime
        stockDictInfoOneType.stockDict.update( stockDict )

        filePath = os.path.join( self.rootPath, FILENAME_STOCK_DICT )
        #open( filePath, 'w' ).write( json.dumps( curStockDictInfo, indent=4 ) )
        open( filePath, 'w' ).write( curStockDictInfo.to_json() )
        pass


    # 获取某一天的股票信息.
    def getStockInfo( self, symbol:str, date:datetime.date ) -> StockInfo: 
        pass