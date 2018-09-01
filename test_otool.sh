#!/bin/bash

echo "" > "./test.log"
for file in $@
do
	echo $file;
	./ft_otool $file > tmp1 && otool -t $file > tmp2 && diff tmp1 tmp2 > tmp
	if [ -s tmp ]; then
		echo $file >> "./test.log"
		sdiff tmp1 tmp2 >> "./test.log"
		echo -e "\x1B[38;5;1m====> ERROR\x1B[0m"
	fi

done
rm tmp tmp1 tmp2

#ARCSRouterIPCDriver.o header	:	@11a330
#ARCSRouterIPCDriver.o	:	@11a388
