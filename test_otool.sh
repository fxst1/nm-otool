#!/bin/bash

function	exec_test
{
	echo $1
	./ft_otool $1 > user 2> user_err && otool -t $1 > ref 2> ref_err
	diff user ref > /dev/null

	#if diff is not empty
	if [ $? -ne 0 ]; then

		echo $file >> "./test_otool.log"
		diff user ref >> "./test_otool.log"

		#if there is an error from otool
		if [ -s ref_err ]; then

			if [ -s user_err ]; then
				echo -e "\x1B[38;5;3m====> DIFF (NO STDERR)\x1B[0m"
			else
				echo -e "\x1B[38;5;3m====> DIFF (output on STDERR)\x1B[0m"
			fi

			echo "= User error:"
			cat user_err
			echo "= Reference error:"
			cat ref_err

		else
			echo -e "\x1B[38;5;1m====> ERROR\x1B[0m"
			exit;
		fi

	else

		echo -e "\x1B[38;5;2m====> OK\x1B[0m"

	fi

	echo ""
}

function	one_by_one
{
	echo "" > "./test_otool.log"
	for file in $@
	do
		exec_test	$file
	done
}

function	all
{
	echo "" > "./test_otool.log"
	exec_test "$*"
}

all $@
#one_by_one $@
rm -f user user_err ref ref_err
