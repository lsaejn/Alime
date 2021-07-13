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

	virtual int GetCharCount(abyte* bytes, int count, bool flush) = 0;
	virtual int GetCharCount(abyte bytes[], int index, int count) = 0;
	virtual int GetCharCount(abyte bytes[], int index, int count, bool flush) = 0;
	//virtual int GetCharCount(const std::vector<abyte> &bytes, bool flush);

	//virtual int GetChars(abyte* bytes, int byteCount, Char* chars, int charCount, bool flush);
	//virtual int GetChars(abyte bytes[], int byteIndex, int byteCount, Char chars[], int charIndex) = 0;
	//virtual int GetChars(abyte bytes[], int byteIndex, int byteCount, Char chars[], int charIndex, bool flush);
	//virtual int GetChars(const std::vector<abyte> &bytes, std::vector<Char> &chars, bool flush);

	virtual void Reset()=0;
};

class NEWUTF8Decoder : public NEWDecoder
{
public:
	//flush意味着当前buffer剩余将视为一个完整码点
	//本函数不会影响buffer
	virtual int GetCharCount(abyte* bytes, int count, bool flush) override;
	virtual int GetCharCount(abyte bytes[], int index, int count) override;
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

	virtual void Reset() override;

private:
	void ClearCacheChar();
	void ClearCacheBytes();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="bytes">一个utf码点</param>
	/// <param name="byteCount">长度</param>
	/// <param name="chars">转换后的宽字符(长度1-2)</param>
	/// <returns>chars transfered</returns>
	int Convert_internal(abyte*bytes, int byteCount, Char chars[2]);

	/// <summary>
	/// 尝试从buffer和bytes里读一个完整码点
	/// 有多种可能: 1.数据长度完整且正确 2.数据长度完整，但数据错误 3.数据长度不完整，但数据正确 4数据长度不完整，且数据错误
	/// </summary>
	/// <param name="bytes"></param>
	/// <param name="byteCount"></param>
	/// <param name="byteUsed">bytes将被使用的字节数</param>
	/// <param name="byteCompleted">完整码点需要从bytes里消耗的字节数</param>
	/// <param name="isLegal">读取时是否遇到了无效字节</param>
	void GetNextCodePointLength(abyte* bytes, int byteCount, int& bytesUsed, int& bytesCompleted, bool& isLegal);

	bool cacheAvailable_ = false;
	wchar_t	 cacheChar_;

	abyte cacheByte_[4];//3
	aint cacheByteSize_ = 0;
};

