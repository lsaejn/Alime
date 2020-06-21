#include "Alime/base/third_party/convert_utf/ConvertUTF.h"
#include "string_conversions.h"
#ifdef _WIN32
#include "windows.h"
#endif

#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))

namespace Alime
{
	namespace base
	{

		std::basic_string<UTF16Char> UTF8ToUTF16(const UTF8Char* utf8, size_t length)
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

		std::basic_string<UTF16Char> UTF8ToUTF16(std::string_view utf8)
		{
			return UTF8ToUTF16(utf8.data(), utf8.length());
		}

		std::string UTF16ToUTF8(std::basic_string_view<UTF16Char> utf16)
		{
			return UTF16ToUTF8(utf16.data(), utf16.length());
		}

		std::basic_string<UTF32Char> UTF8ToUTF32(std::string_view utf8)
		{
			return UTF8ToUTF32(utf8.data(), utf8.length());
		}

		std::string UTF32ToUTF8(const std::basic_string_view<UTF32Char> utf32)
		{
			return UTF32ToUTF8(utf32.data(), utf32.length());
		}

		std::basic_string<UTF32Char> UTF16ToUTF32(std::basic_string_view<UTF16Char> utf16)
		{
			return UTF16ToUTF32(utf16.data(), utf16.length());
		}

		std::wstring UTF32ToUTF16(const std::basic_string<UTF32Char>& utf32)
		{
			return UTF32ToUTF16(utf32.c_str(), utf32.length());
		}

		std::basic_string<UTF16Char> UTF32ToUTF16(std::basic_string_view<UTF32Char> utf32)
		{
			return UTF32ToUTF16(utf32.data(), utf32.length());
		}

#if defined(WCHAR_T_IS_UTF16)

		std::string WideToUTF8(const wchar_t* src, size_t src_len)
		{
			return UTF16ToUTF8(src, src_len);
		}

		std::string WideToUTF8(std::wstring_view wide)
		{
			return UTF16ToUTF8(wide.data(), wide.size());
		}

#elif defined(WCHAR_T_IS_UTF32)

		std::string WideToUTF8(const wchar_t* src, size_t src_len)
		{
			return UTF32ToUTF8(src, src_len);
		}

		std::string WideToUTF8(std::wstring_view wide)
		{
			return UTF32ToUTF8(wide.data(), wide.size());
		}

#endif  // defined(WCHAR_T_IS_UTF32)



#ifdef _WIN32
		std::string SysWideToUTF8(const std::wstring& wide)
		{
			return SysWideToMultiByte(wide, CP_UTF8);
		}

		std::wstring SysUTF8ToWide(std::string_view utf8)
		{
			return SysMultiByteToWide(utf8, CP_UTF8);
		}

		std::string SysWideToNativeMB(const std::wstring& wide)
		{
			return SysWideToMultiByte(wide, CP_ACP);
		}

		std::wstring SysNativeMBToWide(std::string_view native_mb)
		{
			return SysMultiByteToWide(native_mb, CP_ACP);
		}

		std::wstring SysMultiByteToWide(std::string_view mb, uint32_t code_page)
		{
			if (mb.empty())
				return std::wstring();

			int mb_length = static_cast<int>(mb.length());
			int charcount = MultiByteToWideChar(code_page, 0,
				mb.data(), mb_length, NULL, 0);
			if (charcount == 0)
				return std::wstring();

			std::wstring wide;
			wide.resize(charcount);
			MultiByteToWideChar(code_page, 0, mb.data(), mb_length, &wide[0], charcount);

			return wide;
		}

		std::string SysWideToMultiByte(const std::wstring& wide, uint32_t code_page)
		{
			int wide_length = static_cast<int>(wide.length());
			if (wide_length == 0)
				return std::string();

			int charcount = WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
				NULL, 0, NULL, NULL);
			if (charcount == 0)
				return std::string();

			std::string mb;
			mb.resize(charcount);
			WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
				&mb[0], charcount, NULL, NULL);

			return mb;
		}

#else

		std::string SysWideToUTF8(const std::wstring& wide)
		{
			return WideToUTF8(wide);
		}

		std::wstring SysUTF8ToWide(std::string_view utf8)
		{
			std::wstring out;
			UTF8ToWide(utf8.data(), utf8.size(), &out);
			return out;
		}

