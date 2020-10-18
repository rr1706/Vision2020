#!/bin/sh
#param: head(ON/OFF), network(ON/OFF)

#TODO: make this use proper paths

sudo rm --dir -r build
mkdir build
sudo pkill vision
cd build
cmake .. -DHEAD=$1 -DNETWORK=$2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j$nproc
#sudo make install
#sudo rm /usr/local/bin/setCam.sh
#sudo cp ../setCam.sh /usr/local/bin/setCam.sh  
