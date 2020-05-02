REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: 
	make -C ./src/ -j

docker: Dockerfile clean
	docker build -t $(REGISTRY) . -f Dockerfile

clean:
	make -C ./src/ clean
