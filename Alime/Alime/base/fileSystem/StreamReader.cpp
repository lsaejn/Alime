#include "StreamReader.h"



namespace Alime::base::System::IO
{
	StreamReader::StreamReader(String path)
	{

	}

	StreamReader::StreamReader(IStream& stream, Encoding encoding)
		:stream_(&stream),
		encoding_(encoding)
	{

	}

	StreamReader::StreamReader(IStream& stream)
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
}
