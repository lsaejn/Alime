#pragma once
#include <string_view>
#include "Alime/base/macros.h"

namespace Alime
{
    namespace base
    {
        /// quick judge with a char
#define NOT_SPACE(x) ((x) != 0x20 && ((x) < 0 || 0x1d < (x)))

        /// not within /r/n
#define WHITESPACE_ASCII_NO_CR_LF      \
      0x09, /* CHARACTER TABULATION */ \
      0x0B, /* LINE TABULATION */      \
      0x0C, /* FORM FEED (FF) */       \
      0x20  /* SPACE */

        /// all ascii
#define WHITESPACE_ASCII                                                  \
  WHITESPACE_ASCII_NO_CR_LF, /* Comment to make clang-format linebreak */ \
      0x0A,                  /* LINE FEED (LF) */                         \
      0x0D                   /* CARRIAGE RETURN (CR) */

        /// more specific in unicode
#define WHITESPACE_UNICODE_NON_ASCII          \
      0x0085,     /* NEXT LINE (NEL) */           \
      0x00A0, /* NO-BREAK SPACE */            \
      0x1680, /* OGHAM SPACE MARK */          \
      0x2000, /* EN QUAD */                   \
      0x2001, /* EM QUAD */                   \
      0x2002, /* EN SPACE */                  \
      0x2003, /* EM SPACE */                  \
      0x2004, /* THREE-PER-EM SPACE */        \
      0x2005, /* FOUR-PER-EM SPACE */         \
      0x2006, /* SIX-PER-EM SPACE */          \
      0x2007, /* FIGURE SPACE */              \
      0x2008, /* PUNCTUATION SPACE */         \
      0x2009, /* THIN SPACE */                \
      0x200A, /* HAIR SPACE */                \
      0x2028, /* LINE SEPARATOR */            \
      0x2029, /* PARAGRAPH SEPARATOR */       \
      0x202F, /* NARROW NO-BREAK SPACE */     \
      0x205F, /* MEDIUM MATHEMATICAL SPACE */ \
      0x3000  /* IDEOGRAPHIC SPACE */

#define WHITESPACE_UNICODE_NO_CR_LF \
  WHITESPACE_ASCII_NO_CR_LF, WHITESPACE_UNICODE_NON_ASCII

#define WHITESPACE_UNICODE WHITESPACE_ASCII, WHITESPACE_UNICODE_NON_ASCII

        namespace details
        {   
            static bool IsWhitespace(std::string_view sv)
            {
                static const char kWhitespaceASCII[] = { WHITESPACE_ASCII, 0 };
                if (sv.find_first_not_of(kWhitespaceASCII) == std::string_view::npos)
                    return true;
                return false;
            }

            static bool IsWhitespace(std::wstring_view sv)
            {
                static const wchar_t kWhitespaceASCII[] = { WHITESPACE_UNICODE, 0 };
                if (sv.find_first_not_of(kWhitespaceASCII) == std::string_view::npos)
                    return true;
                return false;
            }

            static bool IsWhitespace(char c)
            {
                static const char kWhitespaceASCII[] = { WHITESPACE_ASCII, 0 };
                for (int i = 0; i != ArraySize(kWhitespaceASCII) - 1; ++i)
                {
                    if (c == kWhitespaceASCII[i])
                        return true;
                }
                return false;
            }

            static bool IsWhitespace(wchar_t c)
            {
                static const wchar_t kWhitespaceASCII[] = { WHITESPACE_UNICODE, 0 };
                for (int i = 0; i != ArraySize(kWhitespaceASCII); ++i)
                {
                    if (c == kWhitespaceASCII[i])
                        return true;
                }
                return false;
            }

        }
        /*
        const wchar_t kWhitespaceWide[] = { WHITESPACE_UNICODE, 0 };
        const char16_t kWhitespaceUTF16[] = { WHITESPACE_UNICODE, 0 };
        const char16_t kWhitespaceNoCrLfUTF16[] = { WHITESPACE_UNICODE_NO_CR_LF, 0 };
        const char kWhitespaceASCII[] = { WHITESPACE_ASCII, 0 };
        const char16_t kWhitespaceASCIIAs16[] = { WHITESPACE_ASCII, 0 };
        const char kUtf8ByteOrderMark[] = "\xEF\xBB\xBF";
        */
    }  // namespace base
}
