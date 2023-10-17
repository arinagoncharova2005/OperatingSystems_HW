#!/bin/bash

gcc worker.c -o worker
gcc scheduler_rr.c -o scheduler_rr.o
./scheduler_rr.o data.txt
