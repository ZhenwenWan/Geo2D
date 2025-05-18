#ifndef __H_EW_SCRIPTING_VARIANT__
#define __H_EW_SCRIPTING_VARIANT__

#include "ewa_base/config.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/serialization/serializer.h"

#include "ewa_base/scripting/detail/colondata.h"
#include "ewa_base/scripting/detail/idx_1t.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/scripting/callable_wrap.h"
#include "ewa_base/scripting/callable_array.h"
#include "ewa_base/scripting/callable_buffer.h"

#include "ewa_base/scripting/variant_op.h"

EW_ENTER


typedef tl::mk_typelist<CallableData*,bool,int64_t,double>::type TL_VARIANT_TYPE1;

template<typename T,bool F=TL_VARIANT_TYPE1::template id<T>::value>=0>
class variant_handler_impl;

class DLLIMPEXP_EWA_BASE Variant
{
public:

	template<typename T,bool F>
	friend class variant_handler_impl;

	friend class Executor;

	inline Variant():flag(0)
	{
		data.pval=NULL;
	}

	inline Variant(CallableData* p)
	{
		if(p)
		{
			data.pval=p;
			data.pval->IncRef();
			flag=-1;
		}
		else
		{
			data.pval=NULL;
			flag=0;
		}
	}



#ifdef EW_C11
	template<typename T>
	explicit Variant(T&& v):flag(0)
	{
		reset(std::forward<T>(v));
	}
#else
	template<typename T>
	explicit Variant(const T& v):flag(0)
	{
		reset(v);
	}
#endif


	inline ~Variant()
	{
		if(flag==-1) data.pval->DecRef();
	}

	inline Variant(const Variant& o):data(o.data),flag(o.flag)
	{
		if(flag==-1) data.pval->IncRef();
	}

	inline Variant& operator=(const Variant& o)
	{
		if(o.flag==-1)
		{
			o.data.pval->IncRef();
		}
		if(flag==-1)
		{
			data.pval->DecRef();
		}

		data=o.data;
		flag=o.flag;

		return *this;
	}

	inline Variant& operator=(const String& v)
	{
		reset(v);
		return *this;
	}

#ifdef EW_C11
	inline Variant(Variant&& o) :flag(o.flag), data(o.data)
	{
		o.flag = 0;
	}
	inline Variant& operator=(Variant&& o)
	{
		swap(o);return *this;
	}
#endif

	inline Variant& operator=(const DataPtrT<CallableData>& p)
	{
		this->kptr((CallableData*)p.get());
		return *this;
	}

	inline int type() const
	{
		return flag>=0?flag:data.pval->type();
	}

	inline bool is_nil() const
	{
		return flag==0;
	}

	bool is_array() const;

	void kptr(CallableData* p);

	inline CallableData* kptr()
	{
		return flag==-1?data.pval:NULL;
	}

	inline const CallableData* kptr() const
	{
		return flag==-1?data.pval:NULL;
	}

	template<typename T>
	inline T* kptr_t()
	{
		return dynamic_cast<T*>(kptr());
	}

	template<typename T>
	inline const T* kptr_t() const
	{
		return dynamic_cast<const T*>(kptr());
	}

	inline CallableMetatable* get_metatable()
	{
		return flag==-1?data.pval->GetMetaTable():CallableData::sm_meta[flag];
	}


	template<typename T>
	inline typename tl::enable_cv<T,CallableData>::type kptr(const DataPtrT<T>& p)
	{
		kptr(const_cast<T*>(p.get()));
	}

	void unique();

	// throw if not T
	template<typename T> T& get();
	template<typename T> const T& get() const;


	// change type to T if not T
	template<typename T> T& ref();

	// clear + ref()
	template<typename T> T& ref_new();

	// ref()+unique()
	template<typename T> T& ref_unique();

	template<typename T> T* ptr();
	template<typename T> const T* ptr() const;

	template<typename T> bool set(const T& v);
	template<typename T> bool get(T& v) const;

	bool get(int32_t& v) const
	{
		int64_t val;
		if (!get(val)) return false;
		v = val;
		return true;
	}

	bool get(BitFlags& v) const
	{
		int64_t val;
		if (!get(val)) return false;
		v.clr(val);
		return true;
	}

	bool get(Variant& v) const
	{
		v = *this;
		return true;
	}

#ifdef EW_C11
	template<typename T> void reset(T&& v);
#endif

