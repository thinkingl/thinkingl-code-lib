import os
import xml

taskPath = 'C:\\Windows\\System32\\Tasks'
for root, dirs, files in os.walk( taskPath ):
    for name in files:
        taskFile=open(os.path.join(root,name),'rb')
        content=taskFile.read()
        xmlContent = content.decode('UTF-16')
        if(xmlContent.find('powershell')!=-1):
            print(os.path.join(root,name))
            print(xmlContent)