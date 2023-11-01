#!/bin/bash

file="/tmp/ex1.pid"
pid=$(cat "$file")
#echo "PID: $pid"

addresses=$(sudo grep "heap" /proc/$pid/maps | cut -d " " -f 1)
start=$(echo $addresses | cut -d "-" -f 1)
end=$(echo $addresses | cut -d "-" -f 2)

#line with the password
found_pas_line=$(sudo xxd -s 0x$start -l $((0x$end - 0x$start)) /proc/$pid/mem | grep pass:)

password_address=$(echo $found_pas_line | cut -d ":" -f 1)
echo Address of the password: $password_address
#delete "pass:"
pass=${found_pas_line#*pass:}
#delete last "..."
result_password=${pass%???}
echo Password is $result_password
sudo kill -SIGKILL "$pid"
