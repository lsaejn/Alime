#pragma once

#include <string>

#include <Alime/base/base_define.h>
#include <Alime/base/compiler_specific.h>

namespace Alime::base
{
	/// 传统fopen的api里
	/// r和r+ 要求文件必须存在，否则返回句柄为空
	/// w,a,w+,a+都会创建文件
	/// 在a/a+模式下， 每次写入时会重新seek到文件尾部， 即使先前fseek到前面的某个位置。
	/// a和a+的区别在于，a+可读(视a为特殊的w即可)
	/// 在C#里, 增加了更多模式以便控制文件是否被创建
	enum class FileMode
	{
		Append,//以追加方式打开已有文件。文件不存在则创建。
		Create, //重新创建一个新的文件。如果文件已存在，则删除旧文件，然后创建
		CreateNew,//指定操作系统应创建一个新的文件。文件已存在则抛出异常
		Open,//打开一个已有的文件。文件不存在则抛出异常
		OpenOrCreate,//打开已有文件。文件不存在则创建新文件。
		Truncate//打开并截断已有的文件，保留文件的初始创建日期。文件不存在则抛出异常。
	};

	enum class FileAccess
	{
		Read,
		Write,
		ReadWrite
	};

	/// <summary>
	/// 用于控件进程间文件共享权限，在windows下，这个设置非常模糊
	/// </summary>
	enum class FileShare
	{
		None,
		Read,
		ReadWrite,
		Write,
		Delete,//没用
		Inheritable,//win32不支持
	};

	//unix系统以U32为主，但各个库使用U16,
	//所以我们只支持u8+u16
	enum class Encoding
	{
		Mbcs,
		Utf8,
		Unicode,
		Utf16,
		Utf16BE,
		Utf32,
		Unknown
	};

	enum class FileAttributes
	{
		ReadOnly = 1,
		Hidden = 2,
		System = 4,
		Directory = 16,
		Archive = 32,
		Device = 64,
		Normal = 128,
		Temporary = 256,
		SparseFile = 512,
		ReparsePoint = 1024,
		Compressed = 2048,
		Offline = 4096,
		NotContentIndexed = 8192,
		Encrypted = 16384,
		//IntegrityStream = 32768, // win32 did not support here
		//NoScrubData = 131072
	};

	/*
	extern const Char Delimiter;
	// Null-terminated array of separators used to separate components in path.
	extern const Char kEndChar;
// Each character in this array is a valid separator
	extern const Char kFilePathSeparators[];
	// A special path component meaning "this directory."
	extern const Char kFilePathCurrentDirectory[];
	// A special path component meaning "the parent directory."
	extern const Char kFilePathParentDirectory[];
	// The character used to identify a file extension.
	extern const Char kFilePathExtensionSeparator;
	*/
	//我们不使用extern, 因为file_define只被极少数类包含
#if defined(OS_POSIX)
	const Char kEndChar = '\0';
#define kDelimiter = L'/';
#define kLineBreak L"\n"
	const Char kFilePathSeparators[] = "/";
	const Char kFilePathCurrentDirectory[] = ".";
	const Char kFilePathParentDirectory[] = "..";
	const Char kFilePathExtensionSeparator = '.';
#elif defined(OS_WIN)
	const Char kEndChar = L'\0';
#define kDelimiter L'\\';
#define kLineBreak L"\r\n"
	const Char kFilePathSeparators[] = L"\\/";
	const Char kFilePathCurrentDirectory[] = L".";
	const Char kFilePathParentDirectory[] = L"..";
	const Char kFilePathExtensionSeparator = L'.';
#endif  // OS_WIN

};


