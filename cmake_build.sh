#!/bin/bash
#Building documentation
make docs

#Executing cmake in a build repository
mkdir -p build
cd build
cmake ..
make
cd ..

#Copying the executables in a bin repository
mkdir -p bin
mkdir -p bin/tst
mkdir -p bin/clients
cp build/*.so bin/clients
cp build/server bin/
cp build/tst/test_* bin/tst

#Copying the executables where "make build" and 'make install" build it
cp build/*.so build/server install/
cp build/tst/test_* ./

#Creating a script to run the game from "bin" repository
echo './server clients/*.so' > bin/run.sh
echo built run.sh
