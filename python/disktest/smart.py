import os

class SMART:
    smartInfoRaw = ''

    def SMART( self, smartInfo ):
        self.smartInfoRaw = smartInfo
        return self

    @staticmethod
    def getSMART(disk):
        smartInfo = os.popen( 'smartctl -a ' + disk ).read()
        return SMART( smartInfo )

    def getTotalWrite(self):
        return 0

    
if __name__=="__main__":
    smart = SMART.getSMART( '')