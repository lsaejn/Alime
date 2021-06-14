#include "StreamReader.h"

#include "Alime/base/details/CharEncode.h"
#include "Alime/base/fileSystem/FileStream.h"



namespace Alime::base::System::IO
{
	String TextReader::ReadString(aint length)
	{
		wchar_t* buffer = new wchar_t[length + 1];
		aint i = 0;
		for (; i < length; i++)
		{
			if ((buffer[i] = Read()) == L'\0')
			{
				break;
			}
		}
		buffer[i] = L'\0';
		String result(buffer);
		delete[] buffer;
		return result;
	}

	String TextReader::ReadLine()
	{
		String result;
		auto buffer = new wchar_t[65537];
		buffer[0] = L'\0';
		aint i = 0;
		while (true)
		{
			wchar_t c = Read();
			if (c == L'\n' || c == L'\0')
			{
				buffer[i] = L'\0';
				result += buffer;
				buffer[0] = L'\0';
				i = 0;
				break;
			}
			else
			{
				if (i == 65536)
				{
					buffer[i] = L'\0';
					result += buffer;
					buffer[0] = L'\0';
					i = 0;
				}
				buffer[i++] = c;
			}
		}
		result += buffer;
		delete[] buffer;
		if (result.length() > 0 && result[result.length() - 1] == L'\r')
		{
			return result.substr(result.length() - 1);
		}
		else
		{
			return result;
		}
	}

	String TextReader::ReadToEnd()
	{
		String result;
		auto buffer = new wchar_t[65537];
		buffer[0] = L'\0';
		aint i = 0;
		while (true)
		{
			wchar_t c = Read();
			if (c == L'\0')
			{
				buffer[i] = L'\0';
				result += buffer;
				buffer[0] = L'\0';
				i = 0;
				break;
			}
			else
			{
				if (i == 65536)
				{
					buffer[i] = L'\0';
					result += buffer;
					buffer[0] = L'\0';
					i = 0;
				}
				buffer[i++] = c;
			}
		}
		result += buffer;
		delete[] buffer;
		return result;
	}

	int TextReader::Read(std::vector<Char>& target, int index, int count)
	{
		if (index < 0 || count<=0)
			return 0;

		String result;
		aint bufferLen = count > 65537 ? 65537 : count;
		auto buffer = new wchar_t[bufferLen];
		aint i = index;//index=3 0-1-2
		while (i>0)//or seek?
		{
			wchar_t c = Read();
			if (c == L'\0')
			{
				delete buffer;
				return 0;
			}
			else
				--i;
		}
		while (true)
		{
			wchar_t c = Read();
			if (c == L'\0')
				return i;
			else
			{
				if (i == count)
				{
					break;
				}
				target.push_back(c);
				i++;
			}
		}
		delete buffer;
		return i;
	}

	StreamReader::StreamReader(String path)
	{
		stream_ = new FileStream(path);
		streamHolder_.reset(stream_);
		auto bomEncoder = new BomDecoder();
		bomEncoder->Setup(stream_);
		innerStream_.reset(bomEncoder);
	}

	StreamReader::StreamReader(IStream& stream, Encoding encoding)
		:stream_(&stream),
		encoding_(encoding)
	{
		IDecoder* bomEncoder = new BomDecoder();
		bomEncoder->Setup(stream_);
		innerStream_.reset(bomEncoder);
	}

	StreamReader::StreamReader(IStream& stream)
		: StreamReader(stream, Encoding::Utf8)
	{

	}

	StreamReader::StreamReader(IStream& stream, bool detectEncodingFromByteOrderMarks)
	{

	}

	StreamReader::StreamReader(String path, bool detectEncodingFromByteOrderMarks)
	{

	}

	StreamReader::StreamReader(String path, Encoding encoding)
	{

	}

	Char StreamReader::Read()
	{
		if (stream_)
		{
			Char buffer = 0;
			if (stream_->Read(&buffer, sizeof(buffer)) == 0)
			{
				stream_ = 0;
				return L'\0';
			}
			else
			{
				return buffer;
			}
		}
		else
		{
			return L'\0';
		}
	}
}
