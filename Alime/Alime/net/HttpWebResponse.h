#pragma once
#include <vector>

#include <Alime/base/base_define.h>

class HttpWebResponse
{
	typedef std::vector<Char>	BodyBuffer;
public:
	/// <summary>Status code, like 200.</summary>
	aint				statusCode;
	/// <summary>Response body. This is a binary array using an array container to char.</summary>
	BodyBuffer			body;
	/// <summary>Returned cookie from the server.</summary>
	String				cookie;
	aint fileSize;
	HttpWebResponse();

	/// <summary>If you believe the server returns a text in UTF-8, use it to decode the body.</summary>
	/// <returns>The response body as text.</returns>
	String	GetBodyUtf8();
};