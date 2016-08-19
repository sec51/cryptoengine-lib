#!/bin/sh
cd third-party

WGET_CMD=`which wget`
BUNZIP_CMD=`which bunzip2`

#echo "Getting NaCl..."
$WGET_CMD http://hyperelliptic.org/nacl/nacl-20110221.tar.bz2
$BUNZIP_CMD < nacl-20110221.tar.bz2 | tar -xf -
rm nacl-20110221.tar.bz2
cd nacl-20110221
echo "building NaCl...it may take a while..."
./do


HOST=`hostname -s`
SANITIZED_HOST=`echo $HOST | tr -d -`
HOSTNAME=`hostname`
echo $SANITIZED_HOST

if [ "$SANITIZED_HOST" != "$HOSTNAME" ]; then
    mv build/$SANITIZED_HOST build/$HOSTNAME
fi



echo "building NaCl: DONE"
### ------------------------------------------------------------

