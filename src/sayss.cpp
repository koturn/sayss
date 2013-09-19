/*!
 * @brief クラウド型音声合成を利用したsayコマンドライクなプログラム
 * @author  Koji Ueta
 * @date    2013 09/13
 * @file    sayss.cpp
 * @version 0.3
 */
// C++の標準ライブラリ
#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <sstream>
// インクルードパスに追加する必要のあるライブラリ
#include <clx/http.h>
#include <clx/uri.h>
#include <clx/base64.h>
#include <picojson/picojson.h>

// attributeの指定が使えない処理系では、attributeを消す
#ifndef __GNUC__
#  define __attribute__(attr)
#endif


static int parseArguments(
    int          argc,
    char        *argv[],
    std::string &lang,
    std::string &filename);

__attribute__((noreturn))
static inline void showUsage(const char *progname, int exit_status);

static inline std::string removeSuffix(const std::string &filename);

static void sayTextToSpeach(
    const std::string &lang,
    const std::string &message,
    const std::string &filename);

static std::string postToServer(
    const std::string &json_str,
    const std::string &url_host,
    const std::string &url_path,
    int                port);

static std::string retJsonToWave(const std::string &ret_json);


//! 接続先ポート番号
static const int HTTP_PORT = 80;
//! サーバのホスト部分
static const std::string URL_HOST = "rospeex.ucri.jgn-x.jp";
//! サーバのパス部分
static const std::string URL_PATH = "/nauth_json/jsServices/VoiceTraSS";
//! デフォルトの入力言語
static const std::string DEFAULT_LANG = "ja";
//! デフォルトの出力ファイル名
static const std::string DEFAULT_OUTPUT_FILENAME = "out.wav";
//! 送信するJSON文字列の先頭部分
static const std::string JSON_HEADER =
"{"
  "\"params\" : ["
    "\"";
//! 送信するJSON文字列の末尾
static const std::string JSON_FOOTER =
    "\","
    "\"*\","
    "\"audio/x-wav\""
  "],"
  "\"method\" : \"speak\""
"}";


/*!
 * @brief プログラムのエントリポイント
 * @param [in] argc コマンドライン引数の数
 * @param [in] argv コマンドライン引数
 * @return 終了ステータス
 */
