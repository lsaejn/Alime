#include "string_util.h"
#include <stdio.h>			/* for vsnprintf */
#include <assert.h>			/* for assert */
#include <memory.h>			/* for mem*** */
#include <stdarg.h>			/* va_list, va_start, va_end */
#include <ctype.h>

#include "Alime/base/macros.h"
#include "Alime/base/third_party/convert_utf/ConvertUTF.h"
#include "Alime/base/strings/string_util.h"
#include "Alime/base/details/string_tokenizer.h"
#include "Alime/base/details/string_constants.h"

namespace Alime::base
{
	namespace details
	{
		template<typename CharType>
		int CharCompareNoCase(CharType l, CharType r)
		{
			l = (l < 'A' || l > 'Z') ? l : l - 'A' + 'a';
			r = (r < 'A' || r > 'Z') ? r : r - 'A' + 'a';
			if (l == r) return 0;
			return l > r ? 1 : -1;
		}

		template<typename CharType>
		int StringCompareNoCase(std::basic_string_view<CharType> strA, std::basic_string_view<CharType> strB)
		{
			size_t strA_size = strA.size();
			size_t strB_size = strB.size();
			size_t minSize = strA_size < strB_size ? strA_size : strB_size;
			int ans = 0;
			for (size_t i = 0; i != minSize; ++i)
			{
				ans = CharCompareNoCase(strA.at(i), strB.at(i));
				if (ans != 0)
					return ans;
			}
			if (strA_size < strB_size)
				return -1;
			else if (strA_size > strB_size)
				return 1;
			else
				return 0;
		}
	}

	namespace
	{
		template<typename CharType>
		int StringTokenizeT(const std::basic_string<CharType>& input,
			const std::basic_string<CharType>& delimitor,
			std::list<std::basic_string<CharType> >& output)
		{
			size_t token_begin;
			size_t token_end;

			output.clear();

			for (token_begin = token_end = 0; token_end != std::basic_string<CharType>::npos;)
			{
				token_begin = input.find_first_not_of(delimitor, token_begin);
				if (token_begin == std::basic_string<CharType>::npos)
					break;
				token_end = input.find_first_of(delimitor, token_begin + 1);
				output.push_back(input.substr(token_begin, token_end - token_begin));
				token_begin = token_end + 1;
			}

			return static_cast<int>(output.size());
		}

		template<typename CharType>
		size_t StringReplaceAllT(const std::basic_string_view<CharType>& find,
			const std::basic_string_view<CharType>& replace,
			std::basic_string<CharType>& output)
		{
			size_t find_length = find.size();
			size_t replace_length = replace.size();
			size_t offset = 0, endpos;
			size_t target = 0, found_pos;
			size_t replaced = 0;
			CharType* data_ptr;

			if (find.empty() || output.empty())
				return 0;

			/*
				* to avoid extra memory reallocating,
				* we use two passes to finish the task in the case that replace.size() is greater find.size()
				*/

			if (find_length < replace_length)
			{
				/* the first pass, count all available 'find' to be replaced  */
				for (;;)
				{
					offset = output.find(find, offset);
					if (offset == std::basic_string<CharType>::npos)
						break;
					replaced++;
					offset += find_length;
				}

				if (replaced == 0)
					return 0;

				size_t newsize = output.size() + replaced * (replace_length - find_length);

				/* we apply for more memory to hold the content to be replaced */
				endpos = newsize;
				offset = newsize - output.size();
				output.resize(newsize);
				data_ptr = &output[0];

				memmove((void*)(data_ptr + offset),
					(void*)data_ptr,
					(output.size() - offset) * sizeof(CharType));
			}
			else
			{
				endpos = output.size();
				offset = 0;
				data_ptr = const_cast<CharType*>(&output[0]);
			}

			/* the second pass,  the replacement */
			while (offset < endpos)
			{
				found_pos = output.find(find, offset);
				if (found_pos != std::basic_string<CharType>::npos)
				{
					/* move the content between two targets */
					if (target != found_pos)
						memmove((void*)(data_ptr + target),
							(void*)(data_ptr + offset),
							(found_pos - offset) * sizeof(CharType));

					target += found_pos - offset;

					/* replace */
					memcpy(data_ptr + target,
						replace.data(),
						replace_length * sizeof(CharType));

					target += replace_length;
					offset = find_length + found_pos;
					replaced++;
				}
				else
				{
					/* ending work  */
					if (target != offset)
						memcpy((void*)(data_ptr + target),
							(void*)(data_ptr + offset),
							(endpos - offset) * sizeof(CharType));
					break;
				}
			}

			if (replace_length < find_length)
				output.resize(output.size() - replaced * (find_length - replace_length));

			return replaced;
		}

