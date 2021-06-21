import dataclasses
import datetime

@dataclasses.dataclass
class StockInfo:
    symble:str
    name:str
    date:datetime.date
    open:float
    close:float
    high:float
    low:float
    hfqFactor:float
