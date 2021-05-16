#! /bin/bash

i=1
end=100
inc=5
mask=0x1
db_name="mydb"
grep_cmd=""


key_range[0]=100000
key_range[1]=1000000

ops[0]=1000000
ops[1]=10000000

echo '********************************'
echo key_range = ${key_range[0]}. ops=${ops[0]}. multicore
while [ $i -lt $end ]; do
	rm -rf $db_name
#	echo "value of write frequency is $i"
#	taskset $mask ./main 1000000 $i | grep fraction
	./main -b=purelyrandom $db_name ${ops[0]}  $i ${key_range[0]} $grep_cmd
	((i=i+$inc))
done
i=1
echo '********************************'
echo key_range = ${key_range[0]}. ops=${ops[1]}. multicore
while [ $i -lt $end ]; do
	rm -rf $db_name
	#       echo "value of write frequency is $i"
	#       taskset $mask ./main 1000000 $i | grep fraction
	./main -b=purelyrandom $db_name ${ops[1]}  $i ${key_range[0]} $grep_cmd
	((i=i+$inc))
done
i=1
echo '********************************'
echo key_range = ${key_range[1]}. ops=${ops[1]}. multicore
while [ $i -lt $end ]; do
	rm -rf $db_name
	#       echo "value of write frequency is $i"
	#       taskset $mask ./main 1000000 $i | grep $fraction
	./main -b=purelyrandom $db_name ${ops[1]}  $i ${key_range[1]} $grep_cmd
	((i=i+$inc))
done

i=1

echo '********************************'
echo key_range = ${key_range[0]}. ops=${ops[0]}. singlecore
while [ $i -lt $end ]; do
	rm -rf $db_name
	#       echo "value of write frequency is $i"
	#       taskset $mask ./main 1000000 $i | grep fraction
	taskset $mask ./main -b=purelyrandom $db_name ${ops[0]}  $i ${key_range[0]} $grep_cmd
	((i=i+$inc))
done
i=1
echo '********************************'
echo key_range = ${key_range[0]}. ops=${ops[1]}. singlecore
while [ $i -lt $end ]; do
	rm -rf $db_name
	#       echo "value of write frequency is $i"
	#       taskset $mask ./main 1000000 $i | grep fraction
	taskset $mask ./main -b=purelyrandom $db_name ${ops[1]}  $i ${key_range[0]} $grep_cmd
	((i=i+$inc))
done
i=1
echo '********************************'
echo key_range = ${key_range[1]}. ops=${ops[1]}. singlecore
while [ $i -lt $end ]; do
	rm -rf $db_name
	#       echo "value of write frequency is $i"
	#       taskset $mask ./main 1000000 $i | grep fraction
	taskset $mask ./main -b=purelyrandom $db_name ${ops[1]}  $i ${key_range[1]} $grep_cmd
	((i=i+$inc))
done
