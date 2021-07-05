#pragma once
#include "Encoder.h"
#include "Decoder.h"

#include <memory>

class NEWEncoding
{
public:
	//static std::shared_ptr<Encoding> UTF8();
	//static std::shared_ptr<Encoding> UTF7();
	//static std::shared_ptr<Encoding> UTF32();
	//static std::shared_ptr<Encoding> Unicode();
	//static std::shared_ptr<Encoding> BigEndianUnicode();
	//static std::shared_ptr<Encoding> ASCII();
	//static std::shared_ptr<Encoding> Default();

	virtual std::shared_ptr<NEWDecoder> GetDecoder() = 0;
	virtual std::shared_ptr<NEWEncoder> GetEncoder() = 0;
};

class NEWUTF8Encoding : public NEWEncoding
{
public:
	virtual std::shared_ptr<NEWDecoder> GetDecoder() override;
	virtual std::shared_ptr<NEWEncoder> GetEncoder() override;
};

