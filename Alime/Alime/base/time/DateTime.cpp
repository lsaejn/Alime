#pragma once
#include "DateTime.h"
#include "winsock2.h"
#include "Alime/base/time/GetTimeOfDay.h"
#include <inttypes.h>//for PRId64
#include <time.h>

namespace
{
	struct YearMonthDay
	{
		int year; // [1900..2500]
		int month;  // [1..12]
		int day;  // [1..31]
	};

	int getJulianDayNumber(int year, int month, int day)
	{
		int a = (14 - month) / 12;
		int y = year + 4800 - a;
		int m = month + 12 * a - 3;
		return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
	}

	const int kDaysPerWeek = 7;
	const int kJulianDayOf1970_01_01 = getJulianDayNumber(0, 0, 0);
}


namespace Alime::base
{
	DateTime::DateTime()
		: ticks_(0)
	{
	}

	DateTime::DateTime(int64_t ticks)
		: ticks_(ticks)
	{
	}

	bool DateTime::IsEpoch() const
	{
		return ticks_ == 0;
	}

	DateTime::DateTime(const struct timeval& t)
		: ticks_(static_cast<int64_t>(t.tv_sec) * TimeSpan::kSecond + t.tv_usec * TimeSpan::kMicrosecond)
	{
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
	{
		tm t;
		t.tm_year = year;
		t.tm_mon = month;
		t.tm_mday = day;
		t.tm_hour = hour;
		t.tm_min = minute;
		t.tm_sec = second;
		ticks_ = mktime(&t)* TimeSpan::kSecond + millisecond * TimeSpan::kMillisecond;
	}

	DateTime DateTime::Now()
	{

#if  ALIME_HAS_NOT_CPP11_OR_HIGHER
		return DateTime(std::chrono::TimeSpan_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#else
		return DateTime(static_cast<int64_t>(utcmicrosecond() * TimeSpan::kMicrosecond));
#endif
	}

	void DateTime::Add(TimeSpan d)
	{
		ticks_ += d.Ticks();
	}

	void DateTime::To(struct timeval* t) const
	{
		t->tv_sec = static_cast<long>(ticks_ / TimeSpan::kSecond);
		t->tv_usec = static_cast<long>(ticks_ % TimeSpan::kSecond) / static_cast<long>(TimeSpan::kMicrosecond);
	}

	struct timeval DateTime::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	int64_t DateTime::Ticks()
	{
		return ticks_;
	}

	aint DateTime::Second()
	{
		return ticks_/Alime::TimeSpan::kSecond;
	}

	int64_t DateTime::Unix() const
	{
		return ticks_ / TimeSpan::kSecond;
	}

	int64_t DateTime::UnixNano() const
	{
		return ticks_*100;
	}

	int64_t DateTime::UnixMicro() const
	{
		return ticks_ / TimeSpan::kMicrosecond;
	}

	aint DateTime::Day()
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);
		return tm_time.tm_mday;
	}