		inline int vsnprintfT(char* dst, size_t count, const char* format, va_list ap)
		{
			return vsnprintf(dst, count, format, ap);
		}

		inline int vsnprintfT(wchar_t* dst, size_t count, const wchar_t* format, va_list ap)
		{
#if defined(OS_WIN)	
			return _vsnwprintf(dst, count, format, ap);
#else	
			return vswprintf(dst, count, format, ap);
#endif	
		}

		template<typename CharType>
		void StringAppendVT(const CharType* format, va_list ap, std::basic_string<CharType>& output)
		{
			CharType stack_buffer[1024];

			/* first, we try to finish the task using a fixed-size buffer in the stack */
			va_list ap_copy;
			GG_VA_COPY(ap_copy, ap);

			int result = vsnprintfT(stack_buffer, COUNT_OF(stack_buffer), format, ap_copy);
			va_end(ap_copy);
			if (result >= 0 && result < static_cast<int>(COUNT_OF(stack_buffer)))
			{
				/* It fits */
				output.append(stack_buffer, result);
				return;
			}

			/* then, we have to repeatedly increase buffer size until it fits. */
			int buffer_size = COUNT_OF(stack_buffer);
			std::basic_string<CharType> heap_buffer;
			for (;;)
			{
				if (result != -1)
				{
					assert(0);
					return; /* not expected, result should be -1 here */
				}
				buffer_size <<= 1; /* try doubling the buffer size */
				if (buffer_size > 32 * 1024 * 1024)
				{
					assert(0);
					return;	/* too long */
				}
				/* resize */
				heap_buffer.resize(buffer_size);
				/*
					* NOTE: You can only use a va_list once.  Since we're in a while loop, we
					* need to make a new copy each time so we don't use up the original.
					*/
				GG_VA_COPY(ap_copy, ap);
				result = vsnprintfT(&heap_buffer[0], buffer_size, format, ap_copy);
				va_end(ap_copy);

				if ((result >= 0) && (result < buffer_size)) {
					/* It fits */
					output.append(&heap_buffer[0], result);
					return;
				}
			}
		}

		void StringAppendV(std::string& output, const char* format, va_list ap)
		{
			StringAppendVT<char>(format, ap, output);
		}

		void StringAppendV(std::wstring& output, const wchar_t* format, va_list ap)
		{
			StringAppendVT<wchar_t>(format, ap, output);
		}

		template<typename CharType>
		std::basic_string<CharType> LowerStringT(std::basic_string_view<CharType> input)
		{
			if (input.empty())
				return {};
			std::basic_string<CharType> str(input);
			CharType* start = &str[0];
			CharType* end = start + str.length();
			for (; start < end; start++)
			{
				if (*start >= 'A' && *start <= 'Z')
					*start += 'a' - 'A';
			}
			return str;
		}
		static_assert('a' == L'a', "this world should be true");
		static_assert('Z' == L'Z', "this world should be true");

		template<typename CharType>
		std::basic_string<CharType> UpperStringT(std::basic_string_view<CharType> input)
		{
			if (input.empty())
				return {};
			std::basic_string<CharType> str(input);
			CharType* start = &str[0];
			CharType* end = start + str.length();
			for (; start < end; start++)
			{
				if (*start >= 'a' && *start <= 'z')
					*start -= 'a' - 'A';
			}
			return str;
		}

		template<typename CharType>
		std::basic_string<CharType> StringTrimLeftT(std::basic_string_view<CharType> input)
		{
			std::basic_string<CharType> output(input);
			size_t check = 0;
			size_t length = output.length();
			const CharType* src = output.data();

			for (; check < length; check++)
				if (NOT_SPACE(src[check]))
					break;

			output.erase(0, check);
			return output;
		}

