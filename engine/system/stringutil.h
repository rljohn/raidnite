#pragma once

#include <string>

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

} // namespace raid
} // namespace raid
