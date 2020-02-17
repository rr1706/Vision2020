#!/bin/bash
sudo pkill vision
cmake .. -DWITH_HEAD=ON -DWITH_NETWORK=ON
make -j4
#sudo make install -j4
