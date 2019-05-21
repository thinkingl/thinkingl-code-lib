import requests
import base64
import hashlib

clientDeviceId = "1234567"

#viidServer =  "http://10.255.228.107:8081"
#viidServer =  "http://10.255.228.6:8081"
viidServer =  "http://172.16.251.232:8081"


viidRegisterUrl = viidServer + "/VIID/System/Register"
jsonRegister = { "Register":{ "DeviceID":clientDeviceId}}
registerRep = requests.post( viidRegisterUrl, json=jsonRegister )

print( "register rep----------------------------------------------")
print( registerRep.text )
print( "register rep----------------------------------------------")


viidKeepaliveUrl =  viidServer + "/VIID/System/Keepalive";
jsonKeepalive = {
    "Keepalive":
        {
            "DeviceID":clientDeviceId
        }
}
keepaliveRep = requests.post(viidKeepaliveUrl,json=jsonKeepalive)

print( "keepalive rep:------------------------------------------")
print (keepaliveRep.text)
print( "keepalive rep:------------------------------------------")

imageId = "32050001051191709012022017120613523700088"
viidUploadImageUrl=  viidServer + "/VIID/Images/" # + imageId;

imageFilePath = "D:/tmp/viidtest.jpg"
imageFile = open( imageFilePath, "rb" )
imageData = imageFile.read()
imageFile.close()
base64ImageData = base64.b64encode( imageData )
base64ImageDataStr = base64ImageData.decode( "utf-8" )

imageMD5Obj = hashlib.md5( )
imageMD5Obj.update( imageData )
imageMD5Hex = imageMD5Obj.hexdigest()


jsonImageInfo={
    "ImageInfo":{
        "ImageID":imageId,
        "InfoKind":"1",
        "ImageSource":"3",
        "DeviceID":"32050001051191709012",
        "StoragePath":"Inv",
        "FileHash":imageMD5Hex,
        "FileFormat":"Jpeg",
        "ShotTime":"20180202143544",
        "Title":"viidtest",
        "ContentDescription":"波特曼",
        "Keyword":"波特曼 光头",
        "ShotPlaceFullAddress":"上海",
        "SecurityLevel":"5",
        "Width":"900",
        "Height":"897"
    }
}

jsonFace = {
    "FaceObject":[{
       "FaceID":"32050001051191709012022017120613523700002061",
        "InfoKind":1,
        "SourceID":"3",
        "DeviceID":"32050001051191709012",
        "LeftTopX":1280,
        "LeftTopY":536,
        "RightBtmX":1503,
        "RightBtmY":791,
        "LocationMarkTime":"Inv",
        "FaceAppearTime":"20171206135237",
        "FaceDisAppearTime":"Inv",
        "IsDriver":0,
        "IsForeigner":0,
        "IsSuspectedTerrorist":0,
        "IsDetainees":0,
        "IsSuspiciousPerson":-1
    }]
}

jsonImageList = {
    "ImageListObject":{
         "Image":[
             {
               "ImageInfo":jsonImageInfo,
               "Data":base64ImageDataStr,
                "FaceList":jsonFace
            }
        ]
    }
}

print( jsonImageList );

imageUploadRep = requests.post(viidUploadImageUrl,json=jsonImageList)

print( "image upload rep:------------------------------------------")
print (imageUploadRep.text)
print( "image upload rep:------------------------------------------")

viidQueryServer = viidServer.replace( ":8081", ":8083")   # "http://10.255.228.6:8083"

print( viidQueryServer )

viidUrlImageGet = viidQueryServer + "/VIID/Images/"  + imageId;

imageGetRep = requests.get( viidUrlImageGet );

print( "image get rep:------------------------------------------")
print (imageGetRep.text)
print( "image get rep:------------------------------------------")

viidUrlImageGet = viidQueryServer + "/VIID/Images&ImageID=" + imageId  ;

imageGetRep = requests.get( viidUrlImageGet );

print( "image get rep1:------------------------------------------")
print (imageGetRep.text)
print( "image get rep1:------------------------------------------")

