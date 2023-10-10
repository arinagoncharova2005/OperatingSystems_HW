#!/bin/bash

echo > ex3_res.txt

gcc -pthread ex3.c -o ex3.o
start=`date +%s%N`
 ./ex3.o 10000000 1
end=`date +%s%N`
echo The 1st time is `expr $end - $start` nanoseconds >> ex3_res.txt

start=`date +%s%N`
./ex3.o 10000000 2
end=`date +%s%N`
echo The 2nd time is `expr $end - $start` nanoseconds >> ex3_res.txt

start=`date +%s%N`
./ex3.o 10000000 4
end=`date +%s%N`
echo The 3rd time is `expr $end - $start` nanoseconds >> ex3_res.txt

start=`date +%s%N`
./ex3.o 10000000 10
end=`date +%s%N`
echo The 4th time is `expr $end - $start` nanoseconds >> ex3_res.txt

start=`date +%s%N`
./ex3.o 10000000 100
end=`date +%s%N`
echo The 5th time is `expr $end - $start` nanoseconds >> ex3_res.txt

