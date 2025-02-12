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

# Fetch libssl1.1 libary
wget http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# Install libssl
sudo dpkg -i libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# Remove download
sudo rm libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# Install GuestAttestation Debian Package
sudo dpkg -i package/azguestattestation1_1.0.3_amd64.deb
