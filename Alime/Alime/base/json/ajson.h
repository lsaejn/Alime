#pragma once
#include <windows.h>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <Alime/base/details/string_constants.h>
#include <cassert>
//我们将在子项目中完成json。然后移植过来。
/*
希望像nolhmanjson一样,但少一点歧义
AlimeJson json=AlimeJson.parse("...");
AlimeJson memberValue=json[".."];//here is an copy
AlimeJson &memberValue2=json[".."];//ref
*/
/*
The following are valid json format:

"Design Patterns"
3
true
false
null
5.0
["Design Patterns", 3]
{
	"title": "Design Patterns",
	"subtitle": "Elements of Reusable Object-Oriented Software",
	"author": [
		"Erich Gamma",
		"Richard Helm",
		"Ralph Johnson",
		"John Vlissides"
	],
	"year": 2009,
	"weight": 1.8,
	"hardcover": true,
	"publisher": {
		"Company": "Pearson Education",
		"Country": "India"
	},
	"website": null
}



*/

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
//#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')

template<typename CHAR>
bool ISDIGIT(CHAR c)
{
	return (c) >= '0' && (c) <= '9';
}

template<typename CHAR>
bool ISDIGIT1TO9(CHAR c)
{
	return (c) >= '1' && (c) <= '9';
}

#define out 
#define ref

enum class JsonType
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

enum class JsonParseCode
{
	OK = 0,
	EXPECT_VALUE,
	INVALID_VALUE,
	ROOT_NOT_SINGULAR,
	NUMBER_TOO_BIG
};


struct JsonContext
{
	const char* start = nullptr;
	const char* cur = nullptr;
	int row = -1;
	int colunmn = -1;
};

template <
	template<typename U, typename V, typename... Args> class ObjectType = std::map,
	template<typename U, typename... Args> class ArrayType = std::vector,
	class StringType = std::string,
	class BooleanType = bool,
	class NumberIntegerType = int64_t,
	class NumberFloatType = double
>
class AlimeJsonValueBase
{
public:
	using object_t = ObjectType<StringType, AlimeJsonValueBase>;
	using array_t = ArrayType<AlimeJsonValueBase>;
	using string_t = StringType;
	using boolean_t = BooleanType;
	using number_integer_t = NumberIntegerType;
	using number_float_t = NumberFloatType;
	using list_init_t = std::initializer_list<AlimeJsonValueBase>;

	union json_value
	{
		object_t* object;
		array_t* array_v;
		string_t* string;
		boolean_t boolean;
		number_integer_t number_integer;
		number_float_t number_float;

		json_value() = default;
		json_value(object_t* v) : object(v) {}
		json_value(array_t* v) : array_v(v) {}
		json_value(string_t* v) : string(v) {}
		json_value(boolean_t v) : boolean(v) {}
		json_value(number_integer_t v) : number_integer(v) {}
		json_value(number_float_t v) : number_float(v) {}
	};

public:
	JsonType type_;
	json_value value_;
};
using AlimeJsonValue = AlimeJsonValueBase<>;

class AlimeJson
{
public:
	AlimeJson() = default;
	~AlimeJson() = default;

	AlimeJson(AlimeJson&& other) = default;
	AlimeJson& operator=(AlimeJson&& other) = default;

	AlimeJson(const AlimeJson& other) = delete;//we delete this until we implement copy()
	AlimeJson& operator=(const AlimeJson& other) = delete;//

	static AlimeJson Parse(const char* info)
	{
		//assert(info != NULL);
		JsonContext jsonContext;
		jsonContext.start = info;
		jsonContext.cur = info;

		AlimeJson json;
		AlimeJsonValue *value=new AlimeJsonValue();
		json.ParseValue(value, jsonContext);
		json.SkipWhiteSpace(jsonContext);
		if (jsonContext.cur[0] != '\0')
		{
			value->type_ = JsonType::JSON_UNKNOW;
			throw "bad parse";
		}
		return std::move(json); //why rvo not effect?
	}

	JsonType GetType()
	{
		return ajv_->type_;
	}

private:
	std::shared_ptr<AlimeJsonValue> ajv_;

