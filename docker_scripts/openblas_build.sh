#!/bin/bash
. $1

if [[ $OPENBLAS_BUILD = 1 ]]; then

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"openblas_build\" : \"source\", "
	printf "\"openblas_option\" : \"$OPENBLAS_CMAKE_OPTION\""
	printf "}\n"

	cd /numa_bench/OpenBLAS/; rm -rf build; mkdir build; cd build;\
	cmake .. $OPENBLAS_CMAKE_OPTION &>> /work/result/cblas_build.log
	make -j &>> /work/result/cblas_build.log
	make install &>> /work/result/cblas_build.log
else

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"openblas_build\" : \"apt\""
	printf "}\n"

	apt update -y &>> /work/result/cblas_build.log
	apt install -y libopenblas-dev &>> /work/result/cblas_build.log 

fi
