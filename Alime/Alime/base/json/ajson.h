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

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
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
		object_t* object_;
		array_t* array_v_;
		string_t* string_;
		boolean_t boolean_;
		number_integer_t number_integer_;
		number_float_t number_float_;

		json_value() = default;

		json_value(object_t* v)
			: object(v)
		{
		}

		json_value(array_t* v)
			: array_v(v)
		{
		}

		json_value(string_t* v)
			: string_(v)
		{
		}

		json_value(boolean_t v)
			: boolean_(v)
		{
		}

		json_value(number_integer_t v)
			: number_integer_(v)
		{
		}

		json_value(number_float_t v)
			: number_float_(v)
		{
		}

	};

	void FreeValueBase()
	{
		switch (type_)
		{
		case JsonType::JSON_STRING:
			delete value_.string_;
			break;
		case JsonType::JSON_ARRAY:
			delete value_.array_v_;
			break;
		case JsonType::JSON_OBJECT:
			delete value_.object_;
			break;
		default:
			break;
		}
		type_ = JsonType::JSON_UNKNOW;
	}

	AlimeJsonValueBase() = default;
	//fix me, implement swap(this, other);
	void Swap(AlimeJsonValueBase&& other)
	{
		value_ = other.value_;
		type_ = other.type_;
		other.type_ = JsonType::JSON_UNKNOW;
	}

	
	void Clone(const AlimeJsonValueBase& other)
	{
		FreeValueBase();
		value_ = other.value_;
		type_ = other.type_;
		switch (type_)
		{
		case JsonType::JSON_STRING:
			value_.string_=new string_t(*other.value_.string_);
			break;
		case JsonType::JSON_ARRAY:
			value_.array_v_ = new array_t(*other.value_.array_v_);
			break;
		case JsonType::JSON_OBJECT:
			value_.object_ = new object_t(*other.value_.object_);
			break;
		default:
			break;
		}
	}

	AlimeJsonValueBase(AlimeJsonValueBase&& other)
	{
		Swap(static_cast<AlimeJsonValueBase&&>(other));
	}

	AlimeJsonValueBase(const AlimeJsonValueBase& other)
	{
		Clone(other);
	}

	AlimeJsonValueBase operator=(const AlimeJsonValueBase& other)
	{
		Clone(other);
	}

	AlimeJsonValueBase& operator=(AlimeJsonValueBase&& other)
	{
		Swap(std::forward(other));
	}

	~AlimeJsonValueBase()
	{
		FreeValueBase();
	}

//fix me, private after unitTest?
public:
	JsonType type_;
json_value value_;
};
using AlimeJsonValue = AlimeJsonValueBase<>;


//AlimeJson hold an AlimeJsonValue
class AlimeJson
{
public:
	AlimeJson() = default;
	~AlimeJson() = default;

	AlimeJson(AlimeJson&& other)
	{
		valueBase_ = other.valueBase_;
		other.valueBase_.reset();
	}

	AlimeJson& operator=(AlimeJson&& other)
	{
		valueBase_ = other.valueBase_;
		other.valueBase_.reset();
		return *this;
	}

	//fix me, we now did not support copy 
	AlimeJson(const AlimeJson& other) = delete;
	AlimeJson& operator=(const AlimeJson& other) = delete;//

	static AlimeJson Parse(const char* info)
	{
		//assert(info != NULL);
		JsonContext jsonContext;
		jsonContext.start = info;
		jsonContext.cur = info;

		AlimeJson json;
		AlimeJsonValue* value = new AlimeJsonValue();
		json.ParseValue(jsonContext, value);
		json.valueBase_.reset(value);
		json.SkipWhiteSpace(jsonContext);
		if (jsonContext.cur[0] != '\0')
		{
			throw "bad parse";
		}
		return std::move(json); //why rvo not effect?
	}

	JsonType GetType()
	{
		return valueBase_ ? valueBase_->type_ : JsonType::JSON_UNKNOW;
	}

private:
	std::shared_ptr<AlimeJsonValue> valueBase_;

	static bool IsWhiteSpace(char ch)
	{
		return  ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
	}