	bool IsWhiteSpace(char ch)
	{
		return Alime::base::details::IsWhitespace(ch);
	}

	std::string ReadUntil(char c, JsonContext& context_, void* filterFunction=nullptr)
	{
		const char* begin = context_.cur;
		while (context_.cur && *context_.cur != c)
		{
			context_.cur++;
		}
		while (0 &&context_.cur)
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
						context_.cur+=2;
					}

				}
			}
			context_.cur++;
		}
		std::string ret(begin, context_.cur);
		context_.cur++;
		return ret;
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

	//这里有点恶心
	JsonParseCode ParseNumber(JsonContext& context , out AlimeJsonValue* v)
	{
		const char* p = context.cur;
		if (*p == '-')
			p++;
		if (*p == '0')
			p++;
		else
		{
			if (!ISDIGIT1TO9(*p))
				return JsonParseCode::INVALID_VALUE;
			for (p++; ISDIGIT(*p);p++)
				;
		}
		if (*p == '.')
		{
			p++;
			if (!ISDIGIT(*p))
				return JsonParseCode::INVALID_VALUE;
			for (p++; ISDIGIT(*p); p++)
				;
		}
		if (*p == 'e' || *p == 'E')
		{
			p++;
			if (*p == '+' || *p == '-') p++;
			if (!ISDIGIT(*p)) return JsonParseCode::INVALID_VALUE;
			for (p++; ISDIGIT(*p); p++)
				;
		}
		errno = 0;
		v->value_ = strtod(context.cur, NULL);
		if (errno == ERANGE && (v->value_.number_float == HUGE_VAL || v->value_.number_float == -HUGE_VAL))
			return JsonParseCode::NUMBER_TOO_BIG;
		v->type_ = JsonType::JSON_NUMBER;
		context.cur = p;
		return JsonParseCode::OK;
	}

	JsonParseCode ParseObjectValue(JsonContext& context_)
	{
		return JsonParseCode::INVALID_VALUE;
	}
	/*
	then we expect :
	n, t , f , " , [0-9], [, {

	*/
	JsonParseCode ParseArrayValue(JsonContext& context_, AlimeJsonValue* value)
	{
		Expect('[', context_);
		value->value_.array_v = new AlimeJsonValue::array_t();
		JsonType t = JsonType::JSON_UNKNOW;
		for (;;)
		{
			AlimeJsonValue* arrayElement = new AlimeJsonValue();
			//here check return code
			ParseValue(arrayElement, context_);
			value->value_.array_v->push_back(*arrayElement);
			if (t == JsonType::JSON_UNKNOW)
				t = arrayElement->type_;
			else if (t != arrayElement->type_)
				return JsonParseCode::INVALID_VALUE;
			SkipWhiteSpace(context_);
			if (context_.cur[0] != ',')
				break;
			else
				Expect(',', context_);
			SkipWhiteSpace(context_);
			
		}
		Expect(']', context_);
	}

	JsonParseCode ParseValue(AlimeJsonValue* value, JsonContext& context_)
	{
		SkipWhiteSpace(context_);
		if (*context_.cur == 'n')
		{
			ParseNullValue(context_);
			//if(true)
			//AlimeJsonValue *value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_NULL;
			ajv_.reset(value);
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue(context_);
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_TRUE;
			value->value_ = true;
			ajv_ .reset(value);
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue(context_);
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_FALSE;
			value->value_ = true;
			ajv_.reset(value);
		}
		else if (*context_.cur == '"')
		{
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_STRING;
			value->value_ = new std::string(ParseStringValue(context_));
			ajv_.reset(value);
		}
		else if (*context_.cur == '[')
		{
			ParseArrayValue(context_, value);
			value->type_ = JsonType::JSON_ARRAY;
			ajv_.reset(value);
		}
		else if (*context_.cur == '{')
		{
			ParseObjectValue(context_);
		}
		else if (*context_.cur>='0'&& *context_.cur<='9')
		{
			value->type_ = JsonType::JSON_NUMBER;
			ParseNumber(context_, value);
			ajv_.reset(value);
		}
			
		return JsonParseCode::OK;
	}

};









