#!/bin/bash

mkdir /tmp/lab 2> /dev/null
rm /tmp/lab/* 2> /dev/null

gcc monitor.c -o monitor.o
gnome-terminal -- bash -c "./monitor.o /tmp/lab; exec bash"

gcc ex1.c -o ex1.o
gnome-terminal -- bash -c "./ex1.o /tmp/lab; exec bash"
