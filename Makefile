REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: 
	make -C ./src/blas1/ -j

docker: Dockerfile
	docker build -t $(REGISTRY) . -f Dockerfile

clean:
	make -C ./src/blas1/ clean
