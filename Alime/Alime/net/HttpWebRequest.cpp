#include "HttpWebRequest.h"
#include "HttpWebResponse.h"


#include <Windows.h>
#include <winhttp.h>

#pragma comment(lib, "WinHttp.lib")


HttpWebRequest::HttpWebRequest()
{

}

/// <summary>Set <see cref="server"/>, <see cref="port"/>, <see cref="query"/> and <see cref="secure"/> fields for you using an URL.</summary>
/// <returns>Returns true if this operation succeeded.</returns>
/// <param name="inputQuery">The URL.</param>
bool HttpWebRequest::SetHost(const String& inputQuery)
{
    server = L"";
    query = L"";
    port = 0;
    secure = false;
    {
        if (server == L"")
        {
            if (inputQuery.length() > 7)
            {
                String protocol = inputQuery.substr(0, 8);
                if (_wcsicmp(protocol.data(), L"https://") == 0)
                {
                    const wchar_t* reading = inputQuery.data() + 8;
                    const wchar_t* index1 = wcschr(reading, L':');
                    const wchar_t* index2 = wcschr(reading, L'/');
                    if (index2)
                    {
                        query = index2;
                        server = String(reading, (index1 ? index1 : index2) - reading);
                        port = INTERNET_DEFAULT_HTTPS_PORT;
                        secure = true;
                        if (index1)
                        {
                            String portString(index1 + 1, index2 - index1 - 1);
                            port = _wtoi(portString.data());
                        }
                        return true;
                    }
                }
            }
        }
        if (server == L"")
        {
            if (inputQuery.length() > 6)
            {
                String protocol = inputQuery.substr(0, 7);
                if (_wcsicmp(protocol.data(), L"http://") == 0)
                {
                    const wchar_t* reading = inputQuery.data() + 7;
                    const wchar_t* index1 = wcschr(reading, L':');
                    const wchar_t* index2 = wcschr(reading, L'/');
                    if (index2)
                    {
                        query = index2;
                        server = String(reading, (index1 ? index1 : index2) - reading);
                        port = INTERNET_DEFAULT_HTTP_PORT;
                        if (index1)
                        {
                            String portString(index1 + 1, index2 - index1 - 1);
                            port = _wtoi(portString.data());
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/// <summary>Fill the body with a text using UTF-8 encoding.</summary>
/// <param name="bodyString">The text to fill.</param>
void HttpWebRequest::SetBodyUtf8(const String& bodyString)
{

}

struct BufferPair
{
    char* buffer;
    aint		length;

    BufferPair()
        :buffer(0)
        , length(0)
    {
    }

    BufferPair(char* _buffer, aint _length)
        :buffer(_buffer)
        , length(_length)
    {
    }


    bool operator==(const BufferPair& pair) { return false; }
    bool operator!=(const BufferPair& pair) { return true; }
};

HttpWebResponse HttpWebRequest::GetResponse()
{
    HttpWebResponse response;
    response.statusCode = -1;
    HINTERNET internet = NULL;
    HINTERNET connectedInternet = NULL;
    HINTERNET requestInternet = NULL;
    BOOL httpResult = FALSE;
    DWORD error = 0;
    std::vector<LPCWSTR> acceptTypes;
    std::vector<BufferPair> availableBuffers;

    //fix me
    //IsWindows8Point1OrGreater() ? WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY: WINHTTP_ACCESS_TYPE_DEFAULT_PROXY
    //open
    auto hSession = WinHttpOpen(L"AlimeHttp", 
        WINHTTP_ACCESS_TYPE_NO_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    error = GetLastError();
    if (!internet) goto CLEANUP;
    // connect
    connectedInternet = WinHttpConnect(internet, server.data(), (int)port, 0);
    error = GetLastError();
    if (!connectedInternet) goto CLEANUP;
    // open request
    for (auint i = 0; i < acceptTypes.size(); i++)
    {
        acceptTypes.push_back(acceptTypes.at(i));
    }
    //acceptTypes.Add(0);
    requestInternet = WinHttpOpenRequest(connectedInternet, method.data(), query.data(), NULL, WINHTTP_NO_REFERER, &acceptTypes[0], (secure ? WINHTTP_FLAG_SECURE : 0));
    error = GetLastError();
    if (!requestInternet) goto CLEANUP;

    // authentication, cookie and request
    if (username != L"" && password != L"")
    {
        WinHttpSetCredentials(requestInternet, WINHTTP_AUTH_TARGET_SERVER, WINHTTP_AUTH_SCHEME_BASIC, username.data(), password.data(), NULL);
    }
    if (contentType != L"")
    {
        httpResult = WinHttpAddRequestHeaders(requestInternet, (L"Content-type:" + contentType).data(), -1, WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);
    }
    if (cookie != L"")
    {
        WinHttpAddRequestHeaders(requestInternet, (L"Cookie:" + cookie).data(), -1, WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);
    }
    // extra headers
    for (auto iter = extraHeaders.cbegin(); iter!= extraHeaders.cend(); iter++)
    {
        String key = iter->first;
        String value = iter->second;
        WinHttpAddRequestHeaders(requestInternet, (key + L":" + value).data(), -1, WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);
    }

    if (body.size() > 0)
    {
        httpResult = WinHttpSendRequest(requestInternet, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)&body[0], (int)body.size(), (int)body.size(), NULL);
    }
    else
    {
        httpResult = WinHttpSendRequest(requestInternet, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, NULL);
    }
    error = GetLastError();
    if (httpResult == FALSE) goto CLEANUP;
    // receive response
    httpResult = WinHttpReceiveResponse(requestInternet, NULL);
    error = GetLastError();
    if (httpResult != TRUE) goto CLEANUP;
    // read response status code
    {
        DWORD headerLength = sizeof(DWORD);
        DWORD statusCode = 0;
        httpResult = WinHttpQueryHeaders(requestInternet, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &headerLength, WINHTTP_NO_HEADER_INDEX);
        error = GetLastError();
        if (httpResult == FALSE) goto CLEANUP;
        response.statusCode = statusCode;
    }
    // read respons cookie
    {
        DWORD headerLength = sizeof(DWORD);
        httpResult = WinHttpQueryHeaders(requestInternet, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &headerLength, WINHTTP_NO_HEADER_INDEX);
        error = GetLastError();
        if (error == ERROR_INSUFFICIENT_BUFFER)
        {
            wchar_t* rawHeader = new wchar_t[headerLength / sizeof(wchar_t)];
            ZeroMemory(rawHeader, headerLength);
            httpResult = WinHttpQueryHeaders(requestInternet, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, rawHeader, &headerLength, WINHTTP_NO_HEADER_INDEX);

            const wchar_t* cookieStart = wcsstr(rawHeader, L"Cookie:");
            if (cookieStart)
            {
                const wchar_t* cookieEnd = wcsstr(cookieStart, L";");
                if (cookieEnd)
                {
                    response.cookie = String(cookieStart + 7, cookieEnd - cookieStart - 7);
                }
            }
            delete[] rawHeader;
        }
    }

    // read response body
    DWORD bytesAvailable = 0;
    do
    {
        BOOL bResult = WinHttpQueryDataAvailable(requestInternet, &bytesAvailable);
        error = GetLastError();
        if (bResult && bytesAvailable != 0)
        {
            char* utf8 = new char[bytesAvailable];
            DWORD bytesRead = 0;
            BOOL readDataResult = WinHttpReadData(requestInternet, utf8, bytesAvailable, &bytesRead);
            error = GetLastError();
            if (readDataResult == TRUE)
            {
                availableBuffers.push_back(BufferPair(utf8, bytesRead));
            }
            else
            {
                delete[] utf8;
            }
        }
        else
        {
            break;
        }
    } while (bytesAvailable > 0);

    {
        // concatincate response body
        aint totalSize = 0;
        for(auto& p : availableBuffers)
        {
            totalSize += p.length;
        }
        response.body.resize(totalSize);
        if (totalSize > 0)
        {
            char* utf8 = new char[totalSize];
            {
                char* temp = utf8;
                for(auto & p: availableBuffers)
                {
                    memcpy(temp, p.buffer, p.length);
                    temp += p.length;
                }
            }
            memcpy(&response.body[0], utf8, totalSize);
            delete[] utf8;
        }

    }

CLEANUP:
    if (requestInternet) WinHttpCloseHandle(requestInternet);
    if (connectedInternet) WinHttpCloseHandle(connectedInternet);
    if (internet) WinHttpCloseHandle(internet);
    return response;
}