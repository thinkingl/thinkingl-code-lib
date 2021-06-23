import dataclasses
import datetime
from enum import Flag

@dataclasses.dataclass
class StockExchangeRecord():
    date:datetime.date  # 日期
    id:str              # 交易唯一编号
    symbol:str          # 股票代码(如果有)
    name:str            # 股票名称
    amount:float        # 现金变动金额
    tradeVolume:float   # 成交股份数量
    price:float         # 成交价格
    commission:float    # 佣金
    stampTax:float      # 印花税
    transferFee:float   # 过户费
    otherFee:float      # 其它费用
    dealType:str        # 交易类型
    comments:str = ""   # 备注

    def __init__(self, **kwargs):
        names = set([f.name for f in dataclasses.fields(self)])
        for k, v in kwargs.items():
            if k in names:
                setattr(self, k, v)
    pass



class StockExchangeHistory():
    version:str = '1.0'
    history:dict[str, StockExchangeRecord] = {}

    def addRecord(self, record:StockExchangeRecord):
        self.history[record.id] = record
        #self.history.append( record )
        #self.history.sort(key=lambda record: record.date)
        pass

    def getAllRecordList(self):
        allRecord = list()
        for k,v in self.history.items():
            allRecord.append(v)
        allRecord.sort( key=lambda record: record.date )
        return allRecord