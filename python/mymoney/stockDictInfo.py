import dataclasses
import datetime
from dataclasses_json import dataclass_json
import time


@dataclass_json
@dataclasses.dataclass
class StockDictInfoOneType():
    type:str
    updatedTime:float = dataclasses.field(default=time.time())
    stockDict:dict[str, str] = dataclasses.field(default_factory=dict)

@dataclass_json
@dataclasses.dataclass
class StockDictInfo():
    typeDict:dict[str,StockDictInfoOneType] = dataclasses.field(default_factory=dict)