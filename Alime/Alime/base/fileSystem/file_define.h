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

	enum class Encoding
	{
		Mbcs,
		Utf8,
		Unicode,
		Utf16,
		Utf16BE,
		Utf32,
	};

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

};


