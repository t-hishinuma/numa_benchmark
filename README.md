# numa_benchmark
[![Docker Automated build](https://img.shields.io/badge/Docker-automated-blue)](https://hub.docker.com/repository/docker/hishinumat/numa_benchmark/general)
[![result-Web-portal](https://img.shields.io/badge/result-web-green)](http://34.67.228.30/)

## NUMerical  linear  Algebra Benchmark (with docker)

* Dockerを使ってコンパイラやソフトウェアのバージョンなどを統一し，統一された方法・環境でベンチマークを行うことを目的に開発されています．
* ユーザはDockerやgitなどの最低限のツールを導入するだけでベンチマークを取ることが可能です
* 簡易的ですがベンチマーク結果の集計[Webサービス](http://34.67.228.30/)も開発しています (後述しますがセキュリティ設定は適当なので過度な信頼はしないで下さい)．

# Requirements
- git
- make
- docker

# Current support status
## CPU
- ddot, sdot
- dgemv, sgemv
- dgemm, sgemm
- 1D FFT (forward/backward)
- 2D FFT (forward/backward)
- 3D FFT (forward/backward)

## GPU
- ddot, sdot
- dgemv, sgemv
- dgemm, sgemm
- 1D FFT (forward/backward)
- 2D FFT (forward/backward)
- 3D FFT (forward/backward)

# How to use 
* `benchmark_config` を作成してDockerの`/work`にマウントするとベンチマークし，resultにymlとhtmlが出力される．
* `run`コマンドか`run-upload` コマンドを`docker run` に与えることによってOpenBLASやFFTWのコンパイルからベンチマークまでが行われる．
	* OpenBLASのコンパイルオプションは現状OpenBLASのmakeの判定任せ (SIMDやコア数は判定するはず)
	* FFTWはSIMDオプションが自動選択されなかったので判定して入れるようにしています

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

# benchmark\_configの作成方法
大体読めば分かるように作った．．つもり
基本はgithubから落としてきて必要な箇所だけ変更して下さい

# Todo
- benchmark target
  - CURAND support
  - Himeno Benchmark or sparse Library benchmark support
  - LAPACK Support (LU)

- other
  - other compiler support
  - upload server security
  - さすがにHTMLが雑なのできれいに出したい
  - READMEの英語化
  - CIでテスト

やりきれないくらい課題があるので，手伝ってくれる人が居ましたらissueか[Twitter DM](https://twitter.com/Hishinuma_t)ください

# 結果のresult.ymlを送るとブラウザ経由で結果を見られるサーバを立ててみました．
GCPの無料枠を使ってサーバを立ててみました．
`./docker_scripts/upload.sh result.yml` するか，

コンテナでは`upload` か `run-upload` としてパスを通してありますので，
`make benchmark-upload` か `make benchmark-gpu-upload` でもOKです 

```
docker run hishinumat/numa_benchmark run-upload
```

upload.shは実行すると
> upload done, your file tag is 1dc1b244
などと8文字のタグがでてきます．ブラウザでアクセスするとその名前のディレクトリが見えるはずなので，そこに行けば結果が見られます．

**現状，ろくなセキュリティもなくチェック機構もなく垂れ流しているだけなので注意して下さい**
**万が一，誤ったファイルをuploadしてしまった場合にはDM等いただければ削除しますが，情報については何の責任も負えません**
(まぁ送るのはyamlだけなので，乗っ取られて取られるのはアクセスしたIPアドレスとCPU/GPUのスペック情報だけですが)

# Acknowledgement
The development of this software was supported by [termoshtt](https://github.com/termoshtt) and [RICOS Co. Ltd.](https://www.ricos.co.jp/).
