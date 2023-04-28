#!/bin/bash

# sudo rpm --import https://packman.inode.at/keys/packman.key
sudo zypper ar -cfp 90 https://ftp.fau.de/packman/suse/openSUSE_Tumbleweed/ packman
sudo zypper addrepo -f https://download.opensuse.org/distribution/leap/15.5/repo/oss/ openSUSE-0ss

# Update
sudo zypper refresh

# Install required packages
sudo zypper install -y -t pattern devel_basis
sudo zypper install -y libcurl4
sudo zypper install -y cmake
sudo zypper install -y libjsoncpp19
sudo zypper install -y boost_1_75_0-gnu-hpc
sudo zypper install -y nlohmann_json-devel

# # Fetch libssl1.1 libary
# wget http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# # Conver debian package to alien
# sudo alien -r libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# # Install libssl
# sudo zypper install libssl1.1_1.1.1f-1ubuntu2_amd64.rpm

# # Remove download
# sudo rm libssl1.1_1.1.1f-1ubuntu2_amd64.deb

# # Conver debian package to alien
# sudo alien -r package/azguestattestation1_1.0.3_amd64.deb

# # Install GuestAttestation Debian Package
sudo rpm --nosignature -i package/azguestattestation1-1.0.5-2.x86_64.rpm
