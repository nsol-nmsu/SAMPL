#!/bin/bash
# 
# usage
# ./trials.sh {# users} {#entries per day} {#

#########
# data needed to extract
# 

if [ $# != 1 ]; then
  echo " Enter the file name extension for results"
  exit
fi

SR_FILE=results/SR_res-$1.txt
CO_FILE=results/CO_res-$1.txt
THREE_TIMES=results/THREE_res-$1.txt


# calc the average of id time and sig time from the python
# output to avg.txt
COUNTER=$(grep COUNTER Enforcer.log | awk '{print $2}')
echo $COUNTER
echo $COUNTER > counter.txt
python tally.py

DETAILS=$1

# enforcer times
e_time=$(grep E_TIME Enforcer.log | awk '{print $2}')
e_root_time=$(grep ROOT_TIME avg.txt | awk '{print $2}')
e_id_time=$(grep ID_TIME avg.txt | awk '{print $2}')
e_sig_time=$(grep SIG_TIME avg.txt | awk '{print $2}')



# company times
c_time=$(grep C_TIME Company.log | awk '{print $2}')



echo -e "\n==================\n new trial\n" >> $SR_FILE
echo "Trail details(user-days-datasize-merklesize): $DETAILS" >> $SR_FILE
echo "total time for enforcer to verify file: $e_time ( seconds )"  >> $SR_FILE
echo "time to verify merkle root: $e_root_time (seconds )" >> $SR_FILE
echo "time to verify id proof: $e_id_time ( seconds )" >> $SR_FILE
echo "time to verify root sig: $e_sig_time ( seconds )" >> $SR_FILE
echo -e "\n==============================================\n" >> $SR_FILE

echo -e "\n==================\n new trial\n" >> $CO_FILE
echo "Trial details(users-days): $DETAILS" >> $CO_FILE
echo "total time for company to query all SO: $c_time ( seconds )" >> $CO_FILE
echo -e "\n==============================================\n" >> $CO_FILE



