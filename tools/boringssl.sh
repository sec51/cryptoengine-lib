#!/bin/sh
cd third-party

GIT_CMD=`which git`

# clone google boringssl into the third-party folder
echo "Cloning BoringSSL..."
$GIT_CMD clone https://boringssl.googlesource.com/boringssl
cd boringssl
#$GIT_CMD reset --hard 10a1a9d32ebba0e9db7ef3524a77e2a7b1f3b8bd
mkdir build && cd build
cmake ..
make
echo "Cloning BoringSSL: DONE"

