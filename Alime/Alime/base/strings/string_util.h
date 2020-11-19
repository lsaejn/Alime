#pragma once

#include <list>
#include <vector>
#include <string_view>

#include "Alime/base/base_export.h"
#include "Alime/base/strings/unicode.h"
#include "Alime/base/details/conv.h"


namespace Alime::base
{
	enum class StringSplitOptions
	{
		None = 0,
		RemoveEmptyEntries = 1
	};

	std::string		Format(const char* format, ...);
	std::wstring	Format(const wchar_t* format, ...);
	const std::string&		Format(std::string& output, const char* format, ...);
	const std::wstring&	Format(std::wstring& output, const wchar_t* format, ...);

	std::string		LowerString(std::string_view str);
	std::wstring	LowerString(std::wstring_view str);

	std::string		UpperString(std::string_view src);
	std::wstring	UpperString(std::wstring_view src);

	std::string		StringTrimLeft(std::string_view input);
	std::wstring	StringTrimLeft(std::wstring_view input);

	std::string		StringTrimRight(std::string_view input);
	std::wstring	StringTrimRight(std::wstring_view input);

	std::string		StringTrim(std::string_view input);
	std::wstring	StringTrim(std::wstring_view input);

	std::string		TrimEnd(std::string_view input, std::string_view trimChars="");
	std::wstring	TrimEnd(std::wstring_view input, std::wstring_view trimChars=L"");

	std::string		TrimStart(std::string_view input, std::string_view trimChars="");
	std::wstring	TrimStart(std::wstring_view input, std::wstring_view trimChars=L"");
	std::string		TrimStart(std::string_view input, size_t n);
	std::wstring	TrimStart(std::wstring_view input, size_t n);

	std::string		Concat(std::string_view str0, std::string_view str1);
	std::wstring	Concat(std::wstring_view str0, std::wstring_view str1);

	template<typename Rt, typename... Ts>
	std::enable_if_t<sizeof...(Ts) >= 1, Rt>
	Concat(Ts&&... args)
	{
		Rt re = to<Rt>(args...);
		return re;
	}

	bool StartsWith(std::string_view src, std::string_view input, bool ignoreCase=false);
	bool StartsWith(std::wstring_view src, std::wstring_view input, bool ignoreCase = false);

	bool EndsWith(std::string_view src, std::string_view input, bool ignoreCase = false);
	bool EndsWith(std::wstring_view src, std::wstring_view input, bool ignoreCase = false);

	bool Equals(std::string_view strA, std::string_view strB, bool ignoreCase = false);
	bool Equals(std::wstring_view strA, std::wstring_view strB, bool ignoreCase = false);

	bool Contains(std::string_view value, std::string_view des);
	bool Contains(std::wstring_view value, std::wstring_view des);

	int Compare(std::string_view strA, std::string_view strB, bool ignoreCase = false);
	int Compare(std::wstring_view strA, std::wstring_view strB, bool ignoreCase = false);

	size_t IndexOf(std::string_view src, std::string_view value);
	size_t IndexOf(std::wstring_view src, std::wstring_view value);

	size_t LastIndexOf(std::string_view src, std::string_view value);
	size_t LastIndexOf(std::wstring_view src, std::wstring_view value);

	size_t IndexOfAny(std::string_view src, std::string_view anyOf);
	size_t IndexOfAny(std::wstring_view src, std::wstring_view anyOf);

	size_t LastIndexOfAny(std::string_view src, std::string_view anyOf);
	size_t LastIndexOfAny(std::wstring_view src, std::wstring_view anyOf);

	//折衷的方案
	std::vector<std::string>		Split(std::string_view src, std::string_view separator, std::string_view quote="");
	std::vector<std::wstring>	Split(std::wstring_view src, std::wstring_view separator, std::wstring_view quote=L"");

	//选择了C#的方式。separator意思是char[]
	std::vector<std::string>		Split(std::string_view src, std::string_view separator, StringSplitOptions op);
	std::vector<std::wstring>	Split(std::wstring_view src, std::wstring_view separator, StringSplitOptions op);

	std::string		Insert(std::string_view src, size_t startIndex, std::string_view value);
	std::wstring	Insert(std::wstring_view src, size_t startIndex, std::wstring_view value);

	std::string		Replace(std::string_view src, std::string_view oldValue, std::string_view newValue);
	std::wstring	Replace(std::wstring_view src, std::wstring_view oldValue, std::wstring_view newValue);
	std::string		Replace(std::string_view src, char oldChar, char newChar);
	std::wstring	Replace(std::wstring_view src, wchar_t oldChar, wchar_t newChar);

	std::string		Remove(std::string_view src, size_t startIndex, size_t count=-1);
	std::wstring	Remove(std::wstring_view src, size_t startIndex, size_t count=-1);

	std::string		PadLeft(std::string_view src, size_t totalWidth, char paddingChar=' ');
	std::wstring	PadLeft(std::wstring_view src, size_t totalWidth, wchar_t paddingChar = ' ');

	std::string		PadRight(std::string_view src, size_t totalWidth, char paddingChar=' ');
	std::wstring	PadRight(std::wstring_view src, size_t totalWidth, wchar_t paddingChar = ' ');

	bool IsWhiteSpace(std::string_view value);
	bool IsWhiteSpace(std::wstring_view value);

	std::string Join(std::string_view src, std::vector<std::string_view> value, std::string_view separator = "", size_t startIndex = 0, size_t count = -1);
	std::wstring Join(std::wstring_view src, std::vector<std::wstring_view> value, std::wstring_view separator = L"", size_t startIndex = 0, size_t count = -1);

	template<typename... Ts>
	std::enable_if_t<sizeof...(Ts)>=2, std::string>
	  Join(std::string_view separator, const Ts&... ts)
	{
		return toDelim<std::string>(separator.data(), ts...);
	}

	template<typename... Ts>
	std::enable_if_t<sizeof...(Ts) >= 2, std::wstring>
	  Join(std::wstring_view separator, const Ts&... ts)
	{
		return toDelim<std::wstring>(separator.data(), ts...);
	}

	bool ValidateUTF8Stream(const void* stream, unsigned length);
	bool ValidateGB2312Stream(const void* stream, unsigned length);
	bool ValidateGBKStream(const void* stream, unsigned length);
	bool ValidateBIG5Stream(const void* stream, unsigned length);
}
