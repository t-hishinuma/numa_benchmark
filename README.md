# numa_benchmark
[![Docker Automated build](https://img.shields.io/badge/Docker-automated-blue)](https://hub.docker.com/repository/docker/hishinumat/numa_benchmark/general)

NUMerical  linear  Algebra Benchmark (with docker)

* `benchmark_config` を作成してDockerの`/work`にマウントするとベンチマークし，resultにymlとhtmlが出力される．
* `run`コマンドを与えることによってOpenBLASのコンパイルからベンチマークまでが行われる．
* OpenBLASのコンパイルオプションは現状OpenBLASのcmakeの判定任せ (SIMDやコア数は判定するはず)


# How to use 
## Docker (CPU)
### デフォルト設定で良い場合

あらかじめコンテナ内に[設定ファイル](https://github.com/t-hishinuma/numa_benchmark/blob/master/benchmark_config)があるため，実行すると標準出力にyaml形式のベンチマーク結果が出力される．
結果を集計したい場合は `docker_scripts/yml2html.py` に入力yamlファイルと出力のhtmlのファイル名を与える．

```
docker run hishinumat/numa_benchmark run > result.yml
python3 docker_scripts/yml2html.py result.yml result.html
```

### 設定を変更したい場合
設定ファイルを取得して編集する
```
git clone git@github.com:t-hishinuma/numa_benchmark.git
# vim benchmark_config # if need to change
make benchmark
```
`$(PROJECT_DIR)/result`に結果のymlとhtmlとグラフが生成される

## Docker (GPU)
### デフォルト設定で良い場合

`--gpus all` を与えるとGPUのベンチマークが自動的に実行される

```
docker run --gpus all --privileged hishinumat/numa_benchmark run > result.yml
python3 docker_scripts/yml2html.py result.yml result.html
```

### 設定を変更したい場合
CPUと同様．

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

# 結果のresult.ymlを送るとブラウザ経由で結果を見られるサーバを立ててみました．
GCPの無料枠を使ってサーバを立ててみました．
`./upload.sh result.yml` するとuploadされ，
> upload done, your file tag is 1dc1b244
などと8文字のタグがでてきます．ブラウザでアクセスするとその名前のディレクトリが見えるはずなので，そこに行けば結果が見られます．

**現状，ろくなセキュリティもなくチェック機構もなく垂れ流しているだけなので注意して下さい．**
**万が一，誤ったファイルをuploadしてしまった場合にはDM等いただければ削除しますが，情報については何の責任も負えません**


# Acknowledgement
The development of this software was supported by [termoshtt](https://github.com/termoshtt) and [RICOS Co. Ltd.](https://www.ricos.co.jp/).