	aint DateTime::Year()
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);
		return tm_time.tm_year;
	}

	DateTime DateTime::Date()
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm, &seconds);
		return DateTime(tm.tm_year, tm.tm_mon, tm.tm_mday);
	}

	//DateTimeKind DateTime::Kind()
	//{
	//	return 
	//}

	aint DateTime::Month()
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		gmtime_s(&tm_time, &seconds);
		return tm_time.tm_mon;
	}

	aint DateTime::Minute()
	{
		auto tm = GetTmFromTick();
		return tm.tm_min;
	}

	aint DateTime::Millisecond()
	{
		return ticks_ / TimeSpan::kMillisecond;
	}

	aint DateTime::Hour()
	{
		auto tm = GetTmFromTick();
		return tm.tm_hour;
	}

	aint DateTime::DayOfYear()
	{
		return GetTmFromTick().tm_yday;
	}

	aint DateTime::DayOfWeek()
	{
		return GetTmFromTick().tm_wday;
	}

	bool DateTime::operator< (const DateTime& rhs) const
	{
		return ticks_ < rhs.ticks_;
	}

	bool DateTime::operator==(const DateTime& rhs) const
	{
		return ticks_ == rhs.ticks_;
	}

	DateTime DateTime::operator+=(const TimeSpan& rhs)
	{
		ticks_ += rhs.Ticks();
		return *this;
	}

	DateTime DateTime::operator+(const TimeSpan& rhs) const
	{
		DateTime temp(*this);
		temp += rhs;
		return temp;
	}

	DateTime DateTime::operator-=(const TimeSpan& rhs)
	{
		ticks_ -= rhs.Ticks();
		return *this;
	}

	DateTime DateTime::operator-(const TimeSpan& rhs) const
	{
		DateTime temp(*this);
		temp -= rhs;
		return temp;
	}

	TimeSpan DateTime::operator-(const DateTime& rhs) const
	{
		return TimeSpan(ticks_ - rhs.ticks_);
	}

	//linux只能精确到毫秒
	std::string DateTime::toString() const
	{
		char buf[32] = { 0 };
		int64_t seconds = ticks_ / TimeSpan::kSecond;
		int64_t microseconds = ticks_ % TimeSpan::kMicrosecond;
		snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
		return buf;
	}

	std::string DateTime::toFormattedString(bool showMicroseconds) const
	{
		char buf[64] = { 0 };
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);

		if (showMicroseconds)
		{
			int microseconds = static_cast<int>((ticks_% TimeSpan::kSecond) / 1000);
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
		return  ticks_ > 0;
	}

	DateTime DateTime::invalid()
	{
		return DateTime();
	}

	tm DateTime::GetTmFromTick()
	{
		time_t seconds = static_cast<time_t>(ticks_ / Alime::TimeSpan::kSecond);
		struct tm tm_time;
		//gmtime_r(&seconds, &tm_time);
		gmtime_s(&tm_time, &seconds);
		return tm_time;
	}

	DateTime DateTime::Add(TimeSpan value) const
	{
		return DateTime((ticks_ + value.Ticks())/100);
	}

	DateTime DateTime::AddDays(double value)
	{
		//
		return DateTime((ticks_ + value * Alime::TimeSpan::kDay) / 100);
	}

	DateTime DateTime::AddHours(double value)
	{
		return DateTime((ticks_ + value * Alime::TimeSpan::kHour) / 100);
	}

	DateTime DateTime::AddMilliseconds(double value)
	{
		return DateTime((ticks_ + value * Alime::TimeSpan::kMillisecond) / 100);
	}

	DateTime DateTime::AddMinutes(double value)
	{
		return DateTime((ticks_ + value * Alime::TimeSpan::kMinute) / 100);
	}

	DateTime DateTime::AddMonths(int months)
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		gmtime_s(&tm_time, &seconds);
		
		
		return DateTime(tm_time.tm_year+ (tm_time.tm_mon + months) / 12
			, (tm_time.tm_mon+months)%12, tm_time.tm_mday, tm_time.tm_sec);
	}

	DateTime DateTime::AddSeconds(double value)
	{
		return DateTime(ticks_ + value * Alime::TimeSpan::kSecond);
	}

	DateTime DateTime::AddTicks(long value)
	{
		return DateTime(ticks_ + value);
	}

	DateTime DateTime::AddYears(int value)
	{
		time_t seconds = static_cast<time_t>(ticks_ / TimeSpan::kSecond);
		struct tm tm_time;
		gmtime_s(&tm_time, &seconds);
		return DateTime(tm_time.tm_year + value
			, tm_time.tm_mon, tm_time.tm_mday, tm_time.tm_sec).operator+(Alime::TimeSpan(seconds% TimeSpan::kSecond));
	}

	int DateTime::CompareTo(DateTime value)
	{
		return ticks_ - value.ticks_;
	}

	bool DateTime::Equals(DateTime value)
	{
		return ticks_ == value.ticks_;
	}

	DateTime DateTime::Subtract(TimeSpan value)
	{
		//fix me
		return DateTime(ticks_ - value.Ticks());
	}

	TimeSpan DateTime::Subtract(DateTime value)
	{
		//fix me
		return TimeSpan(ticks_ - value.Ticks());
	}

	long DateTime::ToFileTime()
	{
		return ticks_;
	}

	long DateTime::ToFileTimeUtc()
	{
		return ticks_;
	}

	DateTime DateTime::ToLocalTime()
	{
		return {};
	}

	String DateTime::ToLongDateString()
	{
		return {};
	}

	String DateTime::ToLongTimeString()
	{
		return {};
	}
}