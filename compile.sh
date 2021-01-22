#!/bin/sh
#param: head(ON/OFF), network(ON/OFF)

echo "-DHEAD=$1" 
echo "-DNETWORK=$2"

rm -rf build
mkdir build
pkill vision
cd build
cmake .. -DHEAD=$1 -DNETWORK=$2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j$nproc
#sudo make install
#sudo systemctl enable vision
#sudo systemctl start vision
echo "finished"
