from flask import Flask
from flask import request
import json
from flask import Response
import queue
from threading import Thread
import logging
import time
import os

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

    if lastSmart == None:
        lastSmart = output
    smartInfoCmp( output, lastSmart )
    lastSmart = output

    return output

def isIgnoreDiff( line ):
    if line.find( 'Local Time is:' ) == 0:
        return True
    if line.find( '241 Total_LBAs_Written' ) == 0:
        return True
    if line.find( '242 Total_LBAs_Read' ) == 0:
        return True
    if line.find( '245 Unknown_Attribute' ) == 0:
        return True
    return False

def smartInfoCmp( smart1, smart2 ):
    a1 = smart1.split('\n')
    a2 = smart2.split('\n')
    if len(a1) != len(a2):
        return False

    diff = False
    for i in range(0,len(a1)):
        line1 = a1[i]
        line2 = a2[i]
        if isIgnoreDiff( line1 ) or isIgnoreDiff( line2 ):
            continue

        if line1 != line2 :
            print( 'diff line ' + str(i+1) + ' :'  )
            print( line1 )
            print( line2 )
            print( '\n' )
            diff = True
    return diff
        


if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5002)