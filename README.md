# numa_benchmark
[![Docker Automated build](https://img.shields.io/badge/Docker-automated-blue)](https://hub.docker.com/repository/docker/hishinumat/numa_benchmark/general)

NUMerical  linear  Algebra Benchmark (with docker)

* `benchmark_config` を作成してDockerの`/work`にマウントするとベンチマークし，resultにymlとhtmlが出力される．
* `run`コマンドを与えることによってOpenBLASのコンパイルからベンチマークまでが行われる．
* OpenBLASのコンパイルオプションは現状OpenBLASのcmakeの判定任せ (SIMDやコア数は判定するはず)


# How to use 
## Docker (CPU)

```
git clone git@github.com:t-hishinuma/numa_benchmark.git
# vim benchmark_config # if need to change
make benchmark
```
`$(PROJECT_DIR)/result`に結果のymlとhtmlとグラフが生成される

## Docker (GPU)

```
git clone git@github.com:t-hishinuma/numa_benchmark.git
# vim benchmark_config # if need to change
make benchmark-gpu
```
`$(PROJECT_DIR)/result`に結果のymlとhtmlとグラフが生成される

## local (非推奨)
1. install CBLAS
2. `make`
3. `./test.sh ./benchmark_config`

# benchmark\_configの作成方法
あとで．大体読めば分かるように作った．．つもり

# Requirements
- git
- make
- docker

# Current support status
## CPU
- ddot, sdot
- dgemm, sgemm

## GPU
- ddot, sdot
- dgemm, sgemm

# Todo
- benchmark target
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
  - CIでテスト

やりきれないくらい課題があるので，手伝ってくれる人が居ましたらissueか[Twitter DM](https://twitter.com/Hishinuma_t)ください
