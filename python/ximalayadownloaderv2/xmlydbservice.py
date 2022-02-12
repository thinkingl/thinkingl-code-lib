from flask import Flask
from flask import request
import db
import xmlycfg
import json



app = Flask(__name__)


@app.route("/xmly/")
def hello():
    return "Hello, I'm XMLY db!"


# 更新数据库
@app.route("/xmly/<table>/<id>", methods=['PUT'])
def saveDb(table, id):
    op = request.get_json()
    database = db.XMLYDatabase()
    database.init( xmlycfg.getCurDBPath() )
    database.json2DB( table, id, op )
    result = { "error":"ok"}
    return result


# 查询数据库
@app.route("/xmly/<talbe>/<id>", methods=['GET'])
def queryDb(talbe, id):
    database = db.XMLYDatabase()
    database.init( xmlycfg.getCurDBPath() )
    jsonObj = database.db2Json( talbe, id )
    return jsonObj

# 查询专辑的trackId列表
@app.route("/xmly/tracklist/<albumId>", methods=['GET'])
def trackList( albumId ):
    database = db.XMLYDatabase()
    database.init( xmlycfg.getCurDBPath() )
    tracks = database.getTrackList( albumId )
    result = { "trackList": tracks }
    return result

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000)