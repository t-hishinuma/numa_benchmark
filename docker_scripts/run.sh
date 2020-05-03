#!/bin/bash
RESULT_FILE=/work/result/result.yml

mkdir -p /work/result/

#CPU Spec
echo "# get cpu spec..."
zsh /numa_bench/src/machine_spec/get_cpu_spec.sh > $RESULT_FILE

#OpenBLAS
echo "# openblas build start..."
/numa_bench/openblas_build.sh &> /dev/null


# build benchmark code
echo "# benchmark code build start..."
#cd /numa_bench/src/; make -j &> /dev/null
cd /numa_bench/src/; make -j 


#exec
if [[ -f /work/benchmark_config ]]; then
	cd /numa_bench/; zsh ./test.sh /work/benchmark_config >> $RESULT_FILE

	cd /work/result/
	python3 /numa_bench/yml2html.py ./result.yml ./result.html
else
	cd /numa_bench/; zsh ./test.sh /numa_bench/benchmark_config
fi
