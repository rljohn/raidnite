#pragma once

#include <string>

namespace raid {
namespace stringutil {

std::string WideStringToUtf8(const std::wstring& wide_string);
std::wstring Utf8StringToWide(const std::string& utf8_string);

} // namespace raid
} // namespace raid
