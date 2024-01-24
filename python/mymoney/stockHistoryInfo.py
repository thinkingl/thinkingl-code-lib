import dataclasses
import datetime

@dataclasses.dataclass
class StockHistoryDay:
    date:datetime.date
    open:float
    close:float
    high:float
    low:float

@dataclasses.dataclass
class StockHistory:
    symble:str
    name:str
    updatedTime:float   # 数据更新的时间.
    history:list[StockHistoryDay] = list()

@dataclasses.dataclass
class StockHFQHistory:
    symble:str
    name:str
    updatedTime:float   # 数据更新的时间.
    hfq: list[(datetime.date,float)] = list()  # 复权的历史数据.

@dataclasses.dataclass
class StockQFQHistory:
    symble:str
    name:str
    updatedTime:float   # 数据更新的时间.
    qfq: list[(datetime.date,float)] = list()  # 复权的历史数据.