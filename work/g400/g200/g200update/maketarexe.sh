#!/bin/sh
#Name:maketarexe.sh
#Function: the script is for making g200exe.tar

OutRootDir=../../../10-Common/version/release/linux/g200update; test ! -d $OutRootDir && mkdir -p $OutRootDir
ConfigRootDir=../../../10-Common/version/config 
G200WebDir=../g200web 
localDir=./exe 
HttpServer=../../../10-Common/version/release/linux/httpserver 

#this is where g200 exe file is
ExeRootDir=../../../10-Common/version/release/linux 
sumresult=$OutRootDir/sum.txt;

abort()
{
	echo "maketarexe is abortted,the date is:`date`"
	exit -1
}

touch_dir()
{
    if test ! -d $1
    then
        mkdir -p $1
        if test $? -ne 0
        then
            echo "mkdir $1 fail"
            abort
        fi
    fi
}

copy()
{
    touch_dir $2
    cp -Rf $1 $2 --reply=yes
    if [ $? -ne 0 ]; then
	    echo "copy $1 fail"
	    abort
    fi
}

#remove all files including directory
remove()
{
    rm -Rf $1
    
    if [ $? -ne 0 ]; then
	    echo "remove $1 fail"
	    abort
    fi
}

remove_exist()
{
    if test -f $1 || test -d $1
    then
        remove "$1"
    fi
}

#remove all files excluding directory
remove_files()
{
    rm -f $1/*
    if [ $? -ne 0 ]; then
        echo "remove all files in directory $1,fail"
        abort
    fi
}
move()
{
    mv -f $1 $2
    
    if [ $? -ne 0 ]; then
	    echo "move [$1] to [$2] fail"
	    abort
    fi
}

#move all files(not include direction or in the sub direction) in a direction
#the first argument is src direction, the second argument is the dst direction
copy_files()
{
    touch_dir $2
    
    for f in `ls $1`
    do
        if test -f $1/$f
        then
            echo $f
            copy $1/$f $2
        fi
    done
}

#delete old exe file
if [ -f "$OutRootDir/g200exe.tar" ]; then 
	rm -f $OutRootDir/g200exe.tar
fi

remove_exist $localDir/g200
remove_exist $localDir/g200exe
copy_files $ExeRootDir/g200 $localDir/g200exe
copy_files $ConfigRootDir/g200/conf $localDir/config

copy $ConfigRootDir/g200/script $localDir/g200
copy $G200WebDir $localDir/g200
remove_files $localDir/g200/g200web/download

#the config file has been previously positioned
copy $HttpServer/httpserver $localDir/httpserver


cd exe
tar -P -uvf ../g200exe.tar ./*
cd ..
echo "---sum value of exe tar(`date`)---" >> $sumresult
sum g200exe.tar >> $sumresult
mv -f g200exe.tar $OutRootDir
copy bootsh.sh $OutRootDir
copy inittab $OutRootDir
echo ------------------- End make g200exe.tar --------------------