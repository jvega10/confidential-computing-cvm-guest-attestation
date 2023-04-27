#!/bin/bash

# Update
sudo apt-get update

# Install required packages
sudo apt-get install build-essential -y
sudo apt-get install libcurl4-openssl-dev -y
sudo apt-get install cmake -y
sudo apt-get install libjsoncpp-dev -y
sudo apt-get install libboost-all-dev -y
sudo apt install nlohmann-json3-dev

# Install GuestAttestation Debian Package
sudo dpkg -i package/azguestattestation1_1.0.5_amd64.deb