		template<typename CharType>
		std::basic_string<CharType> StringTrimRightT(std::basic_string_view<CharType> input)
		{
			std::basic_string<CharType> output(input);
			size_t length = output.length();
			const CharType* src = output.data();

			for (; length > 0; length--)
				if (NOT_SPACE(src[length - 1]))
					break;

			output.resize(length);
			return output;
		}

		template<typename CharType>
		std::basic_string<CharType> StringTrimT(std::basic_string_view<CharType> input)
		{
			std::basic_string<CharType> output(input);
			if (output.empty())
				return {};
			size_t bound1 = 0;
			size_t bound2 = output.length();
			const CharType* src = output.data();

			for (; bound2 > 0; bound2--)
				if (NOT_SPACE(src[bound2 - 1]))
					break;

			for (; bound1 < bound2; bound1++)
				if (NOT_SPACE(src[bound1]))
					break;

			if (bound1 < bound2) {
				memmove((void*)src,
					src + bound1,
					sizeof(CharType) * (bound2 - bound1));
			}

			output.resize(bound2 - bound1);
			return output;
		}

		template<typename CharType>
		std::basic_string<typename CharType>
			TrimEndT(std::basic_string_view<CharType> input, std::basic_string_view<CharType> trimChars)
		{
			if (trimChars.empty())
				return StringTrimRightT(input);
			else
			{
				size_t index = input.find_last_not_of(trimChars);
				return std::basic_string<CharType>(input.substr(0, index + 1));
			}
		}

		template<typename CharType>
		std::basic_string<typename CharType>
			TrimStartT(std::basic_string_view<CharType> input, std::basic_string_view<CharType> trimChars)
		{
			if (trimChars.empty())
				return StringTrimLeftT(input);
			else
			{
				size_t index = input.find_first_not_of(trimChars);
				return std::basic_string<CharType>(input.substr(index));
			}
		}

		template<typename CharType>
		bool StartsWithT(std::basic_string_view<CharType> src,
			std::basic_string_view<CharType> input, bool ignoreCase = false)
		{
			if (!ignoreCase)
				return src.size() >= input.size() &&
				src.substr(0, input.size()) == input;
			else
			{
				if (src.size() < input.size())
					return false;
				auto const piece = src.substr(0, input.size());
				//seem like the compiler will raise an warning when chartype inner cast to int
				return std::equal(piece.begin(), piece.end(), input.begin(),
					[](CharType l, CharType r) {
						if (l >= 'A' && l <= 'Z')
							l += 'a' - 'A';
						if (r >= 'A' && r <= 'Z')
							r += 'a' - 'A';
						return l == r;
					});
			}
		}

		template<typename CharType>
		std::basic_string<CharType> JoinT(
			std::basic_string_view<CharType> src, std::vector<std::basic_string_view<CharType>> value,
			std::basic_string_view<CharType> separator, size_t startIndex, size_t count)
		{
			std::basic_string<CharType> result;
			const int minCount = value.size() < count ? count : value.size();
			for (size_t i = 0; i != minCount; ++i)
			{
				result += value[i];
				result += separator;
			}
			return result;
		}

		template<typename CharType>
		std::basic_string<CharType>
			PadRightT(std::basic_string_view<CharType> src, size_t totalWidth, CharType paddingChar)
		{
			if (totalWidth <= src.size())
				return std::basic_string<CharType>(src);
			else
			{
				std::basic_string<CharType> result;
				result.reserve(totalWidth);
				result += src;
				for (size_t i = 0; i < totalWidth - src.size(); ++i)
					result += paddingChar;
				return result;
			}
		}

		template<typename CharType>
		std::basic_string<CharType>
			InsertT(std::basic_string_view<CharType> src, size_t startIndex, std::basic_string_view<CharType> value)
		{
			std::basic_string<CharType> result;
			if (startIndex >= src.size())
				return std::basic_string<CharType>(src) + std::basic_string<CharType>(value);//没有考虑效率
			else
			{
				result.reserve(src.size() + value.length());
				result.append(src, 0, startIndex);
				result.append(value);
				result.append(src, startIndex, src.length()-startIndex);
				//result += src.substr(0, startIndex);
				//result += value;//fix me
				//result += src.substr(startIndex);
				return result;
			}
		}

