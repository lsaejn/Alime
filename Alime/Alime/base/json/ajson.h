#pragma once
#include <windows.h>
#include <Alime/base/details/string_constants.h>

enum JsonType
{
	JSON_NULL,
	JSON_FALSE,
	JSON_TRUE,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
	JSON_UNKNOW
};

struct JsonContext
{
	const char* start;
	const char* cur;
	int row;
	int colunmn;
};

class AlimeJson
{
public:
	static AlimeJson Parse(const  char* info)
	{
		JsonContext ac;
		ac.start = info;
		ac.cur = info;

		AlimeJson av;
		av.context_ = ac;
		av.type_ = JSON_UNKNOW;

		av.ParseValue();
		//av.Expect('{');
		//av.ReadKeyString();
		//av.ParseValue();
		//av.Expect('}');
		return av;
	}
public:
	JsonType type_;
	JsonContext context_;
	std::string key_;
private:
	bool IsWhiteSpace(char ch)
	{
		return Alime::base::details::IsWhitespace(ch);
	}

	std::string ReadUntil(char c)
	{
		const char* begin = context_.cur;
		while (context_.cur && *context_.cur++ != c)
		{
		}
		return std::string(begin, context_.cur-1);
	}

	void Expect(char c)
	{
		do
		{
			if (!context_.cur || *context_.cur != c)
				throw "fuck";
		} while (0);
		context_.cur++;
	}

	void ReadKeyString()
	{
		SkipWhiteSpace();
		//
		Expect('\"');
		key_=ReadUntil('\"');
		SkipWhiteSpace();
		Expect(':');
		SkipWhiteSpace();
	}

	void SkipWhiteSpace()
	{
		while (context_.cur)
		{
			char ch = *context_.cur;
			
			if (IsWhiteSpace(ch))
			{
				context_.cur++;
			}
			else break;
		}
	}

	static bool StringCompare(const char* src, const char* target)
	{
		while (target && src && *target!=0)
		{
			if (*target++ != *src++)
				return false;
		}
		return true;
	}

	void ParseNullValue()
	{
		//expect("null")
		if (!StringCompare(context_.cur, "null"))
			throw "fuck";
		context_.cur += 4;
	}

	void ParseTrueValue()
	{
		//expect("null")
		if (!StringCompare(context_.cur, "true"))
			throw "fuck";
		context_.cur += 4;
	}

	void ParseFalseValue()
	{
		//expect("null")
		if (!StringCompare(context_.cur, "false"))
			throw "fuck";
		context_.cur += 5;
	}

	void ParseStringValue()
	{
		auto value=ReadUntil('\"');
	}

	void ParseIntegerValue()
	{
		//[-]. 0 | [1-9][0-9]* [.][0-9]* [eE][1-9] 
	}

	void ParseObjectValue()
	{
	}
	/*
	then we expect :
	n, t , f , " , [0-9], [, {



	*/
	void ParseValue()
	{
		SkipWhiteSpace();
		if (*context_.cur == 'n')
		{
			ParseNullValue();
			type_ = JSON_NULL;
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue();
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue();
		}
		else if (*context_.cur == '"')
		{
			ParseStringValue();
		}
		else if (*context_.cur == '[')
		{
			//ParseArrayValue();
		}
		else if (*context_.cur == '{')
		{
			ParseObjectValue();
		}
		else if (*context_.cur>=0 && *context_.cur<=9)
		{
			ParseIntegerValue();
		}
		SkipWhiteSpace();
		//if ','
	}

};

