#include "NEWEncoding.h"


//fix me, a factory to make it sealed
std::shared_ptr<NEWDecoder> NEWUTF8Encoding::GetDecoder()
{
	return std::make_shared<NEWUTF8Decoder>();
}

std::shared_ptr<NEWEncoder> NEWUTF8Encoding::GetEncoder()
{
	// fix me
	return {};
}
