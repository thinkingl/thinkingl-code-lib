# Microsoft Developer Studio Project File - Name="g200" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=g200 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "g200.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "g200.mak" CFG="g200 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "g200 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "g200 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "g200"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "g200 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../include" /I "../../../../10-Common/include/cmusdk" /I "../../../../10-common/include/cbb/system" /I "../../../../10-common/include/cbb/protocol" /I "../../../../10-common/include/cbb/platform" /I "../../../../10-common/include/aaa" /I "../../../../10-common/include/cms" /I "../../../../10-common/include/common" /I "../../../../10-common/include/nms" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 osplib.lib kdvsys.lib puilib.lib cmusdk.lib snmpadp.lib /nologo /subsystem:console /machine:I386 /out:"../../release/g200.exe" /libpath:"../../../../10-common/lib/release/win32"
# SUBTRACT LINK32 /incremental:yes /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=del ..\..\release\g200.lib	del ..\..\release\g200.exp	copy .\conf\* ..\..\release\conf
# End Special Build Tool

!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../include" /I "../../../../10-Common/include/cmusdk" /I "../../../../10-common/include/cbb/system" /I "../../../../10-common/include/cbb/protocol" /I "../../../../10-common/include/cbb/platform" /I "../../../../10-common/include/aaa" /I "../../../../10-common/include/cms" /I "../../../../10-common/include/common" /I "../../../../10-common/include/nms" /I "../../../../10-common/include/cbb/license" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 osplib.lib puilib.lib cmusdk.lib camera.lib snmpadp.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../debug/g200.exe" /pdbtype:sept /libpath:"../../../../10-common/lib/debug/win32" /libpath:"../lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=del ..\..\debug\g200.lib	del ..\..\debug\g200.exp	del ..\..\debug\g200.ilk	copy .\conf\* ..\..\debug\conf
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "g200 - Win32 Release"
# Name "g200 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\CfgMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Common.cpp
# End Source File
# Begin Source File

SOURCE=..\src\G200.cpp
# End Source File
# Begin Source File

SOURCE=..\src\G200Data.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\kdvsys.c
# End Source File
# Begin Source File

SOURCE=..\..\common\markup.cpp
# End Source File
# Begin Source File

SOURCE=..\src\PuiSsn.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SdkSsn.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VcCfg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vcg200ms.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VcPui.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VcSched.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VcSdk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\xmlmsg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\CfgMgr.h
# End Source File
# Begin Source File

SOURCE=..\include\Common.h
# End Source File
# Begin Source File

SOURCE=..\include\G200Data.h
# End Source File
# Begin Source File

SOURCE=..\..\include\kdvsys.h
# End Source File
# Begin Source File

SOURCE=..\..\include\markup.h
# End Source File
# Begin Source File

SOURCE=..\include\PuiSsn.h
# End Source File
# Begin Source File

SOURCE=..\include\SdkSsn.h
# End Source File
# Begin Source File

SOURCE=..\include\VcSsn.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlmsg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
