#pragma once
#include <windows.h>

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

struct AjsonContext
{
	const char* content_;

};

class Ajson
{
public:
	static Ajson Parse(const  char* info)
	{
		AjsonContext ac;
		ac.content_ = info;

		Ajson av;
		av.context_ = ac;
		av.type_ = JSON_UNKNOW;

		av.type_ = JSON_NULL;
		av.SkipWhiteSpace();
		av.ParseValue();
		return av;
	}
public:
	JsonType type_;
	AjsonContext context_;
private:
	void SkipWhiteSpace()
	{

	}

	void ParseNullValue()
	{
		GetTickCount64()
	}
	/*
	then we expect :
	n, t , f , " , [0-9], [, {



	*/
	void ParseValue()
	{

	}

};