	template<typename T> void reset(const T& v=T());


	template<typename T> typename tl::enable_if_c<tl::is_convertible<T,CallableData> >::type reset(T* v)
	{
		kptr(v);
	}

	template<typename T> void reset(DataPtrT<T> v)
	{
		kptr(v.get());
	}

	void reset(const Variant& v)
	{
		(*this) = v;
	}


	void clear();

	inline void swap(Variant& o)
	{
		std::swap(flag, o.flag);
		std::swap(data, o.data);
	}

	bool operator==(const Variant& v2) const;
	bool operator!=(const Variant& v2) const;

	void Serialize(SerializerHelper sh);

	void reset_by_var(const Variant& s);

	VariantTable* get_table_ptr();

protected:

	union variant_data_union
	{
		CallableData* pval;
		double dval;
		int64_t ival;
		bool bval;
	}data;

	int64_t flag;

};


DLLIMPEXP_EWA_BASE String string_format_variants(Variant* p,int n);

DEFINE_OBJECT_NAME(Variant,"Variant")


template<>
class variant_handler_impl<CallableData*,true>
{
public:

	static const int value=-1;
	typedef CallableData* T;

	static const T& raw(const Variant& v)
	{
		EW_ASSERT(v.flag<1);
		return v.data.pval;
	}

	static T& raw(Variant& v)
	{
		EW_ASSERT(v.flag<1);
		return v.data.pval;
	}

	static void clone(Variant& d,CallableData* p)
	{
		EW_ASSERT(p!=NULL);
		CallableData* p2=p->DoCloneObject();
		EW_ASSERT(p2!=NULL);
		d.kptr(p2);
	}

	template<typename X>
	static X* ref_unique(Variant& v,X* d)
	{
		v.kptr(d);return d;
	}

	template<typename X>
	static X* ref_unique(Variant& v,DataPtrT<X> d)
	{
		v.kptr(d.get());return d.get();
	}

};


template<typename T>
class variant_handler_impl<T,false>
{
public:

	static const int value=type_flag<T>::value;

	static const T& raw(const Variant& v)
	{
		EW_ASSERT(v.type()==value);
		return static_cast<const CallableDataBaseT<T>*>(v.data.pval)->value;
	}

	static T& raw(Variant& v)
	{
		EW_ASSERT(v.type()==value);
		return static_cast<CallableDataBaseT<T>*>(v.data.pval)->value;
	}

	static T* ptr(Variant& v)
	{
		if(v.flag!=-1)
		{
			return NULL;
		}

		if(type_flag<T>::value==0)
		{
			CallableDataBaseT<T>* p=dynamic_cast<CallableDataBaseT<T>*>(v.data.pval);
			if(!p) return NULL;
			return &p->value;
		}
		else if (type_flag<T>::value==15)
		{
			CallableWrapT<T>* p = dynamic_cast<CallableWrapT<T>*>(v.kptr());
			if (p)
			{
				return &p->value;
			}
			else
			{
				return NULL;
			}
		}
		else if(type_flag<T>::value!=v.data.pval->type())
		{
			return NULL;
		}

		return &raw(v);
	}

	static T& ref(Variant& v)
	{
		T* p=ptr(v);
		if(!p)
		{
			v.kptr(new CallableWrapT<T>());
			return raw(v);
		}
		return *p;
	}

	static T& ref_new(Variant& v)
	{
		v.kptr(new CallableWrapT<T>());
		return raw(v);
	}

	static T& ref_unique(Variant& v)
	{
		T* p=ptr(v);
		if(!p)
		{
			v.kptr(new CallableWrapT<T>());
			return raw(v);
		}
		else if(v.data.pval->GetRef()!=1)
		{
			variant_handler_impl<CallableData*,true>::clone(v,v.data.pval);
			return raw(v);
		}
		return *p;
	}

	static T& ref_unique(Variant& v,const T& d)
	{
		T* p=ptr(v);
		if(!p||v.data.pval->GetRef()!=1)
		{
			v.kptr(new CallableWrapT<T>(d));
			return raw(v);
		}
		else
		{
			*p = d;
		}
		return *p;
	}

#ifdef EW_C11

	static T& ref_unique(Variant& v, T&& d)
	{
		T* p=ptr(v);
		if(!p||v.data.pval->GetRef()!=1)
		{
			v.kptr(new CallableWrapT<T>(std::forward<T>(d)));
			return raw(v);
		}
		else
		{
			*p = std::forward<T>(d);
		}
		return *p;
	}
#endif

};



