#!/bin/bash

echo "/dev"
ls -l /dev
echo "/etc"
ls -l /etc
echo "Number of directories in /etc"
ls -l /etc | grep ^d | wc -l
echo "type of ex1_bonus.c (Hello world) before compilation"
file ex1_bonus.c
gcc ex1_bonus.c -o ex1_bonus.o
echo "type of ex1_bonus.c (Hello world) after compilation"
file ex1_bonus.c
echo "type of ex1_bonus.o"
file ex1_bonus.o
echo "type of ex1_2.c (Привет, мир) before compilation"
file ex1_2.c
gcc ex1_2.c -o ex1_2.o
echo "type of ex1_2.o"
file ex1_2.o

# Result from terminal

# Number of directories in /etc
# 138
# type of ex1_bonus.c (Hello world) before compilation
# ex1_bonus.c: C source, ASCII text
# type of ex1_bonus.c (Hello world) after compilation
# ex1_bonus.c: C source, ASCII text
# type of ex1_bonus.o
# ex1_bonus.o: ELF 64-bit LSB pie executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, BuildID[sha1]=955f7fd5fa42b8e45444f82f29e45eeed912b9c4, for GNU/Linux 3.7.0, not stripped
# type of ex1_2.c (Привет, мир) before compilation
# ex1_2.c: C source, Unicode text, UTF-8 text
# type of ex1_2.o
# ex1_2.o: ELF 64-bit LSB pie executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, BuildID[sha1]=aea1a078f123e22d50c8131ce031a27317c77b54, for GNU/Linux 3.7.0, not stripped



# The differerence between ex1.c and ex1_2.c is that 
# type of ex1.c is displayed as an ASCII text, 
# while ex1_2. is a Unicode text.
# This is because the 1st program is using only English 
# characters that are encoded by ASCII and the 2nd program 
# uses the UTF-8 encoding because it allows to encode 
# more characters, for example, Russian.