#if defined(SYSTEM_NATIVE_UTF8) || defined(OS_ANDROID)

		std::string SysWideToNativeMB(const std::wstring& wide)
		{
			return WideToUTF8(wide);
		}

		std::wstring SysNativeMBToWide(std::string_view native_mb)
		{
			return SysUTF8ToWide(native_mb);
		}
#else
		std::string SysWideToNativeMB(const std::wstring& wide)
		{
			mbstate_t ps;

			// Calculate the number of multi-byte characters.  We walk through the string
			// without writing the output, counting the number of multi-byte characters.
			size_t num_out_chars = 0;
			memset(&ps, 0, sizeof(ps));
			for (auto src : wide) {
				// Use a temp buffer since calling wcrtomb with an output of NULL does not
				// calculate the output length.
				char buf[16];
				// Skip NULLs to avoid wcrtomb's special handling of them.
				size_t res = src ? wcrtomb(buf, src, &ps) : 0;
				switch (res) {
					// Handle any errors and return an empty string.
					case static_cast<size_t>(-1) :
						return std::string();
						break;
					case 0:
						// We hit an embedded null byte, keep going.
						++num_out_chars;
						break;
					default:
						num_out_chars += res;
						break;
				}
			}

			if (num_out_chars == 0)
				return std::string();

			std::string out;
			out.resize(num_out_chars);

			// We walk the input string again, with |i| tracking the index of the
			// wide input, and |j| tracking the multi-byte output.
			memset(&ps, 0, sizeof(ps));
			for (size_t i = 0, j = 0; i < wide.size(); ++i) {
				const wchar_t src = wide[i];
				// We don't want wcrtomb to do its funkiness for embedded NULLs.
				size_t res = src ? wcrtomb(&out[j], src, &ps) : 0;
				switch (res) {
					// Handle any errors and return an empty string.
					case static_cast<size_t>(-1) :
						return std::string();
						break;
					case 0:
						// We hit an embedded null byte, keep going.
						++j;  // Output is already zeroed.
						break;
					default:
						j += res;
						break;
				}
			}
			return out;
		}

		std::wstring SysNativeMBToWide(std::string_view native_mb)
		{
			std::mbstate_t ps = std::mbstate_t(); // fixed
			//mbstate_t ps;
			// Calculate the number of wide characters.  We walk through the string
			// without writing the output, counting the number of wide characters.
			size_t num_out_chars = 0;
			memset(&ps, 0, sizeof(ps));
			for (size_t i = 0; i < native_mb.size(); ) {
				const char* src = native_mb.data() + i;
				size_t res = mbrtowc(nullptr, src, native_mb.size() - i, &ps);
				switch (res) {
					// Handle any errors and return an empty string.
					case static_cast<size_t>(-2) :
						case static_cast<size_t>(-1) :
						return std::wstring();
						break;
						case 0:
							// We hit an embedded null byte, keep going.
							i += 1;
							FALLTHROUGH;
						default:
							i += res;
							++num_out_chars;
							break;
				}
			}

			if (num_out_chars == 0)
				return std::wstring();

			std::wstring out;
			out.resize(num_out_chars);
			ps = std::mbstate_t();//fixed
			//memset(&ps, 0, sizeof(ps));  // Clear the shift state.
			// We walk the input string again, with |i| tracking the index of the
			// multi-byte input, and |j| tracking the wide output.
			for (size_t i = 0, j = 0; i < native_mb.size(); ++j) {
				const char* src = native_mb.data() + i;
				wchar_t* dst = &out[j];
				size_t res = mbrtowc(dst, src, native_mb.size() - i, &ps);
				switch (res) {
					// Handle any errors and return an empty string.
					case static_cast<size_t>(-2) :
						case static_cast<size_t>(-1) :
						return std::wstring();
						break;
						case 0:
							i += 1;  // Skip null byte.
							break;
						default:
							i += res;
							break;
				}
			}

			return out;
		}

#endif  // defined(SYSTEM_NATIVE_UTF8) || defined(OS_ANDROID)
	
#endif // _WIN32
	}
}