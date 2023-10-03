gcc ex3.c -o ex3.o
./ex3.o 4 &
pstree | grep "ex3"
sleep 5
pstree | grep "ex3"
sleep 5
pstree | grep "ex3"
sleep 5
pstree | grep "ex3"
sleep 5
./ex3.o 3 &
pstree | grep "ex3"
sleep 5
./ex3.o 5 &
pstree | grep "ex3"
sleep 5
