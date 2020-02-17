#!/bin/bash
#turn head on when testing and network on when actually using
sudo pkill vision
cmake .. -DWITH_HEAD=ON -DWITH_NETWORK=ON
make -j4
sudo make install -j4
