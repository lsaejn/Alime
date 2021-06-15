#pragma once

#include "Alime/base/base_define.h"

namespace Alime::details
{
	bool CreateFileImpl(const String& path)
	{
		if (path.empty())
			throw L"fix me";

		HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return true;
		}
		return false;
	}
}
