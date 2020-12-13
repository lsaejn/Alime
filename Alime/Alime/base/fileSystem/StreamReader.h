#pragma once

#include "Alime/base/base_define.h"
#include "Alime/base/fileSystem/file_define.h"

#include <vector>
struct IStream;

namespace Alime::base::System::IO
{
	class TextReader
	{
	public:
		virtual String	 ReadString(aint length)=0;
		virtual String	 ReadLine() = 0;
		virtual String	 ReadToEnd() = 0;
		virtual Char Read() = 0;
		// index: The position in buffer at which to begin writing.
		virtual int Read(std::vector<Char>& buffer, int index, int count) = 0;
		virtual wchar_t Peek() = 0;
		virtual void Close() = 0;
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

		IStream BaseStream();
		Encoding CurrentEncoding();
		bool EndOfStream();
		virtual void Close() override;
		virtual String	 ReadString(aint length) override;
		virtual String	 ReadLine() override;
		virtual String	 ReadToEnd() override;
		virtual Char Read() override;
		virtual int Read(std::vector<Char>& buffer, int index, int count) override;
		virtual wchar_t Peek() override;
	};

}

