#pragma once
#include <functional>
#include <type_traits>
#include <assert.h>

#include "Alime/base/notCopyable.h"
#include "Alime/base/time/DateTime.h"
#include "Alime/base/time/TimeSpan.h"


//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//@author: lsaejn
//@date: 2020/07/30
//@version: Alime is not a public lib yet. 
//@summary: thread class as there is no std::thread in vs2010
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
namespace Alime
{
	enum 
	{	
		_Thrd_success,
		_Thrd_error
	};

	typedef struct
	{	/* thread identifier for Win32 */
		void* handle;	/* Win32 HANDLE */
		unsigned int tid;
	} Thread_win32;
	
	typedef Thread_win32 Thrd_ty;//or Runnable

	template<typename T>
	void Thr_set_null(T& thr)
	{
		thr.handle = nullptr; 
		thr.tid = 0;
	}
	template<typename T>
	bool Thr_is_null(T& thr)
	{
		return thr.tid == 0;
	}


/// <summary>
/// <remarks>use std::thread repalce alime::thread as if your compiler support c++11</remarks>
/// ����Сд��Ϊ�˽����滻std::thread
/// </summary>
	class Thread : public NotCopyable
	{
	public:
		typedef unsigned int  id;
		enum class ThreadState
		{
			NotStarted,
			Running,
			Stopped
		};
		typedef void* native_handle_type;
		typedef std::function<void()> ThreadFunc;
		
	public:
		Thread();
		Thread(ThreadFunc ,bool startImmediately=true, const std::string& name="");
		Thread(Thread&&) noexcept;
		Thread& operator=(Thread&&) noexcept;
		~Thread();

		void start();
		void join();
		void detach();

		bool joinable() const;
		unsigned int get_id() const;
		native_handle_type native_handle();

		void swap(Thread&);
		static unsigned int hardware_concurrency();
		static int numCreated();
		static void SetThreadName(id thread_id, const char* name);
	private:
		static unsigned int __stdcall ThreadProcFunc(void*);
	private:
		int Thrd_join(Thrd_ty thr);
		int Thrd_detach(Thrd_ty thr);
		Thread& MoveThread(Thread& );
		Thrd_ty Thr_;
		ThreadFunc func_;
		ThreadState state_;
		std::string threadName_;
		static int numCreated_;
	};

	namespace this_thread 
	{
		Thread::id get_id() noexcept;
		void yield() noexcept;
		//
		void sleep_for(int milliseconds);
		//void sleep_until(Alime::Timestamp t);
	}	// namespace this_thread
}

