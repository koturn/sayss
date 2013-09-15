クラウド型音声合成のコマンド
## 使い方
  1. 一番単純な使い方
    以下の例が、一番単純な利用方法である。
      $ ./sayss こんにちは
    これで、カレントディレクトリに『こんにちは』という音声が入ったout.wavという
    名前の Waveファイルが作成される。

  2. オプション等
    このプログラムのコマンドライン引数として、以下のオプションが使用可能
    である。
      -h, --help
        プログラムの使い方を表示する。
      -l [text-language], --language [text-language]
        入力テキストの言語を [text-language] に指定する。
        (デフォルト値: ja)
      -o [output-filename], --output [output-filename]
        出力ファイル名を [output-filename] に指定する。
        (デフォルト値: out.wav)
    例えば、英語で "Hello world" という音声を音声合成し、その出力ファイル名を
    hello.wavとする場合、以下のようにする。
      $ ./sayss "Hello world" --language en --output hello.wav
    もしくは、オプションの短縮形を用いて、以下のようにしてもよい。
      $ ./sayss "Hello world" -l en -o hello.wav
    なお、"Hello world"の位置は、オプションの引数にならないように注意すれば、
    どこでもよい。
      OK : $ ./sayss -l en "Hello world" -o hello.wav
      OK : $ ./sayss -l en -o hello.wav "Hello world"
      NG : $ ./sayss -l en -o "Hello world"

    なお、試験的に、一度に複数音声の音声合成ができるようにしたが、例外を吐く
    場合が多いので、現在のところ、以下のようにコマンドライン引数を与えることは
    推奨しない。
      $ ./sayss おはようございます こんにちは --output out.wav
    (この場合、out-1.wav, out-2.wavというファイル名で、それぞれの音声合成の結果
    のファイルを作るようにしている)


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
3. getopt(MSVCのみ)
  コマンドライン引数のオプション処理用のライブラリ
  UNIX/Linux系では、標準ライブラリだが、MSVCでは非標準ライブラリ
    - https://svn.xiph.org/trunk/theora-exp/win32/compatibility/


## ビルドについての注意
1. 文字列エンコード
  実行時に与える日本語の文字列がutf-8でないと、PicoJSONを用いたJSONパースの際
  に、Segmentation faultが出る。そのため、Microsoft Visual C++コンパイラを
  用いてビルドしても、うまく動作しない。

2. インクルードパス
各Makefileに記述してあるように、依存ライブラリへのインクルードパスは、
コンパイラのオプションで通している。Makefileを用いずにコンパイルする場合は、
インクルードパスをコンパイラに与えること。

3. 環境とコンパイラについて
  3-1. ビルドと動作に成功した環境とコンパイラ
    Windows7    : Cygwinのg++
    Ubuntu12.04 : g++
  3-2. ビルドに失敗した環境とコンパイラ
    Windows7    : Cygwinのclang
    Windows7    : MinGWのg++
    Ubuntu12.04 : clang
  3-3. ビルドに成功したが、動作しない環境とコンパイラ
    WindowsのMSVCコンパイラ(英語のみ動作可能)

  これ以外の環境については、確認していない。


## 今後の課題
1. clangでもコンパイルできるようにすること
2. MSVCでも日本語の音声合成ができるようにすること
3. 他環境(OS)での動作を確認すること
4. HTTP通信とJSONパースに関しては、他のライブラリも検討すること
5. 文字列エンコードの扱いを考える
