
#ifndef __H_EW_THREADING_FUTURE__
#define __H_EW_THREADING_FUTURE__

#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/functor.h"


EW_ENTER


template<typename T>
class Future
{
public:

	typedef Functor<void(T*)> callback_type;

	class Data : public ObjectData
	{
	public:
		Data() :state(0){}
		Data(const T& v) :state(0), value(v){}
		int state;
		Mutex mutex;
		Condition cond;
		T value;
		callback_type callback;
	};

	Future(){ m_ptr.reset(new Data()); }
	Future(const T& v){ m_ptr.reset(new Data(v)); }


	T& get()
	{
		EW_ASSERT(m_ptr->state != 0);
		return m_ptr->value;
	}

	const T& get() const
	{
		EW_ASSERT(m_ptr->state != 0);
		return m_ptr->value;
	}

	void set(const T& v)
	{
		LockGuard<Mutex> lock(m_ptr->mutex);
		if (m_ptr->state !=0)
		{
			return;
		}

		m_ptr->value = v;
		m_ptr->state = 1;
		_do_callback(&m_ptr->value);
		m_ptr->cond.notify_all();
	}


	void cancel()
	{
		LockGuard<Mutex> lock(m_ptr->mutex);
		if (m_ptr->state == 0)
		{
			m_ptr->state = -1;
			_do_callback(NULL);
			m_ptr->cond.notify_all();
		}
	}

	bool ready() const
	{
		return m_ptr->state != 0;
	}

	int state() const
	{
		return m_ptr->state;
	}

	bool wait_for(TimeSpan ts)
	{
		if (m_ptr->state == 0)
		{
			LockGuard<Mutex> lock(m_ptr->mutex);
			if (m_ptr->state == 0)
			{
				m_ptr->cond.wait_for(m_ptr->mutex,ts);
			}
		}
		return m_ptr->state == 1;
	}

	bool wait()
	{
		if (m_ptr->state == 0)
		{
			LockGuard<Mutex> lock(m_ptr->mutex);
			if (m_ptr->state == 0)
			{
				m_ptr->cond.wait(m_ptr->mutex);
			}
		}
		return m_ptr->state == 1;
	}

	Future& callback(const callback_type& func)
	{
		LockGuard<Mutex> lock(m_ptr->mutex);
		m_ptr->callback = func;

		if (m_ptr->state > 0)
		{
			_do_callback(&m_ptr->value);
		}
		else if (m_ptr->state < 0)
		{
			_do_callback(NULL);
		}
		return *this;
	}

protected:
	DataPtrT<Data> m_ptr;

	void _do_callback(T* v)
	{
		if (!m_ptr->callback)
		{
			return;
		}

		try
		{
			m_ptr->callback(v);
		}
		catch (...)
		{
			System::LogTrace("Future::callback throws");
		}
	}
};



EW_LEAVE

#endif
