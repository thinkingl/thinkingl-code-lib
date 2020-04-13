import os
import json



def check( folder, jsonFilePath ):
    fileNameSet = set(os.listdir(folder) )
    bdFileListJson = json.load( open(jsonFilePath ) )
    for obj in bdFileListJson['list']:
        fileName = obj['server_filename']
        if not fileName in fileNameSet:
            print( 'Fild file! ' + fileName )

folder = 'Z:/disk-soft/Game/PS1Game.PS1游戏合集/A-504'
jsonFilePath = 'Z:/disk-ssd/tmp/ps1-games-file-list/A.json'

folders = [
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/A-504',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/B',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/C',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/D',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/I',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/J',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/K',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/N',
    'Z:/disk-soft/Game/PS1Game.PS1游戏合集/Q',
    'Z:/disk-soft/tmp/E-445',
    'Z:/disk-soft/tmp/F-495',
    'Z:/disk-soft/tmp/G 369',
    'Z:/disk-soft/tmp/H-317',
    'Z:/disk-soft/tmp/L-245',
    'Z:/disk-soft/tmp/M-668',
    'Z:/disk-soft/tmp/O-203',
    'Z:/disk-soft/tmp/P-662',
    'Z:/disk-soft/tmp/R',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/T',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/U',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/V',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/W',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/X',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/Y',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/Z',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/S-1250-3/part1',
    'D:/BaiduNetdiskDownload/PS1 Redump-2020new/S-1250-3/part2'
]

for dirPath in folders:
    dirName = dirPath.split('/')[-1]
    jsonFileName = dirName[0]
    if dirName[0:4] == 'part':
        jsonFileName = 'S.' + dirName
    jsonFilePath = 'Z:/disk-ssd/tmp/ps1-games-file-list/' + jsonFileName + '.json'
    check( dirPath, jsonFilePath )

