#include "TimeSpan.h"
#include <WinSock2.h>

namespace Alime::base::System
{
	TimeSpan::TimeSpan()
		: ticks_(0) {}

	TimeSpan::TimeSpan(const struct timeval& t)
		: ticks_(t.tv_sec * kSecond + t.tv_usec * kMicrosecond) {}

	TimeSpan::TimeSpan(int64_t ticks)
		: ticks_(ticks)
	{
		if (ticks_ < 0)
			throw "ticks should not be a negative value";
	}

	int64_t TimeSpan::Ticks() const
	{
		return ticks_;
	}

	int64_t TimeSpan::toNanoSeconds() const
	{
		return ticks_*100;
	}

	double TimeSpan::toSeconds() const
	{
		return double(ticks_) / kSecond;
	}

	double TimeSpan::toMilliseconds() const
	{
		return double(ticks_) / kMillisecond;
	}

	double TimeSpan::toMicroseconds() const
	{
		return double(ticks_) / kMicrosecond;
	}

	double TimeSpan::toMinutes() const
	{
		return double(ticks_) / kMinute;
	}

	double TimeSpan::toHours() const
	{
		return double(ticks_) / kHour;
	}

	bool TimeSpan::IsZero() const
	{
		return ticks_ == 0;
	}

	bool TimeSpan::Valid() const
	{
		return ticks_ >= 0;
	}

	struct timeval TimeSpan::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	void TimeSpan::To(timeval* t) const
	{
		t->tv_sec = static_cast<long>(ticks_ / kSecond);
		t->tv_usec = static_cast<long>(ticks_ % kSecond) / static_cast<long>(kMicrosecond);
	}

	bool TimeSpan::operator<(const TimeSpan& rhs) const
	{
		return ticks_ < rhs.ticks_;
	}

	bool TimeSpan::operator<=(const TimeSpan& rhs) const
	{
		return ticks_ <= rhs.ticks_;
	}

	bool TimeSpan::operator>(const TimeSpan& rhs) const
	{
		return ticks_ > rhs.ticks_;
	}

	bool TimeSpan::operator>=(const TimeSpan& rhs) const
	{
		return ticks_ >= rhs.ticks_;
	}

	bool TimeSpan::operator==(const TimeSpan& rhs) const 
	{
		return ticks_ == rhs.ticks_;
	}

	TimeSpan TimeSpan::operator+=(const TimeSpan& rhs)
	{
		ticks_ += rhs.ticks_;
		return *this;
	}

	TimeSpan TimeSpan::operator-=(const TimeSpan& rhs)
	{
		ticks_ -= rhs.ticks_;
		return *this;
	}

	TimeSpan TimeSpan::operator*=(int n)
	{
		ticks_ *= n;
		return *this;
	}

	TimeSpan TimeSpan::operator/=(int n)
	{
		ticks_ /= n;
		return *this;
	}

	TimeSpan TimeSpan::Seconds(double count)
	{
		return TimeSpan(count*TimeSpan::kSecond);
	}

	TimeSpan TimeSpan::Milliseconds(double count)
	{
		return TimeSpan(count*TimeSpan::kMillisecond);
	}

	TimeSpan TimeSpan::Microseconds(double count)
	{
		return TimeSpan(count*TimeSpan::kMicrosecond);
	}

	TimeSpan TimeSpan::Minutes(double count)
	{
		return TimeSpan(count*TimeSpan::kMinute);
	}

	TimeSpan TimeSpan::Hours(double count)
	{
		return TimeSpan(count*TimeSpan::kHour);
	}

	TimeSpan TimeSpan::Nanoseconds(int64_t count)
	{
		return TimeSpan(count);
	}
} // namespace end
