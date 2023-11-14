#!/bin/bash

bash gen.sh 10 ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex21.txt
echo "ex1.txt content" > output.txt
cat ex1.txt >> output.txt
echo "ex11.txt content" >> output.txt
cat ex11.txt >> output.txt
echo "ex21.txt content" >> output.txt
cat ex21.txt >> output.txt

# Comparison:
# There is no difference in content of these files 
# because ex11.txt and ex21.txt are hard links to ex1.txt.
# They point to the same file, therefore the content of 
# these 3 files is the same.


#chech the i-node numbers
inode1=$(ls -i ex1.txt | awk '{print $1}')
echo "Inode of ex1.txt is $inode1">> output.txt
inode2=$(ls -i ex11.txt | awk '{print $1}')
echo "Inode of ex11.txt is $inode2">> output.txt
inode3=$(ls -i ex21.txt | awk '{print $1}')
echo "Inode of ex21.txt is $inode3">> output.txt

# Inode numbers of these 3 files are the same because
# ex11.txt and ex21.txt are hard links to ex1.txt and
# hard links are new names for the same i-node.

#check the disk usage
usage=$(du -h ex1.txt)
echo "Disk usage of ex1.txt is $usage">> output.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp
find . -inum $inode1 >> output.txt
sudo find / -inum $inode1 >> output.txt

# Searching in the current path gives files that link
# to ex1.txt in the current directory while searching 
# in the root path gives files that link to ex1.txt 
# in the whole file system.

num_of_hard_links=$(stat -c "%h" ex1.txt)
echo "Number of hard links to ex1.txt is $num_of_hard_links" >> output.txt

#remove all links from ex1.txt
sudo find / -inum $inode1 -exec rm {} \;
