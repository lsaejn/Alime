#pragma once

#include <vector>
#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base
{
	class FilePath
	{
	protected:
		PathStringTy fullPath_;
		void	Initialize();
	public:
		static const wchar_t Delimiter = L'\\';
		FilePath();
		FilePath(const PathStringTy& _filePath);
		FilePath(const wchar_t* _filePath);
		FilePath(const FilePath& _filePath);
		FilePath	 operator/(const PathStringTy& relativePath)const;
		~FilePath();

		FilePath GetFolder() const;
		static std::vector<PathStringTy> GetPathComponents(const PathStringTy& path);

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
		PathStringTy GetName() const;
		PathStringTy GetFullPath() const;
		PathStringTy FilePath::GetRelativePathFor(const FilePath& _filePath);
	};
}
