#!/bin/bash

n=$1
gcc publisher.c -o publisher.o
gnome-terminal -- ./publisher.o  $n
gcc subscriber.c -o subscriber.o
for ((i = 1; i<=$n; i++)) 
do
	gnome-terminal -- ./subscriber.o $i
done
