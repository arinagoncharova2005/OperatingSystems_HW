mkdir ~/week01/first_folder
date
sleep 3
touch ~/week01/first_folder/root.txt
date
sleep 3
mkdir ~/week01/second_folder
date
sleep 3
touch ~/week01/second_folder/home.txt
ls / -t -r > ~/week01/first_folder/root.txt
ls ~ -t -r > ~/week01/second_folder/home.txt
cat ~/week01/first_folder/root.txt ~/week01/second_folder/home.txt
ls ~/week01/first_folder
ls ~/week01/second_folder
