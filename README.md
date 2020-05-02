# numa_benchmark
[![Docker Automated build](https://img.shields.io/badge/Docker-automated-blue)](https://hub.docker.com/repository/docker/hishinumat/numa_benchmark/general)

NUMerical  linear  Algebra Benchmark (with docker)

# How to use 

## Docker

### 設定はデフォルトでいい場合

```
docker run -v hishinumat/numa_benchmark:latest run
```


### 設定ファイルで自分好みの設定にしたい場合
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
