#pragma once

#include "TimeSpan.h"
#include <string>
#include <Alime/base/base_define.h>

/*
由于历史原因，unix下表示时间的结构体极其混乱

struct timeval
{
	long tv_sec;//from 1970/1/1
	long tv_usec;
};

struct tm
{
int tm_sec;  //秒，正常范围0-59， 但允许至61
int tm_min;  //分钟，0-59
int tm_hour; //小时， 0-23
int tm_mday; //日，即一个月中的第几天，1-31
int tm_mon;  //月， 从一月算起，0-11
int tm_year;  //年， 从1900至今已经多少年
int tm_wday; //星期，一周中的第几天， 从星期日算起，0-6
int tm_yday; //从今年1月1日到目前的天数，范围0-365
int tm_isdst; //日光节约时间的旗标
};

时区:
由于欧洲使用夏令时，
美国:每年的三月第二个周日开始,十一月的第一个周日结束
英国:每年三月的最后一个星期日开始，到10月的最后一个星期日结束，10月的最后一个星期日就是冬令时的开始时间。
比如，对于2020年的美国，3月8日是星期日
2020年的夏令时时间是3月8号，周日凌晨2点开始。即周一2点
2020年11月1号，凌晨1点结束，然后我们就又会回归到冬令时啦！
*/

/*
我们先完成windows的部分
*/
namespace Alime::base::System
{
	class IFormatProvider;
	struct YearMonthDay
	{
		int year; // [1900..2500]
		int month;  // [1..12]
		int day;  // [1..31]
	};

	enum class DateTimeKind
	{
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	//
// 摘要:
//     Specifies the day of the week.
	enum class DayOfWeek
	{
		Sunday = 0,
		Monday = 1,
		Tuesday = 2,
		Wednesday = 3,
		Thursday = 4,
		Friday = 5,
		Saturday = 6
	};

	/// <summary>
	/// DateTime 维护一个自0000-01-01(UTC)以来的ticks
	/// 只为计算方便, 并不注重效率, 不要用于定时任务
	/// </summary>
	class DateTime
	{
	public:
		static const DateTime UnixEpoch;
		static const DateTime FileTimeEpoch;
	public:
		DateTime();
		explicit DateTime(int64_t ticks); //DateTimeKind
		explicit DateTime(const struct timeval& t);
		//DateTime(int64_t ticks, DateTimeKind kind);
		DateTime(int year, int month, int day, int hour=0, int minute=0, int second=0, int millisecond=0);

		static DateTime Now(); // returns the current local time.
		static DateTime Today();
		static DateTime UtcNow();
		aint64 Ticks();
		aint Second();
		
		aint Month();
		aint Minute();
		aint Millisecond();
		aint Hour();
		aint DayOfYear();
		DayOfWeek DayInWeek();
		aint Day();
		aint Year();
		DateTime Date();
		//DateTimeKind Kind();

		struct timeval TimeVal() const;
		void To(struct timeval* t) const;

		// we deprecate those
		// Unix returns t as a Unix time, the number of seconds elapsed since January 1, 1970 UTC.
		int64_t Unix() const=delete;
		// the number of nanoseconds/microseconds elapsed since January 1, 1970 UTC. The result is undefined
		// if the Unix time in nanoseconds/microsecond cannot be represented by an int64.
		int64_t UnixNano() const = delete;
		int64_t UnixMicro() const = delete;

		bool IsEpoch() const;

		static int Compare(DateTime t1, DateTime t2);
		static int DaysInMonth(int year, int month);
		static bool Equals(DateTime t1, DateTime t2);
		//static DateTime FromFileTime(long fileTime);
		//static DateTime FromFileTimeUtc(long fileTime);
		static bool IsLeapYear(int year);
		static DateTime Parse(String s);

		void Add(TimeSpan d);
		DateTime Add(TimeSpan value) const;
		DateTime AddDays(double value) const;
		DateTime AddHours(double value);
		DateTime AddMilliseconds(double value);
		DateTime AddMinutes(double value);
		DateTime AddMonths(int months);
		DateTime AddSeconds(double value);
		DateTime AddTicks(long value);
		DateTime AddYears(int value);
		int CompareTo(DateTime value);
		bool Equals(DateTime value);
		DateTime Subtract(TimeSpan value);
		TimeSpan Subtract(DateTime value);
		//long ToFileTime();
		//long ToFileTimeUtc();
		DateTime ToUniversalTime();//仅当dateTime表示localtime时调用
		DateTime ToLocalTime();

		String ToLongDateString();
		String ToLongTimeString();

		String ToShortDateString();
		String ToShortTimeString();

		//String ToString(String format, IFormatProvider provider);
		//String ToString(IFormatProvider provider);
		String ToString() const;// 以字符形式打印ticks

		bool operator< (const DateTime& rhs) const;
		bool operator==(const DateTime& rhs) const;
		DateTime operator+=(const TimeSpan& rhs);
		DateTime operator+ (const TimeSpan& rhs) const;
		DateTime operator-=(const TimeSpan& rhs);
		DateTime operator- (const TimeSpan& rhs) const;
		TimeSpan  operator- (const DateTime& rhs) const;

		bool valid() const;
		static DateTime invalid();
		
	private:
		aint64 TicksLocalTimeAhead();
		tm GetTmFromTick() const;
		String toFormattedString(bool showMicroseconds = true) const;
		//the number of ticks that represent the dateand time of this instance.
		int64_t ticks_;
	};

	static DateTime operator +(DateTime d, TimeSpan t);
	static TimeSpan operator -(DateTime d1, DateTime d2);
	static DateTime operator -(DateTime d, TimeSpan t);
	static bool operator ==(DateTime d1, DateTime d2);
	static bool operator !=(DateTime d1, DateTime d2);
	static bool operator <(DateTime t1, DateTime t2);
	static bool operator >(DateTime t1, DateTime t2);
	static bool operator <=(DateTime t1, DateTime t2);
	static bool operator >=(DateTime t1, DateTime t2);
}
