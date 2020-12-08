﻿#pragma once
#include "DateTime.h"
#include "winsock2.h"
#include "Alime/base/time/GetTimeOfDay.h"
#include <inttypes.h>//for PRId64
#include <time.h>
#include "Alime/base/exceptionBase.h"
#include "Alime/base/strings/string_util.h"


namespace Alime::base::System
{
	const int kDaysInMonth[2][12] = {
		 { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		 { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
	};

	void CheckDate(int year, int month, int day,
		int hour = 0, int minute = 0, int second = 0, int millisecond = 0)
	{
		CHECK_ERROR(!((year > 0 && year < 9999)
			&& (month > 0 && month < 13)
			&& (day > 0 && day <= kDaysInMonth[DateTime::IsLeapYear(year)][month-1])
			&& (hour >= 0 && hour < 24)
			&& (second >= 0 && second < 60)
			&& (millisecond >= 0 && millisecond < 1000))
			, L"bad year");
	}

	//algorithm
	//https://blog.csdn.net/Solstice/article/details/5814486
	int getJulianDayNumber(int year, int month, int day)
	{
		int a = (14 - month) / 12;
		int y = year + 4800 - a;
		int m = month + 12 * a - 3;
		return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
	}

	struct YearMonthDay getYearMonthDay(int julianDayNumber)
	{
		int a = julianDayNumber + 32044;
		int b = (4 * a + 3) / 146097;
		int c = a - ((b * 146097) / 4);
		int d = (4 * c + 3) / 1461;
		int e = c - ((1461 * d) / 4);
		int m = (5 * e + 2) / 153;
		YearMonthDay ymd;
		ymd.day = e - ((153 * m + 2) / 5) + 1;
		ymd.month = m + 3 - 12 * (m / 10);
		ymd.year = b * 100 + d - 4800 + (m / 10);
		return ymd;
	}

	const int64_t kSecondsPerMinute = 60;
	const int64_t kSecondsPerHour = 60 * kSecondsPerMinute;
	const int64_t kSecondsPerDay = 24 * kSecondsPerHour;
	const aint kDaysPerWeek = 7;

	const aint kJulianDayOf1900_01_01 = getJulianDayNumber(1900, 1, 1);
	const aint kJulianDayOf1970_01_01 = getJulianDayNumber(1970, 1, 1);
	const aint kJulianDayOf0001_01_01 = getJulianDayNumber(1, 1, 1);

	const aint64 kTickOf1900_01_01 = (kJulianDayOf1900_01_01 - kJulianDayOf0001_01_01) * TimeSpan::kDay;
	const aint64 kTickOf1970_01_01 = (kJulianDayOf1970_01_01 - kJulianDayOf0001_01_01) * TimeSpan::kDay;

	struct YearMonthDay getYearMonthDayFromTicks(int64_t ticks)
	{
		auto days =static_cast<aint> (ticks / TimeSpan::kDay+ kJulianDayOf0001_01_01);
		return getYearMonthDay(days);
	}
	//Julian=UTC=GMT
	int64_t GetTicksFromDateInfo(int year, int month, int day, 
		int hour=0, int minute = 0,int second = 0, int millisecond = 0)
	{
		return
			(getJulianDayNumber(year, month, day) - kJulianDayOf0001_01_01)* TimeSpan::kDay
			+ hour * TimeSpan::kHour
			+ minute * TimeSpan::kMinute
			+ second * TimeSpan::kSecond
			+ millisecond * TimeSpan::kMillisecond
			/*+ nanosecond / 100*/;
	}

	int64_t GetTimetFromDateInfo(int year, int month, int day,
		int hour = 0, int minute = 0, int second = 0, int millisecond = 0)
	{
		return(getJulianDayNumber(year, month, day) - kJulianDayOf1970_01_01) * kSecondsPerDay
			+ hour * kSecondsPerHour
			+ minute * kSecondsPerMinute
			+ second;
	}
}

namespace Alime::base::System
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
	{
		ticks_ = (t.tv_sec * TimeSpan::kSecond + t.tv_usec * TimeSpan::kMicrosecond) + kTickOf1970_01_01;
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
	{
		if (day>kDaysInMonth[IsLeapYear(year)][month-1])
		{
			throw "invalid day number";
		}
		ticks_ = (getJulianDayNumber(year, month, day)- kJulianDayOf0001_01_01) * kSecondsPerDay* TimeSpan::kSecond
			+ hour*TimeSpan::kHour
			+ minute*TimeSpan::kMinute
			+ second*TimeSpan::kSecond
			+ millisecond * TimeSpan::kMillisecond;
	}

	DateTime DateTime::Now()
	{
		return DateTime(DateTime::UtcNow().ToLocalTime());
	}

	DateTime DateTime::Today()
	{
		auto date=Now();
		auto ymd=getYearMonthDayFromTicks(date.Ticks());
		return DateTime(ymd.year, ymd.month, ymd.day);
		//fix me
		static const int64_t kSecondOf1970_01_01 = (kJulianDayOf1970_01_01 - kJulianDayOf0001_01_01) * 24 * 3600;
		auto second = static_cast<int64_t>(utcsecond());
		second = second - second % (24 * 3600);
		return DateTime((second+ kSecondOf1970_01_01)* TimeSpan::kSecond);
	}

	DateTime DateTime::UtcNow()
	{
#if  ALIME_HAS_NOT_CPP11_OR_HIGHER
		//should use steady_clock but there is a bug that make it useless 
		return DateTime(std::chrono::TimeSpan_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#else
		return DateTime((utcmicrosecond() * TimeSpan::kMicrosecond) + kTickOf1970_01_01);
#endif
	}

	void DateTime::Add(TimeSpan d)
	{
		ticks_ += d.Ticks();
	}

	void DateTime::To(struct timeval* t) const
	{
		t->tv_sec = static_cast<long>(ticks_ - kTickOf1970_01_01 / TimeSpan::kSecond) ;
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
		return (ticks_/TimeSpan::kSecond)%60;
	}

	aint DateTime::Day()
	{
		return getYearMonthDayFromTicks(ticks_).day;
	}

	aint DateTime::Year()
	{
		return getYearMonthDayFromTicks(ticks_).year;
	}

	DateTime DateTime::Date()
	{
		auto ymd= getYearMonthDayFromTicks(ticks_);
		return DateTime(ymd.year, ymd.month, ymd.day);
	}

	aint DateTime::Month()
	{
		 return getYearMonthDayFromTicks(ticks_).month;
	}

	aint DateTime::Minute()
	{
		auto tm = GetTmFromTick();
		return tm.tm_min;
	}

	aint DateTime::Millisecond()
	{
		return (ticks_/ TimeSpan::kMillisecond)%1000;
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

	DayOfWeek DateTime::DayInWeek()
	{
		return DayOfWeek((getJulianDayNumber(Year(), Month(), Day()) + 1) % kDaysPerWeek);
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
	String DateTime::ToString() const
	{
		return toFormattedString(false);
	}

	//fix me
	//String DateTime::ToString(String format, IFormatProvider provider)
	//{
	//	return toFormattedString(false);
	//}

	//fix me
	//String DateTime::ToString(IFormatProvider provider)
	//{
	//	return toFormattedString(false);
	//}

	String DateTime::toFormattedString(bool showMicroseconds) const
	{
		wchar_t buf[64] = { 0 };
		auto tm_time = GetTmFromTick();
		if (showMicroseconds)
		{
			int microseconds = static_cast<int>((ticks_% TimeSpan::kSecond) / 1000);
			VsnprintfT(buf, sizeof(buf), L"%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
		}
		else
		{
			_snwprintf(buf, sizeof(buf), L"%4d%02d%02d %02d:%02d:%02d",
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

	tm DateTime::GetTmFromTick() const
	{
		time_t secondsFromUnixEpoch = static_cast<time_t>((ticks_- kTickOf1970_01_01) / TimeSpan::kSecond);
		struct tm tm_time;
#ifdef OS_POSIX
		//gmtime_r(&seconds, &tm_time);
#endif // OS_POSIX
		gmtime_s(&tm_time, &secondsFromUnixEpoch);
		return tm_time;
	}

	DateTime DateTime::Add(TimeSpan value) const
	{
		return DateTime(ticks_ + value.Ticks());
	}

	DateTime DateTime::AddDays(double value) const
	{
		return DateTime((ticks_ + static_cast<int64_t>(value * TimeSpan::kDay)));
	}

	DateTime DateTime::AddHours(double value)
	{
		return DateTime((ticks_ + static_cast<int64_t>( value * TimeSpan::kHour)));
	}

	DateTime DateTime::AddMilliseconds(double value)
	{
		return DateTime((ticks_ + static_cast<int64_t>(value * TimeSpan::kMillisecond)));
	}

	DateTime DateTime::AddMinutes(double value)
	{
		return DateTime((ticks_ + static_cast<int64_t>(value * TimeSpan::kMinute)));
	}

	DateTime DateTime::AddMonths(int months)
	{
		//整个人都虚脱了.....
		auto ymd=getYearMonthDayFromTicks(ticks_);
		//time_t seconds = static_cast<time_t>((ticks_-kTickOf1970_01_01) / TimeSpan::kSecond);
		//struct tm tm_time;
		//gmtime_s(&tm_time, &seconds);
		//auto ymd=getYearMonthDay(getJulianDayNumber(tm_time.tm_year+1900, tm_time.tm_mon+1, tm_time.tm_mday));
		ymd.month = (ymd.month + months-1) % 12+1;
		ymd.year += months / 12+(ymd.month + months%12)/12 ;
		if (IsLeapYear(ymd.year))//要小心2月份
		{
			auto lastDay = kDaysInMonth[1][ymd.month - 1];
			ymd.day = ymd.day > lastDay ? lastDay : ymd.day;
		}
		else
		{
			ymd.day = ymd.day > kDaysInMonth[0][ymd.month - 1] ? kDaysInMonth[0][ymd.month - 1] : ymd.day;
		}
		return DateTime(DateTime(ymd.year, ymd.month
			, ymd.day, Hour(), Minute(),Second()).Ticks()+ticks_%TimeSpan::kSecond);
	}

	DateTime DateTime::AddSeconds(double value)
	{
		return DateTime(ticks_ + static_cast<int64_t>(value * TimeSpan::kSecond));
	}

	DateTime DateTime::AddTicks(long value)
	{
		return DateTime(ticks_ + value);
	}

	DateTime DateTime::AddYears(int value)
	{
		return AddMonths(12*value);
	}

	int DateTime::CompareTo(DateTime value)
	{
		return DateTime::Compare(*this, value);
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

	String DateTime::ToLongDateString()
	{
		auto ymd=getYearMonthDayFromTicks(Ticks());
		//gmtime_s(&tm_time, &seconds);
		Char buf[64];
		{
			int microseconds = static_cast<int>((ticks_ % TimeSpan::kSecond) / 1000);
			_snwprintf(buf, sizeof(buf), L"%4d/%02d/%02d",
				ymd.year, ymd.month, ymd.day);
		}
		return buf;
	}

	String DateTime::ToLongTimeString()
	{
		Char buf[64] = { 0 };
		auto tm_time = GetTmFromTick();
		int microseconds = static_cast<int>((ticks_ % TimeSpan::kSecond) / 1000);
		_snwprintf(buf, sizeof(buf), L"%02d:%02d:%02d",
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		return buf;
	}

	String DateTime::ToShortDateString()
	{
		return ToLongDateString();
	}

	String DateTime::ToShortTimeString()
	{
		Char buf[64] = { 0 };
		auto tm_time = GetTmFromTick();
		_snwprintf(buf, sizeof(buf), L"%02d:%02d",
			tm_time.tm_hour, tm_time.tm_min);
		return buf;
	}

	int DateTime::Compare(DateTime t1, DateTime t2)
	{
		auto ret = t1.ticks_ - t2.ticks_;
		return ret==0 ? 0: ret>0 ? 1: -1;
	}

	int DateTime::DaysInMonth(int year, int month)
	{
		return kDaysInMonth[IsLeapYear(year)][month-1];
	}

	bool DateTime::IsLeapYear(int year)
	{
		if (year < 0 || year>9999)
			throw "year must be 1-9999";
		return getJulianDayNumber(year, 2, 29) != getJulianDayNumber(year, 3, 1);
	}

	bool DateTime::Equals(DateTime t1, DateTime t2)
	{
		return t1.ticks_ == t2.ticks_;
	}
	
	DateTime DateTime::Parse(String s)
	{
		return {};
	}

	DateTime DateTime::ToUniversalTime()
	{
		return DateTime(ticks_ - TicksLocalTimeAhead());
	}

	aint64 DateTime::TicksLocalTimeAhead()
	{
		static bool cached = false;
		static aint64 secondsAhead = 0;
		if (!cached)
		{
			time_t t = time(nullptr);
			tm pTm1;
			localtime_s(&pTm1, &t);
			tm pTm2;
			gmtime_s(&pTm2, &t);
			time_t tik = mktime(&pTm2);
			secondsAhead = t - tik;
		}
		return secondsAhead;
	}

	DateTime DateTime::ToLocalTime()
	{
		return DateTime( ticks_ + TicksLocalTimeAhead()*TimeSpan::kSecond);
	}
}