#pragma once
#include "Timestamp.h"
#include "winsock2.h"
#include "Alime/base/time/GetTimeOfDay.h"
#include <inttypes.h>//for PRId64
#include <time.h>

namespace Alime::base
{
	DateTime::DateTime()
		: ns_(0) {}

	DateTime::DateTime(int64_t nanoseconds)
		: ns_(nanoseconds) {}

	bool DateTime::IsEpoch() const {
		return ns_ == 0;
	}

	DateTime::DateTime(const struct timeval& t)
		: ns_(static_cast<int64_t>(t.tv_sec) * Duration::kSecond + t.tv_usec * Duration::kMicrosecond) {}

	DateTime DateTime::Now()
	{

#if  ALIME_HAS_NOT_CPP11_OR_HIGHER
		return DateTime(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#else
		return DateTime(static_cast<int64_t>(utcmicrosecond() * Duration::kMicrosecond));
#endif
	}

	void DateTime::Add(Duration d) {
		ns_ += d.toNanoseconds();
	}

	void DateTime::To(struct timeval* t) const {
		t->tv_sec = static_cast<long>(ns_ / Duration::kSecond);
		t->tv_usec = static_cast<long>(ns_ % Duration::kSecond) / static_cast<long>(Duration::kMicrosecond);
	}

	struct timeval DateTime::TimeVal() const {
		struct timeval t;
		To(&t);
		return t;
	}

	int64_t DateTime::Unix() const {
		return ns_ / Duration::kSecond;
	}

	int64_t DateTime::UnixNano() const {
		return ns_;
	}

	int64_t DateTime::UnixMicro() const {
		return ns_ / Duration::kMicrosecond;
	}

	bool DateTime::operator< (const DateTime& rhs) const {
		return ns_ < rhs.ns_;
	}

	bool DateTime::operator==(const DateTime& rhs) const {
		return ns_ == rhs.ns_;
	}

	DateTime DateTime::operator+=(const Duration& rhs) {
		ns_ += rhs.toNanoseconds();
		return *this;
	}

	DateTime DateTime::operator+(const Duration& rhs) const {
		DateTime temp(*this);
		temp += rhs;
		return temp;
	}

	DateTime DateTime::operator-=(const Duration& rhs) {
		ns_ -= rhs.toNanoseconds();
		return *this;
	}

	DateTime DateTime::operator-(const Duration& rhs) const {
		DateTime temp(*this);
		temp -= rhs;
		return temp;
	}

	Duration DateTime::operator-(const DateTime& rhs) const {
		int64_t ns = ns_ - rhs.ns_;
		return Duration(ns);
	}

	//linux只能精确到毫秒
	std::string DateTime::toString() const
	{
		char buf[32] = { 0 };
		int64_t seconds = ns_ / Duration::kSecond;
		int64_t microseconds = ns_ % Duration::kMicrosecond;
		snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
		return buf;
	}

	std::string DateTime::toFormattedString(bool showMicroseconds) const
	{
		char buf[64] = { 0 };
		time_t seconds = static_cast<time_t>(ns_ / Duration::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);

		if (showMicroseconds)
		{
			int microseconds = static_cast<int>((ns_% Duration::kSecond) / 1000);
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
		}
		else
		{
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		}
		return buf;
	}

	bool DateTime::valid() const
	{
		return  ns_ > 0;
	}

	DateTime DateTime::invalid()
	{
		return DateTime();
	}
}