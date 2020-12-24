#include "DirectoryInfo.h"
namespace Alime::base::System::IO
{
	DirectoryInfo::DirectoryInfo(const String& path)
		:FileSystemInfo(path)
	{
	}

	bool DirectoryInfo::Exists() const
	{
		return FileSystemInfo::Exists();
	}

	String DirectoryInfo::FullName()
	{
		return FileSystemInfo::FullName();
	}

	DirectoryInfo DirectoryInfo::Parent()
	{
		return DirectoryInfo{ L"" };
	}

	String DirectoryInfo::Name()
	{
		return name_;
	}
}

