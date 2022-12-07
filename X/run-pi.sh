ulimit -c unlimited
while true
do
	cp xconfig-pi.json xconfig.json
	make && output/x
	rm -f xconfig.json
	sleep 10
done
