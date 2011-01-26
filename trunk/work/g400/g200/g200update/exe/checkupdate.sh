#!/bin/sh
exec 1<>/dev/null
exec 2<>/dev/null
UpdateLog='/opt/kdm/update.log'; test -f $UpdateLog && rm -f $UpdateLog; touch $UpdateLog
G200DestDir=/opt/kdm/g200
HpsDestDir=/opt/kdm/httpserver
G200ExeFiles=/opt/kdm/g200exe

##function defined here
#the first argument is log content
log()
{
    echo "$1" >>$UpdateLog
}

#the first argument is source file, the second argument is the destination dir or new full destination name
copy()
{
    cp -Rf $1 $2 --reply=yes 2>>$UpdateLog
    if [ $? -ne 0 ]; then
	    log "copy $1 fail"
    fi
}
copy_exist()
{
    if test -f $1 || test -d $1
    then
        copy "$1" "$2"
    fi
}

#the first argument is source file, the second argument is the destination dir or new full destination name
move()
{
    mv -f $1 $2 --reply=yes 2>>$UpdateLog
    if test $? -ne 0
    then
        log "mv $1 fail"
    fi
}
move_exist()
{
    if test -f $1 || test -d $1
    then
        move "$1" "$2"
    fi
}

#the first argument is the file
remove()
{
    rm -Rf $1 2>>$UpdateLog
    if test $? -ne 0
    then
        log "rm $1 fail"
    fi
}
remove_exist()
{
    if test -f $1 || test -d $1
    then
        remove "$1"
    fi
}

#the first argument is the touch dir
touch_dir()
{
    if test ! -d $1
    then
        mkdir -p $1 2>>$UpdateLog
        if test $? -ne 0
        then
            log "mkdir $1 fail"
        fi
    fi
}

#the first argument is the exe file name, the file name's arguments as the second argument
execute()
{
    if test -f $1
    then
        chmod +x $1 2>>$UpdateLog
        $1 $2
    else
        log "the exe file not exist, filename=[$1]"
    fi
}
#move all files(not include direction or in the sub direction) in a direction and chmod +x
#the first argument is src direction, the second argument is the dst direction
copy_exefiles()
{
    touch_dir $2
    
    for f in `ls $1 2>>$UpdateLog`
    do
        if test -f $1/$f
        then
            chmod +x $1/$f 2>>$UpdateLog
            copy $1/$f $2
        fi
    done
}

#the first argument is exe source file, the second argument is the destination dir
copy_exefile()
{
		touch_dir $2
		if test -f $1
		then
			chmod +x $1 2>>$UpdateLog
			copy $1 $2
		fi
}

#move all files(not include direction or in the sub direction) in a direction
#the first argument is src direction, the second argument is the dst direction
copy_files()
{
    touch_dir $2
    
    for f in `ls $1 2>>$UpdateLog`
    do
        if test -f $1/$f
        then
            copy $1/$f $2
        fi
    done
}








#there is no config file in the update, but the directory should be existing
touch_dir $G200DestDir/conf
copy_exefiles ./g200/script $G200DestDir/script
copy_exist ./g200/g200web $G200DestDir
	
copy_exefile ./httpserver/httpserver $HpsDestDir
copy_files ./httpserver/config $HpsDestDir/config

#update sofeware
copy_exefiles ./g200exe $G200ExeFiles

#the mplatype is vsb2 by default
#kdmresearch and g200 config file,note that you should export this configs,before updateing
if [ ! -f $G200DestDir/conf/G200.ini ]; then
    copy_exist $G200ExeFiles/g200vsb2 $G200DestDir/g200
    copy_exist $G200ExeFiles/g200msvsb2 $G200DestDir/g200ms
    copy_files ./config $G200DestDir/conf
fi

rm -rf /opt/kdm/update/*
