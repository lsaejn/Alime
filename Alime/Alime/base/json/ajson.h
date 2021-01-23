#pragma once
#include <windows.h>
#include <list>
#include <Alime/base/details/string_constants.h>
//我们将在子项目中完成json。然后移植过来。
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

#include <map>
#include <vector>

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
	/// a type for an object
	using object_t = ObjectType<StringType, AlimeJsonValueBase>;
	/// a type for an array_v
	using array_t = ArrayType<AlimeJsonValueBase>;
	/// a type for a string
	using string_t = StringType;
	/// a type for a boolean
	using boolean_t = BooleanType;
	/// a type for a number (integer)
	using number_integer_t = NumberIntegerType;
	/// a type for a number (floating point)
	using number_float_t = NumberFloatType;
	/// a type for list initialization
	using list_init_t = std::initializer_list<AlimeJsonValueBase>;

	union json_value
	{
		/// object (stored with pointer to save storage)
		object_t* object;
		/// array_v (stored with pointer to save storage)
		array_t* array_v;
		/// string (stored with pointer to save storage)
		string_t* string;
		/// bolean
		boolean_t boolean;
		/// number (integer)
		number_integer_t number_integer;
		/// number (floating point)
		number_float_t number_float;

		/// default constructor (for null values)
		json_value() = default;
		/// constructor for objects
		json_value(object_t* v) : object(v) {}
		/// constructor for arrays
		json_value(array_t* v) : array_v(v) {}
		/// constructor for strings
		json_value(string_t* v) : string(v) {}
		/// constructor for booleans
		json_value(boolean_t v) : boolean(v) {}
		/// constructor for numbers (integer)
		json_value(number_integer_t v) : number_integer(v) {}
		/// constructor for numbers (floating point)
		json_value(number_float_t v) : number_float(v) {}
	};


public:
	JsonType type_;
	json_value value_;
};
using  AlimeJsonValue = AlimeJsonValueBase<>;



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
						context_.cur+2;
					}

				}
			}
			context_.cur++;
		}
		return std::string(begin, context_.cur);
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
			AlimeJsonValue *value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_NULL;
			ajv_ = value;
		}
		else if (*context_.cur == 't')
		{
			ParseTrueValue(context_);
			AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_TRUE;
			value->value_ = true;
			ajv_ = value;
		}
		else if (*context_.cur == 'f')
		{
			ParseFalseValue(context_);
			AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_FALSE;
			value->value_ = true;
			ajv_ = value;
		}
		else if (*context_.cur == '"')
		{
			AlimeJsonValue* value = new AlimeJsonValue();
			value->type_ = JsonType::JSON_STRING;
			value->value_ = new std::string(ParseStringValue(context_));
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









