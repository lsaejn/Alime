#include "TimeSpan.h"
#include <WinSock2.h>

namespace Alime
{
	TimeSpan::TimeSpan()
		: ticks_(0) {}

	TimeSpan::TimeSpan(const struct timeval& t)
		: ticks_(t.tv_sec * kSecond + t.tv_usec * kMicrosecond) {}

	TimeSpan::TimeSpan(int64_t nanoseconds)
		: ticks_(nanoseconds) {}

	TimeSpan::TimeSpan(int nanoseconds)
		: ticks_(nanoseconds) {}

	TimeSpan::TimeSpan(double seconds)
		: ticks_(static_cast<int64_t>(seconds * kSecond)) {}

	int64_t TimeSpan::Ticks() const {
		return ticks_;
	}

	double TimeSpan::toSeconds() const {
		return double(ticks_) / kSecond;
	}

	double TimeSpan::toMilliseconds() const {
		return double(ticks_) / kMillisecond;
	}

	double TimeSpan::toMicroseconds() const {
		return double(ticks_) / kMicrosecond;
	}

	double TimeSpan::toMinutes() const {
		return double(ticks_) / kMinute;
	}

	double TimeSpan::toHours() const {
		return double(ticks_) / kHour;
	}

	bool TimeSpan::IsZero() const {
		return ticks_ == 0;
	}

	bool TimeSpan::Valid() const
	{
		return ticks_ >= 0;
	}

	struct timeval TimeSpan::TimeVal() const {
		struct timeval t;
		To(&t);
		return t;
	}

	void TimeSpan::To(struct timeval* t) const {
		t->tv_sec = static_cast<long>(ticks_ / kSecond);
		t->tv_usec = static_cast<long>(ticks_ % kSecond) / static_cast<long>(kMicrosecond);
	}

	bool TimeSpan::operator<(const TimeSpan& rhs) const {
		return ticks_ < rhs.ticks_;
	}

	bool TimeSpan::operator<=(const TimeSpan& rhs) const {
		return ticks_ <= rhs.ticks_;
	}

	bool TimeSpan::operator>(const TimeSpan& rhs) const {
		return ticks_ > rhs.ticks_;
	}

	bool TimeSpan::operator>=(const TimeSpan& rhs) const {
		return ticks_ >= rhs.ticks_;
	}

	bool TimeSpan::operator==(const TimeSpan& rhs) const {
		return ticks_ == rhs.ticks_;
	}

	TimeSpan TimeSpan::operator+=(const TimeSpan& rhs) {
		ticks_ += rhs.ticks_;
		return *this;
	}

	TimeSpan TimeSpan::operator-=(const TimeSpan& rhs) {
		ticks_ -= rhs.ticks_;
		return *this;
	}

	TimeSpan TimeSpan::operator*=(int n) {
		ticks_ *= n;
		return *this;
	}

	TimeSpan TimeSpan::operator/=(int n) {
		ticks_ /= n;
		return *this;
	}

	TimeSpan TimeSpan::Seconds(double count)
	{
		return TimeSpan(static_cast<int64_t>(count*TimeSpan::kSecond));
	}

	TimeSpan TimeSpan::Milliseconds(double count)
	{
		return TimeSpan(static_cast<int64_t>(count*TimeSpan::kMillisecond));
	}

	TimeSpan TimeSpan::Microseconds(double count)
	{
		return TimeSpan(static_cast<int64_t>(count*TimeSpan::kMicrosecond));
	}

	TimeSpan TimeSpan::Minutes(double count)
	{
		return TimeSpan(static_cast<int64_t>(count*TimeSpan::kMinute));
	}

	TimeSpan TimeSpan::Hours(double count)
	{
		return TimeSpan(static_cast<int64_t>(count*TimeSpan::kHour));
	}

	TimeSpan TimeSpan::Nanoseconds(int64_t count)
	{
		return TimeSpan(count);
	}
} // namespace end
