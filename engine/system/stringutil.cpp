#include "engine/pch.h"
#include "engine/system/stringutil.h"

#include <Windows.h>

namespace raid {
namespace stringutil {

std::string WideStringToUtf8(const std::wstring& wide_string) 
{
    int buffer_size = ::WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8_string(buffer_size, 0);
    ::WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, &utf8_string[0], buffer_size, nullptr, nullptr);
    return utf8_string;
}

std::wstring Utf8StringToWide(const std::string& utf8_string) 
{
    int buffer_size = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, nullptr, 0);

    std::wstring wide_string(buffer_size, 0);
    ::MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wide_string[0], buffer_size);
    return wide_string;
}

} // namespace stringutil
} // namespace raid

