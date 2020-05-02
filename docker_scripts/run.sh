#!/bin/bash

#OpenBLAS
/numa_bench/openblas_build.sh

cd /numa_bench/src/; make -j

if [[ -f /work/benchmark_config ]]; then
	cd /numa_bench/
	./test.sh /work/benchmark_config >> /work/result.txt
else
	cd /numa_bench/
	./test.sh /numa_bench/benchmark_config
fi
