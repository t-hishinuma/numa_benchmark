# numa_benchmark
[![Docker Automated build](https://img.shields.io/badge/Docker-automated-blue)](https://hub.docker.com/repository/docker/hishinumat/numa_benchmark/general)

NUMerical  linear  Algebra Benchmark (with docker)

# How to use 
configをいじって，configのあるディレクトリをDockerにマウントするとconfigのとおりにベンチマークし，resultにymlとhtmlが出力される．

## Docker

```
git clone git@github.com:t-hishinuma/numa_benchmark.git
# vim benchmark_config # if need to change
make benchmark
```
`$(PROJECT_DIR)/result`に結果のymlとhtmlとグラフが生成される

benchmark\_configの中は気合で読んでください

## local (非推奨)
1. install CBLAS
2. `make`
3. `./test.sh ./benchmark_config`

# Requirements
- git
- make
- docker

# Current support status
## CPU
- ddot, sdot

## GPU
none

# Todo
- benchmark target
  - cuBLAS support
  - BLAS関数を増やす
  - LAPACK Support
  - FFT support (fftw, cufft)
  - Himeno Benchmark support
  - CURAND support

- other
  - other compiler support
  - 結果をuploadして共有できる場所と仕組みを用意したい
  - さすがにHTMLが雑なのできれいに出したい
  - READMEの英語化

やりきれないくらい課題があるので，手伝ってくれる人が居ましたらissueか[Twitter DM](https://twitter.com/Hishinuma_t)ください
