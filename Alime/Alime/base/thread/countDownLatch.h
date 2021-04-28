#pragma once
#include "Alime/base/thread/mutex.h"
#include <mutex>
#include "SpinLock.h"

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
			std::lock_guard lg(lock_);
			if (0 == --count_)
				cv_.notify_all();
		}

		void wait()
		{
			//should raise exception as if count==0 here?
			std::unique_lock lg(lock_);
			while(getCount()>0)
				cv_.wait(lg);
		}

	private:
		int getCount() const
		{
			return count_;
		}

		std::mutex lock_;
		std::atomic<int> count_;
		std::condition_variable cv_;
	};

	//我没想好怎么写
	class CountDownLatchXp
	{
	public:
		CountDownLatchXp(int count)
			:count_(count),
			handle_(INVALID_HANDLE_VALUE)
		{
			handle_ = CreateEventW(NULL, FALSE, FALSE, NULL);
		}

		~CountDownLatchXp()
		{
			CloseHandle(handle_);
		}

		void countDown()
		{
			Lock_guard g(mutex_);
			--count_;
			if (count_ == 0)
				SetEvent(handle_);
		}

		void wait()
		{
			int count = 0;
			{
				Lock_guard g(mutex_);
				count = count_;
			}
			//bug will happen here,but thank for Event keeping a pulsing state
			if(count > 0)
			{
				WaitForSingleObject(handle_, INFINITE);
			}
		}

	private:
		int count_;
		FastMutex mutex_;
		HANDLE handle_;
	};
}
