#pragma once

#include <string>

#include "Alime/base/base_export.h"
#include "Alime/base/strings/unicode.h"


namespace Alime
{
	namespace base
	{
		BASE_EXPORT std::wstring UTF8ToUTF16(const UTF8Char* utf8, size_t length);
		BASE_EXPORT std::wstring UTF8ToUTF16(const std::string& utf8);

		BASE_EXPORT std::string UTF16ToUTF8(const UTF16Char* utf16, size_t length);
		BASE_EXPORT std::string UTF16ToUTF8(const std::wstring& utf16);

		BASE_EXPORT std::basic_string<UTF32Char> UTF8ToUTF32(const UTF8Char* utf8, size_t length);
		BASE_EXPORT std::basic_string<UTF32Char> UTF8ToUTF32(const std::string& utf8);

		BASE_EXPORT std::string UTF32ToUTF8(const UTF32Char* utf32, size_t length);
		BASE_EXPORT std::string UTF32ToUTF8(const std::basic_string<UTF32Char>& utf32);

		BASE_EXPORT std::basic_string<UTF32Char> UTF16ToUTF32(const UTF16Char* utf16, size_t length);
		BASE_EXPORT std::basic_string<UTF32Char> UTF16ToUTF32(const std::wstring& utf16);

		BASE_EXPORT std::wstring UTF32ToUTF16(const UTF32Char* utf32, size_t length);	
		BASE_EXPORT std::wstring UTF32ToUTF16(const std::basic_string<UTF32Char>& utf32);
	}
}
