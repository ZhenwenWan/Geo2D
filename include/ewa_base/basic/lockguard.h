#ifndef __H_EW_THREAD_LOCKGUARD__
#define __H_EW_THREAD_LOCKGUARD__

#include "ewa_base/config.h"

EW_ENTER

template<typename T>
class LockPolicyDefault
{
public:

	static inline void lock(T* mtx)
	{
		mtx->lock();
	}

	static inline void unlock(T* mtx)
	{
		mtx->unlock();
	}

	static inline bool try_lock(T* mtx)
	{
		return mtx->try_lock();
	}

};


template<typename T,typename P=LockPolicyDefault<T> >
class LockGuard : public NonCopyable
{
public:

	inline LockGuard(T& mtx_,bool already_locked):pmtx(&mtx_)
	{
		if (!already_locked) P::lock(pmtx);
	}

	inline LockGuard(T& mtx_):pmtx(&mtx_)
	{
		P::lock(pmtx);
	}

	inline ~LockGuard()
	{
		P::unlock(pmtx);
	}

	operator T&()
	{
		return *pmtx;
	}


private:
	T* pmtx;
};


class DLLIMPEXP_EWA_BASE LockGuard2 : public NonCopyable
{
public:
	typedef void(*unlock_func)(void*);

	inline LockGuard2() :pmtx(NULL),func(NULL){}

	template<typename T>
	inline LockGuard2(T& v) 
		: pmtx(NULL)
	{
		try_lock(v);
	}

	template<typename T>
	inline LockGuard2(T* v) 
		: pmtx(NULL)
	{
		if (v)
		{
			try_lock(*v);
		}
	}

	inline ~LockGuard2()
	{
		if (pmtx) func(pmtx);
	}

	template<typename T>
	void reset(T& v)
	{
		release();

	    typedef LockPolicyDefault<T> P;
		P::lock(&v);
		if (pmtx) func(pmtx);
		pmtx = &v;
		func = (unlock_func)P::unlock;
	}

	template<typename T>
	void reset(T* v)
	{
		release();

		if (!v)
		{
			return;
		}

	    typedef LockPolicyDefault<T> P;
		P::lock(v);
		if (pmtx) func(pmtx);
		pmtx = v;
		func = (unlock_func)P::unlock;
	}


	template<typename T>
	bool try_lock(T& v)
	{
		EW_ASSERT(!pmtx);

	    typedef LockPolicyDefault<T> P;
		if (!P::try_lock(&v)) return false;
		if (pmtx) func(pmtx);
		pmtx = &v;
		func = (unlock_func)P::unlock;
		return true;
	}

	void release()
	{
		if (!pmtx) return;
		func(pmtx);
		pmtx = NULL;
	}

	inline operator bool(){ return pmtx!=NULL; }

private:
	void* pmtx;
	unlock_func func;
};


template<typename T,typename B=NonCopyable>
class LockState : public B
{
public:

 	T& value;
	T oldvalue;

	inline LockState(T& v) :value(v), oldvalue(v)
	{

	}

	template<typename T2>
	inline LockState(T& v,const T2 &n):value(v),oldvalue(n)
	{
		std::swap(value, oldvalue);
	}

	inline ~LockState()
	{
		std::swap(value, oldvalue);
	}
};



class LockState2 : private NonCopyableAndNonNewable
{
public:

	LockState2() : p_object(NULL) {}

	template<typename T,typename T2>
	inline LockState2(T& v, const T2& n) :p_object(new LockState<T, mp_obj_v>(v, n)){}

	template<typename T>
	inline LockState2(T& v) :p_object(new LockState<T, mp_obj_v>(v)){}

	template<typename T, typename T2>
	void lock(T& v, const T2& n)
	{
		mp_obj_v* p=new LockState<T, mp_obj_v>(v, n);
		delete p_object;
		p_object = p;
	}

	~LockState2()
	{
		delete p_object;
	}

private:
	mp_obj_v* p_object;
};


class DLLIMPEXP_EWA_BASE AtomicMutex;
class DLLIMPEXP_EWA_BASE StaticMutex : public NonCopyableAndNonNewable
{
public:
	operator AtomicMutex&(){ return *(AtomicMutex*)this;}

	static StaticMutex ms_mutex;
private:
	int64_t val[4];
};


template<>
class DLLIMPEXP_EWA_BASE LockPolicyDefault<StaticMutex>
{
public:
	static void lock(StaticMutex* mtx);
	static void unlock(StaticMutex* mtx);
};


class OnceLocker : public NonCopyableAndNonNewable
{
public:
	OnceLocker() : b_tested(false){}

	inline bool test() const
	{
		if (b_tested)
		{
			return false;
		}

		b_tested = true;
		return true;
	}

private:
	mutable volatile bool b_tested;
};


EW_LEAVE
#endif
