#!/bin/bash

if [ ! -d tests-debug ]; then
	mkdir tests-debug
	cd tests-debug
	cmake -DCMAKE_BUILD_TYPE=Debug ../tests
	cd ..
fi

cd tests-debug
make -j4
for i in 0 3; do
	for f in $(ls depth_0__*); do
		echo $f
		./$f
	done
done
