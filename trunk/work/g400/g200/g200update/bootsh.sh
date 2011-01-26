#!/bin/sh
#exec 1<>/dev/null
#exec 2<>/dev/null

#the first argument is the touch dir
touch_dir()
{
    if test ! -d $1
    then
        mkdir -p $1 
    fi
}

#the first argument is source file, the second argument is the destination dir or new full destination name
move()
{
    mv -f $1 $2 --reply=yes
}
move_exist()
{
    if test -f $1 || test -d $1
    then
        move "$1" "$2"
    fi
}


if [ -f /opt/kdm/httpserver/g200exe.tar ]; then
	touch_dir /opt/kdm/update
	tar -P -xvf /opt/kdm/httpserver/g200exe.tar -C /opt/kdm/update
	rm -f /opt/kdm/httpserver/g200exe.tar
	cd /opt/kdm/update
	if [ -f ./checkupdate.sh ]; then
		chmod +x checkupdate.sh
		./checkupdate.sh
	fi
fi

#update config file,run at the beginning of system rebooting
if [ -f /opt/kdm/httpserver/conf.tar ]; then
	tar -P -xvf /opt/kdm/httpserver/conf.tar
	rm -f /opt/kdm/httpserver/conf.tar
fi


#move licensekey into g200 conf
if [ -f /opt/kdm/httpserver/kedalicense.key ]; then
	move_exist /opt/kdm/httpserver/kedalicense.key /opt/kdm/g200/conf
fi