#!/usr/bin/env bash

# generate a list of N names
# pass in a number to tell how many names to generate

# generate a names list using useraccounts.json

# make sure json is there
if [ ! -f useraccounts.json ]; then
    echo "no user_accounts.json"
    echo "need that file... exitting."
    exit
fi

if [ $# == 0 ]; then
    echo "names list will not be shortened, pass a number for number of names"
    echo "pass in a number to alter how many names in the final list"
fi

############
############

# gen the names from the python json
python python/read_users.py | awk '{print $1}' > names.txt.a

# sort the unique names
sort names.txt.a | cut -d '.' -f1 | uniq > names.txt.a.uniq

# cheap way to get more names out of the list
# triple the size
for i in $(cat names.txt.a.uniq); do
    echo "11$i" >> names.txt.a.uniq
done

for i in $(cat names.txt.a.uniq); do
    echo "22$i" >> names.txt.a.uniq
done


# output NUM number of names
if [ $# != 0 ]; then
    NUM=$1
    echo "adjusting final file for $NUM names"
    head -$NUM names.txt.a.uniq > names.txt.final
    rm names.txt.a.uniq
else
    mv names.txt.a.uniq names.txt.final
fi


rm names.txt.a
mv names.txt.final names.txt
