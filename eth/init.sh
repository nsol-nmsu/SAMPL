#!/bin/bash

# init an entity by starting it with the genesis block

# pass in what entity to ini
# example:
#    ./init.sh judge

if [ $# == 0 ]; then
  echo "usage:"
  echo "  ./init.sh {entity}"
  exit
fi


geth --datadir $1 init genesis.json
