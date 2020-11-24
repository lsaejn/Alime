#pragma once

#include <vector>
#include <list>
#include <map>

#include <Alime/base/base_define.h>

class HttpWebResponse;

class HttpWebRequest
{
	typedef std::vector<Char>	BodyBuffer;
	typedef std::list<String>	StringList;
	typedef std::map<String, String>	HeaderMap;
public:
	/// <summary>Name of the server, like "gaclib.net".</summary>
	String server;

	/// <summary>Query of the request, like "/GettingStart.html".</summary>
	String query;
	/// <summary>Set to true if the request uses SSL.</summary>
	bool	secure;
	/// <summary>User name to authorize. Set to empty if you don't want to provide it.</summary>
	String	username;
	/// <summary>Password to authorize. Set to empty if you don't want to provide it.</summary>
	String	password;
	/// <summary>HTTP method, like "GET", "POST", "PUT", "DELETE", etc.</summary>
	String	method;
	/// <summary>Cookie. Set to empty if you don't want to provide it.</summary>
	String	cookie;
	/// <summary>Content type, like "text/xml".</summary>
	String	contentType;
	/// <summary>Request body. This is a binary array using an array container to char.</summary>
	BodyBuffer	body;
	/// <summary>Accept type list, elements of it like "text/xml".</summary>
	StringList	acceptTypes;
	/// <summary>Port of the server, like 80.</summary>
	aint	port;
	/// <summary>A dictionary to contain extra headers.</summary>
	HeaderMap	extraHeaders;

	/// <summary>Create an empty request.</summary>
	HttpWebRequest();

	/// <summary>Set <see cref="server"/>, <see cref="port"/>, <see cref="query"/> and <see cref="secure"/> fields for you using an URL.</summary>
	/// <returns>Returns true if this operation succeeded.</returns>
	/// <param name="inputQuery">The URL.</param>
	bool				SetHost(const String& inputQuery);

	/// <summary>Fill the body with a text using UTF-8 encoding.</summary>
	/// <param name="bodyString">The text to fill.</param>
	void				SetBodyUtf8(const String& bodyString);

	HttpWebResponse GetResponse();
};



