#!/bin/bash

gcc worker.c -o worker
gcc scheduler.c -o scheduler.o
./scheduler.o data.txt
