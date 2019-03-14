#!/bin/bash

testsNumber=9
sum=0

make clean > /dev/null

make build > /dev/null

rm -r output
mkdir output

echo ""
echo "--------------- Tests ---------------"
for i in $(seq 0 $testsNumber); do
    make -s run var="./in/test$i/file*.txt" < ./in/test$i/input > ./output/test$i.out
	diff -Z ./output/test$i.out ref/test$i.ref > /dev/null
	if [ $? -eq 0 ]; then
		echo "Test $i ......................... passed"
		sum=$(($sum + 1))
	else
		echo "Test $i ......................... failed"
	fi
done
