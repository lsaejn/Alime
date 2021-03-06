#ifndef BUILD_BUILD_CONFIG_H_
#define BUILD_BUILD_CONFIG_H_

// Define platform macro
#if defined(_WIN32)
#define OS_WIN      1

#if defined(_WIN64)
#define Alime64
#else
#define Alime32
#endif

#elif defined(__linux__)
#define OS_LINUX    1
#define OS_POSIX    1
#else
#error Please add support for your platform in build_config.h
#endif

// Use tcmalloc
#if (defined(OS_WIN) || defined(OS_POSIX)) && !defined(NO_TCMALLOC)
#define USE_TCMALLOC 1
#endif

// Compiler detection.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler
#endif

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64     1
#define ARCH_CPU_64_BITS    1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86        1
#define ARCH_CPU_32_BITS    1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL      1
#define ARCH_CPU_32_BITS    1
#define WCHAR_T_IS_UNSIGNED 1
#else
#error Please add support for your architecture in build/build_config.h
#endif

// Type detection for wchar_t.
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_POSIX) && \
	defined(COMPILER_GCC) && \
	defined(__WCHAR_MAX__) && \
	(__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32

#elif defined(OS_POSIX) && \
	defined(COMPILER_GCC) && \
	defined(__WCHAR_MAX__) && \
	(__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
// compile in this mode (in particular, Chrome doesn't). This is intended for
// other projects using base who manage their own dependencies and make sure
// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif


// GNU libc offers the helpful header <endian.h> which defines
// __BYTE_ORDER
#if defined(__GLIBC__)
#include <endian.h>
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define ALIME_LITTLE_ENDIAN
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define ALIME_BIG_ENDIAN
#elif (__BYTE_ORDER == __PDP_ENDIAN)
#define ALIME_PDP_ENDIAN
#else
#error Unknown machine endianness detected.
#endif
#define ALIME_BYTE_ORDER __BYTE_ORDER
#elif defined(_BIG_ENDIAN)
#define ALIME_BIG_ENDIAN
#define ALIME_BYTE_ORDER 4321
#elif defined(_LITTLE_ENDIAN)
# define ALIME_LITTLE_ENDIAN
# define ALIME_BYTE_ORDER 1234
#elif defined(__sparc) || defined(__sparc__) \
	|| defined(_POWER) || defined(__powerpc__) \
	|| defined(__ppc__) || defined(__hpux) \
	|| defined(_MIPSEB) || defined(_POWER) \
	|| defined(_MIPSEB) || defined(_POWER) \
	|| defined(__s390__)
# define ALIME_BIG_ENDIAN
# define ALIME_BYTE_ORDER 4321
#elif defined(__i386__) || defined(__alpha__) \
	|| defined(__ia64) || defined(__ia64__) \
	|| defined(_M_IX86) || defined(_M_IA64) \
	|| defined(_M_ALPHA) || defined(__amd64) \
	|| defined(__amd64__) || defined(_M_AMD64) \
	|| defined(__x86_64) || defined(__x86_64__) \
	|| defined(_M_X64) || defined (__ARMEL__)
# define ALIME_LITTLE_ENDIAN
# define ALIME_BYTE_ORDER 1234
#else
# error Please set up your byte order in build/build_config.h.
#endif

// Collapse the plethora of ARM flavors available to an easier to manage set
// Defs reference is at https://wiki.edubuntu.org/ARM/Thumb2PortingHowto
#if defined(__ARM_ARCH_6__)   || \
    defined(__ARM_ARCH_6J__)  || \
    defined(__ARM_ARCH_6K__)  || \
    defined(__ARM_ARCH_6Z__)  || \
    defined(__ARM_ARCH_6T2__) || \
    defined(__ARM_ARCH_6ZK__) || \
    defined(__ARM_ARCH_7__)   || \
    defined(__ARM_ARCH_7A__)  || \
    defined(__ARM_ARCH_7R__)  || \
    defined(__ARM_ARCH_7M__)
#define ARMV6_OR_7 1
#endif

#ifdef OS_WIN
#include "Alime/base/details/windows/winsdk_config.h"
#include "Alime/base/details/windows/xp_style_manifest_win.h"
#endif // OS_WIN

#endif  // BUILD_BUILD_CONFIG_H_
