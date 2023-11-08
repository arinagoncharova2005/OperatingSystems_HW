#!/bin/bash

gcc pager.c -o pager
./pager 4 2 random&

pid_pager=$(pidof -s pager)

gcc mmu.c -o mmu
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager

./pager 4 2 nfu&

pid_pager=$(pidof -s pager)

gcc mmu.c -o mmu
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager

./pager 4 2 aging&

pid_pager=$(pidof -s pager)

gcc mmu.c -o mmu
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager
