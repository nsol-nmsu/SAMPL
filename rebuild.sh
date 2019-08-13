#!/bin/bash

# rebuild all 4 entities

cd Law
./build.sh
cd ..

cd Enforcer
./build.sh
cd ..

cd Judge
./build.sh
cd ..

cd Company
./build.sh
cd ..