		template<typename CharType>
		std::basic_string<CharType>
			RemoveT(std::basic_string_view<CharType> src, size_t startIndex, size_t count)
		{
			if (startIndex >= src.size())
				return std::basic_string<CharType>(src);
			std::basic_string_view<CharType> left = src.substr(0, startIndex);
			if (startIndex + count >= src.length())
				return std::basic_string<CharType>(left);
			std::basic_string_view<CharType> right = src.substr(startIndex + count);
			return std::basic_string<CharType>(left) + std::basic_string<CharType>(right);
		}

		template<typename CharType>
		std::basic_string<typename CharType>
			TrimStartNT(std::basic_string_view<CharType> input, size_t n)
		{
			if (n >= input.length())
				return {};
			else
				return std::basic_string<CharType>(input.substr(n));
		}

	} // anonymous namespace

	std::string Format(const char* format, ...)
	{
		va_list	args;
		va_start(args, format);
		std::string output;
		StringAppendV(output, format, args);
		va_end(args);

		return output;
	}

	std::wstring Format(const wchar_t* format, ...)
	{
		va_list	args;
		va_start(args, format);
		std::wstring output;
		StringAppendV(output, format, args);
		va_end(args);

		return output;
	}

	const std::string& Format(std::string& output, const char* format, ...)
	{
		va_list	args;
		va_start(args, format);
		output.clear();
		StringAppendV(output, format, args);
		va_end(args);
		return output;
	}

	const std::wstring& Format(std::wstring& output, const wchar_t* format, ...)
	{
		va_list	args;
		va_start(args, format);
		output.clear();
		StringAppendV(output, format, args);
		va_end(args);
		return output;
	}

	void StringPrintfV(std::string& output, const char* format, va_list ap)
	{
		output.clear();
		StringAppendV(output, format, ap);
	}

	void StringPrintfV(std::wstring& output, const wchar_t* format, va_list ap)
	{
		output.clear();
		StringAppendV(output, format, ap);
	}

	void StringAppendF(std::string& output, const char* format, ...)
	{
		va_list	args;
		va_start(args, format);
		StringAppendV(output, format, args);
		va_end(args);
	}

	void StringAppendF(std::wstring& output, const wchar_t* format, ...)
	{
		va_list	args;
		va_start(args, format);
		StringAppendV(output, format, args);
		va_end(args);
	}

	std::string LowerString(std::string_view str)
	{
		return LowerStringT(str);
	}

	std::wstring LowerString(std::wstring_view str)
	{
		return LowerStringT(str);
	}

	std::string UpperString(std::string_view src)
	{
		return UpperStringT(src);
	}

	std::wstring UpperString(std::wstring_view src)
	{
		return UpperStringT(src);
	}

	void UTF8CreateLengthTable(unsigned table[256])
	{
		int c;

		for (c = 0; c < 256; c++)
		{
			if (c < 0x80)
				table[c] = 1;
			else if (0xBF < c && c < 0xE0)
				table[c] = 2;
			else if (0xDF < c && c < 0xF0)
				table[c] = 3;
			else if (0xEF < c && c < 0xF8)
				table[c] = 4;
			else
				table[c] = 0;
		}
	}

	bool ValidateUTF8Stream(const void* stream, unsigned length)
	{
		/*
			*	根据RFC3629（http://tools.ietf.org/html/rfc3629），
			*	UTF-8流一个字符的第一个字节由0-4个数值为1的二进制位
			*	这些位之后的第一位必须是0，当这些位的个数为0的时候表
			*	示这个字符是ASCII，占用一个字节，除此之外表示这个字符
			*	所占用的字节数；第二个字节开始，每个字节必须使用10的
			*	二进制位开头，这样利于快速定位一个字符的起始字节，例如：
			*
			*	0XXXXXXX
			*	110XXXXX 10XXXXXX
			*	1110XXXX 10XXXXXX 10XXXXXX
			*	11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
			*
			*	另，UTF-8理论上支持6字节长度，但是标准将其限定为4字节
			*/

		unsigned i, j, k;
		unsigned char* s = (unsigned char*)stream;
		static unsigned int table_created = 0;
		static unsigned int table[256];

		/* 保证多线程安全 */
		if (!table_created)
		{
			/* 利用lock-free的思想创建一模一样的表 */
			UTF8CreateLengthTable(table);
			/* 标记，之后的线程将不会重复创建该表 */
			table_created = 1;
		}

		/* 这里使用查表法是因为考虑到这个表会被放入CPU的Cache */

		for (i = 0; i < length;)
		{
			k = table[s[i]];
			if (k == 0)
				break;
			for (j = 1; j < k; j++)
				if (table[s[i + j]])
					return false;
			i += j;
		}

		return i == length;
	}

