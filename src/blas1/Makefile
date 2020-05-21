CXX=g++

CPU_CXXFLAGS?=-O3 -fopenmp -std=c++17
CPU_LDFLAGS=-lopenblas

GPU_CXXFLAGS?=-O3 -fopenmp -srd=c++17
GPU_LDFLAGS= -lcublas -lcudart
#GPU_LDFLAGS+= -I/usr/local/cuda-10.0/include -L/usr/local/cuda-10.0/lib64

EXES= dot_cpu dot_gpu

.PHONY: all clean

all: $(EXES)

%_cpu : %_cpu.cpp 
		$(CXX) $(CPU_CXXFLAGS) $< -o $@.o $(CPU_LDFLAGS)

%_gpu : %_gpu.cpp 
		$(CXX) $(GPU_CXXFLAGS) $< -o $@.o $(GPU_LDFLAGS)

clean:
		@rm -rf $(EXES).o
