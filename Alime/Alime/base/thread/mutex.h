#pragma once

#include "windows.h"
#include "winbase.h"
#include <exception>
#include  "Alime/base/thread/thread.h"

namespace Alime
{
	class FastMutex
	{
	public:
		FastMutex()
		{
			::InitializeCriticalSection(&cs_);
		}
		~FastMutex()
		{
			::DeleteCriticalSection(&cs_);
		}
		void lock()
		{
			EnterCriticalSection(&cs_);
		}

		bool try_lock()
		{
			return TryEnterCriticalSection(&cs_) != 0;
		}

		bool try_lock_for(unsigned int seconds)
		{
			//fix me
			return try_lock();
		}

		void unlock()
		{
			LeaveCriticalSection(&cs_);
		}

		LPCRITICAL_SECTION native_handle()
		{
			return &cs_;
		}

		FastMutex(const FastMutex&) = delete;
		FastMutex& operator=(const FastMutex&) = delete;
	private:
		CRITICAL_SECTION cs_;
	};

	class FastCondition
	{
	public:
		FastCondition()
		{
			InitializeConditionVariable(&m_condition_variable);
		}

		~FastCondition()
		{

		}
		FastCondition(const FastCondition&) = delete;
		FastCondition& operator=(const FastCondition&) = delete;

		virtual void destroy() {}

		virtual void wait(FastMutex* lock)
		{
			if (!FastCondition::wait_for(lock, INFINITE))
				std::terminate();
		}

		virtual bool wait_for(FastMutex* lock, unsigned int milliseconds)
		{
			return SleepConditionVariableCS(&m_condition_variable, static_cast<FastMutex*>(lock)->native_handle(), milliseconds) != 0;
		}

		virtual void notify_one()
		{
			WakeConditionVariable(&m_condition_variable);
		}

		virtual void notify_all()
		{
			WakeAllConditionVariable(&m_condition_variable);
		}

	private:
		CONDITION_VARIABLE m_condition_variable;
	};

	/*
	¼òµ¥µÄ·ÇµÝ¹éËø
	*/
	class Mutex/*:public Lockable*/
	{
	public:
		Mutex():
			holder_(0)
		{
			InitializeSRWLock(&m_srw_lock);
		}

		~Mutex()
		{
			assert(holder_ == 0);
			destroy();
		}
		Mutex(const Mutex&) = delete;
		Mutex& operator=(const Mutex&) = delete;

		virtual void destroy()
		{

		}

		virtual void lock()
		{
			AcquireSRWLockExclusive(&m_srw_lock);
			assignHolder();
		}

		virtual bool try_lock()
		{
			//If the lock is successfully acquired, the return value is nonzero.
			bool ret= TryAcquireSRWLockExclusive(&m_srw_lock) != 0;
			if (ret)
				assignHolder();
			return ret;
		}

		virtual bool try_lock_for(unsigned int seconds)
		{
			return Mutex::try_lock();
		}

		virtual void unlock()
		{
			clearHolder();
			ReleaseSRWLockExclusive(&m_srw_lock);		
		}

		PSRWLOCK native_handle()
		{
			return &m_srw_lock;
		}

	private:
		void assignHolder()
		{
			holder_ = Alime::this_thread::get_id();
		}
		void clearHolder()
		{
			holder_ = 0;
		}

	private:
		SRWLOCK m_srw_lock;
		Alime::Thread::id holder_;
	};


	class Condition
	{
	public:
		Condition()
		{
			InitializeConditionVariable(&m_condition_variable);
		}

		~Condition()
		{

		}
		Condition(const Condition&) = delete;
		Condition& operator=(const Condition&) = delete;

		virtual void destroy() {}

		virtual void wait(Mutex *lock)
		{
			if (!Condition::wait_for(lock, INFINITE))
				std::terminate();
		}

		virtual bool wait_for(Mutex *lock, unsigned int timeout)
		{
			return SleepConditionVariableSRW(&m_condition_variable, static_cast<Mutex *>(lock)->native_handle(), timeout, 0) != 0;
		}

		virtual void notify_one()
		{
			WakeConditionVariable(&m_condition_variable);
		}

		virtual void notify_all()
		{
			WakeAllConditionVariable(&m_condition_variable);
		}

	private:
		CONDITION_VARIABLE m_condition_variable;
	};



	template<typename T>
	class Lock_guard
	{
	public:
		Lock_guard( T& lock)
			:lock_(lock)
		{
			lock_.lock();
		}
		~Lock_guard()
		{
			lock_.unlock();
		}
	private:
		 T& lock_;
	};
}
