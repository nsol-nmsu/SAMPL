#!/bin/bash

# for running local tests, company writes to a file, and
# enforcer reads in the file to verify

# When the tests are done, Enfocer will 'release' the lock, and delete
# this file
touch fin.lock


function usage 
{
    echo "Enter the args for Law"
    echo "options are: "
    echo "users days offset"
}

[[ $# != 3 ]] && usage && exit


# Run each entitiy, and get the PID
./Judge/./Judge &
JUDGEPID=$!

./Enforcer/./Enforcer &
ENFORCERPID=$!

./Company/./Company &
COMPANYPID=$!

echo "sleeping for Company, 1 seconds..."
sleep 1

./Law/./Law $1 $2 $3 &
LAWPID=$!

echo "PID of programs running"
echo $JUDGEPID
echo $ENFORCERPID
echo $COMPANYPID
echo $LAWPID

while [ -f "fin.lock" ]
do 
	sleep 2
done


kill $JUDGEPID
kill $ENFORCERPID
kill $COMPANYPID
kill $LAWPID

echo -e "\nFinished\n"


