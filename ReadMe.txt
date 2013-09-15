クラウド型音声合成のコマンド
## 使い方
  $ ./sayss こんにちは
  で、カレントディレクトリに『こんにちは』という音声が入ったout.wavという名前の
  Waveファイルが作成される。


## ファイルについて
srcディレクトリにソースコードがあり、各Makefileとコンパイラの対応は以下のように
なっている。
  Makefile : g++用
  clang.mk : clang用(コンパイルできない)
  msvc.mk  : Microsoft Visual C++コンパイラ用


## 依存ライブラリ
1. CLX C++ Libraries
  C++のネットワークプログラミング用ライブラリ(ヘッダのみで構成されている)
    - http://sourceforge.jp/projects/clxcpp/releases/51423
2. PicoJSON
  JSONパーサ(ヘッダのみで構成されている)
    - https://github.com/kazuho/picojson


## ビルドについての注意
1. 文字列エンコード
  実行時に与える日本語の文字列がutf-8でないと、PicoJSONを用いたJSONパースの際
  に、Segmentation faultが出る。そのため、Microsoft Visual C++コンパイラを
  用いてビルドしても、うまく動作しない。

2. 最適化オプション
  g++に最適化オプション(-O2や-O3など)を与えてコンパイルしようとすると、
  リンク時にエラーが発生する。
  clangの場合、最適化オプションを与えなくても、リンクエラーが発生する。

3. インクルードパス
各Makefileに記述してあるように、依存ライブラリへのインクルードパスは、
コンパイラのオプションで通している。Makefileを用いずにコンパイルする場合は、
インクルードパスをコンパイラに与えること。

4. 環境とコンパイラについて
  4-1. ビルドと動作に成功した環境とコンパイラ
    Windows7    : Cygwinのg++
    Ubuntu12.04 : g++
  4-2. ビルドに失敗した環境とコンパイラ
    Windows7    : Cygwinのclang
    Windows7    : MinGWのg++
    Ubuntu12.04 : clang
  4-3. ビルドに成功したが、動作しない環境とコンパイラ
    WindowsのMSVCコンパイラ

  これ以外の環境については、確認していない。


## 今後の課題
1. Windows Cygwinのg++以外でもコンパイルできるようにすること
2. 他環境(OS)での動作を確認すること
3. HTTP通信とJSONパースに関しては、他のライブラリも検討すること
4. 文字列エンコードの扱いを考える
