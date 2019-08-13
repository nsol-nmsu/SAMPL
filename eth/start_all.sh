#!/bin/bash

# -i to just init
# -s to just start ( already init )
# just run without args to do both ( clean slate )

echo "Starting all for local test"

function init() {
  ./init.sh law
  ./init.sh company
  ./init.sh judge
  ./init.sh enforcer
}

function start() {
  ./ent_start.sh law
  ./ent_start.sh company -m
  ./ent_start.sh judge
  ./ent_start.sh enforcer
}

if [ "$1" == '-i' ]; then
  init
  exit
elif [ "$1" == '-s' ]; then 
  start
  exit
fi


echo "[eth] initializing..."
init

echo "[eth] Starting..."
start

echo "[eth] Connecting peers..."


python3 connect_peers.py company
python3 connect_peers.py judge
python3 connect_peers.py law
python3 connect_peers.py enforcer
