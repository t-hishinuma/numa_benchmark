REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: 
	make -C ./src/blas1_stream/ -j

docker: Dockerfile
	docker build -t $(REGISTRY) . -f Dockerfile

clean:
	make -C ./src/blas1_stream/ clean
