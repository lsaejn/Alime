#pragma once
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

	struct YearMonthDay
	{
		int year; // [1900..2500]
		int month;  // [1..12]
		int day;  // [1..31]
	};

	struct TimeInfo
	{
		aint hour;
		aint minute;
		aint second;
		aint millisecond;
		aint microsecond;
		aint nanosecond;
	};

	void CheckDateInfo(int year, int month, int day,
		int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond=0,
		int nanosecond=0)
	{
		CHECK_ERROR(((year > 0 && year < 9999)
			&& (month > 0 && month < 13)
			&& (day > 0 && day <= kDaysInMonth[DateTime::IsLeapYear(year)][month-1])
			&& (hour >= 0 && hour < 24)
			&& (second >= 0 && second < 60)
			&& (millisecond >= 0 && millisecond < 1000)
			&& (microsecond >= 0 && microsecond < 1000)
			&& (nanosecond >= 0 && nanosecond < 1000)
			)
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

	struct TimeInfo getTimeInfoFromTicks(int64_t ticks)
	{
		ticks = ticks % TimeSpan::kDay;
		TimeInfo ti;
		ti.hour =static_cast<aint>( ticks / TimeSpan::kHour);
		ticks = ticks % TimeSpan::kHour;
		ti.minute = static_cast<aint>(ticks / TimeSpan::kMinute);
		ticks = ticks % TimeSpan::kMinute;
		ti.second = static_cast<aint>(ticks / TimeSpan::kSecond);
		ticks = ticks % TimeSpan::kSecond;
		ti.millisecond = static_cast<aint>(ticks / TimeSpan::kMillisecond);
		ticks = ticks % TimeSpan::kMillisecond;
		ti.microsecond = static_cast<aint>(ticks / TimeSpan::kMicrosecond);
		ticks = ticks % TimeSpan::kMicrosecond;
		ti.nanosecond = static_cast<aint>(ticks * 100);
		return ti;
	}

	//Julian=UTC=GMT
	int64_t GetTicksFromDateInfo(int year=0, int month=0, int day=0, int hour=0,
		int minute = 0,int second = 0, int millisecond = 0, int microsecond=0,
		int nanosecond=0)
	{
		return
			(getJulianDayNumber(year, month, day) - kJulianDayOf0001_01_01)* TimeSpan::kDay
			+ hour * TimeSpan::kHour
			+ minute * TimeSpan::kMinute
			+ second * TimeSpan::kSecond
			+ millisecond * TimeSpan::kMillisecond
			+ microsecond* TimeSpan::kMicrosecond
			+ nanosecond/100;
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
		: year_(0),
		month_(0),
		day_(0),
		hour_(0),
		minute_(0),
		second_(0),
		millisecond_(0),
		microsecond_(0),
		nanosecond_(0),
		kind_(DateTimeKind::Unspecified)
	{
	}

	DateTime::DateTime(int64_t ticks)
		:DateTime()
	{
		InitFromTicks(ticks);
	}

	bool DateTime::IsEpoch() const
	{
		return GetTicksFromDateInfo (year_, month_, day_, minute_, 
			second_, millisecond_, microsecond_, nanosecond_)== 0;
	}

	DateTime::DateTime(const timeval& t)
		:DateTime((t.tv_sec* TimeSpan::kSecond + t.tv_usec * TimeSpan::kMicrosecond) + kTickOf1970_01_01)
	{
	}

	DateTime::DateTime(int64_t ticks, DateTimeKind kind)
		:DateTime(ticks)
	{
		kind_ = kind;
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
		: DateTime(year, month, day, hour, minute, second)
	{
		kind_ = kind;
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
		: DateTime(year, month, day, hour, minute, second, millisecond)
	{
		kind_ = kind;
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
		: year_(year),
		month_(month),
		day_(day),
		hour_(hour),
		minute_(minute),
		second_(second),
		millisecond_(millisecond),
		microsecond_(0),
		nanosecond_(0),
		kind_(DateTimeKind::Unspecified)
	{
		CheckDate();
		if (day>kDaysInMonth[IsLeapYear(year)][month-1])
		{
			throw "invalid day number";
		}
	}

	void DateTime::CheckDate() const
	{
		CheckDateInfo(year_, month_, day_, hour_, minute_, second_, 
			millisecond_, microsecond_, nanosecond_);
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

	void DateTime::InitFromTicks(aint64 ticks)
	{
		auto ymd = getYearMonthDayFromTicks(ticks);
		year_ = ymd.year;
		month_ = ymd.month;
		day_ = ymd.day;
		auto t_info = getTimeInfoFromTicks(ticks);
		hour_ = t_info.hour;
		minute_ = t_info.minute;
		second_ = t_info.second;
		millisecond_ = t_info.millisecond;
		microsecond_ = t_info.microsecond;
		nanosecond_ = t_info.nanosecond;
	}

	void DateTime::To(struct timeval* t) const
	{
		t->tv_sec = static_cast<long>(Ticks() - kTickOf1970_01_01 / TimeSpan::kSecond) ;
		t->tv_usec = static_cast<long>(Ticks() % TimeSpan::kSecond) / static_cast<long>(TimeSpan::kMicrosecond);
	}

	struct timeval DateTime::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	int64_t DateTime::Ticks() const
	{
		return GetTicksFromDateInfo(year_, month_, day_, hour_, minute_, second_, millisecond_, microsecond_, nanosecond_);
	}

	aint DateTime::Second() const
	{
		return second_;
	}

	aint DateTime::Day() const
	{
		return day_;
	}

	aint DateTime::Year() const
	{
		return year_;
	}

	DateTime DateTime::Date() const
	{
		return DateTime(year_, month_, day_);
	}

	DateTimeKind DateTime::Kind() const
	{
		return kind_;
	}

	aint DateTime::Month() const
	{
		 return month_;
	}

	aint DateTime::Minute() const
	{
		return minute_;
	}

	aint DateTime::Millisecond() const
	{
		return millisecond_;
	}

	aint DateTime::Hour() const
	{
		return hour_;
	}

	aint DateTime::DayOfYear() const
	{
		auto year = Year();
		int firstDay=getJulianDayNumber(year, 1, 1);
		int today = getJulianDayNumber(Year(), Month(), Day());
		return today- firstDay+1;
	}

	DayOfWeek DateTime::DayInWeek() const
	{
		return DayOfWeek((getJulianDayNumber(Year(), Month(), Day()) + 1) % kDaysPerWeek);
	}

	bool DateTime::operator< (const DateTime& rhs) const
	{
		return Ticks() < rhs.Ticks();
	}

	bool DateTime::operator==(const DateTime& rhs) const
	{
		return Ticks() == rhs.Ticks();
	}

	DateTime DateTime::operator+=(TimeSpan rhs)
	{
		auto total=Ticks()+ rhs.Ticks();
		InitFromTicks(total);
		return *this;
	}

	DateTime DateTime::operator+(TimeSpan rhs) const
	{
		DateTime temp(*this);
		temp += rhs;
		return temp;
	}

	DateTime DateTime::operator-=(TimeSpan rhs)
	{
		auto total = Ticks() - rhs.Ticks();
		InitFromTicks(total);
		return *this;
	}

	DateTime DateTime::operator-(TimeSpan rhs) const
	{
		DateTime temp(*this);
		temp -= rhs;
		return temp;
	}

	TimeSpan DateTime::operator-(const DateTime& rhs) const
	{
		return TimeSpan(Ticks() - rhs.Ticks());
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
			int microseconds = static_cast<int>((Ticks()% TimeSpan::kSecond) / 1000);
			VsnprintfT(buf, sizeof(buf), L"%4d/%02d/%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
		}
		else
		{
			VsnprintfT(buf, sizeof(buf), L"%4d/%02d/%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		}
		return buf;
	}

	bool DateTime::valid() const
	{
		return  Ticks() > 0;
	}

	DateTime DateTime::invalid()
	{
		return DateTime();
	}

	tm DateTime::GetTmFromTick() const
	{
		time_t secondsFromUnixEpoch = static_cast<time_t>((Ticks()- kTickOf1970_01_01) / TimeSpan::kSecond);
		struct tm tm_time;
#ifdef OS_POSIX
		//gmtime_r(&seconds, &tm_time);
#endif // OS_POSIX
		gmtime_s(&tm_time, &secondsFromUnixEpoch);
		return tm_time;
	}

	DateTime DateTime::Add(TimeSpan value) const
	{
		return DateTime(Ticks() + value.Ticks());
	}

	DateTime DateTime::AddDays(double value) const
	{
		return DateTime((Ticks() + static_cast<int64_t>(value * TimeSpan::kDay)));
	}

	DateTime DateTime::AddHours(double value) const
	{
		return DateTime((Ticks() + static_cast<int64_t>( value * TimeSpan::kHour)));
	}

	DateTime DateTime::AddMilliseconds(double value) const
	{
		return DateTime((Ticks() + static_cast<int64_t>(value * TimeSpan::kMillisecond)));
	}

	DateTime DateTime::AddMinutes(double value) const
	{
		return DateTime((Ticks() + static_cast<int64_t>(value * TimeSpan::kMinute)));
	}

	DateTime DateTime::AddMonths(int months) const
	{
		//整个人都虚脱了.....
		auto ymd=getYearMonthDayFromTicks(Ticks());
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
			, ymd.day, Hour(), Minute(),Second()).Ticks()+ Ticks() %TimeSpan::kSecond);
	}

	DateTime DateTime::AddSeconds(double value) const
	{
		return DateTime(Ticks() + static_cast<int64_t>(value * TimeSpan::kSecond));
	}

	DateTime DateTime::AddTicks(long value) const
	{
		return DateTime(Ticks() + value);
	}

	DateTime DateTime::AddYears(int value) const
	{
		return AddMonths(12*value);
	}

	int DateTime::CompareTo(DateTime value) const
	{
		return DateTime::Compare(*this, value);
	}

	bool DateTime::Equals(DateTime value) const
	{
		return Ticks() == value.Ticks();
	}

	DateTime DateTime::Subtract(TimeSpan value) const
	{
		//fix me
		return DateTime(Ticks() - value.Ticks());
	}

	TimeSpan DateTime::Subtract(const DateTime& value) const
	{
		//fix me
		return TimeSpan(Ticks() - value.Ticks());
	}

	aint64 DateTime::ToFileTime() const
	{
		return Ticks()+TicksLocalTimeAhead();
	}

	aint64 DateTime::ToFileTimeUtc() const
	{
		return Ticks();
	}

	String DateTime::ToLongDateString() const
	{
		auto ymd=getYearMonthDayFromTicks(Ticks());
		//gmtime_s(&tm_time, &seconds);
		Char buf[64];
		{
			int microseconds = static_cast<int>((Ticks() % TimeSpan::kSecond) / 1000);
			VsnprintfT(buf, sizeof(buf), L"%4d/%02d/%02d",
				ymd.year, ymd.month, ymd.day);
		}
		return buf;
	}

	String DateTime::ToLongTimeString() const
	{
		Char buf[64] = { 0 };
		auto tm_time = GetTmFromTick();
		int microseconds = static_cast<int>((Ticks() % TimeSpan::kSecond) / 1000);
		_snwprintf(buf, sizeof(buf), L"%02d:%02d:%02d",
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		return buf;
	}

	String DateTime::ToShortDateString() const
	{
		return ToLongDateString();
	}

	String DateTime::ToShortTimeString() const
	{
		Char buf[64] = { 0 };
		auto tm_time = GetTmFromTick();
		VsnprintfT(buf, sizeof(buf), L"%02d:%02d",
			tm_time.tm_hour, tm_time.tm_min);
		return buf;
	}

	int DateTime::Compare(const DateTime &t1, const DateTime &t2)
	{
		auto ret = t1.Ticks() - t2.Ticks();
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

	bool DateTime::Equals(const DateTime& t1, const DateTime& t2)
	{
		return t1.Ticks() == t2.Ticks();
	}
	
	DateTime DateTime::Parse(const String& s)
	{
		return {};
	}

	DateTime DateTime::ToUniversalTime() const
	{
		if (kind_ == DateTimeKind::Local)
			return DateTime(Ticks() - TicksLocalTimeAhead(), DateTimeKind::Utc);
		else
			return *this;
	}

	aint64 DateTime::TicksLocalTimeAhead() const
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
		return secondsAhead* TimeSpan::kSecond;
	}

	DateTime DateTime::ToLocalTime() const
	{
		if (kind_ == DateTimeKind::Local)
			return *this;
		else
			return DateTime(Ticks() + TicksLocalTimeAhead(), DateTimeKind::Local);
	}



}