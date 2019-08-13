#!/bin/bash

# run all tests using valgrind

# users days offset

# set the lock so script knows when the simulation is done
touch fin.lock

function usage 
{
    echo "Enter the args for Law"
    echo "options are: "
    echo "users days offset"
}

[[ $# != 3 ]] && usage && exit

echo "============================================"
echo " Running through valgrind with output files "
echo "============================================"

echo "Cleaning up any leftover valgrind outputs"
rm valgrind-law.log 2>&1 > /dev/null
rm valgrind-enforcer.log 2>&1 > /dev/null
rm valgrind-company.log 2>&1 > /dev/null
rm valgrind-judge.log 2>&1 > /dev/null

echo "============================================"
echo " Starting ethereum for all nodes"
echo "============================================"

#cd eth
#./start_all.sh
#cd ..


#OPTIONS="--leak-check=full --show-leak-kinds=all --track-origins=yes"
OPTIONS=""

# Run each entitiy, and get the PID
valgrind $OPTIONS --log-file="valgrind-judge.log" ./Judge/./Judge &
JUDGEPID=$!

valgrind $OPTIONS --log-file="valgrind-enforcer.log" ./Enforcer/./Enforcer &
ENFORCERPID=$!

valgrind $OPTIONS --log-file="valgrind-company.log" ./Company/./Company &
COMPANYPID=$!

echo "sleeping for Company, 1 second(s)..."
sleep 1

# pass in the options here
valgrind $OPTIONS --log-file="valgrind-law.log" ./Law/./Law $1 $2 $3 &
LAWPID=$!


echo $JUDGEPID
echo $ENFORCERPID
echo $COMPANYPID
echo $LAWPID


while [ -f "fin.lock" ] 
do
	sleep 2
done


# After a key is hit, this will be reached
# and kill the running programs from their
# PID retrieved earlier
kill $JUDGEPID
kill $ENFORCERPID
kill $COMPANYPID
kill $LAWPID
killall  memcheck-amd64-
echo "FINISHED !!!"
#killall geth


