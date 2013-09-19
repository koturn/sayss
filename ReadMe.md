クラウド型音声合成のコマンド
===========================
# 概要
以下のURLに記されている、クラウド型音声合成を利用するCUIツールである  
  - http://komeisugiura.jp/software/software_jp.html  
なお、このプログラムは、私がアルバイトで作成したものであり、公開許可があった上で公開している。




# ビルド方法
UNIX/Linux、あるいはWindowsにおけるCygwinにおいて、添付しているMakefileを利用してビルドすればよい。  
srcディレクトリに移動した上で、以下のようにすること。
```sh
$ make
```
デバッグ用のバイナリを作成する場合は、以下のようにすること。
```sh
$ make DEBUG=true
```
余談だが、最近のg++なら、以下のようにすれば、かなりの最適化がかかるようにしてある。  
(古いg++だと未対応のオプションを用いている)
```sh
$ make OPT=true
```




# 使い方
### 1. 一番単純な使い方
以下の例が、一番単純な利用方法である。  
```sh
$ ./sayss こんにちは
```
これで、カレントディレクトリに『こんにちは』という音声が入ったout.wavという名前の Waveファイルが作成される。


### 2. オプション等
このプログラムのコマンドライン引数として、以下のオプションが使用可能である。  

1. -h, --help
  - プログラムの使い方を表示する。
2. -l [text-language], --language=[text-language]
  - 入力テキストの言語を [text-language] に指定する。
  - (デフォルト値: ja)
3. -o [output-filename], --output=[output-filename]
  - 出力ファイル名を [output-filename] に指定する。
  - (デフォルト値: out.wav)

例えば、英語で "Hello world" という音声を音声合成し、その出力ファイル名をhello.wavとする場合、以下のようにする。
```sh
$ ./sayss "Hello world" --language=en --output=hello.wav
```
もしくは、オプションの短縮形を用いて、以下のようにしてもよい。
```sh
$ ./sayss "Hello world" -l en -o hello.wav
```
なお、"Hello world"の位置は、オプションの引数にならないように注意すれば、どこでもよい。
```sh
OK : $ ./sayss -l en "Hello world" -o hello.wav
OK : $ ./sayss -l en -o hello.wav "Hello world"
NG : $ ./sayss -l en -o "Hello world"
```

なお、試験的に、一度に複数音声の音声合成ができるようにしたが、例外を吐く場合が多いので、現在のところ、以下のようにコマンドライン引数を与えることは推奨しない。
```sh
$ ./sayss おはようございます こんにちは --output=out.wav
```
(この場合、out-1.wav, out-2.wavというファイル名で、それぞれの音声合成の結果のファイルを作るようにしている)




# ファイルについて
srcディレクトリにソースコードがあり、各Makefileとコンパイラの対応は以下のようになっている。

Makefile名 | 対象コンパイラ
-----------|---------------------------------
Makefile   | g++
clang.mk   | clang(ただし、未対応)
msvc.mk    | Microsoft Visual C++コンパイラ用

clang.mkとmsvc.mkは、とりあえず置いているだけなので、用いないこと。  
いずれ、対応したいと考えている。




# 依存ライブラリ
以下のライブラリを、このリポジトリ内に組み込んでいる。

1. CLX C++ Libraries (BSDライセンス)  
  C++のネットワークプログラミング用ライブラリ(ヘッダのみで構成されている)  
    - http://sourceforge.jp/projects/clxcpp/releases/51423
2. PicoJSON (BSDライセンス)  
  JSONパーサ(ヘッダのみで構成されている)  
    - https://github.com/kazuho/picojson
3. getopt(MSVCのみ) (BSDライセンス)  
  コマンドライン引数のオプション処理用のライブラリ  
  UNIX/Linux系では、標準ライブラリだが、MSVCでは非標準ライブラリ  
    - https://svn.xiph.org/trunk/theora-exp/win32/compatibility/




# ビルドについての注意
### 1. 文字列エンコード
実行時に与える日本語の文字列がutf-8でないと、PicoJSONを用いたJSONパースの際に、Segmentation faultが出る。  
そのため、Microsoft Visual C++コンパイラを用いてビルドしても、うまく動作しない。

### 2. インクルードパス
各Makefileに記述してあるように、依存ライブラリへのインクルードパスは、コンパイラのオプションで通している。  
Makefileを用いずにコンパイルする場合は、インクルードパスをコンパイラに与えること。

### 3. 環境とコンパイラについて

1. ビルドと動作に成功した環境とコンパイラ  
  - Windows7    : Cygwinのg++  
  - Ubuntu12.04 : g++  
2. ビルドに失敗した環境とコンパイラ  
  - Windows7    : Cygwinのclang  
  - Windows7    : MinGWのg++  
  - Ubuntu12.04 : clang  
3. ビルドに成功したが、動作しない環境とコンパイラ  
  - WindowsのMSVCコンパイラ(英語のみ動作可能)  

これ以外の環境については、確認していない。