template<typename X>
class variant_handler_impl<X*, false>
{
public:

	static const int value = -1;
	typedef X* T;

};

template<typename T>
class variant_handler_impl<T,true>
{
public:
	static const int value=type_flag<T>::value;

	static const T& raw(const Variant& v)
	{
		EW_ASSERT(v.type()==value);
		return *(T*)&v.data;
	}

	static T& raw(Variant& v)
	{
		EW_ASSERT(v.type()==value);
		return *(T*)&v.data;
	}
	static T* ptr(Variant& v)
	{
		if(v.flag!=value) return NULL;
		return (T*)&v.data;
	}

	static T& get(Variant& v)
	{
		if(v.flag!=value) Exception::XBadCast();
		return raw(v);
	}

	static T& ref(Variant& v)
	{
		return v.flag!=value ? ref_unique(v,T()):raw(v);
	}

	static T& ref_new(Variant& v)
	{
		return v.flag!=value ? ref_unique(v,T()):raw(v);
	}

	static T& ref_unique(Variant& v)
	{
		return v.flag!=value ? ref_unique(v,T()):raw(v);
	}

	static T& ref_unique(Variant& v,const T& d)
	{
		if(v.flag==-1) v.data.pval->DecRef();
		v.flag=value;
		return (raw(v)=d);
	}
};


template<>
class variant_handler_impl<Variant,false>
{
public:

	typedef Variant T;
	static const T& raw(const Variant& v)
	{
		return v;
	}

	static T& raw(Variant& v)
	{
		return v;
	}
	static T* ptr(Variant& v)
	{
		return &v;
	}

	static T& get(Variant& v)
	{
		return v;
	}

	static T& ref(Variant& v)
	{
		return v;
	}

	static T& ref_unique(Variant& v)
	{
		return v;
	}

	static T& ref_unique(Variant& v,const T& d)
	{
		return (v=d);
	}
};


template<typename T> class variant_handler : public variant_handler_impl<T>{};


inline void Variant::unique()
{
	if(flag==-1&&data.pval->GetRef()!=1)
	{
		variant_handler<CallableData*>::clone(*this,data.pval);
	}
}

template<typename T>
inline T& Variant::get()
{
	T* p=variant_handler<T>::ptr(*this);
	if(!p) Exception::XBadCast();
	return *p;
}

template<typename T>
inline const T& Variant::get() const
{
	const T* p=variant_handler<T>::ptr(*(Variant*)this);
	if(!p) Exception::XBadCast();
	return *p;
}


template<typename T>
inline T& Variant::ref()
{
	return variant_handler<T>::ref(*this);
}

template<typename T>
inline T& Variant::ref_unique()
{
	return variant_handler<T>::ref_unique(*this);
}

template<typename T>
inline T& Variant::ref_new()
{
	return variant_handler<T>::ref_new(*this);
}

template<typename T>
inline T* Variant::ptr()
{
	return variant_handler<T>::ptr(*this);
}

template<typename T>
inline const T* Variant::ptr() const
{
	return variant_handler<T>::ptr(*(Variant*)this);
}

template<typename T>
inline bool Variant::get(T& v) const
{
	const T* p=ptr<T>();
	if(!p) return false;
	v=*p;
	return true;
}

template<typename T>
inline bool Variant::set(const T& v)
{
	T* p=ptr<T>();
	if(!p) return false;
	*p=v;
	return true;
}

class CallableData;

namespace detail
{
	template<typename T> struct variant_type_internal{typedef T type;};

	template<> struct variant_type_internal<long>{typedef int64_t type;};
	template<> struct variant_type_internal<int32_t>{typedef int64_t type;};
	template<> struct variant_type_internal<uint32_t>{typedef int64_t type;};
	template<> struct variant_type_internal<uint64_t>{typedef int64_t type;};

	template<> struct variant_type_internal<float>{typedef double type;};

	template<> struct variant_type_internal<std::complex<double> >{typedef dcomplex type;};
	template<> struct variant_type_internal<std::complex<float> >{typedef dcomplex type;};
	template<> struct variant_type_internal<fcomplex>{typedef dcomplex type;};

	template<> struct variant_type_internal<const char*>{typedef String type;};
	template<> struct variant_type_internal<char*>{typedef String type;};

