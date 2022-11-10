:START

del /Q xconfig.json
copy /Y xconfig-pc90-com.json xconfig.json

call win_run.bat

ping 1.1.1.1

goto START