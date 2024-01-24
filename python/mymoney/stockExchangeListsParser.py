import os
import re
from stockExchangeHistory import *
import pandas as pd
import xlrd

'''
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
    comments:str        # 备注

'''
StockExchangeListTitleDict = {
    "date":["发生日期","成交日期","交收日期"],
    "symbol":["证券代码"],
    "name":['证券名称'],
    "amount":['发生金额','资金发生数'],
    "tradeVolume":['成交数量'],
    "price":['成交价格'],
    'commission':['佣金'],
    'stampTax':['印花税'],
    'transferFee':['过户费'],
    'otherFee':['其他费','规费'],
    'comments':['备注'],
    
    'dealNo':['成交编号','合同号'],
    'dealType':['买卖标志','交易类别']
}

StockExchangeItemTypeTrans = {
    "date":lambda s: datetime.datetime.strptime(s, '%Y%m%d').date(),
    "symbol":str,
    "name":str,
    "amount":float,
    "tradeVolume":float,
    "price":float,
    'commission':float,
    'stampTax':float,
    'transferFee':float,
    'otherFee':float,
    'comments':str,
    'dealType':str,
    
    'dealNo':str,
}

class StockExchangeListsParser():
    def __init__( self, rootDir : str ) -> None:
        self._rootDir = rootDir;
        #self.parseAll();

    def strip( self, s : str ) -> str :
        try:
            value = re.match( '^="(.*)"', s.strip() ).group(1)
            if value != None:
                return value.strip()
        except:
            pass
        return s

    def parseRecord(self, titles, items) -> StockExchangeRecord:
        record = {}
        for i in range(0, len(items)):
            item = items[i]
            value = self.strip(item)
            title = titles[i].strip()
            #print( title, '=', value )
            for k in StockExchangeListTitleDict:
                if title in StockExchangeListTitleDict[k]:
                    record[k] = value

        # 没有交易的月份
        if '暂无记录' == record.get('date'):
            return None
        record['id'] = str(record.get('date')) + '-' + record.get( 'symbol' ) + '-' + record.get('dealNo') + '-' + record.get( 'dealType' ) + '-' + record.get( 'amount')

        for k,v in record.items():
            typeTransFun = StockExchangeItemTypeTrans.get( k )
            if typeTransFun != None:                
                record[k] = typeTransFun( v )

        r = StockExchangeRecord( **record )

        return r

    def parseCSV(self, filePath:str, history:StockExchangeHistory):
        titles = None
        for line in open(filePath).readlines():
            if titles == None:
                titles = line.split('\t')
            else:
                items = line.split('\t')
                record = self.parseRecord( titles, items )
                #print( record )
                if record != None:
                    history.addRecord( record )

    def parseXLS(self, filePath:str, history:StockExchangeHistory):
        bk = xlrd.open_workbook( filePath, encoding_override='utf-8' )
        for table in bk.sheets():
            titles = None

            for row in table.get_rows():
                items = list()
                emptyCount = 0
                for cell in row:
                    v = cell.value
                    items.append( v )
                    if v == '':
                        emptyCount += 1
                if emptyCount > 5:
                    continue
                if titles == None:
                    titles = items
                else:
                    record = self.parseRecord( titles, items )
                    if record != None:
                        #print( record )
                        history.addRecord( record )
                
        pass

    def parseAll( self )-> StockExchangeHistory:
        history = StockExchangeHistory()
        for root, dirs, files in os.walk( self._rootDir ):
            for file in files:
                filePath = os.path.join( root, file )
                print( 'stock exchange list file: ', filePath )
                if root.find( '国泰君安' ) != -1:
                    self.parseXLS( filePath, history )
                else:
                    self.parseCSV( filePath, history )
        return history