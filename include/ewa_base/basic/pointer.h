#ifndef __H_EW_SMART_PTR__
#define __H_EW_SMART_PTR__

#include "ewa_base/basic/object_info.h"
#include "ewa_base/basic/pointer_detail.h"
#include "ewa_base/basic/platform.h"

EW_ENTER

// LitePtrT almost same as T*, but will initalize m_ptr to null pointer in default constructor.
template<typename T>
class LitePtrT : public detail::BasePtrT<T,detail::po_lite<T> >
{
public:

	typedef detail::BasePtrT<T,detail::po_lite<T> > basetype;
	typedef T* pointer;
	using basetype::m_ptr;

	LitePtrT(pointer p=NULL):basetype(p) {}

	inline operator pointer()
	{
		return m_ptr;
	}

	inline operator const T*() const
	{
		return m_ptr;
	}


	inline LitePtrT operator=(pointer p)
	{
		m_ptr=p;
		return *this;
	}

	template<typename Y>
	inline LitePtrT& operator=(Y* p)
	{
		m_ptr=p;
		return *this;
	}
};

template<typename T>
class LiteArrT : public LitePtrT<T>
{
public:
	typedef LitePtrT<T> basetype;
	typedef typename basetype::pointer pointer;
	using basetype::m_ptr;

	LiteArrT(pointer p = NULL) :basetype(p) {}

	T& operator[](size_t i){ return m_ptr[i]; }
	const T& operator[](size_t i) const { return m_ptr[i]; }

};

// Reference counted, T must derive from ObjectData.
template<typename T>
class DataPtrT : public detail::BasePtrT<T,detail::po_data<T> >
{
public:
    typedef detail::BasePtrT<T,detail::po_data<T> > basetype;

	template<typename X>
	DataPtrT(const DataPtrT<X>& o)
	{
		basetype::reset((X*)o.get());
	}

	DataPtrT(const DataPtrT& o)
	{
		basetype::reset((T*)o.get());		
	}



	DataPtrT(T *p=NULL) : basetype(p) {}

	DataPtrT& operator=(const DataPtrT& o)
	{
		basetype::reset((T*)o.get());
		return *this;
	}

#ifdef EW_C11
	DataPtrT(DataPtrT&& o)
	{
		this->swap(o);
	}
	DataPtrT& operator=(DataPtrT&& o)
	{
		this->swap(o);
		return *this;
	}
#endif

	template<typename Y>
	bool cast_and_set(Y* b)
	{
		if (this->m_ptr)
		{
			this->m_ptr->DecRef();
		}
		if (!b)
		{
			this->m_ptr = NULL;
			return false;
		}
		b->IncRef();
		this->m_ptr = dynamic_cast<T*>(b);
		if (this->m_ptr)
		{
			return true;
		}
		else
		{
			b->DecRef();
			return false;
		}
	}

	template<typename Y>
	bool cast_and_set(DataPtrT<Y>& b)
	{
		basetype::reset(dynamic_cast<T*>(b.get()));
		return this->m_ptr!=NULL;
	}

};

// AutoPtrT almost same as std::auto_ptr, but no copy constructor and operator=
template<typename T>
class AutoPtrT : public detail::BasePtrT<T,detail::po_auto<T> > , private NonCopyable
{
	using detail::BasePtrT<T,detail::po_auto<T> >::m_ptr;

public:
	explicit AutoPtrT(T *p=NULL) : detail::BasePtrT<T,detail::po_auto<T> >(p) {}

	inline T *release()
	{
		T *tmp=m_ptr;
		m_ptr=NULL;
		return tmp;
	}

};

template<typename T>
class TempPtrT : public AutoPtrT<T>
{
public:

	TempPtrT() {}
	TempPtrT(T* p):AutoPtrT<T>(p) {}
	TempPtrT(const TempPtrT& o)
	{
		this->swap(const_cast<TempPtrT&>(o));
	}
	TempPtrT& operator=(const TempPtrT& o)
	{
		this->swap(const_cast<TempPtrT&>(o));
		return *this;
	}
};


