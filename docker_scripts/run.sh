#!/bin/bash
RESULT_FILE=/work/result/result.yml

if [[ -f /work/benchmark_config ]]; then
	CONFIG="/work/benchmark_config"
else
	CONFIG="/numa_bench/benchmark_config"
fi

. $CONFIG

$INITIAL_COMMANDS

mkdir -p /work/result/

#CPU Spec
echo "# get cpu spec..."
zsh /numa_bench/src/machine_spec/get_cpu_spec.sh > $RESULT_FILE
zsh /numa_bench/src/machine_spec/get_cpu_spec.sh > $RESULT_FILE

#OpenBLAS
echo "# openblas build start..."
/numa_bench/openblas_build.sh $CONFIG &> /work/result/cblas.log


# build benchmark code
echo "# benchmark code build start..."
cd /numa_bench/src/; make -j &> /dev/null


echo "# benchmark start..."

#exec
if [[ -f /work/benchmark_config ]]; then
	cd /numa_bench/; zsh ./test.sh >> $RESULT_FILE

	cd /work/result/
	python3 /numa_bench/yml2html.py ./result.yml ./result.html
else
	cd /numa_bench/; zsh ./test.sh
fi

echo "# finish"