	//fix me
	std::string ReadString(JsonContext& context_, bool& succ)
	{
		std::string result;

		const char* p = context_.cur;
		assert(p);
		while (true)
		{
			auto ch = *p;
			if (ch == '\\')
			{
				char v = *(++p);
				if (v == '\"' || v == '\\' || v == '/' || v == 'b' || v == 'f'
					|| v == 'n' || v == 'r' || v == 't')
				{
					result.push_back('\\');
					result.push_back(v);
					++p;
				}
				else if (v == 'u')
				{
					unsigned u;
					auto ret = ReadHex(p, &u);
					if (ret != JsonParseCode::OK)
					{
						succ = false;
						return "";
					}

					if (u >= 0xD800 && u <= 0xDBFF)
					{
						unsigned u2;
						if (*p++ != '\\' || *p++ != 'u' || JsonParseCode::OK != ReadHex(p, &u2))
						{
							succ = false;
							return "";
						}
						if (u2 < 0xDC00 || u2 > 0xDFFF)
						{
							succ = false;
							return "";
						}
						u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
						result+=unicode_to_utf8(u);
						break;
					}

				}
			}
			else if(context_.cur[0] == '\"')
			{
				context_.cur++;
				break;
			}
			else if(context_.cur[0] == '\0')
			{
				succ = false;
				return "";
			}
			else if ((unsigned char)context_.cur[0] < 0x20)
			{
				succ = false;
				return "";
			}
			else
			{
				result += context_.cur[0];
			}
		}
		
		succ = true;
		return result;//note that we skip character c
	}

	JsonParseCode Expect(char c, JsonContext& context_)
	{
		do
		{
			if (!context_.cur || *context_.cur != c)
				return JsonParseCode::INVALID_VALUE;
		} while (0);
		context_.cur++;
		return JsonParseCode::OK;
	}

	JsonParseCode ReadKeyString(JsonContext& context_, std::string* str)
	{
		assert(str);
		SkipWhiteSpace(context_);
		//
		Expect('\"', context_);
		bool succ = false;
		*str = ReadString(context_, succ);
		if (!succ)
			return JsonParseCode::INVALID_VALUE;
		//fix me, skip here?
		SkipWhiteSpace(context_);
		Expect(':', context_);
		SkipWhiteSpace(context_);
		return JsonParseCode::OK;
	}

	static void SkipWhiteSpace(JsonContext& context_)
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

	JsonParseCode ParseNullValue(JsonContext& context_)
	{
		//Expect('n', context_);
		if (!StringCompare(context_.cur, "null"))
			return JsonParseCode::INVALID_VALUE;
		context_.cur += 4;
		return JsonParseCode::OK;
	}

	JsonParseCode ParseTrueValue(JsonContext& context_)
	{
		//expect("null")
		if (!StringCompare(context_.cur, "true"))
			return JsonParseCode::INVALID_VALUE;
		context_.cur += 4;
		return JsonParseCode::OK;
	}

	JsonParseCode ParseFalseValue(JsonContext& context_)
	{
		//expect("null")
		if (!StringCompare(context_.cur, "false"))
			return JsonParseCode::INVALID_VALUE;
		context_.cur += 5;
		return JsonParseCode::OK;
	}

	JsonParseCode ParseStringValue(JsonContext& context_, std::string* str)
	{
		context_.cur++;
		bool flag = false;
		auto value=ReadString(context_, flag);
		if (!flag)
			return JsonParseCode::INVALID_VALUE;
		*str=value;
		return JsonParseCode::OK;
	}

	//这里有点恶心
	JsonParseCode ParseNumber(JsonContext& context , out AlimeJsonValue* v)
	{
		assert(v);
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
		if (errno == ERANGE &&
			(v->value_.number_float_ == HUGE_VAL || v->value_.number_float_ == -HUGE_VAL))
			return JsonParseCode::NUMBER_TOO_BIG;
		v->type_ = JsonType::JSON_NUMBER;
		context.cur = p;
		return JsonParseCode::OK;
	}

