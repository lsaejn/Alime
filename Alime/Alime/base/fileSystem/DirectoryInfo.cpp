#include "DirectoryInfo.h"

#include <Alime/base/build_config.h>
namespace Alime::base::System::IO
{
	DirectoryInfo::DirectoryInfo(const String& path)
		:FileSystemInfo(path)
	{
	}

	bool DirectoryInfo::Exists() const
	{
		return filePath_.IsFolder();
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

	DirectoryInfo DirectoryInfo::Root()
	{
#ifdef OS_WIN
		auto fp = filePath_.GetFullPath();
		//assert fp[1]==L':';
		auto index=fp.find(':');
		if (index == -1)
			throw "fuckkkkk";
		return DirectoryInfo(fp.substr(0, index+1)+L"\\");
#else
		return DirectoryInfo("/");
#endif // OS_WIN
	}

	bool DirectoryInfo::Create(bool recursively)
	{
		if (recursively)
		{
			auto folder = filePath_.GetFolder();
			if (folder.IsFile())
				return false;
			if (folder.IsFolder())
				return Create(false);
			return DirectoryInfo(folder.GetFullPath()).Create(true) && Create(false);
		}
		else
		{
#if defined OS_WIN
			return CreateDirectory(filePath_.GetFullPath().data(), NULL);
#else
			String path = wtoa(filePath_.GetFullPath());
			return mkdir(path.data(), 0777) == 0;
#endif
		}
	}

	void DirectoryInfo::Create()
	{
		if (!Create(true))
		{
			throw "create failed";
		}
	}

	DirectoryInfo DirectoryInfo::CreateSubdirectory(String path)
	{
		if (!Exists())
		{
			throw "";
		}
		DirectoryInfo subDir(filePath_.GetFullPath() + path);
		bool successed=subDir.Create(true);
		if (successed)
			return subDir;
		else
			throw "";//值语义的代码没法写。。
	}

	void DirectoryInfo::Delete(bool recursive)
	{
		if (!Exists())
			return;

		if (recursive)
		{
			//delete sub Files here
			return Delete(false);
		}
#if defined OS_WIN
		RemoveDirectory(filePath_.GetFullPath().data());
#else
		return rmdir(filePath_.GetFullPath().data()) == 0;
#endif
	}

	void DirectoryInfo::Delete()
	{
	}

}