	template<int N> struct variant_type_internal<const char[N]>{typedef String type;};
	template<int N> struct variant_type_internal<char[N]>{typedef String type;};

	template<typename T> struct variant_type_internal<const T> : public variant_type_internal<T>{};
	template<typename T> struct variant_type_internal<T&> : public variant_type_internal<T>{};

	template<> struct variant_type_internal < CallableData* >
	{
		typedef CallableData* type;
	};

	template<typename T> struct variant_type_internal<DataPtrT<T> > : public variant_type_internal<CallableData*>{};
	template<typename T> struct variant_type_internal<T*> : public variant_type_internal<CallableData*>{};

	template<typename T> struct variant_type_internal<arr_xt<T> >{ typedef arr_xt<typename variant_type_internal<T>::type> type; };
	template<typename T> struct variant_type_internal<arr_1t<T> >{ typedef arr_xt<typename variant_type_internal<T>::type> type; };

	template<> struct variant_type_internal<arr_1t<String> >{typedef arr_xt<Variant> type;};



}

#ifdef EW_C11
template<typename T>
EW_FORCEINLINE void Variant::reset(T &&v)
{
	typedef typename std::remove_const<T>::type type0;
	typedef typename std::remove_reference<T>::type type1;
	typedef typename detail::variant_type_internal<type1>::type type2;
	variant_handler<type2>::ref_unique(*this,std::forward<T>(v));
}
#endif

template<typename T>
EW_FORCEINLINE void Variant::reset(const T& v)
{
	variant_handler<typename detail::variant_type_internal<T>::type>::ref_unique(*this, v);
}


EW_FORCEINLINE void Variant::kptr(CallableData* p)
{
	if(p)
	{
		p->IncRef();
		if(flag==-1)
		{
			data.pval->DecRef();
		}
		else
		{
			flag=-1;
		}
		data.pval=p;
		return;
	}
	else if(flag==-1)
	{
		data.pval->DecRef();
	}

	flag=0;
	data.pval=NULL;

}

EW_FORCEINLINE void Variant::clear()
{
	if(flag==-1) data.pval->DecRef();
	data.pval=NULL;
	flag=0;
}

inline void Variant::reset_by_var(const Variant& s)
{
	if (s.flag == -1 && s.data.pval->GetRef() != 1)
	{
		variant_handler<CallableData*>::clone(*this, s.data.pval);
	}
	else
	{
		*this = s;
	}
}

template<>
class hash_t<Variant>
{
public:
	inline uint32_t operator()(const Variant& o)
	{
		switch(o.type())
		{
		case 0: return 0;
		case 1:return hash_t<bool>()(variant_handler<bool>::raw(o));
		case 2:return hash_t<int64_t>()(variant_handler<int64_t>::raw(o));
		case 3:return hash_t<double>()(variant_handler<double>::raw(o));
		default: return o.kptr()?o.kptr()->hashcode():0;
		}
	}
};

template<template<unsigned> class P,typename T>
class lookup_table_4bit
{
public:
	static T cmap[16];

	static inline T test(unsigned text)
	{
		EW_ASSERT(text<16);
		return cmap[text];
	}
};

template<template<unsigned> class P,typename T>
class lookup_table_8bit
{
public:
	static inline T test(unsigned text)
	{
		EW_ASSERT(text<256);
		return cmap[text];
	}

	static T cmap[256];
};

template<template<unsigned> class P,typename T>
T lookup_table_4bit<P,T>::cmap[16]={
	P<0x00>::value,P<0x01>::value,P<0x02>::value,P<0x03>::value,
	P<0x04>::value,P<0x05>::value,P<0x06>::value,P<0x07>::value,
	P<0x08>::value,P<0x09>::value,P<0x0A>::value,P<0x0B>::value,
	P<0x0C>::value,P<0x0D>::value,P<0x0E>::value,P<0x0F>::value
};

