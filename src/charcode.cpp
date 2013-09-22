/*!
 * @brief Shift_JISをutf-8に変換する関数を提供する(Windows用)
 * @author  Koji Ueta
 * @date    2013 09/23
 * @file    charcode.cpp
 * @version 0.1
 */
#include <cstring>
#include <iostream>
#include <windows.h>
#include "charcode.h"


std::string sjisToUtf8(const char *c_str)
{
  // Convert SJIS -> UTF-16
  const int n_size = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)c_str, -1, NULL, 0);
  BYTE* buf_utf16 = new BYTE[n_size * 2 + 2];
  ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)c_str, -1, (LPWSTR)buf_utf16, n_size);

  // Convert UTF-16 -> UTF-8
  const int n_size_utf8 = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)buf_utf16, -1, NULL, 0, NULL, NULL);
  BYTE* buf_utf8 = new BYTE[n_size_utf8 * 2];
  ZeroMemory(buf_utf8, n_size_utf8 * 2);
  ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)buf_utf16, -1, (LPSTR)buf_utf8, n_size_utf8, NULL, NULL);

  // char -> std::string
  std::string utf8_str = (const char *)buf_utf8;
  delete buf_utf16;
  delete buf_utf8;

  return utf8_str;
}


std::string sjisToUtf8(const std::string &str)
{
  return sjisToUtf8(str.c_str());
}
