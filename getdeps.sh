#!/bin/sh

set -e

BUILD_DIR="build"
THIRD_PARTY="third-party"

# check if the build dir already exists
# if it does notify the user but do not delete it. rm -rf it's dangerous !!!!
if [ -d "$BUILD_DIR" ]; then
  echo "The build dir is already present. REMOVE IT and re-run the script."
  exit 1
fi

if [ ! -d "$THIRD_PARTY" ]; then
  mkdir $THIRD_PARTY
fi

# check if git command is present
GIT_CMD=`which git`
if [ -z "$GIT_CMD" ]; then
    echo "git command not found - install git first"
    exit 1
fi

# check if wget command is present
WGET_CMD=`which wget`
if [ -z "$WGET_CMD" ]; then
    echo "wget command not found - install wget first"
    exit 1
fi

# check if tar command is present
TAR_CMD=`which tar`
if [ -z "$TAR_CMD" ]; then
    echo "tar command not found - install tar first"
    exit 1
fi

# check if gzip command is present
GZIP_CMD=`which gzip`
if [ -z "$GZIP_CMD" ]; then
    echo "gzip command not found - install gzip first"
    exit 1
fi

# check if bunzip2 command is present
BUNZIP_CMD=`which bunzip2`
if [ -z "$BUNZIP_CMD" ]; then
    echo "bunzip2 command not found - install bunzip2 first"
    exit 1
fi

cd third-party

# clone google boringssl into the third-party folder
echo "Cloning BoringSSL..."
$GIT_CMD clone https://boringssl.googlesource.com/boringssl
cd boringssl
$GIT_CMD reset --hard 10a1a9d32ebba0e9db7ef3524a77e2a7b1f3b8bd
cd ..
echo "Cloning BoringSSL: DONE"


#echo "Getting NaCl..."
$WGET_CMD http://hyperelliptic.org/nacl/nacl-20110221.tar.bz2
$BUNZIP_CMD < nacl-20110221.tar.bz2 | tar -xf -
rm nacl-20110221.tar.bz2
cd nacl-20110221
echo "building NaCl...it may take a while..."
./do
HOST=`hostname -s`
HOSTNAME=`hostname`
if [ "$HOST" -ne "$HOSTNAME" ]; then
    mv build/$HOST build/$HOSTNAME
fi
cd ..
echo "building NaCl: DONE"
### ------------------------------------------------------------

echo "Getting Google testing library..."
$WGET_CMD https://github.com/google/googletest/archive/release-1.7.0.tar.gz
$TAR_CMD xzf release-1.7.0.tar.gz
mv googletest-release-1.7.0 gtest-1.7.0
cd ..
echo "Getting Google testing library: DONE"
### ------------------------------------------------------------

# create the build directory
mkdir $BUILD_DIR

echo "All dependencies ready"
echo "Now enter the $BUILD_DIR directory"
echo "Execute: cmake -DCMAKE_BUILD_TYPE=Release -DGTEST=ON -DBORINGSSL=ON .."
echo "Execute: make"
echo "Execute: make test"
echo "Execute: make install"

