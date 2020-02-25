#!/bin/bash
#turn head on when testing and network on when actually using
sudo systemctl stop vision
sudo pkill vision
cmake .. -DHEAD=ON -DNETWORK=OFF
make -j4
sudo make install
sudo cp ../setCam.sh /usr/local/bin/setCam.sh