// if T is not derived from ObjectData, it's same as LitePtrT, else DataPtrT
template<typename T>
class FakePtrT : public tl::meta_if<tl::is_convertible<T,ObjectData>::value,DataPtrT<T>,LitePtrT<T> >::type
{
public:
	typedef typename tl::meta_if<tl::is_convertible<T,ObjectData>::value,DataPtrT<T>,LitePtrT<T> >::type basetype;
	FakePtrT(T* p=NULL):basetype(p) {}

};



template<typename T>
class WeakPtrT;

template<typename T>
class SharedPtrT : public KO_Handle<KO_Policy_pointer<T> >
{
public:

	typedef T element_type;
	typedef KO_Handle<KO_Policy_pointer<T> > basetype;
	using basetype::m_pCounter;
	using basetype::m_pHandle;

	friend class WeakPtrT<T>;

	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	explicit SharedPtrT(pointer p = NULL) : basetype(p){}
	SharedPtrT(const basetype& o) :basetype(o){}
	SharedPtrT(const SharedPtrT& o) :basetype(o){}

	template<typename X>
	SharedPtrT(const SharedPtrT<X>& o)
	{
		basetype::reset((X*)o.get(),o.counter());
	}

	pointer get()
	{
		return m_pHandle;
	}

	const_pointer get() const
	{
		return m_pHandle;
	}

	pointer operator->()
	{
		EW_ASSERT(m_pHandle!=NULL);
		return m_pHandle;
	}

	const_pointer operator->() const
	{
		EW_ASSERT(m_pHandle!=NULL);
		return m_pHandle;
	}

	operator bool() const
	{
		return m_pHandle != NULL;
	}

	reference operator*()
	{
		EW_ASSERT(m_pHandle!=NULL);
		return *m_pHandle;
	}

	const_reference operator*() const
	{
		EW_ASSERT(m_pHandle!=NULL);
		return *m_pHandle;
	}

};


template<typename T>
class SharedRefT
{
public:

	SharedRefT()
	{

	}

	SharedRefT(T& t)
	{
		m_ptr.reset(&t, NULL);
	}

	SharedRefT(const SharedRefT& o) : m_ptr(o.m_ptr)
	{

	}


	void reset(T* t, bool need_delete = false)
	{
		if (need_delete)
		{
			m_ptr.reset(t);
		}
		else
		{
			m_ptr.reset(t, NULL);
		}		
	}

	operator T&()
	{
		return *m_ptr;
	}

	operator const T&() const
	{
		return *m_ptr;
	}

	T& ref()
	{
		return *m_ptr;
	}

	const T& ref() const
	{
		return *m_ptr;
	}

	void swap(T& t)
	{
		m_ptr->swap(t);
	}

	SharedRefT& operator=(const T& o)
	{
		(*m_ptr) = o;
		return *this;
	}

protected:

	SharedPtrT<T> m_ptr;

};


template<typename T>
class WeakPtrT : public KO_Weak<KO_Policy_pointer<T> >
{
public:
	typedef T element_type;
	typedef KO_Weak<KO_Policy_pointer<T> > basetype;

	WeakPtrT(){}
	WeakPtrT(const SharedPtrT<T>& o) :basetype(o){}
	WeakPtrT(const WeakPtrT<T>& o) :basetype(o){}

	SharedPtrT<T> lock()
	{
		return basetype::lock();
	}

};


template<typename T,typename B=void>
class ObjectT : public tl::meta_if<tl::is_same_type<B,void>::value,tl::emptytype,B>::type
{
public:
	typedef T obj_type;

	ObjectT() {}
	ObjectT(T* p):m_refData(p) {}

	inline void SetData(T* d)
	{
		m_refData.reset(d);
	}

	inline T* GetData()
	{
		return m_refData.get();
	}

