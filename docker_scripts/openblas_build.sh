#!/bin/bash
. $1

if [[ $OPENBLAS_BUILD = 1 ]]; then

	printf "-"
   	printf " {"
	printf "\"type\" : \"buildlog\", "
	printf "\"lib\" : \"openblas\", "
	printf "\"how\" : \"source\", "
	printf "\"version\" : \"$OPENBLAS_VERSION\", "
	printf "\"option\" : \"$OPENBLAS_CMAKE_OPTION\""
	printf "}\n"
	
 	cd /numa_bench/
	git clone $OPENBLAS_SRC -b $OPENBLAS_VERSION ./OpenBLAS &> /dev/null
	cd OpenBLAS
	make -j &> /work/result/cblas_build.log
	make install &> /work/result/cblas_build.log
	cd / &> /dev/null
else

	printf "-"
   	printf " {"
	printf "\"type\" : \"buildlog\", "
	printf "\"lib\" : \"openblas\", "
	printf "\"how\" : \"apt\", "
	printf "\"option\" : \"none\""
	printf "}\n"

	apt update -y &>> /work/result/cblas_build.log
	apt install -y libopenblas-dev &>> /work/result/cblas_build.log 

fi
