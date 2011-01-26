@If /I "%1"=="" (
del /F /Q *.dep
del ..\..\debug\g200ms.exe
nmake /NOLOGO /A /B /f g200ms.mak CFG="g200ms - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200msd.txt

del /F /Q *.dep
del ..\..\release\g200ms.exe
nmake /NOLOGO /A /B /f g200ms.mak CFG="g200ms - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200ms.txt
)

@if /I "%1"=="noclean" (
del /F /Q *.dep
del ..\..\debug\g200ms.exe
nmake /NOLOGO /B /f g200ms.mak CFG="g200ms - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200msd.txt

del /F /Q *.dep
del ..\..\release\g200ms.exe
nmake /NOLOGO /B /f g200ms.mak CFG="g200ms - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200ms.txt
)

@if /I "%1"=="clean" (
del /F /Q *.dep
del ..\..\debug\g200ms.exe
nmake /NOLOGO /A /B /f g200ms.mak CFG="g200ms - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200msd.txt

del /F /Q *.dep
del ..\..\release\g200ms.exe
nmake /NOLOGO /A /B /f g200ms.mak CFG="g200ms - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200ms.txt
)

copy /Y ..\..\debug\g200ms.exe ..\..\..\..\10-common\version\debug\win32\g200

copy /Y ..\..\release\g200ms.exe ..\..\..\..\10-common\version\release\win32\g200