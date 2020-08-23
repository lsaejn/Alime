#pragma once

#include <vector>
#include <string>

#include <Alime/base/fileSystem/file_define.h>
#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/IStream.h>

namespace Alime::base
{
	enum Encoding
	{
		Mbcs,
		Utf8,
		Utf16,
		Utf16BE
	};

	class File
	{
	public:
		File()=default;
		File(const FilePath& filePath);
		~File()=default;

		const FilePath& GetFilePath() const;
		bool ReadAllText(std::string& text, Encoding& encoding, bool& containsBom);
		bool ReadAllText(std::string& text) const;
		bool ReadAllLines(std::vector<std::string>& lines) const;
		std::string ReadAllText() const;

		bool WriteAllText(const PathStringTy& text);
		bool WriteAllLines(std::vector<std::string>& lines);

		bool Exists()const;
		bool Delete()const;
		bool Rename(const PathStringTy& newName)const;
		void TestEncoding(unsigned char* buffer, int size, Encoding& encoding, bool& containsBom);
	private:
		FilePath	 filePath_;
	};

}
