#pragma once

#include <string>

#include "Alime/base/base_export.h"
#include "Alime/base/strings/unicode.h"


namespace Alime
{
	namespace base
	{	
		std::string	UTF16ToUTF8(std::basic_string_view<UTF16Char> utf16);
		std::string	UTF32ToUTF8(std::basic_string_view<UTF32Char> utf32);
		std::basic_string<UTF16Char>	UTF8ToUTF16(std::string_view utf8);
		std::basic_string<UTF32Char>	UTF8ToUTF32(std::string_view utf8);
		std::basic_string<UTF32Char>	UTF16ToUTF32(std::basic_string_view<UTF16Char> utf16);
		std::basic_string<UTF16Char>   UTF32ToUTF16(std::basic_string_view<UTF32Char> utf32);

		std::string		SysWideToUTF8(const std::wstring& wide);
		std::wstring	SysUTF8ToWide(std::string_view utf8);

		std::string		SysWideToNativeMB(const std::wstring& wide);
		std::wstring	SysNativeMBToWide(std::string_view native_mb);

		//windows¸¨Öúº¯Êý
#ifdef _WIN32
		std::wstring	SysMultiByteToWide(std::string_view mb, uint32_t code_page);
		std::string		SysWideToMultiByte(const std::wstring& wide, uint32_t code_page);
#endif // _WIN32

	}
}
