#!/bin/bash

if [ -d "./tmp" ]; then
    rm -r "./tmp"
fi
ln -s $(pwd)/tmp tmp1
ls -li
echo "tmp created"
mkdir ./tmp
ls -li

# The difference is in the in the amount of memory occupied by the files in the current folder 
# on disk and in the list of files in the considered directory.
# Before mkdir command the space  on disk occupied by files was 64 blocks and after it became 68 blocks.
# Also before mkdir command there was no directory tmp, only symbolic link tmp1 that was pointing to ./tmp.
# But after we can see that the folder tmp is in the list.
# I think that this is because the result of the command ln -s ./tmp tmp1\u00bb is a symbolic link 
# (reference to the folder ./tmp) and this command does not allocate space 
# for the folder ./tmp.

bash gen.sh 10 ex1.txt
mv ex1.txt ./tmp/ex1.txt
echo "Files in tmp1"
ls tmp1

#creating symbolic link
ln -s $(pwd)/tmp tmp1/tmp2
bash gen.sh 10 ex1.txt
mv ex1.txt ./tmp1/tmp2/ex1.txt
echo "The content of tmp1"
ls tmp1
echo "The content of tmp2"
ls tmp1/tmp2

cd ./tmp1/tmp2/tmp2/tmp2/

# I noticed that we can go to the tmp2 directory infinitely many times
# because tmp2 point to tmp that contains tmp2

rm -rf tmp
cd ./tmp1/tmp2
# Now we cannot access tmp2 because we deleted tmp and now tmp2
# is deleted (ex24.sh: line 39: cd: ./tmp1/tmp2: No such file or directory)
rm -rf tmp1
