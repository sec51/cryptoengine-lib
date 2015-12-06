#!/bin/sh

set -e

echo "Installing dependencies..."

if ( test "`uname -s`" = "Darwin" ); then
  echo "Detected OS X"
  #cmake v2.8.12 is installed on the Mac workers now
  #brew update
  #brew install cmake
  brew install llvm35 && brew link --force llvm35
  brew install nacl && brew link --force nacl
else
  echo "Detected LINUX"
  #install a newer cmake since at this time Travis only has version 2.8.7

  sudo apt-get install software-properties-common
  sudo add-apt-repository --yes ppa:george-edison55/cmake-3.x

  sudo apt-get update -qq
  sudo apt-get install -y llvm-3.4 llvm-3.4-dev git wget gzip libssl-dev cmake build-essential libnacl-dev clang-3.5
  #sudo wget --no-check-certificate https://cmake.org/files/v3.4/cmake-3.4.1.tar.gz
  #sudo tar zxf cmake-3.4.1.tar.gz
  #cd cmake-3.4.1
  #sudo mkdir build
  #cd build
  #sudo cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
  #sudo make
  #sudo make install
  #sudo ldconfig
fi

echo "Installing dependencies: DONE"