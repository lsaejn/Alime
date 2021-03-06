#pragma once

#include <vector>
#include <Alime/base/base_define.h>
#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base::System::IO
{
	/*
	inner class now
	*/
	class FilePath
	{
	public:
		static Char Delimiter;
		FilePath();
		FilePath(const String& filePath);
		FilePath(const wchar_t* filePath);
		FilePath(const FilePath& filePath);
		FilePath& operator=(const FilePath& filePath);

		FilePath	 operator/(const String& relativePath)const;
		~FilePath();

		FilePath GetFolder() const;
		static std::vector<String> GetPathComponents(const String& path);

		static int Compare(const FilePath& a, const FilePath& b);
		bool	operator==(const FilePath& filePath)const { return Compare(*this, filePath) == 0; }
		bool	operator!=(const FilePath& filePath)const { return Compare(*this, filePath) != 0; }
		bool	operator< (const FilePath& filePath)const { return Compare(*this, filePath) < 0; }
		bool	operator<=(const FilePath& filePath)const { return Compare(*this, filePath) <= 0; }
		bool	operator> (const FilePath& filePath)const { return Compare(*this, filePath) > 0; }
		bool	operator>=(const FilePath& filePath)const { return Compare(*this, filePath) >= 0; }

		bool	IsFile() const;
		bool	IsFolder() const;
		bool	IsRoot() const;
		String GetName() const;
		String GetFullPath() const;
		String GetRelativePathFor(const FilePath& _filePath);
	protected:
		String fullPath_;
		void	Initialize();
	};
}