	inline const T* GetData() const
	{
		return m_refData.get();
	}

	void UnShare()
	{
		if(m_refData && m_refData->GetRef()!=1) SetData(static_cast<T*>(m_refData->Clone(1)));
	}

	void swap(ObjectT& other)
	{
		m_refData.swap(other.m_refData);
	}

protected:
	DataPtrT<T> m_refData;
};


#define EW_BASEPTRT_REL_OP(X) \
	template<typename T,typename P1,typename P2>\
	inline bool operator X(const detail::BasePtrT<T,P1>& lhs,const detail::BasePtrT<T,P2>& rhs)\
	{\
		return lhs.get() X rhs.get();\
	}\
	template<typename T,typename P>\
	inline bool operator X(const detail::BasePtrT<T,P>& lhs,T* rhs)\
	{\
		return lhs.get() X rhs;\
	}\
	template<typename T,typename P>\
	inline bool operator X(T* lhs,const detail::BasePtrT<T,P>& rhs)\
	{\
		return lhs X rhs.get();\
	}\
	template<typename T>\
	inline bool operator X(const SharedPtrT<T>& lhs,const SharedPtrT<T>& rhs)\
	{\
		return lhs.get() X rhs.get();\
	}\
	template<typename T>\
	inline bool operator X(const SharedPtrT<T>& lhs,T* rhs)\
	{\
		return lhs.get() X rhs;\
	}\
	template<typename T>\
	inline bool operator X(T* lhs,const SharedPtrT<T>& rhs)\
	{\
		return lhs X rhs.get();\
	}\


EW_BASEPTRT_REL_OP(==)
EW_BASEPTRT_REL_OP(!=)
EW_BASEPTRT_REL_OP(>=)
EW_BASEPTRT_REL_OP(>)
EW_BASEPTRT_REL_OP(<)
EW_BASEPTRT_REL_OP(<=)

#undef EW_BASEPTRT_REL_OP


template<typename T> class hash_t<DataPtrT<T> >
{
public:
	inline uint32_t operator()(const DataPtrT<T>& val)
	{
		return (uintptr_t(val.get()))>>4;
	}
};


template<typename T, typename B>
class ObjectInfoT<T, B, 3> : public B
{
public:
	typedef B basetype;

	ObjectInfoT(const String &name) :basetype(name) {}

	DataPtrT<T> m_ptr;

	void _ensure_ptr()
	{
		if (!m_ptr)
		{
			GMutexLocker lock;
			if (!m_ptr)
			{
				m_ptr.reset(new T);
			}
		}
	}

	T *CreateObject()
	{
		_ensure_ptr();
		return new T(*m_ptr);
	}

	virtual T* GetCachedInstance()
	{
		_ensure_ptr();
		return m_ptr.get();
	}

	static inline T* Clone(const T& obj, ObjectCloneState& cs)
	{
		return cs.type == 0 ? (T*)&obj : new T(obj);
	}

};



template<typename T, typename B>
class ObjectInfoT<T, B, 4> : public B
{
public:
	typedef B basetype;


	ObjectInfoT(const String &name) :basetype(name) {}

	DataPtrT<T> m_ptr;

	void _ensure_ptr()
	{
		if (!m_ptr)
		{
			GMutexLocker lock;
			if (!m_ptr)
			{
				m_ptr.reset(new T);
			}
		}
	}

	T *CreateObject()
	{
		_ensure_ptr();
		return m_ptr.get();

	}

	virtual T* GetCachedInstance()
	{
		_ensure_ptr();
		return m_ptr.get();
	}

	inline T* Clone(const T& obj, ObjectCloneState&)
	{
		_ensure_ptr();
		return m_ptr.get();
	}
};


EW_LEAVE

namespace std
{
	template<typename T>
	inline void swap(ew::DataPtrT<T>& v1, ew::DataPtrT<T>& v2)
	{
		v1.swap(v2);
	}
}

#endif

