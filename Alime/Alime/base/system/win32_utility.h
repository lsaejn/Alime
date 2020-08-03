#pragma once
#include "windows.h"

namespace Alime::base::win32
{
	bool CreateProcessWithCommand(const wchar_t* application, const wchar_t* command, HANDLE* process, DWORD*);

	size_t GetMonitorCount();
}


