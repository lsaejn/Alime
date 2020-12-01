#pragma once

#include "Alime/base/base_define.h"
#include "Alime/base/fileSystem/file_define.h"

#include <vector>
class IStream;

namespace Alime::base::System::IO
{ 
	class TextReader
	{
	public:
		virtual String	 ReadString(aint length);
		virtual String	 ReadLine();
		virtual String	 ReadToEnd();
		virtual Char Read() = 0;
		// index: The position in buffer at which to begin writing.
		virtual int Read(std::vector<Char>& buffer, int index, int count);
		virtual wchar_t Peek();
		virtual void Close();
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

