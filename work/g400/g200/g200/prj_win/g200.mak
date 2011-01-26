# Microsoft Developer Studio Generated NMAKE File, Based on g200.dsp
!IF "$(CFG)" == ""
CFG=g200 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to g200 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "g200 - Win32 Release" && "$(CFG)" != "g200 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "g200 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\release\g200.exe"


CLEAN :
	-@erase "$(INTDIR)\CfgMgr.obj"
	-@erase "$(INTDIR)\Common.obj"
	-@erase "$(INTDIR)\G200.obj"
	-@erase "$(INTDIR)\G200Data.obj"
	-@erase "$(INTDIR)\PuiSsn.obj"
	-@erase "$(INTDIR)\SdkSsn.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VcCfg.obj"
	-@erase "$(INTDIR)\VcPui.obj"
	-@erase "$(INTDIR)\VcSched.obj"
	-@erase "$(INTDIR)\VcSdk.obj"
	-@erase "..\..\release\g200.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../include" /I "../../include" /I "../../../../10-Common/include/cmusdk" /I "../../../../10-common/include/cbb/system" /I "../../../../10-common/include/cbb/protocol" /I "../../../../10-common/include/cbb/platform" /I "../../../../10-common/include/aaa" /I "../../../../10-common/include/cms" /I "../../../../10-common/include/common" /I "../../../../10-common/include/nms" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\g200.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\g200.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=osplib.lib kdvsys.lib puilib.lib cmusdk.lib snmpadp.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\g200.pdb" /machine:I386 /out:"../../release/g200.exe" /libpath:"../../../../10-common/lib/release/win32" 
LINK32_OBJS= \
	"$(INTDIR)\CfgMgr.obj" \
	"$(INTDIR)\Common.obj" \
	"$(INTDIR)\G200.obj" \
	"$(INTDIR)\G200Data.obj" \
	"$(INTDIR)\PuiSsn.obj" \
	"$(INTDIR)\SdkSsn.obj" \
	"$(INTDIR)\VcCfg.obj" \
	"$(INTDIR)\VcPui.obj" \
	"$(INTDIR)\VcSched.obj" \
	"$(INTDIR)\VcSdk.obj"

"..\..\release\g200.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

$(DS_POSTBUILD_DEP) : "..\..\release\g200.exe"
   del ..\..\release\g200.lib
	del ..\..\release\g200.exp
	copy .\conf\* ..\..\release\conf
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\debug\g200.exe" "$(OUTDIR)\g200.bsc"


CLEAN :
	-@erase "$(INTDIR)\CfgMgr.obj"
	-@erase "$(INTDIR)\CfgMgr.sbr"
	-@erase "$(INTDIR)\Common.obj"
	-@erase "$(INTDIR)\Common.sbr"
	-@erase "$(INTDIR)\G200.obj"
	-@erase "$(INTDIR)\G200.sbr"
	-@erase "$(INTDIR)\G200Data.obj"
	-@erase "$(INTDIR)\G200Data.sbr"
	-@erase "$(INTDIR)\PuiSsn.obj"
	-@erase "$(INTDIR)\PuiSsn.sbr"
	-@erase "$(INTDIR)\SdkSsn.obj"
	-@erase "$(INTDIR)\SdkSsn.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VcCfg.obj"
	-@erase "$(INTDIR)\VcCfg.sbr"
	-@erase "$(INTDIR)\VcPui.obj"
	-@erase "$(INTDIR)\VcPui.sbr"
	-@erase "$(INTDIR)\VcSched.obj"
	-@erase "$(INTDIR)\VcSched.sbr"
	-@erase "$(INTDIR)\VcSdk.obj"
	-@erase "$(INTDIR)\VcSdk.sbr"
	-@erase "$(OUTDIR)\g200.bsc"
	-@erase "$(OUTDIR)\g200.pdb"
	-@erase "..\..\debug\g200.exe"
	-@erase "..\..\debug\g200.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../include" /I "../../../../10-Common/include/cmusdk" /I "../../../../10-common/include/cbb/system" /I "../../../../10-common/include/cbb/protocol" /I "../../../../10-common/include/cbb/platform" /I "../../../../10-common/include/aaa" /I "../../../../10-common/include/cms" /I "../../../../10-common/include/common" /I "../../../../10-common/include/nms" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\g200.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\g200.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CfgMgr.sbr" \
	"$(INTDIR)\Common.sbr" \
	"$(INTDIR)\G200.sbr" \
	"$(INTDIR)\G200Data.sbr" \
	"$(INTDIR)\PuiSsn.sbr" \
	"$(INTDIR)\SdkSsn.sbr" \
	"$(INTDIR)\VcCfg.sbr" \
	"$(INTDIR)\VcPui.sbr" \
	"$(INTDIR)\VcSched.sbr" \
	"$(INTDIR)\VcSdk.sbr"

"$(OUTDIR)\g200.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=osplib.lib kdvsys.lib puilib.lib cmusdk.lib snmpadp.lib camera.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\g200.pdb" /debug /machine:I386 /out:"../../debug/g200.exe" /pdbtype:sept /libpath:"../../../../10-common/lib/debug/win32" 
LINK32_OBJS= \
	"$(INTDIR)\CfgMgr.obj" \
	"$(INTDIR)\Common.obj" \
	"$(INTDIR)\G200.obj" \
	"$(INTDIR)\G200Data.obj" \
	"$(INTDIR)\PuiSsn.obj" \
	"$(INTDIR)\SdkSsn.obj" \
	"$(INTDIR)\VcCfg.obj" \
	"$(INTDIR)\VcPui.obj" \
	"$(INTDIR)\VcSched.obj" \
	"$(INTDIR)\VcSdk.obj"

"..\..\debug\g200.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "..\..\debug\g200.exe" "$(OUTDIR)\g200.bsc"
   del ..\..\debug\g200.lib
	del ..\..\debug\g200.exp
	del ..\..\debug\g200.ilk
	copy .\conf\* ..\..\debug\conf
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("g200.dep")
!INCLUDE "g200.dep"
!ELSE 
!MESSAGE Warning: cannot find "g200.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "g200 - Win32 Release" || "$(CFG)" == "g200 - Win32 Debug"
SOURCE=..\src\CfgMgr.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\CfgMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\CfgMgr.obj"	"$(INTDIR)\CfgMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Common.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\Common.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\Common.obj"	"$(INTDIR)\Common.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\G200.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\G200.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\G200.obj"	"$(INTDIR)\G200.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\G200Data.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\G200Data.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\G200Data.obj"	"$(INTDIR)\G200Data.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\PuiSsn.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\PuiSsn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\PuiSsn.obj"	"$(INTDIR)\PuiSsn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\SdkSsn.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\SdkSsn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\SdkSsn.obj"	"$(INTDIR)\SdkSsn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\VcCfg.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\VcCfg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\VcCfg.obj"	"$(INTDIR)\VcCfg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\VcPui.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\VcPui.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\VcPui.obj"	"$(INTDIR)\VcPui.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\VcSched.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\VcSched.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\VcSched.obj"	"$(INTDIR)\VcSched.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\VcSdk.cpp

!IF  "$(CFG)" == "g200 - Win32 Release"


"$(INTDIR)\VcSdk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "g200 - Win32 Debug"


"$(INTDIR)\VcSdk.obj"	"$(INTDIR)\VcSdk.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

