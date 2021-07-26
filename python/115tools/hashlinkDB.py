import sqlite3

class HashlinkDB():
    dbConnect = None
    def __init__(self) -> None:
        self.dbFilePath = 'hashs.db'
        self.initDb()
        pass

    def initDb(self):
        cur=self.connectDb().cursor()
        #        return '115://' + filename+'|'+str(os.path.getsize(localFilePath))+'|'+TotalHASH+'|'+BlockHASH+'|' + dirname
        sqlCreatePic='Create Table  IF NOT EXISTS hash (fileName TEXT NOT NULL, dirName TEXT, fileSize BIGINT, totalHash TEXT NOT NULL, blockHash TEXT NOT NULL, updateTime REAL NOT NULL, PRIMARY KEY ( dirName, fileName) );'
        cur.execute(sqlCreatePic)
        self.closeDb()
        pass

    def connectDb(self)-> sqlite3.Connection:
        self.closeDb()
        self.dbConnect = sqlite3.connect(self.dbFilePath,timeout=60)
        return self.dbConnect
    
    def closeDb(self):
        if not self.dbConnect == None:
            self.dbConnect.close()
            self.dbConnect = None

    def addHashlink(self, dirName:str,fileName:str, fileSize:int,totalHash:str, blockHash:str, updateTime:float, **kw):
        dbCursor = self.connectDb()
        dbCursor.execute( 'Replace into hash values( ?,?,?,?,?,? );', (fileName,dirName,fileSize,totalHash,blockHash,updateTime) )   
        dbCursor.commit()
        self.closeDb()
        pass

    def getHashlink(self, dirName:str, fileName:str) -> dict:
        self.connectDb()
        self.dbConnect.row_factory = sqlite3.Row # This enables column access by name: row['column_name'] 
        for row in self.dbConnect.execute( 'select * from hash where DirName = ? and FileName = ?;', (dirName, fileName)):
            self.closeDb()
            return dict(row)
        self.closeDb()
        return None

    def queryHashlink(self):
        pass