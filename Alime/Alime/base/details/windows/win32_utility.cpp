#include "win32_utility.h"

namespace Alime::base::win32
{

	bool CreateProcessWithCommand(const wchar_t* application, const wchar_t* command, HANDLE* process, DWORD* pid)
	{
		PROCESS_INFORMATION pi;
		STARTUPINFOW si;

		memset(&si, 0, sizeof(si));
		si.dwFlags = 0;
		si.cb = sizeof(si);
		si.hStdInput = NULL;
		si.hStdOutput = NULL;
		si.hStdError = NULL;

		wchar_t* command_dup = _wcsdup(command);

		if (::CreateProcessW(application,
			command_dup,
			NULL,
			NULL,
			(si.dwFlags & STARTF_USESTDHANDLES) ? TRUE : FALSE,
			NULL,
			NULL,
			NULL,
			&si,
			&pi))
		{
			::CloseHandle(pi.hThread);
			if (process == NULL)
				::CloseHandle(pi.hProcess);
			else
			{
				*process = pi.hProcess;
				if(pid)
					*pid = pi.dwProcessId;
			}
			free(command_dup);
			return true;
		}
		free(command_dup);
		return false;
	}

	size_t GetMonitorCount()
	{
		return GetSystemMetrics(SM_CMONITORS);
	}

}