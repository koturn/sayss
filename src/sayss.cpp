/*!
 * @brief クラウド型音声合成を利用したsayコマンドライクなプログラム
 * @author  Koji Ueta
 * @date    2013 09/13
 * @file    sayss.cpp
 * @version 0.2
 */
// C++の標準ライブラリ
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
// インクルードパスに追加する必要のあるライブラリ
#include <clx/http.h>
#include <clx/uri.h>
#include <clx/base64.h>
#include <picojson/picojson.h>


static void sayTextToSpeach(
    const std::string &lang,
    const std::string &message,
    const std::string &filename);
static std::string postToServer(
    const std::string &json_str,
    const std::string &url_host,
    const std::string &url_path,
    int port);
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
  "{\"params\" : ["
  "   \"";
//! 送信するJSON文字列の末尾
static const std::string JSON_FOOTER =
  ","
  "   \"*\","
  "   \"audio/x-wav\""
  " ],"
  " \"method\" : \"speak\""
  "}";


/*!
 * @brief プログラムのエントリポイント
 * @param [in] argc コマンドライン引数の数
 * @param [in] argv コマンドライン引数
 * @return 終了ステータス
 */
int main(int argc, char* argv[])
{
  if (argc < 2 || 4 < argc) {  // 引数の数のチェック
    std::cerr << "Invalid argument" << std::endl
              << "[Usage]" << std::endl
              << "  $" << argv[0] << " [message] {[language] {[Output filename]}}"
              << std::endl;
    return EXIT_FAILURE;
  }
  std::string lang     = DEFAULT_LANG;
  std::string filename = DEFAULT_OUTPUT_FILENAME;
  std::string message  = argv[1];
  if (argc > 2) lang     = argv[2];
  if (argc > 3) filename = argv[3];

  try {
    sayTextToSpeach(lang, message, filename);
  } catch (clx::socket_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (const char *errmsg) {
    std::cerr << errmsg << std::endl;
  }
  return EXIT_SUCCESS;
}


static void sayTextToSpeach(
    const std::string &lang,
    const std::string &message,
    const std::string &filename)
{
  // サーバに投げるJSON文字列の作成
  std::string json_str = JSON_HEADER + lang + "\"," + message + JSON_FOOTER;
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
  picojson::value  v;
  const char      *ret_json_cstr = ret_json.c_str();
  std::string      err;
  picojson::parse(v, ret_json_cstr, ret_json_cstr + strlen(ret_json_cstr), &err);

  picojson::object& o = v.get<picojson::object>();                // JSON全体のオブジェクトを取得
  picojson::object result = o["result"].get<picojson::object>();  // キー:resultに対応するサブのJSONを取得
  std::string      audio  = result["audio"].get<std::string>();   // audio項目を取得
  return clx::base64::decode(audio);  // base64をデコードし、Waveファイルのバイナリを取得(型はstringだが)
}
