#include "Decoder.h"

#include "Alime/base/third_party/convert_utf/ConvertUTF.h"

#include "assert.h"


static_assert(sizeof abyte == sizeof auint8, "size error");

namespace
{
	/// <summary>
	/// ���Ǻ�C#�ӿڱ���һ�£�0xxxxxxxx��10xxxxxxx����Ϊһ���������
	/// </summary>
	int GetUTF8CodePointLengthByFirstByte(auint8 byte)
	{
		if ((byte & 0xF0) == 0xF0)
		{
			return 4;
		}
		else if ((byte & 0xE0) == 0xE0)
		{
			return 3;
		}
		else if ((byte & 0xC0) == 0xC0)
		{
			return 2;
		}
		else
			return 1;
	}

	bool IsValidUTF8Value(auint8 byte)
	{
		if (byte & 0x80 == 0x80)
			return true;
		else
			return false;
	}
}


//��C#�ӿ�������ut8decoder���Ỻ��һ������Char
//Ҳ����˵��ut8decoder�����ֽڵ��������ַ���������Ժ�����˼
// �������L"���" ��bufferֻ��һ���ַ������Ỻ��bytes������ֽ�
//�����������L"���"ǰ����ֽڣ������2���ֽ���ᱻ����������������Alime��bufferһ��wchar_t��ԭ��
void NEWUTF8Decoder::Convert(abyte bytes[], int byteIndex,
	int byteCount, Char chars[], int charIndex, int charCount,
	bool flush, int& bytesUsed, int& charsUsed, bool& completed)
{
	//out
	bytesUsed = 0;
	charsUsed = 0;
	completed = false;

	auint8 source[4];
	aint sourceCount = 0;

	wchar_t target[2];
	wchar_t* writing = (chars+ charIndex);
	auto src = (bytes+ byteIndex);
	aint byteLeft = byteCount;

	if (cacheByteSize_ > 0)
	{
		memcpy(source, cacheByte_, cacheByteSize_);
		sourceCount = GetUTF8CodePointLengthByFirstByte(*source);
		assert(cacheByteSize_ < sourceCount&& sourceCount>1 && sourceCount <= 4 && cacheByteSize_ >= 1);

		const int surfixBytesNeeded = sourceCount - cacheByteSize_;

		int validBytesCount = 0;
		for (int i = 0; i < byteCount; ++i)
		{
			if (IsValidUTF8Value(src[i]))
			{
				if (++validBytesCount == surfixBytesNeeded)
					break;
			}
		}

		if (validBytesCount == surfixBytesNeeded)//����
		{
			int nTransfered= Convert_internal(source, surfixBytesNeeded, NULL);
			src += surfixBytesNeeded;
			if (charCount < nTransfered)
			{
				bytesUsed += validBytesCount;//�쳣ʱ������Ӧ������Щֵ
				completed = byteLeft == 0;
				throw "The output char buffer is too small to contain the decoded characters, encoding 'Unicode (UTF-8)'";
			}
			else
			{
				Convert_internal(source, surfixBytesNeeded, writing);
				charsUsed+= nTransfered;
				byteLeft -= surfixBytesNeeded;
			}	
		}
		else//������
		{
			assert(validBytesCount < surfixBytesNeeded);
			if (validBytesCount == byteCount)//�����������ǺϷ�(ȱ�ֽ�)
			{
				byteLeft -= validBytesCount;
				if (flush)
				{
					cacheByteSize_ = 0;
					if (charCount)
					{
						charsUsed = 1;
						memset(chars, 1, sizeof(Char));
					}
					else//fix me, should set out var?
						throw "The output char buffer is too small to contain the decoded characters, encoding 'Unicode (UTF-8)'";
				}
				else//cache src and quit
				{
					memcpy(cacheByte_+ cacheByteSize_, src, validBytesCount);
					charsUsed = 0;
				}
				return;
			}
			else//���������ҷǷ�, ��cache, �ض�src,����תsrc
			{
				assert(validBytesCount < byteCount);
				cacheByteSize_ = 0;
				src += validBytesCount;
				byteLeft -= validBytesCount;

				if (charCount)
				{
					charsUsed = 1;
					memset(chars, 1, sizeof(Char));
				}

			}
		}
	}

	while (charsUsed < charCount)//fix me, check chars' index
	{
		if (cacheAvailable_)
		{
			*writing++ = cacheChar_;
			cacheChar_ = L'0';
			charsUsed++;
			cacheAvailable_ = false;
		}
		else
		{
			if (byteLeft < 1)
				break;

			sourceCount = GetUTF8CodePointLengthByFirstByte(*src);

			if (byteLeft < sourceCount)
			{
				bool isValidByte = true;
				int i = 1;
				for (; i < sourceCount; ++i)
				{
					if ((src[i] & 0x80) != 0x80)
					{
						isValidByte = false;
						break;
					}
				}
				if(isValidByte)
					break;
			}
				


			int nChar = Convert_internal(src, sourceCount, target);
			if (1 == nChar)
				*writing++ = target[0];
			else if (2 == nChar)
			{
				*writing++ = target[0];
				cacheAvailable_ = true;
				cacheChar_ = target[1];
			}
			else
			{
				//fix me, log here
				memset(writing, 1, sizeof(Char));
			}
				

			src += sourceCount;
			byteLeft -= sourceCount;
			charsUsed++;
		}
	}

	if (byteLeft == 0)
	{
		completed = true;
		cacheByteSize_ = 0;
	}
	else
	{
		assert(byteLeft > 0);

		if (flush)
			return;

		auto fb = (auint8)(*src);
		if ((fb & 0xF0) == 0xF0 && byteLeft < 4)
		{
			cacheByteSize_ = byteLeft;
		}
		else if ((fb & 0xE0) == 0xE0 && byteLeft < 3)
		{
			cacheByteSize_ = byteLeft;
		}
		else if ((fb & 0xC0) == 0xC0 && byteLeft < 2)
		{
			cacheByteSize_ = byteLeft;
		}
		memcpy(cacheByte_, src, cacheByteSize_);
	}
	bytesUsed = byteCount - byteLeft+ cacheByteSize_;
	return;

}

