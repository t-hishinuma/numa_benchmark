REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: 
	make -C ./src/ -j

docker: Dockerfile clean
	docker build -t $(REGISTRY) . -f Dockerfile

benchmark: 
	docker run -v $(PWD):/work $(REGISTRY) run

in: 
	docker run -it -v $(PWD):/work $(REGISTRY) 

benchmark-gpu: 
	docker run --gpus all --privileged -v $(PWD):/work $(REGISTRY) run

in-gpu: 
	docker run --gpus all --privileged -it -v $(PWD):/work $(REGISTRY) 

clean:
	make -C ./src/ clean
