#!/bin/bash

gcc controller.c -o controller.o
./controller.o
gcc agent.c -o agent.o
gnome-terminal -- ./agent.o
