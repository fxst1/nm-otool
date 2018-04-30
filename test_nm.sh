#!/bin/bash

echo "" > "./test_nm.log"
for file in $1
do
	echo $file;
	./ft_nm $file > tmp1 && nm $file > tmp2 && diff tmp1 tmp2 > tmp
	if [ -s tmp ]; then
		echo $file >> "./test_nm.log"
		diff tmp tmp2 >> "./test_nm.log"
		echo -e "\x1B[38;5;1m====> ERROR\x1B[0m"
	fi

done
rm tmp tmp1 tmp2
