#!/bin/bash

if [ ! -d tests-debug ]; then
	mkdir tests-debug
	cd tests-debug
	cmake ../tests
	cd ..
fi

cd tests-debug
make -j4
./depth_0__eq
./depth_0__lt
./depth_3__eq
./depth_3__lt
