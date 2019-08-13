#!/bin/bash
# run this to start up an entity
# including getting correct enode and ip to share


# start the entities eth node

if [ $# == 0 ]; then
  echo "usage:"
  echo "  ./ent_start.sh {entitiy} {options}"
  echo "options:"
  echo "  -m:  run as miner"
  exit
fi

ENT=$1

echo "[*] Preparing and starting eth node for $ENT"

# set the eth port by entity
if [ $ENT == "judge" ]; then
  PORT=30303
elif [ $ENT == "law" ]; then
  PORT=30305
elif [ $ENT == "company" ]; then
  PORT=30304
elif [ $ENT == "enforcer" ]; then
  PORT=30306
fi


MINER=""
NETID="1115"

# run this node as a miner if -m is passed
# use {entitity} -m to start as a miner
if [ "$2" == '-m' ]; then
  echo "starting '$ENT' as miner"
  MINER="--mine --miner.threads 1"
fi


echo "[*] Cleaning $ENT"
./clean.sh $ENT
echo "[*] Initializing $ENT"
./init.sh $ENT



echo "[*] Starting $ENT eth node in the background"
# run this command now for the specific entitity
geth --datadir $ENT --networkid $NETID --syncmode "full" $MINER --port $PORT 2>>$ENT/$ENT.log &

echo "[*] Getting enode and ip of running $ENT eth node"
python3 get_enode.py $ENT 



