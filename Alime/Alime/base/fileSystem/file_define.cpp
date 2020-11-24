#include "file_define.h"

#if defined(OS_POSIX)
const Char kEndChar = '\0';
const Char kFilePathSeparators[] = "/";
const Char kFilePathCurrentDirectory[] = ".";
const Char kFilePathParentDirectory[] = "..";
const Char kFilePathExtensionSeparator = '.';
#elif defined(OS_WIN)
const Char kEndChar = L'\0';
const Char kFilePathSeparators[] = L"\\/";
const Char kFilePathCurrentDirectory[] = L".";
const Char kFilePathParentDirectory[] = L"..";
const Char kFilePathExtensionSeparator = L'.';
#endif  // OS_WIN