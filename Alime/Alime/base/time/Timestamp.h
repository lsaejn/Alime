#pragma once

#include "duration.h"
#include <string>


namespace Alime::base
{
	enum class DateTimeKind
	{
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	class DateTime
	{
	public:
		DateTime();
		/// <summary>
		/// <param name="ticks">
		/// A date and time expressed in the number of 100-nanosecond intervals that have
		/// elapsed since January 1, 0001 at 00:00:00.000 in the Gregorian calendar.
		///</param>
		/// </summary>
		explicit DateTime(int64_t ticks); //rarely used
		explicit DateTime(const struct timeval& t);

		static DateTime Now(); // returns the current local time.

		struct timeval TimeVal() const;
		void To(struct timeval* t) const;

		// Unix returns t as a Unix time, the number of seconds elapsed since January 1, 1970 UTC.
		int64_t Unix() const;

		// the number of nanoseconds/microseconds elapsed since January 1, 1970 UTC. The result is undefined
		// if the Unix time in nanoseconds/microsecond cannot be represented by an int64.
		int64_t UnixNano() const;
		int64_t UnixMicro() const;

		void Add(Duration d);

		bool IsEpoch() const;
		bool operator< (const DateTime& rhs) const;
		bool operator==(const DateTime& rhs) const;

		DateTime operator+=(const Duration& rhs);
		DateTime operator+ (const Duration& rhs) const;
		DateTime operator-=(const Duration& rhs);
		DateTime operator- (const Duration& rhs) const;
		Duration  operator- (const DateTime& rhs) const;

		bool valid() const;
		static DateTime invalid();
		std::string toString() const;// 以字符形式打印ticks
		std::string toFormattedString(bool showMicroseconds = true) const;
	private:
		//the number of ticks that represent the dateand time of this instance.
		int64_t ns_;
	};
}
