#!/bin/sh
#param: head(ON/OFF), network(ON/OFF)

echo "check this file before you run it"
sleep 1
rm -rf Build
mkdir Build
pkill vision
cd Build
cmake .. -DHEAD=$1 -DNETWORK=$2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j$nproc
#sudo make install
#sudo systemctl enable vision
#sudo systemctl start vision
echo "finished"