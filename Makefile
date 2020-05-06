REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: 
	make -C ./src/ -j

docker: Dockerfile clean
	docker build -t $(REGISTRY) . -f Dockerfile

# CPU
benchmark: 
	docker run -v $(PWD):/work $(REGISTRY) run

benchmark-upload: 
	docker run -v $(PWD):/work $(REGISTRY) /bin/bash -c "run; upload"

in: 
	docker run -it -v $(PWD):/work $(REGISTRY) 

#GPU
benchmark-gpu: 
	docker run --gpus all --privileged -v $(PWD):/work $(REGISTRY) run

benchmark-gpu-upload: 
	docker run --gpus all --privileged -v $(PWD):/work $(REGISTRY) /bin/bash -c "run; upload"


in-gpu: 
	docker run --gpus all --privileged -it -v $(PWD):/work $(REGISTRY) 

clean:
	make -C ./src/ clean
