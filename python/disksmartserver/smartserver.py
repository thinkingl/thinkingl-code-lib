from flask import Flask
from flask import request
import json
from flask import Response
import queue
from threading import Thread
import logging
import time
import os
import re

app = Flask(__name__)

lastSmart = None


@app.route("/")
def hello():
    return "Hello SMART!"

@app.route('/smart/dev/<devname>')
def smartByDev( devname ):
    global lastSmart
    devPath = '/dev/' + devname
    smartCmd = 'smartctl -a ' + devPath
    output = os.popen( smartCmd ).read()

    lastSmart = readLastSMART(devname)
    smartInfoCmpDiff( devPath, output, lastSmart )
    lastSmart = output

    output = transSMARTInfo( output )

    output += '\n'
    output += '---------------------log----------------\n'
    smartLogFileName = 'smart-log.txt'
    with open( smartLogFileName, 'r' ) as f:
        smartlog = f.read()
        output += smartlog

    return output

@app.route('/smart/dev/disk/by-id/<diskid>')
def smartByDevId( diskid ):
    devPath = '/dev/disk/by-id/' + diskid
    smartCmd = 'smartctl -a ' + devPath
    output = os.popen( smartCmd ).read()

    lastSmart = readLastSMART(diskid)
    smartInfoCmpDiff( devPath, output, lastSmart )
    lastSmart = output

    output = transSMARTInfo( output )

    output += '\n'
    output += '---------------------log----------------\n'
    smartLogFileName = 'smart-log.txt'
    with open( smartLogFileName, 'r' ) as f:
        smartlog = f.read()
        output += smartlog

    return output

def isIgnoreDiff( line ):
    if line.find( 'Local Time is:' ) == 0:
        return True
    if line.find( '9 Power_On_Hours' ) != -1:
        return True
    if line.find( '164 Unknown_Attribute' ) == 0:
        return True
    if line.find( '194 Temperature' ) == 0:
        return True
    if line.find( '197 Current_Pending_Sector' ) == 0:
        return True
    if line.find( '241 Total_LBAs_Written' ) == 0:
        return True
    if line.find( '242 Total_LBAs_Read' ) == 0:
        return True
    if line.find( '245 Unknown_Attribute' ) == 0:
        return True
    
    return False

def writeSmartCmpLog( dev, text ):
    cur_time = time.time()
    strnow = time.strftime('%Y-%m-%d-%H%M%S',time.localtime(cur_time))
    lognow = 'SMART changed!! dev ' + dev + ' cur time: ' + strnow + '\n'
    smartLogFileName = 'smart-log.txt'
    with open( smartLogFileName, 'a' ) as f:
        f.write( lognow )
        f.write( text )
        f.close()
    print( lognow )
    print( text )


def saveSmart( dev, smartText ):
    dev = dev.split('/')[-1]
    cur_time = time.time()
    strnow = time.strftime('%Y-%m-%d-%H%M%S',time.localtime(cur_time))
    lognow = 'cur time: ' + strnow + '\n'
    smartFileName = 'smart-' + dev + '-' + strnow + '.txt'
    with open( smartFileName, 'w' ) as f:
        f.write( lognow )
        f.write( smartText )
        f.close()
    pass

def smartInfoCmpDiff( dev, smart1, smart2 ):
    a1 = smart1.split('\n')
    a2 = smart2.split('\n')
    if len(a1) != len(a2):
        return True

    diff = False
    for i in range(0,len(a1)):
        line1 = a1[i]
        line2 = a2[i]
        if isIgnoreDiff( line1 ) or isIgnoreDiff( line2 ):
            continue

        if line1 != line2 :
            logText = ''
            logText +=( 'diff line ' + str(i+1) + ' :\n'  )
            logText +=( line1 + '\n')
            logText +=( line2 + '\n')
            logText += 'transed:\n' 
            logText +=( transSMARTLine( line1 ) + '\n')
            logText +=( transSMARTLine( line2 ) + '\n')
            logText +=( '\n' )
            writeSmartCmpLog( dev, logText)
            diff = True
    return diff

def readLastSMART(devName):
    devName = devName.split('/')[-1]
    lastFileName = 'last-' + devName
    if not os.path.isfile( lastFileName ):
        return ''
    with open( lastFileName, 'r') as f:
        smart = f.read()
        f.close()
        return smart
    return ''

def saveLastSMART(devName, smartInfo):
    devName = devName.split('/')[-1]
    lastFileName = 'last-' + devName
    with open( lastFileName, 'w' ) as f:
        f.write(smartInfo)
        f.close


def threadMonitorSMART( devList ):
    while( True ):
        for dev in devList:
            smartCmd = 'smartctl -a ' + dev
            output = os.popen( smartCmd ).read()
            last = readLastSMART( dev )
            if smartInfoCmpDiff( dev, output, last ):
                saveSmart( dev, output )
            saveLastSMART( dev, output )
        time.sleep(60)

def transSMARTLine( smartLine ):
    transedLine = smartLine
    try:
        transMap = {}
        with open( 'smartInfoTrans.txt' ) as f:
            transLines = f.readlines()
            for line in transLines:
                line = line.strip()
                id, value = line.split( '=' )
                id = int( id, 16 )
                
                transMap[id] = value

        values = re.split( '[\s]+', smartLine.strip() )
        if len( values ) == 10:
            smartId = int( values[0] )
            smartDesc = values[1]
            newSmartDesc = smartDesc
            if smartId in transMap:
                newSmartDesc = transMap[smartId]
            # newSmartDesc 长度补齐到最长, 以对齐显示.
            newSmartDesc += ( ' '*(47-len(newSmartDesc) ) )

            transedLine = re.sub( smartDesc + '[\s]*', newSmartDesc, transedLine ) # transedLine.replace( smartDesc, newSmartDesc )
    except:
        pass
    print( 'trans [', smartLine, '] to [', transedLine, ']' )
    return transedLine

def transSMARTInfo( smartInfo ):
    transText = ''
    for smartLine in smartInfo.split( '\n' ):
        transText += transSMARTLine( smartLine )
        transText += '\n'
    return transText

if __name__ == '__main__':
    devList = ['/dev/disk/by-id/scsi-SCV1-MB51_2LIT_152D20329000']
    t = Thread( target=threadMonitorSMART, args=(devList,))
    t.start()
    app.run(host='0.0.0.0',port=5002)