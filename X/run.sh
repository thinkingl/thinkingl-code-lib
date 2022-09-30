ulimit -c unlimited
while true
do
	make && output/x
	sleep 10
done