	JsonParseCode ParseObjectValue(JsonContext& context, AlimeJsonValue* v)
	{
		assert(v);
		v->value_.object_ = new AlimeJsonValue::object_t();
		SkipWhiteSpace(context);
		std::string keyString;
		//fix me, check return code here
		ReadKeyString(context, &keyString);
		SkipWhiteSpace(context);
		Expect(',', context);
		SkipWhiteSpace(context);
		AlimeJsonValue value;
		ParseValue(context, &value);
		v->value_.object_->insert({ keyString, std::move(value) });
		SkipWhiteSpace(context);
		return JsonParseCode::OK;
	}
	/*
	then we expect :
	n, t , f , " , [0-9], [, {

	*/
	JsonParseCode ParseArrayValue(JsonContext& context_, AlimeJsonValue* value)
	{
		Expect('[', context_);
		value->value_.array_v_ = new AlimeJsonValue::array_t();
		AlimeJsonValue* arrayElement = new AlimeJsonValue();
		for (;;)
		{
			
			//here check return code
			ParseValue(context_, arrayElement);
			value->value_.array_v_->push_back(std::move(*arrayElement));
			
			//assert(t == JsonType::JSON_UNKNOW);
			//if (t == JsonType::JSON_UNKNOW)
			//	t = arrayElement->type_;
			//else if (t != arrayElement->type_)
			//	return JsonParseCode::INVALID_VALUE;
			SkipWhiteSpace(context_);
			if (context_.cur[0] != ',')
				break;
			else
				Expect(',', context_);
			SkipWhiteSpace(context_);
			
		}
		delete arrayElement;
		Expect(']', context_);
		return JsonParseCode::OK;
	}

	JsonParseCode ParseValue(JsonContext& context_, AlimeJsonValue* value)
	{
		assert(value);
		SkipWhiteSpace(context_);
		if (*context_.cur == 'n')
		{
			ParseNullValue(context_);
			//if(true)
			//AlimeJsonValue *value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_NULL;
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue(context_);
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_TRUE;
			value->value_ = true;
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue(context_);
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_FALSE;
			value->value_ = true;
		}
		else if (*context_.cur == '"')
		{
			//AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_STRING;
			std::string *stringValue=new std::string();
			//fix me, check here
			ParseStringValue(context_, stringValue);
			value->value_ = stringValue;
		}
		else if (*context_.cur == '[')
		{
			ParseArrayValue(context_, value);
			value->type_ = JsonType::JSON_ARRAY;
		}
		else if (*context_.cur == '{')
		{
			ParseObjectValue(context_, value);
			value->type_ = JsonType::JSON_OBJECT;
		}
		else if (*context_.cur>='0'&& *context_.cur<='9')
		{
			value->type_ = JsonType::JSON_NUMBER;
			ParseNumber(context_, value);
		}
			
		return JsonParseCode::OK;
	}

	//read a unicode codepoint
	static JsonParseCode ReadHex(const char* &p, unsigned* u)
	{
		int i;
		*u = 0;
		for (i = 0; i < 4; i++)
		{
			char ch = *p++;
			*u <<= 4;
			if (ch >= '0' && ch <= '9')
				*u |= ch - '0';
			else if (ch >= 'A' && ch <= 'F')
				*u |= ch - ('A' - 10);
			else if (ch >= 'a' && ch <= 'f')
				*u |= ch - ('a' - 10);
			else
				return JsonParseCode::INVALID_VALUE;
		}
		return JsonParseCode::OK;
	};

	static std::string unicode_to_utf8(unsigned u)
	{
		std::string result;
		if (u <= 0x7F)
			result += char(u & 0xFF);
		else if (u <= 0x7FF)
		{
			result += 0xC0 | ((u >> 6) & 0xFF);
			result += 0x80 | (u & 0x3F);
		}
		else if (u <= 0xFFFF)
		{
			result += 0xE0 | ((u >> 12) & 0xFF);
			result += 0x80 | ((u >> 6) & 0x3F);
			result += 0x80 | (u & 0x3F);
		}
		else
		{
			assert(u <= 0x10FFFF);
			result += 0xF0 | ((u >> 18) & 0xFF);
			result += 0x80 | ((u >> 12) & 0x3F);
			result += 0x80 | ((u >> 6) & 0x3F);
			result += 0x80 | (u & 0x3F);
		}
		return result;
	}
};







