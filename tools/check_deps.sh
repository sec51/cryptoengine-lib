#!/bin/sh

#!/bin/sh

set -e

BUILD_DIR="build"
THIRD_PARTY="third-party"

# check if the build dir already exists
# if it does notify the user but do not delete it. rm -rf is dangerous !!!!
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