	bool ValidateGB2312Stream(const void* stream, unsigned length)
	{
		/*
			*	根据http://zh.wikipedia.org/zh-cn/Gb2312：
			*	01-09区为特殊符号。
			*	16-55区为一级汉字，按拼音排序。
			*	56-87区为二级汉字，按部首／笔画排序。
			*	10-15区及88-94区则未有编码。
			*
			*	每个汉字及符号以两个字节来表示。第一个字节称为“高位字节”，第二个字节称为“低位字节”。
			*	“高位字节”使用了0xA1-0xF7（把01-87区的区号加上0xA0），“低位字节”使用了0xA1-0xFE（把01-94加上 0xA0）。
			*	由于一级汉字从16区起始，汉字区的“高位字节”的范围是0xB0-0xF7，“低位字节”的范围是0xA1-0xFE，
			*	占用的码位是 72*94=6768。其中有5个空位是D7FA-D7FE。
			*/

		unsigned char* s = (unsigned char*)stream;
		unsigned char* e = s + length;

		for (; s < e; s++)
		{
			if (*s < 0x80)
				continue;
			if (*s < 0xA1 || 0xE7 < *s)
				break;
			if (++s == e)
				return false;
			if (*s < 0xA1 || 0xFE < *s)
				break;
		}

		return s == e;
	}

	bool ValidateGBKStream(const void* stream, unsigned length)
	{
		unsigned char* s = (unsigned char*)stream;
		unsigned char* e = s + length;

		for (; s < e; s++)
		{
			if (*s < 0x80)
				continue;
			if (*s < 0x81 || 0xFE < *s)
				break;
			if (++s == e)
				return false;
			if (*s < 0x40 || 0xFE < *s)
				break;
		}

		return s == e;
	}

	bool ValidateBIG5Stream(const void* stream, unsigned length)
	{
		unsigned char* s = (unsigned char*)stream;
		unsigned char* e = s + length;

		for (; s < e; s++)
		{
			if (*s < 0x80)
				continue;
			if (*s < 0xA1 || 0xF9 < *s)
				break;
			if (++s == e)
				return false;
			if (*s < 0x40 || 0xFE < *s || (0x7E < *s && *s < 0xA1))
				break;
		}

		return s == e;
	}

	std::string StringTrimLeft(std::string_view input)
	{
		return StringTrimLeftT(input);
	}

	std::wstring StringTrimLeft(std::wstring_view input)
	{
		return StringTrimLeftT(input);
	}

	std::string StringTrimRight(std::string_view input)
	{
		return StringTrimRightT(input);
	}

	std::string StringTrim(std::string_view input)
	{
		return StringTrimT(input);
	}

	std::wstring StringTrimRight(std::wstring_view input)
	{
		return StringTrimRightT(input);
	}

	std::wstring StringTrim(std::wstring_view input)
	{
		return StringTrimT(input);
	}


	std::string TrimEnd(std::string_view input, std::string_view trimChars)
	{
		return TrimEndT(input, trimChars);
	}

	std::wstring TrimEnd(std::wstring_view input, std::wstring_view trimChars)
	{
		return TrimEndT(input, trimChars);
	}

	std::string TrimStart(std::string_view input, std::string_view trimChars)
	{
		return TrimStartT(input, trimChars);
	}

	std::wstring TrimStart(std::wstring_view input, std::wstring_view trimChars)
	{
		return TrimStartT(input, trimChars);
	}

	std::string TrimStart(std::string_view input, size_t n)
	{
		return TrimStartNT(input, n);
	}

	std::wstring TrimStart(std::wstring_view input, size_t n)
	{
		return TrimStartNT(input, n);
	}

