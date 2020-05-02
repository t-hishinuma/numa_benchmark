# numa_benchmark
NUMerical  linear  Algebra Benchmark (with docker)

# How to use 

## Docker

```
wget https://raw.githubusercontent.com/t-hishinuma/numa_benchmark/master/benchmark_config
```

change benchmark\_config

```
docker run -v $PWD:/work/ hishinumat/numa_benchmark:latest run
```

## local
1. install CBLAS
2. `make`
3. `./test.sh ./benchmark_config`

# Current support status
## CPU
- ddot, sdot

## GPU
none
