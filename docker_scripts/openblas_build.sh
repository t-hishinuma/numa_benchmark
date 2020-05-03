#!/bin/bash
. $1

if [[ $OPENBLAS_BUILD = 1 ]]; then
	cd /numa_bench/OpenBLAS/; rm -rf build; mkdir build; cd build; cmake .. $OPENBLAS_CMAKE_OPTION ; make -j ; make install 
else
	apt update -y
	apt install -y libopenblas-dev
fi