	bool StartsWith(std::string_view src, std::string_view input, bool ignoreCase)
	{
		return StartsWithT(src, input, ignoreCase);
	}

	bool StartsWith(std::wstring_view src, std::wstring_view input, bool ignoreCase)
	{
		return StartsWithT(src, input, ignoreCase);
	}

	bool EndsWith(std::string_view src, std::string_view input, bool ignoreCase)
	{
		if (src.size() < input.size())
			return false;
		std::string_view trunc = src.substr(src.size() - input.size());
		return StartsWithT(trunc, input, ignoreCase);
	}

	bool EndsWith(std::wstring_view src, std::wstring_view input, bool ignoreCase)
	{
		if (src.size() < input.size())
			return false;
		std::wstring_view trunc = src.substr(src.size() - input.size());
		return StartsWithT(trunc, input, ignoreCase);
	}

	std::string Concat(std::string_view str0, std::string_view str1)
	{
		return std::string(str0) + std::string(str1);
	}

	std::wstring Concat(std::wstring_view str0, std::wstring_view str1)
	{
		return std::wstring(str0) + std::wstring(str1);
	}

	int Compare(std::string_view strA, std::string_view strB, bool ignoreCase)
	{
		if (!ignoreCase)
			return strA.compare(strB);
		else
			return details::StringCompareNoCase(strA, strB);
	}

	int Compare(std::wstring_view strA, std::wstring_view strB, bool ignoreCase)
	{
		if (!ignoreCase)
			return strA.compare(strB);
		else
			return details::StringCompareNoCase(strA, strB);
	}

	bool Equals(std::string_view strA, std::string_view strB, bool ignoreCase)
	{
		return Compare(strA, strB, ignoreCase) == 0;
	}

	bool Equals(std::wstring_view strA, std::wstring_view strB, bool ignoreCase)
	{
		return Compare(strA, strB, ignoreCase) == 0;
	}

	bool Contains(std::string_view value, std::string_view des)
	{
		return value.find(des) != std::string_view::npos;
	}

	bool Contains(std::wstring_view value, std::wstring_view des)
	{
		return value.find(des) != std::wstring_view::npos;
	}

	size_t IndexOf(std::string_view src, std::string_view value)
	{
		return src.find(value);
	}

	size_t IndexOf(std::wstring_view src, std::wstring_view value)
	{
		return src.find(value);
	}

	size_t LastIndexOf(std::string_view src, std::string_view value)
	{
		return src.rfind(value);
	}

	size_t LastIndexOf(std::wstring_view src, std::wstring_view value)
	{
		return src.rfind(value);
	}

	size_t IndexOfAny(std::string_view src, std::string_view anyOf)
	{
		return src.find_first_of(anyOf);
	}

	size_t IndexOfAny(std::wstring_view src, std::wstring_view anyOf)
	{
		return src.find_first_of(anyOf);
	}

	size_t LastIndexOfAny(std::string_view src, std::string_view anyOf)
	{
		return src.find_last_of(anyOf);
	}

	size_t LastIndexOfAny(std::wstring_view src, std::wstring_view anyOf)
	{
		return src.find_last_of(anyOf);
	}

	std::string Join(std::string_view src, std::vector<std::string_view> value, std::string_view separator, size_t startIndex, size_t count)
	{
		return JoinT(src, value, separator, startIndex, count);
	}

	std::wstring Join(std::wstring_view src, std::vector<std::wstring_view> value, std::wstring_view separator, size_t startIndex, size_t count)
	{
		return JoinT(src, value, separator, startIndex, count);
	}

	//wcstok系列是线程安全的，Split和wcstok函数混用时，容易出现难以察觉的错误。
	std::vector<std::string> Split(std::string_view src, std::string_view separator, std::string_view quote)
	{
		std::string input(src);
		StringTokenizer tokenizer(input, std::string(separator));
		if(!quote.empty())
			tokenizer.set_quote_chars(std::string(quote));
		std::vector<std::string> result;
		while (tokenizer.GetNext())
		{
			result.push_back(tokenizer.token().c_str());
		}
		return result;
	}

