#pragma once

#include <windows.h>
#include "processIterator.h"

namespace Alime::base::win32
{

    class WindowIterator
    {
    private:
        HWND hWindow;

    public:
        WindowIterator()
        {
            hWindow = ::GetTopWindow(0);
        }

        void operator++()
        {
            hWindow = ::GetNextWindow(hWindow, GW_HWNDNEXT);
        }

        bool hasNext()
        {
           return hWindow != NULL;
        }

        HWND getHWND()
        {
            return hWindow;
        }

        DWORD getThreadId()
        {
            return ::GetWindowThreadProcessId(hWindow, NULL);
        }

        DWORD getProcessId()
        {
            DWORD dwPID;
            ::GetWindowThreadProcessId(hWindow, &dwPID);
            return dwPID;
        }

        std::wstring getWindowCaption()
        {
            wchar_t buffer[1024 * 4];
            ::GetWindowTextW(hWindow, buffer, sizeof(buffer) / sizeof(buffer[0]));
            return buffer;
        }

        std::wstring getWindowClassName()
        {
            wchar_t buffer[1024 * 4];
            ::GetClassName(hWindow, buffer, sizeof(buffer) / sizeof(buffer[0]));
            return buffer;
        }

        static std::wstring getAppName(HWND hwnd)
        {
            DWORD processID;
            ::GetWindowThreadProcessId(hwnd, &processID);
            ProcessIterator pIter;
            while (pIter.hasNext())
            {
                if (pIter->th32ProcessID== processID)
                {
                    return pIter->szExeFile;
                }
                ++pIter;
            }
            return {};
        }

        static std::wstring GetWindowCaptionName(HWND hwnd)
        {
            wchar_t buffer[1024] = { 0 };
            ::GetWindowText(hwnd, buffer, sizeof(buffer) / sizeof(buffer[0]));
            return buffer;
        }

        static std::wstring GetWindowClassName(HWND hwnd)
        {
            wchar_t buffer[1024] = { 0 };
            ::GetClassName(hwnd, buffer, sizeof(buffer) / sizeof(buffer[0]));
            return buffer;
        }

    };

}
