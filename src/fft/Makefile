CXX=g++
CPU_CXXFLAGS?=-O3 -fopenmp

# float
F_CPU_LDFLAGS+=-lm -I/usr/local/include/ -L/usr/local/lib/
F_CPU_LDFLAGS+=-lfftw3f -lfftw3f_threads 
F_LIB_PREC=fftwf
F_PREC=	-DFFT_TYPE=$(F_LIB_PREC)_complex \
		-DFFT_PLAN=$(F_LIB_PREC)_plan \
		-DFFT_MALLOC=$(F_LIB_PREC)_malloc \
		-DFFT_PLAN_DFT_1D=$(F_LIB_PREC)_plan_dft_1d \
		-DFFT_PLAN_DFT_2D=$(F_LIB_PREC)_plan_dft_2d \
		-DFFT_PLAN_DFT_3D=$(F_LIB_PREC)_plan_dft_3d \
		-DFFT_EXE=$(F_LIB_PREC)_execute \
		-DFFT_FREE=$(F_LIB_PREC)_free \
		-DFFT_DESTROY=$(F_LIB_PREC)_destroy_plan \
		-DFFT_NTHREAD=$(F_LIB_PREC)_plan_with_nthreads \
		-DPREC=\"float\"

# double
D_CPU_LDFLAGS=-lfftw3 -lfftw3_threads -lm
D_LIB_PREC=fftw
D_PREC=	-DFFT_TYPE=$(D_LIB_PREC)_complex \
		-DFFT_PLAN=$(D_LIB_PREC)_plan \
		-DFFT_MALLOC=$(D_LIB_PREC)_malloc \
		-DFFT_PLAN_DFT_1D=$(D_LIB_PREC)_plan_dft_1d \
		-DFFT_PLAN_DFT_2D=$(D_LIB_PREC)_plan_dft_2d \
		-DFFT_PLAN_DFT_3D=$(D_LIB_PREC)_plan_dft_3d \
		-DFFT_EXE=$(D_LIB_PREC)_execute \
		-DFFT_FREE=$(D_LIB_PREC)_free \
		-DFFT_DESTROY=$(D_LIB_PREC)_destroy_plan \
		-DFFT_NTHREAD=$(D_LIB_PREC)_plan_with_nthreads \
		-DPREC=\"double\"

#GPU
GPU_CXXFLAGS?=-O3 -fopenmp
GPU_LDFLAGS= -lcudart -lcufftw -lm

EXES= fft_cpu fft_gpu

.PHONY: all clean

all: $(EXES)

%_cpu : %_cpu.cpp 
		$(CXX) $(CPU_CXXFLAGS) $< -o $@_f.o $(F_CPU_LDFLAGS) $(F_PREC)
		$(CXX) $(CPU_CXXFLAGS) $< -o $@_d.o $(D_CPU_LDFLAGS) $(D_PREC)

%_gpu : %_gpu.cpp 
		$(CXX) $(GPU_CXXFLAGS) $< -o $@_f.o $(GPU_LDFLAGS) $(F_PREC)
		$(CXX) $(GPU_CXXFLAGS) $< -o $@_d.o $(GPU_LDFLAGS) $(D_PREC)

clean:
		@rm -rf *.o
