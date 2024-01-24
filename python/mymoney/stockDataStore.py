import abc
import datetime
from typing import Dict
from stockInfo import StockInfo
from stockDictInfo import *

class StockDataStore(metaclass=abc.ABCMeta):

    # 更新股票字典
    @abc.abstractmethod
    def updateStockDict(self, type:str, endTime:float, stockDict:dict ) -> None:
        pass

    # 获取完整的股票字典.
    @abc.abstractclassmethod
    def getStockDict(self) -> StockDictInfo:
        pass

    # 股票是否存在
    @abc.abstractclassmethod
    def isStock(self, symbol:str) -> bool:
        pass

    # 更新股票历史行情
    def updateStockHistory(self) -> None:
        pass


    # 获取股票历史行情更新时间
    def getStockHistoryUpdateTime(self, symbol: str) -> datetime.datetime:
        pass

    # 获取某一天的股票信息.
    @abc.abstractmethod
    def getStockInfo( self, symbol:str, date:datetime.date ) -> StockInfo: 
        pass