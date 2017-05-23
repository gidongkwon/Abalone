#pragma once
#include <string>
#include <codecvt>

// Convert std::string to std::wstring.
// Use this for Windows API function parameters.
// wstring_convert and codecvt are deprecated in C++17,
// but we use it for simple code.
std::wstring widen(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes(str);
}