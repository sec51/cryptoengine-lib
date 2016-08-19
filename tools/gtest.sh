#!/bin/sh

WGET_CMD=`which wget`
TAR_CMD=`which tar`

cd third-party

echo "Getting Google testing library..."
$WGET_CMD https://github.com/google/googletest/archive/release-1.7.0.tar.gz
$TAR_CMD xzf release-1.7.0.tar.gz
mv googletest-release-1.7.0 gtest-1.7.0
rm release-1.7.0.tar.gz
echo "Getting Google testing library: DONE"
### ------------------------------------------------------------

