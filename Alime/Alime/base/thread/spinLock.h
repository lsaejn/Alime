#pragma once
#include <atomic>
#include <thread>

#include <Windows.h>



class SpinLockWin32
{
protected:
	volatile long token_;//token_==1 means locked
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
		//��ô˼����ԭʼֵΪ0���򽻻���Ȼ�ɹ�
		return _InterlockedExchange(&token_, 1) == 0;
	}

	void Enter()
	{
		//������ȴ����ͷţ��ٳ��Լ���
		size_t timeTried = 0;
		while (timeTried<3000 && _InterlockedCompareExchange(&token_, 1, 0) != 0)
		{
			timeTried++;
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