template<template<unsigned> class P,typename T>
T lookup_table_8bit<P,T>::cmap[256]={
	P<0x00>::value,P<0x01>::value,P<0x02>::value,P<0x03>::value,
	P<0x04>::value,P<0x05>::value,P<0x06>::value,P<0x07>::value,
	P<0x08>::value,P<0x09>::value,P<0x0A>::value,P<0x0B>::value,
	P<0x0C>::value,P<0x0D>::value,P<0x0E>::value,P<0x0F>::value,
	P<0x10>::value,P<0x11>::value,P<0x12>::value,P<0x13>::value,
	P<0x14>::value,P<0x15>::value,P<0x16>::value,P<0x17>::value,
	P<0x18>::value,P<0x19>::value,P<0x1A>::value,P<0x1B>::value,
	P<0x1C>::value,P<0x1D>::value,P<0x1E>::value,P<0x1F>::value,
	P<0x20>::value,P<0x21>::value,P<0x22>::value,P<0x23>::value,
	P<0x24>::value,P<0x25>::value,P<0x26>::value,P<0x27>::value,
	P<0x28>::value,P<0x29>::value,P<0x2A>::value,P<0x2B>::value,
	P<0x2C>::value,P<0x2D>::value,P<0x2E>::value,P<0x2F>::value,
	P<0x30>::value,P<0x31>::value,P<0x32>::value,P<0x33>::value,
	P<0x34>::value,P<0x35>::value,P<0x36>::value,P<0x37>::value,
	P<0x38>::value,P<0x39>::value,P<0x3A>::value,P<0x3B>::value,
	P<0x3C>::value,P<0x3D>::value,P<0x3E>::value,P<0x3F>::value,
	P<0x40>::value,P<0x41>::value,P<0x42>::value,P<0x43>::value,
	P<0x44>::value,P<0x45>::value,P<0x46>::value,P<0x47>::value,
	P<0x48>::value,P<0x49>::value,P<0x4A>::value,P<0x4B>::value,
	P<0x4C>::value,P<0x4D>::value,P<0x4E>::value,P<0x4F>::value,
	P<0x50>::value,P<0x51>::value,P<0x52>::value,P<0x53>::value,
	P<0x54>::value,P<0x55>::value,P<0x56>::value,P<0x57>::value,
	P<0x58>::value,P<0x59>::value,P<0x5A>::value,P<0x5B>::value,
	P<0x5C>::value,P<0x5D>::value,P<0x5E>::value,P<0x5F>::value,
	P<0x60>::value,P<0x61>::value,P<0x62>::value,P<0x63>::value,
	P<0x64>::value,P<0x65>::value,P<0x66>::value,P<0x67>::value,
	P<0x68>::value,P<0x69>::value,P<0x6A>::value,P<0x6B>::value,
	P<0x6C>::value,P<0x6D>::value,P<0x6E>::value,P<0x6F>::value,
	P<0x70>::value,P<0x71>::value,P<0x72>::value,P<0x73>::value,
	P<0x74>::value,P<0x75>::value,P<0x76>::value,P<0x77>::value,
	P<0x78>::value,P<0x79>::value,P<0x7A>::value,P<0x7B>::value,
	P<0x7C>::value,P<0x7D>::value,P<0x7E>::value,P<0x7F>::value,
	P<0x80>::value,P<0x81>::value,P<0x82>::value,P<0x83>::value,
	P<0x84>::value,P<0x85>::value,P<0x86>::value,P<0x87>::value,
	P<0x88>::value,P<0x89>::value,P<0x8A>::value,P<0x8B>::value,
	P<0x8C>::value,P<0x8D>::value,P<0x8E>::value,P<0x8F>::value,
	P<0x90>::value,P<0x91>::value,P<0x92>::value,P<0x93>::value,
	P<0x94>::value,P<0x95>::value,P<0x96>::value,P<0x97>::value,
	P<0x98>::value,P<0x99>::value,P<0x9A>::value,P<0x9B>::value,
	P<0x9C>::value,P<0x9D>::value,P<0x9E>::value,P<0x9F>::value,
	P<0xA0>::value,P<0xA1>::value,P<0xA2>::value,P<0xA3>::value,
	P<0xA4>::value,P<0xA5>::value,P<0xA6>::value,P<0xA7>::value,
	P<0xA8>::value,P<0xA9>::value,P<0xAA>::value,P<0xAB>::value,
	P<0xAC>::value,P<0xAD>::value,P<0xAE>::value,P<0xAF>::value,
	P<0xB0>::value,P<0xB1>::value,P<0xB2>::value,P<0xB3>::value,
	P<0xB4>::value,P<0xB5>::value,P<0xB6>::value,P<0xB7>::value,
	P<0xB8>::value,P<0xB9>::value,P<0xBA>::value,P<0xBB>::value,
	P<0xBC>::value,P<0xBD>::value,P<0xBE>::value,P<0xBF>::value,
	P<0xC0>::value,P<0xC1>::value,P<0xC2>::value,P<0xC3>::value,
	P<0xC4>::value,P<0xC5>::value,P<0xC6>::value,P<0xC7>::value,
	P<0xC8>::value,P<0xC9>::value,P<0xCA>::value,P<0xCB>::value,
	P<0xCC>::value,P<0xCD>::value,P<0xCE>::value,P<0xCF>::value,
	P<0xD0>::value,P<0xD1>::value,P<0xD2>::value,P<0xD3>::value,
	P<0xD4>::value,P<0xD5>::value,P<0xD6>::value,P<0xD7>::value,
	P<0xD8>::value,P<0xD9>::value,P<0xDA>::value,P<0xDB>::value,
	P<0xDC>::value,P<0xDD>::value,P<0xDE>::value,P<0xDF>::value,
	P<0xE0>::value,P<0xE1>::value,P<0xE2>::value,P<0xE3>::value,
	P<0xE4>::value,P<0xE5>::value,P<0xE6>::value,P<0xE7>::value,
	P<0xE8>::value,P<0xE9>::value,P<0xEA>::value,P<0xEB>::value,
	P<0xEC>::value,P<0xED>::value,P<0xEE>::value,P<0xEF>::value,
	P<0xF0>::value,P<0xF1>::value,P<0xF2>::value,P<0xF3>::value,
	P<0xF4>::value,P<0xF5>::value,P<0xF6>::value,P<0xF7>::value,
	P<0xF8>::value,P<0xF9>::value,P<0xFA>::value,P<0xFB>::value,
	P<0xFC>::value,P<0xFD>::value,P<0xFE>::value,P<0xFF>::value
};

