#!/bin/bash
sudo systemctl stop vision
sudo systemctl disable vision
sudo systemctl enable vision
sudo systemctl start vision
