#!/bin/bash

#check the inode number
inode=$(ls -i ex1_bonus.c)
echo "Inode of ex1_bonus.c is $inode"

num_of_blocks=$(stat -c "%b" ex1_bonus.c)
echo "Number of blocks is $num_of_blocks"

size_of_block=$(stat -c "%B" ex1_bonus.c)
echo "Size of block is $size_of_block bytes"

total_size=$(stat -c "%s" ex1_bonus.c)
echo "Total size is $total_size"

permissions=$(stat -c "%a" ex1_bonus.c)
echo "Permissions $permissions"

#check the number of links of the ex2.c
num_of_links=$(stat -c "%h" ex2.c)
echo "Number of links of ex2.c is $num_of_links"
inode_2=$(ls -i ex2.c)
echo "Inode of ex2.c is $inode_2"

# Result:
# Inode of ex1_bonus.c is 659897 ex1_bonus.c
# Number of blocks is 8
# Size of block is 512 bytes
# Total size is 76
# Permissions 664
# Number of links of ex2.c is 1
# Inode of ex2.c is 660777 ex2.c


# Explanation:

# ex2.c and ex1_bonus.c have different inode numbers because we copied file 
# and obtained one another (separate) file with with the same content
# but we did not link them

# identify the files who have 3 links in the path /etc
stat -c "%h - %n" /etc/* | grep ^3

# This number represents the number of files that map to the same inode 
# as the given file has (link count). It means that for each such file (in the output) 
# there are 2 directories that contain a mapping to the same inode 
# as the inode of this file.