// Variant Helper

struct variant_helper
{
	/*
	CallableData*,
	bool,
	int64_t,
	double,
	dcomplex,
	String,
	VariantTable,
	arr_xt<int64_t>,
	arr_xt<double>,
	arr_xt<dcomplex>,
	arr_xt<Variant>
	*/

	template<typename T>
	static bool is_same_primitive(const Variant& v)
	{
		return v.type() == type_flag<T>::value;
	}

	static bool is_arr(const Variant& v);

	static bool is_scr(const Variant& v);

	static bool is_num(const Variant& v);

	static bool is_nested_array(const Variant& v);

	// just for generic calling
	static bool is_integer(int64_t t) { return true; }
	static bool is_integer(double t) { return fmod(t, 1) == 0; }
	template<typename T>
	static bool is_integer(const tiny_cpx<T>& d)
	{
		return d.imag() == 0 && is_integer(d.real());
	}

	template<typename T>
	static bool is_integer_arr(const arr_xt<T>& d)
	{
		for (size_t i = 0; i < d.size(); ++i)
		{
			if (!is_integer(d[i]))
			{
				return false;
			}
		}
		return true;
	}

	static bool is_integer(const Variant& v);
	static bool is_real(const Variant& v);

	static bool is_complex(const Variant& v);

};

template<>
class pl_cast_base<Variant>
{
public:
	typedef Variant type;

	template<typename O>
	static type g(const O& o)
	{
		return Variant(o);
	}

	static type g(const Variant& o)
	{
		return o;
	}

};

template<>
class pl_cast<Variant>
{
public:

	template<typename T>
	static inline Variant g(const T& v)
	{
		Variant t;
		t.reset(v);
		return t;
	}

	static inline const Variant& g(const Variant& v)
	{
		return v;
	}

	static inline const Variant& g(const Variant& v,const Variant&)
	{
		return v;
	}
};


template<typename T, typename P>
template<unsigned N>
T vv_cast<T,P>::fk<N>::value(const Variant& v)
{
	EW_ASSERT(N == v.type());
	if (tl::is_same_type<T, String>::value && N == type_flag<VariantTable>::value)
	{
		return P::g((CallableData*)v.kptr());
	}
	return P::g(variant_handler<typename flag_type<N>::type>::raw(v));
}



template<typename T, typename P>
inline T vv_cast<T, P>::g(const Variant& v)
{
	typedef T(*fn)(const Variant&);

	return lookup_table_4bit<fk, fn>::test(v.type())(v);
}

