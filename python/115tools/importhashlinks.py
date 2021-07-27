from logging import NOTSET
import hashcfg
import sys
import argparse
import time
import hashClient
from hashlink import Hashlink

class Importor():
    def __init__(self, onlyTest:bool, runForever:bool) -> None:
        self.onlyTest = onlyTest
        self.runForever = runForever
        pass
    

    def importHashlink(self, hashlink:str):
        if self.onlyTest:
            print( 'only test ', hashlink)
        else:
            hash = Hashlink(hashlink=hashlink).getHash()
            client = hashClient.HashClient( hashcfg.HashServerUrl )
            hashOnServer = client.gethashlink( hash.get('dirName'), hash.get('fileName')).get('hash')
            if hashOnServer != None:
                if hashOnServer['totalHash'] == hash['totalHash'] and hashOnServer['blockHash'] == hash['blockHash']:
                    return
                else:
                    print( 'Hash ', hashlink, ' exist on server, but hash is diff!' )
                    print( 'Hash on server: ', hashOnServer )
                    print( 'Local hash: ', hash )

            client.addHashlink( **hash )
        pass

    def importHashlinks(self, files : list):
        allImportedLinks = set()
        while True:
            for file in files:
                try:
                    with open(file, encoding='utf-8') as f:
                        lines = f.readlines()
                        for line in lines:
                            hashlink = line.strip()
                            if not hashlink in allImportedLinks:
                                try:
                                    self.importHashlink( hashlink )
                                    allImportedLinks.add( hashlink )
                                except Exception as e:
                                    print( 'import link ', hashlink, "fail, e: ", e )
                                    pass
                except Exception as e:
                    print( 'import file ', file, ' fail! e:', e )
            if self.runForever:
                time.sleep( 10 )
            else:
                break


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Import hash links from file')
    
    parser.add_argument('files', metavar='FILENAME', type=str, nargs='+',
                    help='imported file')

    parser.add_argument('--test', 
                    help='Only test, do not commit to server.', action="store_true")

    parser.add_argument('--run',
                    help='Only test, do not commit to server.', action="store_true")

    args = parser.parse_args()
    
    importor = Importor(args.test, args.run )

    importor.importHashlinks( args.files )


    #args.doJob( args.files )