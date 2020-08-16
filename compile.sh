#!/bin/bash
#param: head(on/off), network(on/off), threads to build with(#), 

sudo rm --dir -r build
mkdir build
sudo pkill vision
cmake -B build/ -DHEAD=$1 -DNETWORK=$2
make -j$3
sudo make install
sudo rm /usr/local/bin/setCam.sh
sudo cp setCam.sh /usr/local/bin/setCam.sh