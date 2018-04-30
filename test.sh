#!/bin/bash

for file in $1
do

	echo $file;
	./ft_nm $file > tmp && nm $file > tmp2 && diff tmp tmp2

done