//10 , 0xxxxxxx, 110xxxxx, 1110xxxx, 1111xxxx
//fix me, windows only
int NEWUTF8Decoder::GetCharCount(abyte bytes[], int index, int count, bool flush)
{
	int charsCaculated = 0;

	auint8 source[4];
	aint sourceCount = 0;

	auto src = (bytes + index);
	aint byteLeft = count;

	// 0 1 2 3
	if (cacheByteSize_ > 0)
	{
		memcpy(source, cacheByte_, cacheByteSize_);
		sourceCount =GetUTF8CodePointLengthByFirstByte(*source);
		assert(cacheByteSize_ < sourceCount && sourceCount>1 && sourceCount<=4 && cacheByteSize_ >=1);

		int surfixBytesNeeded = sourceCount - cacheByteSize_;

		int validBytesCount = 0;
		for (int i = 0; i < count; ++i)
		{
			if (IsValidUTF8Value(src[i]))
			{
				if (++validBytesCount == surfixBytesNeeded)
					break;
			}
		}

		if (validBytesCount == surfixBytesNeeded)//����
		{
			charsCaculated+=Convert_internal(source, surfixBytesNeeded, NULL);
			src += validBytesCount;
			byteLeft -= validBytesCount;
		}
		else
		{
			assert(validBytesCount < surfixBytesNeeded);
			if (validBytesCount == count)//�����������ǺϷ�
			{
				return flush ? 1 : 0;
			}
			else//���������ҷǷ�
			{
				assert(validBytesCount< count);
				charsCaculated++;
				src += validBytesCount;
				byteLeft -= validBytesCount;
			}
		}		
	}

	auint8* pos = src;
	auint8* end = src+ byteLeft;// [src, end)
	while (pos!= end)
	{
		aint legalCount = GetUTF8CodePointLengthByFirstByte(*pos);
		if (1 == legalCount)
		{
			pos++;
			charsCaculated++;
			continue;
		}
		else
		{
			auto begin = pos;
			sourceCount = legalCount;
			pos++; sourceCount--;

			while (pos != end && sourceCount)
			{
				if ((*pos >>6) == 2)
				{
					pos++;
					sourceCount--;
				}
				else
				{
					break;
				}
			}
			if (0 == sourceCount)
			{
				int charsTransfered = Convert_internal(begin, legalCount, NULL);
				charsCaculated += charsTransfered;
			}
			else
			{
				if (pos == end)//�Ϸ�
				{
					if(flush)
						charsCaculated++;
				}
				else
					charsCaculated++;
			}
		}	
	}
	return charsCaculated;
}

int NEWUTF8Decoder::Convert_internal(abyte* bytes, int byteCount, Char chars[2])
{
	UTF16 temp[2];
	if (!chars)
	{
		chars = reinterpret_cast<Char*>(temp);
	}

	const UTF8* sourceStart = reinterpret_cast<const UTF8*> (bytes);
	const UTF8* sourceEnd = reinterpret_cast<const UTF8*> (bytes + byteCount);

	UTF16* targetU16 = reinterpret_cast<UTF16*>(chars);
	UTF16* targetEndU16 = reinterpret_cast<UTF16*>(chars + 2);

	ConversionResult ret = ConvertUTF8toUTF16(
		&sourceStart, sourceEnd,
		&targetU16, targetEndU16, lenientConversion);

	int targetCount = reinterpret_cast<wchar_t*>(targetU16) - chars;
	wchar_t test = chars[0];
	if (ret == conversionOK)
		return targetCount;
	else
		return 0;
}
