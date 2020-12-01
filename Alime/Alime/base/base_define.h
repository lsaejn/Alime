#pragma once
//portable header
#include <string>
#include <stdint.h>
#include <sys/types.h>

#include <Alime/base/build_config.h>

typedef int8_t		aint8;
typedef uint8_t		auint8;
typedef int16_t		aint16;
typedef uint16_t	auint16;
typedef int32_t		aint32;
typedef uint32_t	auint32;
typedef int64_t		aint64;
typedef uint64_t	auint64;
typedef unsigned char abyte;

#ifdef Alime64
typedef aint64	 aint;
typedef aint64	 asint;
typedef auint64 auint;
#else
typedef aint32	 aint;
typedef aint32	 asint;
typedef auint32	 auint;
#endif
typedef aint64 pos_t;

#ifdef OS_WIN
using String = std::wstring;
using Char = wchar_t;
#else
using String = std::string
using Char = char;
#endif


#ifdef _WIN32
#include <basetsd.h>
using pid_t = int;//fix me, a compromise define
using ssize_t = SSIZE_T;
using mode_t = unsigned short;
#endif




