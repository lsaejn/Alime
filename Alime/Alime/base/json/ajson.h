#pragma once
#include <windows.h>
#include <list>
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

class AlimeJsonObject
{

};

class AlimeJsonMember;

class AlimeJsonMemberManager : public AlimeJsonObject
{
	std::list< AlimeJsonMember*> members_;
};


class AlimeJson : public AlimeJsonObject
{
public:
	AlimeJson() = default;

	static AlimeJson Parse(const  char* info)
	{
		JsonContext jsonContext;
		jsonContext.start = info;
		jsonContext.cur = info;

		AlimeJson av;
		av.type_ = JSON_UNKNOW;

		av.ParseValue(jsonContext);
		return av;
	}
public:
	JsonType type_;
	AlimeJsonObject* members_;

private:
	bool IsWhiteSpace(char ch)
	{
		return Alime::base::details::IsWhitespace(ch);
	}

	std::string ReadUntil(char c, JsonContext& context_)
	{
		const char* begin = context_.cur;
		while (context_.cur && *context_.cur++ != c)
		{
		}
		return std::string(begin, context_.cur-1);
	}

	void Expect(char c, JsonContext& context_)
	{
		do
		{
			if (!context_.cur || *context_.cur != c)
				throw "fuck";
		} while (0);
		context_.cur++;
	}

	void ReadKeyString(JsonContext& context_)
	{
		SkipWhiteSpace(context_);
		//
		Expect('\"', context_);
		//key_=ReadUntil('\"');
		SkipWhiteSpace(context_);
		Expect(':', context_);
		SkipWhiteSpace(context_);
	}

	void SkipWhiteSpace(JsonContext& context_)
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

	void ParseNullValue(JsonContext& context_)
	{
		//expect("null")
		if (!StringCompare(context_.cur, "null"))
			throw "fuck";
		context_.cur += 4;
	}

	void ParseTrueValue(JsonContext& context_)
	{
		//expect("null")
		if (!StringCompare(context_.cur, "true"))
			throw "fuck";
		context_.cur += 4;
	}

	void ParseFalseValue(JsonContext& context_)
	{
		//expect("null")
		if (!StringCompare(context_.cur, "false"))
			throw "fuck";
		context_.cur += 5;
	}

	void ParseStringValue(JsonContext& context_)
	{
		auto value=ReadUntil('\"', context_);
	}

	void ParseIntegerValue(JsonContext& context_)
	{
		//[-]. 0 | [1-9][0-9]* [.][0-9]* [eE][1-9] 
	}

	void ParseObjectValue(JsonContext& context_)
	{
	}
	/*
	then we expect :
	n, t , f , " , [0-9], [, {



	*/
	void ParseValue(JsonContext& context_)
	{
		SkipWhiteSpace(context_);
		if (*context_.cur == 'n')
		{
			ParseNullValue(context_);
			type_ = JSON_NULL;
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue(context_);
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue(context_);
		}
		else if (*context_.cur == '"')
		{
			ParseStringValue(context_);
		}
		else if (*context_.cur == '[')
		{
			//ParseArrayValue();
		}
		else if (*context_.cur == '{')
		{
			ParseObjectValue(context_);
		}
		else if (*context_.cur>=0 && *context_.cur<=9)
		{
			ParseIntegerValue(context_);
		}
		SkipWhiteSpace(context_);
		//if ','
	}

};



class AlimeJsonMember: public AlimeJsonObject
{
public:
	std::string key_;
	AlimeJson json;
};



