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

class AlimeJsonValue
{
public:
	JsonType type_;
};

class AlimeJsonNull : public AlimeJsonValue
{
public:
	JsonType type_ = JSON_NULL;
};

class AlimeJsonFalse : public AlimeJsonValue
{
public:
	AlimeJsonFalse()
	{
		type_ = JSON_FALSE;
	}
	bool value_ = false;//debug
};

class AlimeJsonTrue : public AlimeJsonValue
{
public:
	AlimeJsonTrue()
	{
		type_ = JSON_TRUE;
	}

	bool value_ = true;//debug
};

class AlimeJsonString : public AlimeJsonValue
{
public:
	AlimeJsonString(std::string v)
		:value_(std::move(v))
	{
		type_ = JSON_STRING;
	}

	std::string value_;
};

#include <list>
class AlimeJsonArray : public AlimeJsonValue
{
public:
	JsonType type_ = JSON_ARRAY;
	std::list<std::string> values_;
};

class AlimeJsonUnknow :public AlimeJsonValue
{
public:
	JsonType type_ = JSON_UNKNOW;
};

class AlimeJsonObject :public AlimeJsonValue
{
public:
	JsonType type_ = JSON_OBJECT;
};


class AlimeJsonMember : public AlimeJsonValue
{
public:
	JsonType type_ = JSON_NUMBER;
	std::string key_;
	AlimeJsonValue* value;
};

class AlimeJsonMemberManager : public AlimeJsonValue
{
	std::list< AlimeJsonMember*> members_;
};


class AlimeJson
{
public:
	AlimeJson() = default;
	AlimeJson(AlimeJson&& other)
	{
		ajv_ = other.ajv_;
		other.ajv_ = nullptr;
	}

	~AlimeJson()
	{
		if(ajv_)
			delete ajv_;
	}

	AlimeJson& operator=(AlimeJson&& other)
	{
		ajv_ = other.ajv_;
		other.ajv_ = nullptr;
		return *this;
	}

	AlimeJson(const AlimeJson& other) = delete;//we delete this until we implement copy()
	AlimeJson& operator=(const AlimeJson& other) = delete;//

	static AlimeJson Parse(const char* info)
	{
		JsonContext jsonContext;
		jsonContext.start = info;
		jsonContext.cur = info;

		AlimeJson aj;
		aj.ParseValue(jsonContext);
		return std::move(aj); //why rvo not effect?
	}

	JsonType GetType()
	{
		return ajv_->type_;
	}

private:
	AlimeJsonValue* ajv_=nullptr;

	bool IsWhiteSpace(char ch)
	{
		return Alime::base::details::IsWhitespace(ch);
	}

	std::string ReadUntil(char c, JsonContext& context_)
	{
		const char* begin = context_.cur;
		while (context_.cur)
		{
			if (*context_.cur == '\\')
			{
				if (*(context_.cur + 1) == 'u')
				{
					//parseHex();
				}
				else
				{
					auto c = *(context_.cur + 1);
					if(c== '\"' || c== '\\' || c== '/' || c== 'b' || c=='f'
						|| c== 'n' || c== 'r' || c== 't')
					{		
						context_.cur+2;
					}

				}
			}
			context_.cur++;
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

	std::string ParseStringValue(JsonContext& context_)
	{
		context_.cur++;
		auto value=ReadUntil('\"', context_);
		return value;
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
			//if(true)
			AlimeJsonValue *value = new AlimeJsonNull();
			ajv_ = value;
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue(context_);
			AlimeJsonValue* value = new AlimeJsonTrue();
			ajv_ = value;
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue(context_);
			AlimeJsonValue* value = new AlimeJsonFalse();
			ajv_ = value;
		}
		else if (*context_.cur == '"')
		{
			AlimeJsonValue* value = new AlimeJsonString(ParseStringValue(context_));
			ajv_ = value;
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









