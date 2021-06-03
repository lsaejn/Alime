#pragma once

#include "TimeSpan.h"
#include <string>
#include <Alime/base/base_define.h>

/*
������ʷԭ��unix�±�ʾʱ��Ľṹ�弫�����

struct timeval
{
	long tv_sec;//from 1970/1/1
	long tv_usec;
};

struct tm
{
int tm_sec;  //�룬������Χ0-59�� ��������61
int tm_min;  //���ӣ�0-59
int tm_hour; //Сʱ�� 0-23
int tm_mday; //�գ���һ�����еĵڼ��죬1-31
int tm_mon;  //�£� ��һ������0-11
int tm_year;  //�꣬ ��1900�����Ѿ�������
int tm_wday; //���ڣ�һ���еĵڼ��죬 ������������0-6
int tm_yday; //�ӽ���1��1�յ�Ŀǰ����������Χ0-365
int tm_isdst; //�չ��Լʱ������
};

ʱ��:
����ŷ��ʹ������ʱ��
����:ÿ������µڶ������տ�ʼ,ʮһ�µĵ�һ�����ս���
Ӣ��:ÿ�����µ����һ�������տ�ʼ����10�µ����һ�������ս�����10�µ����һ�������վ��Ƕ���ʱ�Ŀ�ʼʱ�䡣
���磬����2020���������3��8����������
2020�������ʱʱ����3��8�ţ������賿2�㿪ʼ������һ2��
2020��11��1�ţ��賿1�������Ȼ�����Ǿ��ֻ�ع鵽����ʱ����
*/

/*
���������windows�Ĳ���
*/
namespace Alime::base::System
{
	class IFormatProvider;


	enum class DateTimeKind
	{
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	//
// ժҪ:
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
	/// DateTime ά��һ����0001-01-01(UTC)������ticks
	/// ֻΪ���㷽��, ����ע��Ч��, ��Ҫ���ڶ�ʱ����
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
		DateTime(int64_t ticks, DateTimeKind kind);
		DateTime(int year, int month, int day, int hour=0, int minute=0, int second=0, int millisecond=0);
		DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
		DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);

		static DateTime Now(); // returns the current local time.
		static DateTime Today();
		static DateTime UtcNow();
		aint64 Ticks() const;
		aint Second() const;
		
		aint Month() const;
		aint Minute() const;
		aint Millisecond() const;
		aint Hour() const;
		aint DayOfYear() const; //from 0
		DayOfWeek DayInWeek() const;
		aint Day() const;
		aint Year() const;
		DateTime Date() const;
		DateTimeKind Kind() const;

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

		static int Compare(const DateTime &t1, const DateTime &t2);
		static int DaysInMonth(int year, int month);
		static bool Equals(const DateTime& t1, const DateTime& t2);
		//static DateTime FromFileTime(long fileTime);
		//static DateTime FromFileTimeUtc(long fileTime);
		static bool IsLeapYear(int year);
		static DateTime Parse(const String& s);

		DateTime Add(TimeSpan value) const;
		DateTime AddDays(double value) const;
		DateTime AddHours(double value) const;
		DateTime AddMilliseconds(double value) const;
		DateTime AddMinutes(double value) const;
		DateTime AddMonths(int months) const;
		DateTime AddSeconds(double value) const;
		DateTime AddTicks(long value) const;
		DateTime AddYears(int value) const;
		int CompareTo(DateTime value) const;
		bool Equals(DateTime value) const;
		DateTime Subtract(TimeSpan value) const;
		TimeSpan Subtract(const DateTime& value) const;
		aint64 ToFileTime() const;
		aint64 ToFileTimeUtc() const;
		DateTime ToUniversalTime() const;//����dateTime��ʾlocaltimeʱ����
		DateTime ToLocalTime() const;

		String ToLongDateString() const;
		String ToLongTimeString() const;

		String ToShortDateString() const;
		String ToShortTimeString() const;

		//String ToString(String format, IFormatProvider provider);
		//String ToString(IFormatProvider provider);
		String ToString() const;// ���ַ���ʽ��ӡticks

		bool operator< (const DateTime& rhs) const;
		bool operator==(const DateTime& rhs) const;
		DateTime operator+=(TimeSpan rhs);
		DateTime operator+ (TimeSpan rhs) const;
		DateTime operator-=(TimeSpan rhs);
		DateTime operator- (TimeSpan rhs) const;
		TimeSpan operator- (const DateTime& rhs) const;

		bool valid() const;
		static DateTime invalid();
		
	private:
		aint64 TicksLocalTimeAhead() const;
		tm GetTmFromTick() const;
		String toFormattedString(bool showMicroseconds = true) const;
		void CheckDate() const;
		void InitFromTicks(aint64 ticks);
		//the number of ticks that represent the dateand time of this instance.
		//int64_t ticks_;
		aint year_;
		aint month_;
		aint day_;
		aint hour_;
		aint minute_;
		aint second_;
		aint millisecond_;
		aint microsecond_;
		aint nanosecond_;
		DateTimeKind kind_;
	};

	//static DateTime operator +(const DateTime& d, TimeSpan t);
	//static TimeSpan operator -(const DateTime& d1, const DateTime& d2);
	//static DateTime operator -(const DateTime& d, TimeSpan t);
	//static bool operator ==(const DateTime& d1, const DateTime& d2);
	//static bool operator !=(const DateTime& d1, const DateTime& d2);
	//static bool operator <(const DateTime& t1, const DateTime& t2);
	//static bool operator >(const DateTime& t1, const DateTime& t2);
	//static bool operator <=(const DateTime& t1, const DateTime& t2);
	//static bool operator >=(const DateTime& t1, const DateTime& t2);
}
