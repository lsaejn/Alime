#pragma once
#include <stdint.h>


struct timeval;
namespace Alime 
{
	/*
A Duration represents the elapsed time between two instantsas an int64
nanosecond count. The representation limits the largest representable duration to approximately 290 years.
This class is modified from Evpp
*/
	class TimeSpan 
	{
	public:
		static const int64_t kNanosecond = 1LL;
		static const int64_t kMicrosecond = 1000 * kNanosecond;
		static const int64_t kMillisecond = 1000 * kMicrosecond;
		static const int64_t kSecond = 1000 * kMillisecond;
		static const int64_t kMinute = 60 * kSecond;
		static const int64_t kHour = 60 * kMinute;

		static TimeSpan Seconds(double count);
		static TimeSpan Milliseconds(double count);
		static TimeSpan Microseconds(double count);
		static TimeSpan Minutes(double count);
		static TimeSpan Hours(double count);
		static TimeSpan Nanoseconds(int64_t count);
	public:
		TimeSpan();
		explicit TimeSpan(const struct timeval& t);
		explicit TimeSpan(int64_t nanoseconds);
		explicit TimeSpan(int nanoseconds);
		explicit TimeSpan(double seconds);

		// Nanoseconds returns the duration as an integer nanosecond count.
		int64_t toNanoseconds() const;
		double toSeconds() const;
		double toMilliseconds() const;
		double toMicroseconds() const;
		double toMinutes() const;
		double toHours() const;

		struct timeval TimeVal() const;
		void To(struct timeval* t) const;
		bool IsZero() const;
		bool Valid() const;

		bool operator< (const TimeSpan& rhs) const;
		bool operator<=(const TimeSpan& rhs) const;
		bool operator> (const TimeSpan& rhs) const;
		bool operator>=(const TimeSpan& rhs) const;
		bool operator==(const TimeSpan& rhs) const;

		TimeSpan operator+=(const TimeSpan& rhs);
		TimeSpan operator-=(const TimeSpan& rhs);
		TimeSpan operator*=(int ns);
		TimeSpan operator/=(int ns);

	private:
		int64_t ns_; // nanoseconds
	};
} // namespace end

