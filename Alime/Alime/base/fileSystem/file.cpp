#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileStream.h>




namespace Alime::base
{
	void File::TestEncoding(unsigned char* buffer, int size, Encoding& encoding, bool& containsBom)
	{
		if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
		{
			encoding = Encoding::Utf8;
			containsBom = true;
		}
		else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
		{
			encoding = Encoding::Utf16;
			containsBom = true;
		}
		else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
		{
			encoding = Encoding::Utf16BE;
			containsBom = true;
		}
		else
		{
			encoding = Encoding::Mbcs;
			containsBom = false;
		}
	}



	File::File(const FilePath& filePath)
		:filePath_(filePath)
	{
	}

	const FilePath& File::GetFilePath()const
	{
		return filePath_;
	}

	bool File::ReadAllText(std::string& text, Encoding& encoding, bool& containsBom)
	{
		std::vector<unsigned char> buffer;

		FileStream fileStream(filePath_.GetFullPath(),FileMode::Open,FileAccess::Read,FileShare::ReadWrite);
		if (!fileStream.IsAvailable())
			return false;
		if (fileStream.Size() == 0)
		{
			text = "";
			encoding = Encoding::Mbcs;
			containsBom = false;
			return true;
		}
		buffer.resize((int)fileStream.Size());
		int count = fileStream.Read(&buffer[0], buffer.size());

		TestEncoding(&buffer[0], buffer.size(), encoding, containsBom);

		int bomSize = 0;
		if (containsBom)
		{
			if (encoding == Encoding::Utf8)
				bomSize = 3;
			else if(encoding== Encoding::Utf16 || encoding == Encoding::Utf16BE)
				bomSize = 2;
			text.reserve(buffer.size()- bomSize);
		}
		for (int i = bomSize; i != buffer.size(); ++i)
		{
			text.push_back(buffer[i]);
		}
		return true;
	}

}



