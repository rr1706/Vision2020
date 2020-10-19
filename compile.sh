#!/bin/sh
#param: head(ON/OFF), network(ON/OFF)

echo "check this file before you run it"
sleep 1
sudo rm --dir -r build
mkdir build
sudo pkill vision
cd build
cmake .. -DHEAD=$1 -DNETWORK=$2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j$nproc
#sudo make install
#sudo systemctl enable vision2020
#sudo systemctl start vision2020
echo "finished"