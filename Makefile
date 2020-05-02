REGISTRY  := hishinumat/numa_benchmark
.PHONY: test numa_benchmark

all: numa_benchmark

login:
	docker login 

numa_benchmark: Dockerfile
	docker build -t $(REGISTRY) . -f Dockerfile

push: login numa_benchmark
	docker push $(REGISTRY)

in:  
	docker run -it --gpus all --privileged --mount type=bind,src=$(PWD)/test,dst=/test $(REGISTRY)
	
test:  
	docker run -it --gpus all --privileged --mount type=bind,src=$(PWD)/test,dst=/test $(REGISTRY) \
	/bin/bash -c "cd test; make; make test; make clean"
