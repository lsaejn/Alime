#pragma once

#include <string>

#include <Alime/base/compiler_specific.h>

namespace Alime::base
{

#if defined(OS_POSIX)
	typedef std::string PathString;
#elif defined(OS_WIN)
	using PathString = std::wstring;
	using PathStringTy = std::wstring;
#endif

	using PathChar = typename PathString::value_type;
	using PathCharTy = typename PathStringTy::value_type;
	using pos_t = int64_t;
	using aint= int64_t;


	///传统fopen的api里
	/// r和r+ 要求文件必须存在，否则返回句柄为空
	/// w,a,w+,a+都会创建文件
	/// 在a/a+模式下， 每次写人时会重新seek到文件尾部， 即使先前fseek到前面的某个位置。
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

	enum class FileShare
	{
		None,
		Read,
		ReadWrite,
		Write,
		Delete,//没用
		Inheritable,//win32不支持
	};

};


