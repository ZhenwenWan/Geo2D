
#ifndef __H_EW_THREAD_SPIN__
#define __H_EW_THREAD_SPIN__

#include "ewa_base/config.h"

#ifndef EW_WINDOWS
#include <pthread.h>
#else
#include <windows.h>
#endif

EW_ENTER

class DLLIMPEXP_EWA_BASE SpinLock
{
public:

	SpinLock();
	SpinLock(const SpinLock&);
	SpinLock& operator=(const SpinLock&)
	{
		return *this;
	}

	~SpinLock();

	void lock();
	void unlock();
	bool try_lock();

protected:
#ifdef EW_MSVC
	CRITICAL_SECTION impl;
#else
	pthread_spinlock_t impl;
#endif
};


EW_LEAVE
#endif
