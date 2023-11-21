#!/bin/bash

# creating a file lofs.img of size 50 MiB
fallocate -l 50MiB lofs.img
# setting up loop device on lofs.img, -f is used to find the next available name for loop device
sudo losetup -f ./lofs.img 
# create ext4 filesystem
sudo mkfs.ext4 lofs.img
# creating lofsdisk directory to use it as mount point
mkdir ./lofsdisk
# mounting device with lofsdisk
sudo mount ./lofs.img ./lofsdisk

# add read, write and  execute permission for user, group and others for lofsdisk
sudo chmod -R a+rwx lofsdisk
# go to the lofsdisk directory
cd lofsdisk
# create file1 with my first name Arina
echo "Arina" > file1
# create file2 with my second name Goncharova
echo "Goncharova" > file2

# define  function that returns the paths of all shared libraries of a binary file 
# the first argument - path of file
function get_libs() {
     # getting path as the first argument
     path=$1
     
     a=$(ldd $path | awk '{print $3}')
     # ldd lists shared libraries for file whose path function got
     # then cut the result by ">" and take the second field and then take the first field from obtained line
     ldd $path | cut -d '>' -f 2 | awk '{print $1}'
}

# create lib directory
mkdir lib
# create bin directory
mkdir bin
# move to current parent directory (inside which lofsdisk is located)
cd ..

# copy shared librarues for bash 
cp $(get_libs /bin/bash) lofsdisk/lib 2> /dev/null
# copy /bin/bash
cp /bin/bash lofsdisk/bin

# copy shared librarues for cat 
cp $(get_libs /bin/cat) lofsdisk/lib  2> /dev/null
#copy /bin/cat
cp /bin/cat lofsdisk/bin

# copy shared libraries for echo 
cp $(get_libs /bin/echo) lofsdisk/lib  2> /dev/null
#copy /bin/echo
cp /bin/echo lofsdisk/bin

# copy shared librarues for ls 
cp $(get_libs /bin/ls) lofsdisk/lib  2> /dev/null
#copy /bin/ls
cp /bin/ls lofsdisk/bin

#complie program ex1.c
gcc --static ex1.c -o ex1
#copy ex1 to lofdisk to use it insode lofsdisk
cp ex1 ./lofsdisk
#change the root directory to lofsdisk, execute ex1 and add the result of ex1 to ex1.txt
sudo chroot lofsdisk ./ex1 >> ex1.txt
# print separator to ex1.txt for better visibility
echo "------------------" >> ex1.txt
# print message for better visibility and convenience
echo "With usual root" >> ex1.txt
# print separator to ex1.txt for better visibility
echo "------------------" >> ex1.txt
#execut ex1 with usual root and add the result of ex1 to ex1.txt
./ex1 >> ex1.txt

