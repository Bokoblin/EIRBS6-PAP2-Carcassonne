#!/bin/bash
#Building documentation
make docs

#Executing cmake in a build repository
mkdir -p build
cd build
cmake ..
make
cd ..

#Copying the executables where "make build" and "make install" build it
cp build/*.so build/server install/
cp build/tst/test_* ./
