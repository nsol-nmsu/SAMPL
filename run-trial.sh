#!/bin/bash

# !!! make sure the right database is in here



# set the database that is being used
# manually put it in the root working dir
DB=75-64

# sanity check to make sure the correct db is being used
DB_HASH=$(md5sum DB/$DB.db | awk '{print $1}')
T_HASH=$(md5sum test.db | awk '{print $1}')


# quit if the wrong db is in root
if [ $T_HASH == $DB_HASH ]; then
	echo "[*] Database is correct ( $DB )"
else
	echo "[!] Error: test.db does not appear to be correct"
	exit
fi


#exit

# run trial
# write data
# clean up

days=(  30 40 50 )
users=(  15 30 )

for x in ${days[@]}
do
	for i in {0..1}
	do
		for user in ${users[@]}
		do
			echo "[command] val-run $user $x $i"   
			./val-run.sh $user $x $i
			echo "[command] trials.sh $user-$x-$DB" 
			./trials.sh $user-$x-$DB
			./clean.sh
		done
	done
done

echo "FINAL"

