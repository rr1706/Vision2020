#!/bin/sh
#param: head(on/off), network(on/off), install(0/1)

sudo rm --dir -r build
mkdir build
sudo pkill vision
cd build
cmake -DHEAD=$1 -DNETWORK=$2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j$nproc
if [ $3 > 0 ]; then;
    sudo make install
    sudo rm /usr/local/bin/setCam.sh
    sudo cp ../setCam.sh /usr/local/bin/setCam.sh  
fi