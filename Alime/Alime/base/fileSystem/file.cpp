#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileStream.h>

#include "windows.h"


namespace Alime::base::System::IO
{
	//void File::TestEncoding(unsigned char* buffer, int size, Encoding& encoding, bool& containsBom)
	//{
	//	if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
	//	{
	//		encoding = Encoding::Utf8;
	//		containsBom = true;
	//	}
	//	else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
	//	{
	//		encoding = Encoding::Utf16;
	//		containsBom = true;
	//	}
	//	else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
	//	{
	//		encoding = Encoding::Utf16BE;
	//		containsBom = true;
	//	}
	//	else
	//	{
	//		encoding = Encoding::Mbcs;
	//		containsBom = false;
	//	}
	//}



	//File::File(const FilePath& filePath)
	//	:filePath_(filePath)
	//{
	//}

	//const FilePath& File::GetFilePath()const
	//{
	//	return filePath_;
	//}

	/*bool File::ReadAllText(String& text, Encoding& encoding, bool& containsBom)
	{
		std::vector<unsigned char> buffer;

		FileStream fileStream(filePath_.GetFullPath(),FileMode::Open,FileAccess::Read,FileShare::ReadWrite);
		if (!fileStream.IsAvailable())
			return false;
		if (fileStream.Size() == 0)
		{
			text = L"";
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
	}*/

	void File::AppendAllText(String path, String contents)
	{

	}

	void File::AppendAllText(String path, String contents, Encoding encoding)
	{

	}

	void File::Copy(String sourceFileName, String destFileName)
	{

	}

	void File::Copy(String sourceFileName, String destFileName, bool overwrite)
	{

	}

	FileStream File::Create(String path)
	{
		return {L""};
	}

	FileStream File::Create(String path, int bufferSize)
	{
		return { L"" };
	}

	void File::Decrypt(String path)
	{

	}

	bool File::Delete(String path)
	{
		if (::DeleteFileW(path.c_str()) != 0)
			return true;
		return false;
	}

	void File::Encrypt(String path)
	{

	}

	bool File::Exists(String path)
	{
		const DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			return true;
		}
		return false;
	}

}



