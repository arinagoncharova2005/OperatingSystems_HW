#!/bin/bash

# compile create.c program
gcc create_fs.c -o create_fs.o

# take the first line from input.txt
file_name=$(head -n 1 input.txt)

# run program for creating file system
./create_fs.o $file_name

# compile program ex2.c
gcc ex2.c -o ex2.o

# run program ex2
./ex2.o input.txt
