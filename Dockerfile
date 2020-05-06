FROM nvidia/cuda:10.2-base
LABEL maintainer "Toshiaki Hishinuma <hishinuma.toshiaki@gmail.com>"

RUN apt update -y \
	&& apt install -y python3 python3-matplotlib python3-pandas python3-yaml \
	&&  apt install -y make cmake git zsh curl wget\
	&&  apt install -y gcc g++ gfortran \
	&& apt install -y cuda-cublas-dev-10-0 cuda-cudart-dev-10-2 cuda-compiler-10.2 \
	&& apt install -y cuda-cufft-dev-10.2

#&&  apt install -y cuda-nvprof-10-2 cuda-cusolver-dev-10-2 cuda-cusparse-dev-10-2 

RUN apt-get clean \
	&&  rm -rf /var/lib/apt/lists/* 

# Library download
RUN mkdir /numa_bench; cd /numa_bench \
	&& git clone https://github.com/xianyi/OpenBLAS.git /numa_bench/OpenBLAS \
	&& wget http://www.fftw.org/fftw-3.3.8.tar.gz \
	&& tar xzf fftw-3.3.8.tar.gz \
    && rm -rf fftw-3.3.8.tar.gz	\
	&& cd /

# ENV
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/lib/:/usr/local/cuda-10.2/lib64/:/usr/local/cuda-10.0/lib64/
ENV LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/lib/:/usr/local/cuda-10.2/lib64/:/usr/local/cuda-10.0/lib64/
ENV CPLUS_INCLUDE_PATH $CPLUS_INCLUDE_PATH:/usr/local/include/openblas/:/usr/local/cuda-10.2/include/:/usr/local/cuda-10.0/targets/x86_64-linux/include
# source copy
COPY ./docker_scripts/run.sh /usr/local/bin/run
COPY ./docker_scripts/upload.sh /usr/local/bin/upload
COPY ./docker_scripts/* ./test.sh ./benchmark_config /numa_bench/
COPY ./src/ /numa_bench/src/
