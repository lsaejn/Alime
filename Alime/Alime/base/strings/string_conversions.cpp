
#include "Alime/base/third_party/convert_utf/ConvertUTF.h"

#include "string_conversions.h"

#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))

namespace Alime
{
	namespace base
	{


		std::wstring UTF8ToUTF16(const UTF8Char* utf8, size_t length)
		{
			UTF16Char output[4096];
			const UTF8* src_begin = reinterpret_cast<const UTF8*>(utf8);
			const UTF8* src_end = src_begin + length;
			UTF16* dst_begin = reinterpret_cast<UTF16*>(output);

			std::wstring  utf16;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF8toUTF16(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf16.append(output, dst_begin - reinterpret_cast<UTF16*>(output));
				dst_begin = reinterpret_cast<UTF16*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf16.clear();
					break;
				}
			}

			return utf16;
		}

		std::string UTF16ToUTF8(const UTF16Char* utf16, size_t length)
		{
			UTF8Char output[8192];
			const UTF16* src_begin = reinterpret_cast<const UTF16*>(utf16);
			const UTF16* src_end = src_begin + length;
			UTF8* dst_begin = reinterpret_cast<UTF8*>(output);

			std::string utf8;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF16toUTF8(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf8.append(output, dst_begin - reinterpret_cast<UTF8*>(output));
				dst_begin = reinterpret_cast<UTF8*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf8.clear();
					break;
				}
			}

			return utf8;
		}

		std::basic_string<UTF32Char> UTF8ToUTF32(const UTF8Char* utf8, size_t length)
		{
			UTF32Char output[4096];
			const UTF8* src_begin = reinterpret_cast<const UTF8*>(utf8);
			const UTF8* src_end = src_begin + length;
			UTF32* dst_begin = reinterpret_cast<UTF32*>(output);

			std::basic_string<UTF32Char> utf32;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF8toUTF32(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf32.append(output, dst_begin - reinterpret_cast<UTF32*>(output));
				dst_begin = reinterpret_cast<UTF32*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf32.clear();
					break;
				}
			}

			return utf32;
		}

		std::string UTF32ToUTF8(const UTF32Char* utf32, size_t length)
		{
			UTF8Char output[8192];
			const UTF32* src_begin = reinterpret_cast<const UTF32*>(utf32);
			const UTF32* src_end = src_begin + length;
			UTF8* dst_begin = reinterpret_cast<UTF8*>(output);

			std::string utf8;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF32toUTF8(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf8.append(output, dst_begin - reinterpret_cast<UTF8*>(output));
				dst_begin = reinterpret_cast<UTF8*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf8.clear();
					break;
				}
			}

			return utf8;
		}

		std::basic_string<UTF32Char> UTF16ToUTF32(const UTF16Char* utf16, size_t length)
		{
			UTF32Char output[4096];
			const UTF16* src_begin = reinterpret_cast<const UTF16*>(utf16);
			const UTF16* src_end = src_begin + length;
			UTF32* dst_begin = reinterpret_cast<UTF32*>(output);

			std::basic_string<UTF32Char> utf32;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF16toUTF32(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf32.append(output, dst_begin - reinterpret_cast<UTF32*>(output));
				dst_begin = reinterpret_cast<UTF32*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf32.clear();
					break;
				}
			}

			return utf32;
		}

		std::wstring UTF32ToUTF16(const UTF32Char* utf32, size_t length)
		{
			UTF16Char output[8192];
			const UTF32* src_begin = reinterpret_cast<const UTF32*>(utf32);
			const UTF32* src_end = src_begin + length;
			UTF16* dst_begin = reinterpret_cast<UTF16*>(output);

			std::wstring utf16;
			while (src_begin < src_end)
			{
				ConversionResult result = ConvertUTF32toUTF16(&src_begin,
					src_end,
					&dst_begin,
					dst_begin + COUNT_OF(output),
					lenientConversion);

				utf16.append(output, dst_begin - reinterpret_cast<UTF16*>(output));
				dst_begin = reinterpret_cast<UTF16*>(output);
				if (result == sourceIllegal || result == sourceExhausted)
				{
					utf16.clear();
					break;
				}
			}

			return utf16;
		}

		std::wstring UTF8ToUTF16(const std::string& utf8)
		{
			return UTF8ToUTF16(utf8.c_str(), utf8.length());
		}

		std::string UTF16ToUTF8(const std::wstring& utf16)
		{
			return UTF16ToUTF8(utf16.c_str(), utf16.length());
		}

		std::basic_string<UTF32Char> UTF8ToUTF32(const std::string& utf8)
		{
			return UTF8ToUTF32(utf8.c_str(), utf8.length());
		}

		std::string UTF32ToUTF8(const std::basic_string<UTF32Char>& utf32)
		{
			return UTF32ToUTF8(utf32.c_str(), utf32.length());
		}

		std::basic_string<UTF32Char> UTF16ToUTF32(const std::wstring& utf16)
		{
			return UTF16ToUTF32(utf16.c_str(), utf16.length());
		}

		std::wstring UTF32ToUTF16(const std::basic_string<UTF32Char>& utf32)
		{
			return UTF32ToUTF16(utf32.c_str(), utf32.length());
		}

	}
}