
#ifndef __H_EW_THREAD_RWLOCK__
#define __H_EW_THREAD_RWLOCK__


#include "ewa_base/config.h"

#ifndef EW_WINDOWS
#include <pthread.h>
#else
#include <windows.h>
#endif

EW_ENTER



class DLLIMPEXP_EWA_BASE AtomicRWLock
{
public:

	bool try_lock_r();

	void lock_r();

	void unlock_r();

	bool try_lock_w();

	void lock_w();

	void unlock_w();

protected:
	AtomicInt64 m_val;
};


class DLLIMPEXP_EWA_BASE RWLock
{
public:
	RWLock();
	RWLock(const RWLock&);
	RWLock& operator=(const RWLock&)
	{
		return *this;
	}

	~RWLock();

	void lock_r();
	void lock_w();

	//bool trylock_r();
	//bool trylock_w();

	void unlock_r();
	void unlock_w();

protected:
#ifdef EW_MSVC
	SRWLOCK impl;
#else
	pthread_rwlock_t impl;
#endif
};



class LockPolicyRead
{
public:

	static inline void lock(RWLock& mtx)
	{
		mtx.lock_r();
	}

	static inline void unlock(RWLock& mtx)
	{
		mtx.unlock_r();
	}
};

class LockPolicyWrite
{
public:

	static inline void lock(RWLock& mtx)
	{
		mtx.lock_w();
	}

	static inline void unlock(RWLock& mtx)
	{
		mtx.unlock_w();
	}
};


EW_LEAVE


#endif
