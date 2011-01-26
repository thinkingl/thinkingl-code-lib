@If /I "%1"=="" (
del /F /Q *.dep
del ..\..\debug\g200.exe
nmake /NOLOGO /A /B /f g200.mak CFG="g200 - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200d.txt

del /F /Q *.dep
del ..\..\release\g200.exe
nmake /NOLOGO /A /B /f g200.mak CFG="g200 - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200.txt
)

@if /I "%1"=="noclean" (
del /F /Q *.dep
del ..\..\debug\g200.exe
nmake /NOLOGO /B /f g200.mak CFG="g200 - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200d.txt

del /F /Q *.dep
del ..\..\release\g200.exe
nmake /NOLOGO /B /f g200.mak CFG="g200 - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200.txt
)

@if /I "%1"=="clean" (
del /F /Q *.dep
del ..\..\debug\g200.exe
nmake /NOLOGO /A /B /f g200.mak CFG="g200 - Win32 Debug" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200d.txt

del /F /Q *.dep
del ..\..\release\g200.exe
nmake /NOLOGO /A /B /f g200.mak CFG="g200 - Win32 Release" /x nmakeError.log > ..\..\..\..\10-common\version\compileinfo\g200.txt
)

copy /Y ..\..\debug\g200.exe ..\..\..\..\10-common\version\debug\win32\g200

copy /Y ..\..\release\g200.exe ..\..\..\..\10-common\version\release\win32\g200