template<typename T>
EW_FORCEINLINE T pl_cast<T>::k(const Variant& v)
{
	if (v.type() == type_flag<T>::value)
	{
		return variant_handler<T>::raw(v);
	}
	else
	{
		return basetype::g(v);
	}
}

// 尝试类型转换，如果失败抛出异常
template<typename T, typename T2>
T variant_cast(const T2& v){ return pl_cast<T>::g(v); }

// 尝试类型转换，如果失败用d这个作为默认值构造返回对象
template<typename T, typename T2>
T variant_cast(const T2& v, const T& d){ return pl_cast<T>::g(v, d); }

// 将T类型对象尝试转换为T1, T2, ...
// 如果最后不成功则状态flag为负数，否则返回匹配成功的编号
// 建议用于纯数值类型转换，类似输入字符串/复数 -> 整数/浮点数/复数，包括对应的数组
// 不建议用于Variant -> VariantTable/CallableData之类的东西
// 适合有匹配优先级的情况
template<typename T>
class serial_cast
{
public:
	serial_cast(const T& _value) : value(_value), flag(-1) {}

	template<typename Ty>
	serial_cast<T>& cast(Ty& target)
	{
		try
		{
			if (flag < 0)
			{
				target = variant_cast<Ty, T>(value);
				flag = -flag - 1;
			}
		}
		catch (...)
		{
			--flag;
		}
		return *this;
	}

	operator int() const { return flag; }

private:
	const T& value;
	int flag;
};

template<typename P, unsigned N>
class flag_info1
{
public:

	typedef typename flag_type<N>::scalar scalar;
	typedef typename flag_type<N>::type type;

	static const bool num_enable = P::template rebind<scalar>::value;
	static const bool arr_enable = P::template rebind<type>::value;

	static const int value = (P::dispatch_type == pl_base1::DISPATCH_NUM) ? (num_enable ? 1 : 0) : (arr_enable ? 2 : 0);
};


template<typename B, typename P>
B& FormatHelper<B, P>::operator << (const Variant& v)
{
	if (const String* p = v.ptr<String>())
	{
		return P::append(_fmt_container(), p->c_str());
	}
	else if (const StringBuffer* p = v.ptr<StringBuffer>())
	{
		return P::append(_fmt_container(), p->data(), p->size());
	}
	else
	{
		return P::append(_fmt_container(), variant_cast<String>(v).c_str());
	}
}


template<typename T>
const void* cast_to_pointer(const T& v){ return &v; }

template<typename T>
const void* cast_to_pointer(const T* v){ return v; }

template<typename T>
const void* cast_to_pointer(const String& v){ return v.c_str(); }

bool load_var(const String&, Variant&);
bool load_var(const String&, arr_1t<Variant>&);
bool save_var(const String&, const Variant&);
bool save_var(const String&, const arr_1t<Variant>&);


class LogRecord;

Variant vec2var(const vec3d& vec);

Variant box2var(const box3d& box);

Variant rcd2var(const LogRecord& rcd);




namespace detail
{
	class SetArrayHelperBase
	{
	public:

		template<typename T>
		static inline void g(Variant& lhs, const T& rhs)
		{
			lhs.reset(rhs);
		}

		template<typename X1, typename X2>
		static inline void k(X1& lhs, const X2& rhs)
		{
			lhs = rhs;
		}

		template<typename T>
		static int vector_dim(const arr_xt<T>& val)
		{
			size_t sz = val.size();
			for (int k = 0; k < 6; ++k)
			{
				size_t nd = val.size(k);
				if (nd == sz)
				{
					return k;
				}

				if (nd != 1)
				{
					return -1;
				}
			}
			return -1;
		}

		template<typename T>
		static int vector_dim(const arr_1t<T>& val)
		{
			return 0;
		}

	};
}



template<typename T, bool K = true>
class kCallableWrapT_getarray_xt
{
public:

	static void g(const arr_xt<T>& value, Variant* bp, size_t pm)
	{
		kCallableWrapT_getarray_xt<T, K> ctx(value);
		ctx.handle(bp, pm);
	}

private:

	const T* pval;
	size_t jump;
	idx_1t idx[6];
	size_t maxdim;
	arr_xt_dims dims;

	kCallableWrapT_getarray_xt(const arr_xt<T>& value)
	{
		dims = value.get_dims();
		pval = value.data();
		maxdim = 0;
	}

