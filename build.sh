#!/bin/bash

BASE_DIR=$(dirname $0)

# Remove .build directory
rm -r ${BASE_DIR}/build
if [ $? -eq 0 ]; then
    echo ./build dir deleted
else
    echo FAIL to remove ./build dir.
fi

# Create cmakefile
cmake -G "Unix Makefiles" -B build -DCMAKE_VERBOSE_MAKEFILE=TRUE
if [ $? -eq 0 ]; then
    echo ./build dir deleted
else
    echo FAIL to create CMakefile
    exit 1
fi

# Build
cmake --build build
if [ $? -eq 0 ]; then
    echo ./build dir deleted
else
    echo FAIL to remove ./build dir.
fi



