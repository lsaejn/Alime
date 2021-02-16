#pragma once

#include "Alime/base/base_define.h"
#include "Alime/base/fileSystem/file_define.h"
#include "Alime/base/fileSystem/IStream.h"

#include <vector>


namespace Alime::base::System::IO
{
	class TextReader
	{
	public:
		virtual String	 ReadString(aint length);
		virtual String	 ReadLine();
		virtual String	 ReadToEnd();
		virtual int Read(std::vector<Char>& buffer, int index, int count);

		virtual Char Read() = 0;
		//virtual Char Peek() = 0;
		//virtual void Close() = 0;
	};

	class StreamReader : public TextReader
	{
	public:
		StreamReader(IStream& stream);
		StreamReader(IStream& stream, bool detectEncodingFromByteOrderMarks);
		StreamReader(IStream& stream, Encoding encoding);
		StreamReader(String path);
		StreamReader(String path, bool detectEncodingFromByteOrderMarks);
		StreamReader(String path, Encoding encoding);

		//IStream BaseStream();
		Encoding CurrentEncoding();
		bool EndOfStream();
		//virtual void Close() override;
		virtual Char Read() override;
		//virtual wchar_t Peek() override;

		IStream* stream_;
		Encoding encoding_;
	};

}

