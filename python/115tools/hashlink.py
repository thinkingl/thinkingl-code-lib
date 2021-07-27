

class Hashlink():

    def __init__(self, hash:dict = None, hashlink : str = None) -> None:
        if hash == None:
            hash = self.parseHashlink(hashlink)
        self.hash = hash
        if self.hash == None:
            raise ValueError( "Invalid hashlink" )        

    def getHash(self)->dict:
        return self.hash

    def parseHashlink(self, hashlink:str) -> dict:
        # 115://MDVR-059-1.mp4|5270109163|5BA19FECD7AA93281109B6767A9626198CB15B28|DBCE5D3128D873198B237BBCA37A847057E394CF|MDVR-059
        ar = hashlink.strip().split( '|' )
        hash = {}
        hash['fileName'] = ar[0].replace( '115://', '')
        hash['fileSize'] = int(ar[1])
        hash['totalHash'] = ar[2]
        hash['blockHash'] = ar[3]
        hash['dirName'] = ar[4]
        return hash

    def getHashlink(self) ->str:
        #        return '115://' + filename+'|'+str(os.path.getsize(localFilePath))+'|'+TotalHASH+'|'+BlockHASH+'|' + dirname
        filename = self.hash.get('fileName')
        dirname = self.hash.get('dirName')
        filesize = self.hash.get('fileSize')
        totalHash = self.hash.get( 'totalHash' )
        blockHash = self.hash.get( 'blockHash' )
        return '115://' + filename+'|'+str(filesize)+'|'+ totalHash +'|'+ blockHash +'|' + dirname