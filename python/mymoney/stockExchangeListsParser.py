import os
import xlrd

class StockExchangeListsParser():
    def __init__( self, rootDir : str ) -> None:
        self._rootDir = rootDir;
        self.parseAll();


    def parseCSV(self, filePath):
        title = None
        for line in open(filePath).readlines():
            if title == None:
                title = line.split()
            else:
                items = line.split()
                for i in range(0, len(items)):
                    item = items[i]
                    

            pass

    def parseAll( self ):
        for root, dirs, files in os.walk( self._rootDir ):
            for file in files:
                filePath = os.path.join( root, file )
                print( 'stock exchange list file: ', filePath )
                try:
                    bk = xlrd.open_workbook( filePath )
                    for table in bk.sheets():
                        print( table )
                    continue
                except:
                    pass

                self.parseCSV( filePath )
        pass