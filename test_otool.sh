#!/bin/bash

echo "" > "./test.log"
for file in $1
do
	echo $file;
	./ft_otool $file > tmp1 && otool -t $file > tmp2 && sdiff tmp1 tmp2 > tmp
	if [ -s tmp ]; then
		echo $file >> "./test.log"
		diff tmp tmp2 >> "./test.log"
		echo -e "\x1B[38;5;1m====> ERROR\x1B[0m"
	fi

done
rm tmp tmp1 tmp2
