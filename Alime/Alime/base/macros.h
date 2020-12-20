#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_
#include <stdarg.h>
#include "Alime/base/build_config.h"


#define implements :public

#define extends :public

#define USE_API(NAME, API) static auto proc_##API = (decltype(&API))(GetProcAddress(GetModuleHandle(L#NAME), #API))

#ifdef __has_include
#if __has_include(<string_view>) &&_USE_STRINGVIEW
#define ALIME_HAS_STRINGVIEW_
#endif
#endif

// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) \
  TypeName(const TypeName&) = delete

// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete

// Put this in the declarations for a class to be uncopyable and unassignable.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  DISALLOW_COPY(TypeName);                 \
  DISALLOW_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
// This is especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                           \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

// Used to explicitly mark the return value of a function as unused. If you are
// really sure you don't want to do anything with the return value of a function
// that has been marked WARN_UNUSED_RESULT, wrap it with this. Example:
//
//   std::unique_ptr<MyType> my_var = ...;
//   if (TakeOwnership(my_var.get()) == SUCCESS)
//     ignore_result(my_var.release());
//
template<typename T>
inline void ignore_result(const T&)
{
}

template<typename... Ts>
inline void ignore_varaible(const Ts&&... )
{
}



#define GG_LONGLONG(x) x##LL
#define GG_ULONGLONG(x) x##ULL

// Per C99 7.8.14, define __STDC_CONSTANT_MACROS before including <stdint.h>
// to get the INTn_C and UINTn_C macros for integer constants.  It's difficult
// to guarantee any specific ordering of header includes, so it's difficult to
// guarantee that the INTn_C macros can be defined by including <stdint.h> at
// any specific point.  Provide GG_INTn_C macros instead.

#define GG_INT8_C(x)    (x)
#define GG_INT16_C(x)   (x)
#define GG_INT32_C(x)   (x)
#define GG_INT64_C(x)   GG_LONGLONG(x)

#define GG_UINT8_C(x)   (x ## U)
#define GG_UINT16_C(x)  (x ## U)
#define GG_UINT32_C(x)  (x ## U)
#define GG_UINT64_C(x)  GG_ULONGLONG(x)

// It's possible for functions that use a va_list, such as StringPrintf, to
// invalidate the data in it upon use.  The fix is to make a copy of the
// structure before using it and use that copy instead.  va_copy is provided
// for this purpose.  MSVC does not provide va_copy, so define an
// implementation here.  It is not guaranteed that assignment is a copy, so the
// StringUtil.VariableArgsFunc unit test tests this capability.
#if defined(COMPILER_GCC)
#define GG_VA_COPY(a, b) (va_copy(a, b))
#elif defined(COMPILER_MSVC)
#define GG_VA_COPY(a, b) (a = b)
#endif

// Define an OS-neutral wrapper for shared library entry points
#if defined(OS_WIN)
#define API_CALL __stdcall
#else
#define API_CALL
#endif


#define PTR_NULL(p)				{ assert(p); if (NULL == p) return NULL; }
// check the pointer is null and return void
#define PTR_VOID(p)				{ assert(p); if (NULL == p) return; }
// check the pointer avalible£¬ if not then return false
#define PTR_FALSE(p)				{ assert(p); if (NULL == p) return false; }

// delete pointer on safe mode
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if (NULL != p) { delete (p); (p) = NULL; } }
#endif

// release object on safe mode
#define SAFE_RELEASE(p)			{ if (NULL != p) { (p)->Release(); (p) = NULL; } }

#ifndef COUNT_OF
#define COUNT_OF(array)			(sizeof(array)/sizeof(array[0]))

template<typename T, int N>
constexpr int ArraySize(const T(&arg)[N])
{
	return N;
}
#endif

// define some format string
#define CHAR_FORMAT                             "%c"
#define POINTER_FORMAT                          "%p"
#define STRING_FORMAT                           "%s"

#define INT8_FORMAT                             "%d"
#define UINT8_FORMAT                            "%u"
#define INT16_FORMAT                            "%d"
#define UINT16_FORMAT                           "%u"
#define INT32_FORMAT                            "%d"
#define UINT32_FORMAT                           "%u"

#define HEX8_FORMAT                             "%x"
#define HEX16_FORMAT                            "%x"
#define HEX32_FORMAT                            "%x"

#define OCT8_FORMAT                             "%o"
#define OCT16_FORMAT                            "%o"
#define OCT32_FORMAT                            "%o"

#define FLOAT_FORMAT                            "%f"
#define DOUBLE_FORMAT                           "%lf"

#if defined(COMPILER_MSVC)
#define INT64_FORMAT                            "%I64d"
#define UINT64_FORMAT                           "%I64u"
#define HEX64_FORMAT                            "%I64x"
#define OCT64_FORMAT                            "%I64o"
#else  //OS_WIN
#define UINT64_FORMAT                           "%llu"
#define INT64_FORMAT                            "%lld"
#define HEX64_FORMAT                            "%llx"
#define OCT64_FORMAT                            "%llo"
#endif //OS_WIN

#endif  // BASE_MACROS_H_
