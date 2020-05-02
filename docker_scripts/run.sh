#!/bin/bash

#OpenBLAS
#cd /work/OpenBLAS; mkdir build; cd build; cmake .. ; make -j ; make install 
apt update -y
apt install -y libopenblas-dev

cd /numa_bench/src/; make -j


if [[ -f /work/benchmark_config ]]; then
	cd /numa_bench/
	./test.sh /work/benchmark_config >> /work/result.txt
else
	cd /numa_bench/
	./test.sh /numa_bench/benchmark_config
fi
