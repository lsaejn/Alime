#pragma once
#include <atomic>
#include <thread>

#include <Windows.h>



class SpinLockWin32
{
protected:
	volatile long token_;
public:
	
	SpinLockWin32()
		:token_(0)
	{
	}

	~SpinLockWin32()
	{
	}

	bool TryEnter()
	{
		return _InterlockedExchange(&token_, 1) == 0;
	}

	void Enter()
	{
		while (_InterlockedCompareExchange(&token_, 1, 0) != 0)
		{
			while (token_ != 0)
				_mm_pause();
		}
	}

	void Leave()
	{
		_InterlockedExchange(&token_, 0);
	}
};


class SpinLock
{
public:
	using this_type = SpinLock;
	using atomic_type = std::atomic_flag;
public:
	SpinLock() = default;
	~SpinLock() = default;

	SpinLock(const this_type&) = delete;
	SpinLock& operator=(const  this_type&) = delete;

	void Lock() noexcept
	{
		while (1)
		{
			if (!m_lock.test_and_set())
				return;
		}
	}

	void unlock() noexcept
	{
		m_lock.clear();
	}

private:
	atomic_type m_lock;
};