	void handle(Variant* bp, size_t pm)
	{
		size_t span = 1;
		bool scalar;
		bool all_scalar = true;

		for (size_t i = 0; i < pm; i++)
		{
			size_t span2 = span * dims[i];

			if (idx[maxdim].update(bp[i], 0, dims[i], scalar) != 0)
			{
				Exception::XInvalidIndex();
			}

			if (!scalar)
			{
				all_scalar = false;
				dims[maxdim++] = span;
			}
			else if (!K)
			{
				EW_ASSERT(idx[maxdim].size == 1);
				pval += idx[maxdim](0) * span;
			}
			else
			{
				EW_ASSERT(idx[maxdim].size == 1);
				pval += idx[maxdim](0) * span;
				idx[maxdim].type = idx_1t::IDX_NONE;
				dims[maxdim++] = span;
			}

			span = span2;
		}

		if (all_scalar)
		{
			bp[0].reset(pval[0]);
			return;
		}


		arr_xt<T> tmp;
		if (maxdim == 1)
		{
			tmp.resize(idx[0].size);
			for (size_t i = 0; i < idx[0].size; i++)
			{
				tmp(i) = pval[dims[0] * idx[0](i)];
			}
		}
		else if (maxdim == 2)
		{
			tmp.resize(idx[0].size, idx[1].size);
			for (size_t j = 0; j < idx[1].size; j++)
			{
				for (size_t i = 0; i < idx[0].size; i++)
				{
					tmp(i, j) = pval[dims[0] * idx[0](i) + dims[1] * idx[1](j)];
				}
			}
		}
		else
		{
			for (size_t i = maxdim; i < 6; i++)
			{
				idx[i].size = 1;
				dims[i] = 1;
			}

			tmp.resize(idx[0].size, idx[1].size, idx[2].size, idx[3].size, idx[4].size, idx[5].size);

			for (size_t k5 = 0; k5 < idx[5].size; k5++)
				for (size_t k4 = 0; k4 < idx[4].size; k4++)
					for (size_t k3 = 0; k3 < idx[3].size; k3++)
						for (size_t k2 = 0; k2 < idx[2].size; k2++)
							for (size_t k1 = 0; k1 < idx[1].size; k1++)
								for (size_t k0 = 0; k0 < idx[0].size; k0++)
								{
									tmp(k0, k1, k2, k3, k4, k5) = pval[
										dims[0] * idx[0](k0) +
											dims[1] * idx[1](k1) +
											dims[2] * idx[2](k2) +
											dims[3] * idx[3](k3) +
											dims[4] * idx[4](k4) +
											dims[5] * idx[5](k5)

									];
								}
		}

		bp[0].reset(EW_MOVE(tmp));
	}

};

template<typename T, bool K = true>
class kCallableWrapT_getarray_1t
{
public:

	template<typename A, typename H = detail::SetArrayHelperBase>
	static void g(A& value, Variant& val, const H& h = H())
	{
		size_t sz = value.size();
		int k = val.type();
		if (k == type_flag<int64_t>::value)
		{
			size_t n = variant_handler<int64_t>::raw(val);
			if (n >= sz)
			{
				Exception::XInvalidIndex();
			}
			h.g(val, value[n]);
			return;
		}

		if (k == type_flag<double>::value || k == type_flag<bool>::value || k == type_flag<dcomplex>::value || k == type_flag<String>::value)
		{
			size_t n = variant_cast<int64_t>(val);
			if (n >= sz)
			{
				Exception::XInvalidIndex();
			}
			h.g(val, value[n]);
			return;
		}

		CallableData* p = val.kptr();
		if (!p)
		{
			Exception::XInvalidIndex();
		}

		idx_1t id0;
		if (p->__update_idx(id0, 0, sz) != 0)
		{
			Exception::XInvalidIndex();
		}

		arr_xt<T> tmp;
		tmp.resize(id0.size);

		if (!K)
		{
			int maxdim = h.vector_dim(value);
			if (maxdim > 0)
			{
				arr_xt_dims& dims = const_cast<arr_xt_dims&>(tmp.get_dims());
				std::swap(dims[0], dims[maxdim]);
			}
		}
		for (size_t k0 = 0; k0 < id0.size; k0++)
		{
			h.k(tmp[k0], value[id0(k0)]);
		}
		val.reset(EW_MOVE(tmp));
	}
};



EW_LEAVE



#endif
