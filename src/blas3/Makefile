CXX=g++
CXXFLAGS?=-O3 -fopenmp
LDFLAGS=-lopenblas

EXES= gemm

.PHONY: all clean

all: $(EXES)

% : %.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@.o $(LDFLAGS)

clean:
	@rm -rf $(EXES).o
