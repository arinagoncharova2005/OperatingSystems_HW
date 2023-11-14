#!/bin/bash

chmod a-w ex5.txt
ls -l ex5.txt
chmod uo+rwx ex5.txt
ls -l ex5.txt
chmod -R g=u ex5.txt
ls -l ex5.txt

# Answers on questions:
# What does 660 mean for ex5.txt?
# 6 = 4+2+0 means rw-
# rw-rw- - - - 
# User and group can read and write, others have no permission 

# What does 775 mean for ex5.txt? 
# 7 = 4+2+1 means rwx
# 5 = 4+1 means -wx
# rwx rwx -wx
# User, group and others can read, write, execute and others can only write and execute

# What does 777 mean for ex5.txt? 
# 7 = 4+2+1 means rwx
# rwx rwx rwx
# User, group and others can read write and execute

