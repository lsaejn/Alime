#pragma once

#include "stdint.h"

#include <string>

#include "Alime/base/build_config.h"

typedef char UTF8Char;
static_assert(CHAR_MAX == 127, "char type need be signed");

#if defined(WCHAR_T_IS_UTF16)
typedef wchar_t UTF16Char;
typedef char32_t UTF32Char;
#else
typedef char16_t UTF16Char;
typedef wchar_t UTF32Char;
#endif

typedef std::basic_string<UTF8Char> UTF8String;
typedef UTF8String U8String;
typedef std::basic_string<UTF16Char> UTF16String;
typedef UTF16String U16String;
typedef std::basic_string<UTF32Char> UTF32String;
typedef UTF32String U32String;

