#pragma once
#include "Alime/base/fileSystem/file_define.h"

#include <vector>

class NEWDecoder
{
public:
	//virtual void Convert(abyte* bytes, int byteCount, Char* chars, int charCount,
	//	bool flush, int& bytesUsed, int& charsUsed, bool& completed);

	virtual void Convert(abyte bytes[], int byteIndex, int byteCount, Char chars[],
		int charIndex, int charCount, bool flush, int& bytesUsed, int& charsUsed, bool &completed) = 0;

	//virtual void Convert(const std::vector<abyte> &bytes, std::vector<Char> chars,
	//	bool flush, int& bytesUsed, int& charsUsed, bool& completed);

	//virtual int GetCharCount(abyte* bytes, int count, bool flush);
	//virtual int GetCharCount(abyte bytes[], int index, int count) = 0;
	virtual int GetCharCount(abyte bytes[], int index, int count, bool flush) = 0;
	//virtual int GetCharCount(const std::vector<abyte> &bytes, bool flush);

	//virtual int GetChars(abyte* bytes, int byteCount, Char* chars, int charCount, bool flush);
	//virtual int GetChars(abyte bytes[], int byteIndex, int byteCount, Char chars[], int charIndex) = 0;
	//virtual int GetChars(abyte bytes[], int byteIndex, int byteCount, Char chars[], int charIndex, bool flush);
	//virtual int GetChars(const std::vector<abyte> &bytes, std::vector<Char> &chars, bool flush);

	//virtual void Reset();
};

class NEWUTF8Decoder : public NEWDecoder
{
public:
	//行为不同于C#, 不太清楚flush机制。注意，不能连续GetCharCount
	virtual int GetCharCount(abyte bytes[], int index, int count, bool flush) override;

    // 摘要:
//     Converts an array of encoded bytes to UTF-16 encoded characters and stores the
//     result in a character array.
//
// 参数:
//   bytes:
//     A byte array to convert.
//
//   byteIndex:
//     The first element of bytes to convert.
//
//   byteCount:
//     The number of elements of bytes to convert.
//
//   chars:
//     An array to store the converted characters.
//
//   charIndex:
//     The first element of chars in which data is stored.
//
//   charCount:
//     The maximum number of elements of chars to use in the conversion.
//
//   flush:
//     true to indicate that no further data is to be converted; otherwise, false.
//
//   bytesUsed:
//     When this method returns, contains the number of bytes that were used in the
//     conversion. This parameter is passed uninitialized.
//
//   charsUsed:
//     When this method returns, contains the number of characters from chars that were
//     produced by the conversion. This parameter is passed uninitialized.
//
//   completed:
//     When this method returns, contains true if all the characters specified by byteCount
//     were converted; otherwise, false. This parameter is passed uninitialized.
	virtual void Convert(abyte bytes[], int byteIndex, int byteCount, Char chars[], int charIndex,
		int charCount, bool flush, int& bytesUsed, int& charsUsed, bool& completed) override;

private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="bytes">一个utf码点</param>
	/// <param name="byteCount">长度</param>
	/// <param name="chars">转换后的宽字符(长度1-2)</param>
	/// <returns>chars transfered</returns>
	int Convert_internal(abyte*bytes, int byteCount, Char chars[2]);

	bool cacheAvailable_ = false;
	wchar_t	 cacheChar_;
	abyte cacheByte_[4];//3
	aint cacheByteSize_ = 0;
};

