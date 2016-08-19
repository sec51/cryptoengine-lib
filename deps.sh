#!/bin/sh -e

BUILD_DIR="build"

/bin/sh tools/check_deps.sh
/bin/sh tools/boringssl.sh
/bin/sh tools/nacl.sh
/bin/sh tools/gtest.sh

# create the build directory
mkdir $BUILD_DIR

echo "All dependencies ready"
echo "Now enter the $BUILD_DIR directory"
echo "Execute: cmake -DCMAKE_BUILD_TYPE=Release .."
echo "Execute: make"
echo "Execute: make test"
echo "Execute: make install"

