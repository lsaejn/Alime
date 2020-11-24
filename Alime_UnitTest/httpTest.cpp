#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/strings/string_conversions.h>
#include <Alime/net/HttpWebRequest.h>
#include <Alime/net/HttpWebResponse.h>
#include "easyTest.h"

TEST_UNIT(HttpWebRequest)
{
	HttpWebRequest req;
	auto re=req.GetResponse();
}