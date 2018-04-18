#!/bin/bash
make docs
mkdir -p build
cd build
cmake ..
make
cd ..
mkdir -p bin
mkdir -p bin/tst
mkdir -p bin/clients
cp build/*.so bin/clients
cp build/server bin/
cp build/tst/test_* bin/tst
echo './server clients/*.so' > bin/run.sh
echo built run.sh
