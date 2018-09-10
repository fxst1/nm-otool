#!/bin/bash

echo "" > "./test.log"
for file in $@
do

	echo $file;
	./ft_nm $file > user 2> user_err && nm $file > ref 2> ref_err && diff user ref > tmp

	#if diff is not empty
	if [ -s tmp ]; then

		echo $file >> "./test.log"
		diff user ref >> "./test.log"

		#if there is an error	 from ft_nm
		if [ -s user_err ]; then

			echo -e "\x1B[38;5;3m====> WARNING\x1B[0m"
			echo "= User error:"
			cat user_err
			echo "= Reference error:"
			cat ref_err

		else
			echo -e "\x1B[38;5;1m====> ERROR\x1B[0m"
		fi

	else

		echo -e "\x1B[38;5;2m====> OK\x1B[0m"

	fi

	echo ""

done
rm -f tmp user user_err ref ref_err
