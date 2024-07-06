#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <cwctype>

namespace raid {
namespace stringutil {

std::string WideStringToUtf8(const std::wstring& wide_string);
std::wstring Utf8StringToWide(const std::string& utf8_string);

inline bool NullOrEmpty(const char* str)
{
	return (str == nullptr) || (str[0] == '\0');
}

inline void StringSetEmpty(char* str)
{
	str[0] = '\0';
}

static const char* LogBool(bool b) { return b ? "True" : "False"; }
static const char* LogResult(bool b) { return b ? "Success" : "Fail"; }

} // namespace stringutil
} // namespace raid
