import os
import logging
import json
from scapy.all import *
import threading

class vomit():
    filePath = ''
    packList = {}
    threadTable = {}

    def __init__(self, filePath) -> None:
        super().__init__()
        self.filePath = filePath

        #with PcapReader(filePath) as fdesc:
        #    self.scrapyReder = fdesc.read_all(count=-1)
        self.packList = rdpcap( filePath )
        print( "open file " + filePath + " obj: " + str(self.packList) )

        sessions= self.packList.sessions()
        self.initConfig( sessions )
        
    def initConfig(self, sessions):
        vomitCfg = self.readConfig()

        
        vomitMap = vomitCfg.get('map')

        for session in sessions:
            sessionCfg = vomitMap.get(session)
            if sessionCfg == None:
                vomitMap[session] = {}
        # save.
        configFilePath = self.filePath.rsplit('.',1)[0] + '.json'
        try:
            jsonStr = json.dumps( vomitCfg, indent=4)
            with open( configFilePath, 'w', encoding="utf-8" ) as f:
                f.write( jsonStr )
        except Exception as e:
            print( "save config file %s fail! file:", configFilePath, " exceptio: ", e )

    def readConfig(self):
        configFilePath = self.filePath.rsplit('.',1)[0] + '.json'
        vomitCfg = {}
        if os.path.isfile( configFilePath ):
            try:
                with open(configFilePath, 'r', encoding='utf-8') as f: # f is a local var, it would clean up anyway.
                    vomitCfg = json.load( f )
            except:
                pass 
        if vomitCfg.get('map') == None:
            vomitCfg['map'] = {}
        return vomitCfg

    def ThreadSender(self, packets, vomitCfg):
        srcPort = vomitCfg.get('psrc')
        if srcPort == None:
            srcPort = any
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

        # bind
        # to be continued.
        targets = vomitCfg.get('target')
        
        while True:
            firstTime = None
            startTime = time.time()
            for packet in packets:
                packetTime = packet.time
                if firstTime == None:
                    firstTime = packetTime
                while packetTime - firstTime > time.time() - startTime:
                    time.sleep( 0.001 )
                for target in targets:
                    dst = target.get( 'dst' )
                    pdst = target.get( 'pdst' )
                    s.sendto( packet.payload.load, (dst, pdst) )
        
    def start(self):
        self.stop()
        vomitCfg = self.readConfig()
        mapCfg = vomitCfg.get( 'map' )
        sessions = self.packList.sessions()
        for session in sessions:
            print( "find session ", session )
            sessionCfg = mapCfg.get( session )
            sessionPackets = sessions[session]
            if sessionCfg != None and sessionCfg.get( 'target' ) != None and len(sessionCfg.get( 'target' )) > 0:
                print( 'Create thread for session ', session, " config: ", sessionCfg )
                t = threading.Thread( name=session, target=self.ThreadSender(sessionPackets, sessionCfg) )
                self.threadTable[session] = t
                t.start()
                print( "Start thread for session %s"%session )
        return

    def stop(self):
        return
    




if __name__=="__main__":
    filePath = './sample.pcapng'

    if len( sys.argv ) > 1:
        filePath = sys.argv[1]
        print( 'package file:', filePath )
    

    v = vomit(filePath)
    v.start()
    while True:
        time.sleep(3600)