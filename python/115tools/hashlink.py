

class Hashlink():

    def __init__(self, hash:dict) -> None:
        self.hash = hash
        pass

    def getHashlink(self) ->str:
        #        return '115://' + filename+'|'+str(os.path.getsize(localFilePath))+'|'+TotalHASH+'|'+BlockHASH+'|' + dirname
        filename = self.hash.get('fileName')
        dirname = self.hash.get('dirName')
        filesize = self.hash.get('fileSize')
        totalHash = self.hash.get( 'totalHash' )
        blockHash = self.hash.get( 'blockHash' )
        return '115://' + filename+'|'+str(filesize)+'|'+ totalHash +'|'+ blockHash +'|' + dirname