	//code modified from chromium
	std::vector<std::wstring> Split(std::wstring_view src, std::wstring_view separator, std::wstring_view quote)
	{
		std::wstring input(src);
		WstringTokenizer tokenizer(input, std::wstring(separator));
		if (!quote.empty())
			tokenizer.set_quote_chars(std::wstring(quote));
		std::vector<std::wstring> result;
		while (tokenizer.GetNext())
		{
			result.push_back(tokenizer.token().c_str());
		}
		return result;
	}

	std::vector<std::string> Split(std::string_view src, std::string_view separator, StringSplitOptions op)
	{
		std::string input(src);
		StringTokenizer tokenizer(input, std::string(separator));
		if (op != StringSplitOptions::RemoveEmptyEntries)
			tokenizer.set_options(StringTokenizer::RETURN_EMPTY_TOKENS);
		std::vector<std::string> result;
		while (tokenizer.GetNext())
		{
			result.push_back(tokenizer.token().c_str());
		}
		return result;
	}

	std::vector<std::wstring> Split(std::wstring_view src, std::wstring_view separator, StringSplitOptions op)
	{
		std::wstring input(src);
		WstringTokenizer tokenizer(input, std::wstring(separator));
		if (op == StringSplitOptions::None)
			tokenizer.set_options(WstringTokenizer::RETURN_EMPTY_TOKENS);
		std::vector<std::wstring> result;
		while (tokenizer.GetNext())
		{
			result.push_back(tokenizer.token().c_str());
		}
		return result;
	}

	std::string Insert(std::string_view src, size_t startIndex, std::string_view value)
	{
		return InsertT(src, startIndex, value);
	}

	std::wstring Insert(std::wstring_view src, size_t startIndex, std::wstring_view value)
	{
		return InsertT(src, startIndex, value);
	}

	std::string Replace(std::string_view src, std::string_view oldValue, std::string_view newValue)
	{
		std::string result(src);
		StringReplaceAllT(oldValue, newValue, result);
		return result;
	}

	std::wstring Replace(std::wstring_view src, std::wstring_view oldValue, std::wstring_view newValue)
	{
		std::wstring result(src);
		StringReplaceAllT(oldValue, newValue, result);
		return result;
	}

	std::string Replace(std::string_view src, char oldChar, char newChar)
	{
		std::string result(src);
		for (size_t i = 0; i != result.size(); ++i)
		{
			if (result[i] == oldChar)
				result[i] = newChar;
		}
		return result;
	}

	std::wstring Replace(std::wstring_view src, wchar_t oldChar, wchar_t newChar)
	{
		std::wstring result(src);
		for (size_t i = 0; i != result.size(); ++i)
		{
			if (result[i] == oldChar)
				result[i] = newChar;
		}
		return result;
	}

	std::string Remove(std::string_view src, size_t startIndex, size_t count)
	{
		return RemoveT(src, startIndex, count);
	}

	std::wstring Remove(std::wstring_view src, size_t startIndex, size_t count)
	{
		return RemoveT(src, startIndex, count);
	}

	template<typename CharType>
	std::basic_string<CharType>
		PadLeftT(std::basic_string_view<CharType> src, size_t totalWidth, CharType paddingChar)
	{
		if (totalWidth <= src.size())
			return std::basic_string<CharType>(src);
		else
		{
			std::basic_string<CharType> result;
			result.reserve(totalWidth);
			for (size_t i = 0; i < totalWidth - src.size(); ++i)
				result += paddingChar;
			result += src;
			return result;
		}
	}

	std::string PadLeft(std::string_view src, size_t totalWidth, char paddingChar)
	{
		return PadLeftT(src, totalWidth, paddingChar);
	}

	std::wstring PadLeft(std::wstring_view src, size_t totalWidth, wchar_t paddingChar)
	{
		return PadLeftT(src, totalWidth, paddingChar);
	}

	std::string PadRight(std::string_view src, size_t totalWidth, char paddingChar)
	{
		return PadRightT(src, totalWidth, paddingChar);
	}

	std::wstring PadRight(std::wstring_view src, size_t totalWidth, wchar_t paddingChar)
	{
		return PadRightT(src, totalWidth, paddingChar);
	}

	bool IsWhiteSpace(std::string_view value)
	{
		return details::IsWhitespace(value);
	}

	bool IsWhiteSpace(std::wstring_view value)
	{
		return details::IsWhitespace(value);
	}

}
