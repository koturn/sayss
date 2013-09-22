/*!
 * @brief Shift_JISをutf-8に変換する関数を提供する(Windows用)
 * @author  Koji Ueta
 * @date    2013 09/23
 * @file    charcode.h
 * @version 0.1
 */
#ifndef CHARCODE_H
#define CHARCODE_H


#include <string>

std::string sjisToUtf8(const char *c_str);
std::string sjisToUtf8(const std::string &str);


#endif
