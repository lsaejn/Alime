#pragma once

#include "TimeSpan.h"
#include <string>
#include <Alime/base/base_define.h>

/*
������ʷԭ��unix�±�ʾʱ��Ľṹ�弫�����
struct timeval
{
	long tv_sec;
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

*/

namespace Alime::base
{
	enum class DateTimeKind
	{
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	/// <summary>
	/// Ϊ�˺�C#����, �����е�ticks��100nanosecondΪ��λ��since January 1, 0001 at 00:00:00.000
	/// Ϊ��Ч�ʣ�������Э�ˣ�c++ʱ�����������fd��
	/// �⵼��ʵʱ���ԵĲ���
	/// </summary>
	class DateTime
	{
	public:
		static const DateTime MaxValue;
		static const DateTime MinValue;
		static const DateTime UnixEpoch;
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
		aint DayOfWeek();
		aint Day();
		aint Year();
		DateTime Date();
		//DateTimeKind Kind();

		struct timeval TimeVal() const;
		void To(struct timeval* t) const;

		// Unix returns t as a Unix time, the number of seconds elapsed since January 1, 1970 UTC.
		int64_t Unix() const;

		// the number of nanoseconds/microseconds elapsed since January 1, 1970 UTC. The result is undefined
		// if the Unix time in nanoseconds/microsecond cannot be represented by an int64.
		int64_t UnixNano() const;
		int64_t UnixMicro() const;

		bool IsEpoch() const;

		static int Compare(DateTime t1, DateTime t2);
		static int DaysInMonth(int year, int month);
		static bool Equals(DateTime t1, DateTime t2);
		static DateTime FromFileTime(long fileTime);
		static DateTime FromFileTimeUtc(long fileTime);
		static bool IsLeapYear(int year);
		static DateTime Parse(String s);

		void Add(TimeSpan d);
		DateTime Add(TimeSpan value) const;
		DateTime AddDays(double value);
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
		long ToFileTime();
		long ToFileTimeUtc();
		DateTime ToLocalTime();
		String ToLongDateString();
		String ToLongTimeString();

		bool operator< (const DateTime& rhs) const;
		bool operator==(const DateTime& rhs) const;
		DateTime operator+=(const TimeSpan& rhs);
		DateTime operator+ (const TimeSpan& rhs) const;
		DateTime operator-=(const TimeSpan& rhs);
		DateTime operator- (const TimeSpan& rhs) const;
		TimeSpan  operator- (const DateTime& rhs) const;

		bool valid() const;
		static DateTime invalid();
		std::string toString() const;// ���ַ���ʽ��ӡticks
		std::string toFormattedString(bool showMicroseconds = true) const;
		
	private:
		tm GetTmFromTick();
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
