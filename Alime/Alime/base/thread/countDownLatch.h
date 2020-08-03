#pragma once
#include "Alime/base/thread/mutex.h"
#include <atomic>

namespace Alime
{
	class CountDownLatch
	{
	public:
		CountDownLatch(int count)
			:count_(count),
			cv_(),
			lock_()
		{

		}
		void countDown()
		{
			Alime::Lock_guard<Alime::Mutex> lg(lock_);
			if (0 == --count_)
				cv_.notify_all();
		}

		void wait()
		{
			Alime::Lock_guard<Alime::Mutex> lg(lock_);
			cv_.wait(&lock_);
		}

		int getCount() const
		{
			return count_;
		}

	private:
		std::atomic<int> count_;
		Alime::Condition cv_;
		Alime::Mutex lock_;
	};
}
