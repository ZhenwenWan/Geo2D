#ifndef __H_EW_STRINGBUFFER__
#define __H_EW_STRINGBUFFER__

#include "ewa_base/config.h"

#include "ewa_base/basic/hashing.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/string_detail.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/memory/allocator.h"


#ifdef EW_MSVC
#pragma warning(disable:4307)
#endif

EW_ENTER



template<typename T>
class DLLIMPEXP_EWA_BASE StringBufferT 
	: public arr_1t<T,AllocatorN<def_allocator,1> >
	,public FormatHelper<StringBufferT<T>,typename tl::meta_if<sizeof(T)==1,FormatPolicy2,FormatPolicy1>::type>
{
public:
	typedef arr_1t<T,AllocatorN<def_allocator,1> > basetype;
	typedef typename basetype::size_type size_type;

	friend class String;

	using basetype::assign;
	using basetype::resize;
	using basetype::data;
	using basetype::size;

	StringBufferT() {}

	StringBufferT(const T* p1);
	StringBufferT(const T* p1,size_t ln){assign(p1,ln);}
	StringBufferT(const T* p1,const T* p2){assign(p1,p2);}

	StringBufferT(const StringBufferT& o):basetype(o) {}



#if defined(EW_C11)
	StringBufferT(StringBufferT&& p)
	{
		this->swap(p);
	}

	StringBufferT& operator=(StringBufferT&& p)
	{
		this->swap(p);
		return *this;
	}
#endif

	StringBufferT(const String& o){(*this)=o;}

	StringBufferT& operator=(const String& o);
	StringBufferT& operator=(const T* p1);

	StringBufferT& operator=(const StringBufferT& p)
	{
		this->impl = p.impl;
		return *this;
	}

	StringBufferT& operator+=(const StringBufferT& o)
	{
		basetype::append(o.begin(),o.end());
		return *this;
	}
	StringBufferT& operator+=(const String& o);

	bool load(const String& file, int t = FILE_TYPE_TEXT);
	bool save(const String& file, int t = FILE_TYPE_TEXT | FLAG_FILE_WC | FLAG_FILE_TRUNCATE) const;
	bool save_append(const String& file) const;

	T* c_str();
	const T* c_str() const;

	using basetype::append;

	template<typename X>
	void append(const X* p,size_t n)
	{
		if(sizeof(X)==sizeof(T)) basetype::append((T*)p,n);
		else (*this)+=String(p,n);
	}

};


template<typename T> class hash_t<StringBufferT<T> >
{
public:
	inline uint32_t operator()(const StringBufferT<T>& o)
	{
		return hash_array<T>::hash(o.data(),o.size());
	}
};

template<typename T>
inline const T* FormatPolicy::cast(const StringBufferT<T>& v)
{
	return v.c_str();
}

template<typename B, typename P>
B& FormatHelper<B,P>::operator << (const StringBuffer& v){ return P::append(_fmt_container(), v.data(), v.size()); }



inline int FormatHandlerString::printf_s(char* p, const char* s, const StringBuffer& v)
{
	return ::sprintf(p, s, v.c_str());
}


EW_LEAVE

namespace std
{
	template<typename T>
	void swap(ew::StringBufferT<T>& v1, ew::StringBufferT<T>& v2)
	{
		v1.swap(v2);
	}
}

#endif