int main(int argc, char* argv[])
{
  std::string lang     = DEFAULT_LANG;
  std::string filename = DEFAULT_OUTPUT_FILENAME;
  int remidx = parseArguments(argc, argv, lang, filename);
  if (argc == remidx) {  // 引数のチェック
    std::cerr << "Invalid argument" << std::endl;
    showUsage(argv[0], EXIT_FAILURE);
  }

  try {
    if (argc == optind + 1) {  // 発話語句が1つなら
      std::string message = argv[remidx];
      sayTextToSpeach(lang, message, filename);
    } else {  // TODO: unstable
      std::string base_filename = removeSuffix(filename);
      std::stringstream ss;
      for (int i = remidx, file_idx = 1; i < argc; i++, file_idx++) {
        std::string message = argv[i];
        ss << file_idx;
        std::string i_filename = base_filename + "-" + ss.str() + ".wav";

        ss.str("");
        ss.clear(std::stringstream::goodbit);

        sayTextToSpeach(lang, message, i_filename);
      }
    }
  } catch (clx::socket_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (const char *errmsg) {
    std::cerr << errmsg << std::endl;
  }
  return EXIT_SUCCESS;
}


/*!
 * @brief コマンドライン引数の解析を行う
 *
 * 引数argvのうち、オプションに関係しないものは、後ろに並び替えられる。
 * その開始インデックスを返り値とする
 * @param [in]     argc      コマンドライン引数の数
 * @param [in,out] argv      コマンドライン引数
 * @param [out]    lang      音声合成の言語
 * @param [out]    filename  出力ファイル名
 * @return  残った引数のインデックス
 */
static int parseArguments(
    int          argc,
    char        *argv[],
    std::string &lang,
    std::string &filename)
{
  static const struct option opts[] = {
    {"help",     no_argument,       NULL, 'h'},
    {"language", required_argument, NULL, 'l'},
    {"output",   required_argument, NULL, 'o'},
    {0, 0, 0, 0}   // must be filled with zero
  };

  int ret;
  int optidx;
  while ((ret = getopt_long(argc, argv, "hl:o:", opts, &optidx)) != -1) {
    switch (ret) {
      case 'h':  // -h or --help
        showUsage(argv[0], EXIT_SUCCESS);
        break;
      case 'l':  // -l or --language
        lang = optarg;
        break;
      case 'o':  // -o or --output
        filename = optarg;
        break;
    }
  }
  return optind;
}


/*!
 * @brief 使い方を表示し、プログラムを終了する
 * @attention この関数は呼び出し元に戻らないので、g++/clangではnoreturn属性を付加した
 * @param [in] progname     プログラム名
 * @param [in] exit_status  終了ステータス
 */
static inline void showUsage(const char *progname, int exit_status)
{
  std::cout << "[Usage]" << std::endl
            << "  $ " << progname << " [text] {options}" << std::endl
            << std::endl
            << "[options]" << std::endl
            << "  -h, --help" << std::endl
            << "    Show usage of this program" << std::endl
            << "  -l [text-language], --language [text-language]" << std::endl
            << "    DEFAULT VALUE: ja" << std::endl
            << "    Specify language of text" << std::endl
            << "  -o [output-filename], --output [output-filename]" << std::endl
            << "    DEFAULT VALUE: out.wav" << std::endl
            << "    Specify filename of wave file" << std::endl;
  exit(exit_status);
}


/*!
 * @brief ファイル名の拡張子を覗いた部分を返す
 * @param [in] filename  ファイル名
 * @return  ファイル名の拡張子を覗いた部分
 */
static inline std::string removeSuffix(const std::string &filename)
{
  int idx = filename.find_last_of(".");
  return filename.substr(0, idx);
}


/*!
 * @brief 音声合成を行う
 * @param [in] lang      messageの言語
 * @param [in] argv      音声合成するテキスト
 * @param [in] filename  出力ファイル名
 */
static void sayTextToSpeach(
    const std::string &lang,
    const std::string &message,
    const std::string &filename)
{
  // サーバに投げるJSON文字列の作成
  std::string json_str = JSON_HEADER + lang + "\", \"" + message + JSON_FOOTER;
  std::cout << json_str << std::endl;
  // 例外はmain側に投げる
  std::string body_str = postToServer(json_str, URL_HOST, URL_PATH, HTTP_PORT);

  std::ofstream ofs(filename.c_str(), std::ofstream::binary);
  if (!ofs) {
    throw "Cannot open " + filename;
  }
  ofs << retJsonToWave(body_str);
  ofs.close();
}



/*!
 * @brief サーバと接続し、JSONデータを投げる
 * @param [in] json_str  サーバに投げるJSON文字列
 * @param [in] url_host  サーバのURLのホスト部
 * @param [in] url_path  サーバのURLのパス部
 * @param [in] port      接続先ポート番号
 * @return  レスポンス本体(JSON文字列を想定)
 */
static std::string postToServer(
    const std::string &json_str,
    const std::string &url_host,
    const std::string &url_path,
    int port)
{
  clx::http session(clx::uri::encode(url_host), port);
  session.post(clx::uri::encode(url_path), json_str);  // JSONをサーバに投げる
  return session.body();                               // 受信したデータの本体を取得
}


/*!
 * @brief サーバから受信したJSON文字列からWaveデータを取り出す
 * @param [in] ret_json  サーバから受信したJSON文字列
 * @return  Waveファイルのバイナリ(std::string型であることに注意)
 */
static std::string retJsonToWave(const std::string &ret_json)
{
  picojson::value  pico_value;
  const char      *ret_json_cstr = ret_json.c_str();
  std::string      err;
  picojson::parse(pico_value, ret_json_cstr, ret_json_cstr + strlen(ret_json_cstr), &err);

  picojson::object &pico_obj = pico_value.get<picojson::object>();          // JSON全体のオブジェクトを取得
  picojson::object  result   = pico_obj["result"].get<picojson::object>();  // キー:resultに対応するサブのJSONを取得
  std::string       audio    = result["audio"].get<std::string>();          // audio項目を取得
  return clx::base64::decode(audio);  // base64をデコードし、Waveファイルのバイナリを取得(型はstringだが)
}
