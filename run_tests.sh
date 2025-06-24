#!/bin/bash

if [ ! -d tests-debug ]; then
	mkdir tests-debug
	cd tests-debug
	cmake -DCMAKE_BUILD_TYPE=Debug ../tests
	cd ..
fi

cd build-debug/tests
ctest .
cd ../..
