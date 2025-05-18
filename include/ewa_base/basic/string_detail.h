#ifndef __H_EW_STRING_DETAIL_H__
#define __H_EW_STRING_DETAIL_H__

#include "ewa_base/config.h"
#include "ewa_base/basic/exception.h"
#include "ewa_base/math/tiny_cpx.h"
#include <string>
#include <cstring>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


EW_ENTER

DLLIMPEXP_EWA_BASE void* mp_alloc(size_t n);
DLLIMPEXP_EWA_BASE void mp_free(void* p);

class DLLIMPEXP_EWA_BASE String;
class DLLIMPEXP_EWA_BASE Variant;

template<typename T>
class DLLIMPEXP_EWA_BASE StringBufferT;

typedef StringBufferT<char> StringBuffer;
typedef StringBufferT<wchar_t> WStringBuffer;

namespace detail
{
	class DLLIMPEXP_EWA_BASE StringHolder
	{
	public:
		StringHolder();
		StringHolder(const StringHolder&);
		StringHolder& operator=(const StringHolder&);

		StringHolder(const Variant& v);
		StringHolder(const wchar_t* p);
		StringHolder(const wchar_t* p,size_t n);
		~StringHolder();

		void reset(const char* p);
		void reset(const wchar_t* p);
		void reset(const wchar_t* p,size_t n);
		void reset(const Variant& v,const char* f=NULL);

		operator const char*() const {return m_ptr;}
		const char* c_str() const {return m_ptr;}
		size_t size() const {return m_size;}

	private:
		const char* m_ptr;
		size_t m_size;
	};

	class DLLIMPEXP_EWA_BASE RawStringPtr
	{
	public:
		char* m_ptr;
	};
}



class DLLIMPEXP_EWA_BASE FormatState0
{
public:
	typedef const char* char_pointer;

	char_pointer p1, p2;
	char_pointer f1, f2;

	size_t n_vpos;
	size_t n_vpos_old;
	size_t n_fmt_width[2];
	int n_fmt_flag;
	int n_fmt_type;

	bool b_fmt_ok;
	const char* p0;

	const char* c_str(){ return p0; }
	operator String&(){ return *(String*)&p0; }

	void init(char_pointer p){ p0 = p1 = p2 = p; n_vpos = 0; }
	void init(char_pointer p, bool copy);

	void init(const wchar_t* p);
	void init(const Variant& v);

	detail::StringHolder wproxy;

};

template<typename T>
class FormatHandlerNumber
{
public:

	static int printf_s(char* p, const char* s, int32_t v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, int64_t v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, uint32_t v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, uint64_t v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, void* v)
	{
		return ::sprintf(p, s, (T)intptr_t(v));
	}

	static int printf_s(char* p, const char* s, double v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, float v)
	{
		return ::sprintf(p, s, (T)v);
	}

	static int printf_s(char* p, const char* s, const String& v);

	static int printf_s(char* p, const char* s, const char* const v);

	template<typename T2>
	static int printf_s(char* p, const char* s, T2* const &v)
	{
		return ::sprintf(p, s, (T)intptr_t(v));
	}

	template<typename T2>
	static int printf_s(char* p, const char* s, const T2& v)
	{
		return ::sprintf(p, s, variant_cast<T>(v));
	}
};


class FormatHandlerString
{
public:

	static int printf_s(char* p, const char* s, const char* const v)
	{
		return ::sprintf(p, s, v);
	}

	static int printf_s(char* p, const char* s, const std::string& v)
	{
		return ::sprintf(p, s, v.c_str());
	}

	template<int N>
	static int printf_s(char* p, const char* s, char const v[N])
	{
		return ::sprintf(p, s, v);
	}


	static int printf_s(char* p, const char* s, const String& v);

	static int printf_s(char* p, const char* s, const StringBuffer& v);

	static int printf_s(char* p, const char* s, const wchar_t* v);


	static int printf_s(char* p, const char* s, int32_t v);
	static int printf_s(char* p, const char* s, int64_t v);

	static int printf_s(char* p, const char* s, uint32_t v);
	static int printf_s(char* p, const char* s, uint64_t v);



	template<typename T2>
	static int printf_s(char* p, const char* s, const T2& v)
	{
		return ::sprintf(p, s, variant_cast<String>(v).c_str());
	}
};


template<typename T>
class FormatHandlerPointer : public FormatHandlerNumber<intptr_t>
{
public:



};




class DLLIMPEXP_EWA_BASE FormatHandlerBase
{
public:

	static inline void init(FormatState0& sb, const char* v)
	{
		sb.init(v);
	}

	static inline void init(FormatState0& sb, const wchar_t* v)
	{
		sb.init(v);
	}

	static inline void init(FormatState0& sb, const Variant& v)
	{
		sb.init(v);
	}

	static void init(FormatState0& sb, const String& v);
	static void init(FormatState0& sb, const StringBuffer& v);
	static void init(FormatState0& sb, const StringBufferT<wchar_t>& v);
	static void init(FormatState0& sb, const std::basic_string<char>& v);

	template<typename X>
	static inline void handle_n(size_t n, const X& v)
	{
		on_format_error("format_error, not pointer");
	}

	template<typename T2>
	static int printf_s(char* p, const char* s, const T2& v)
	{
		return ::sprintf(p, s, v);
	}

	template<typename T2>
	static int printf_s(char* p, FormatState0& s, const T2& v)
	{
		switch (s.n_fmt_type)
		{
		case 1:
			return FormatHandlerNumber<int64_t>::printf_s(p, s.f1, v);
		case 2:
			return FormatHandlerNumber<double>::printf_s(p, s.f1, v);
		case 3:
			return FormatHandlerString::printf_s(p, s.f1, v);
		case 4:
			return FormatHandlerPointer<T2>::printf_s(p, s.f1, v);
		default:
			on_format_error(s.f1);
			return 0;
		}
	}

	static void on_format_error(const char* p);
};


template<typename T>
class FormatHandler : public FormatHandlerBase
{
public:

	template<typename G>
	static inline void handle_s(G& sb, const T& v)
	{
		sb << v;
	}

	template<typename G>
	static inline void handle_t(G& sb, const T& v)
	{
		sb << v;
	}


	using FormatHandlerBase::handle_n;

	template<typename X>
	static inline typename tl::enable_tl<tl::integer_type, X, void>::type handle_n(size_t n, X* v)
	{
		*v = n;
	}



};




template<int N>
class CharCompare;


template<>
class CharCompare<0>
{
public:

	static int g(char ch1, char ch2)
	{
		if (ch1 == ch2)
		{
			return 0;
		}
		return ch1 < ch2 ? -1 : +1;
	}

	static int g(wchar_t ch1, wchar_t ch2)
	{
		if (ch1 == ch2)
		{
			return 0;
		}
		return ch1 < ch2 ? -1 : +1;
	}

};

template<>
class CharCompare<1>
{
public:

	static int g(char ch1, char ch2)
	{
		if (ch1 == ch2)
		{
			return 0;
		}

		if (ch1 >= 'A' && ch1 <= 'Z')
		{
			ch1 = ch1 - 'A' + 'a';
		}

		if (ch2 >= 'A' && ch2 <= 'Z')
		{
			ch2 = ch2 - 'A' + 'a';
		}

		return CharCompare<0>::g(ch1, ch2);
	}

	static int g(wchar_t ch1, wchar_t ch2)
	{
		if (ch1 == ch2)
		{
			return 0;
		}

		if (ch1 >= 'A' && ch1 <= 'Z')
		{
			ch1 = ch1 - 'A' + 'a';
		}

		if (ch2 >= 'A' && ch2 <= 'Z')
		{
			ch2 = ch2 - 'A' + 'a';
		}

		return CharCompare<0>::g(ch1, ch2);
	}

};

template<int N>
class StringFunctions
{
public:
	typedef CharCompare<N> fn;

	static int compare(const char* p1, const char* p2)
	{
		while (*p1)
		{
			int ret = fn::g(*p1, *p2);
			if (ret != 0)
			{
				return ret;
			}
			++p1;
			++p2;
		}

		return *p2 ? -1 : 0;
	}

	static bool start_with(const char* p1, const char* p2)
	{
		while (1)
		{
			if (*p2 == 0)
			{
				return true;
			}

			if (fn::g(*p1, *p2) != 0)
			{
				return false;
			}

			++p1;
			++p2;
		}
	}


	static bool end_with(const char* p1, const char* p2)
	{
		size_t n1 = ::strlen(p1);
		size_t n2 = ::strlen(p2);
		if (n1 < n2)
		{
			return false;
		}
		return compare(p1 + n1 - n2, p2) == 0;
	}
};


class DLLIMPEXP_EWA_BASE FormatPolicy
{
public:
	template<typename G>
	static inline size_t width(const G& o)
	{
		return 128;
	}

	template<typename T>
	static inline size_t width(const std::basic_string<T>& o)
	{
		return width(o.c_str());
	}

	template<typename T>
	static inline size_t width(const StringBufferT<T>& o)
	{
		return sizeof(T)*o.size()+8;
	}

	static size_t width(const Variant& o);


	static inline size_t width(const void* o)
	{
		return 32;
	}

	static inline size_t width(const char* o)
	{
		return std::char_traits<char>::length(o)+8;
	}

	static inline size_t width(const wchar_t* o)
	{
		return std::char_traits<wchar_t>::length(o)*2+8;
	}


	template<typename G>
	static inline G cast(const G& v){ return v; }

	static inline const char* cast(const char* v){return v;}
	static inline const wchar_t* cast(const wchar_t* v){return v;}

	static const char* cast(const String& v);

	template<typename T>
	static inline const T* cast(const std::basic_string<T>& v){return v.c_str();}

	template<typename T>
	static const T* cast(const StringBufferT<T>& v);

	template<typename T,typename G>
	static T* do_format_integer(T* p,G v)
	{
		typedef typename unsigned_integer_type<sizeof(G)>::type U;

		p[0]=T(0);
		if(v==0)
		{
			*--p='0';
			return p;
		}

		bool sign=v<0;
		U u=sign?~v+1:v;
		while(u>0)
		{
			*--p='0'+(u%10);
			u=u/10;
		}

		if(sign) *--p='-';

		return p;
	}
};


// BContainerPolicy1 container B has no get_uninitialized_buffer/return_initialized_buffer
template<typename B>
class BContainerPolicy1
{
public:

	typedef B container_type;

	char buffer[1024];

	container_type& return_initialized_buffer(container_type& b,size_t n,char* p)
	{
		b.append(p,n);
		if(p!=buffer)
		{
			mp_free(p);
		}
		return b;
	}

	char* get_uninitialized_buffer(container_type& b,size_t n)
	{
		if(n<sizeof(buffer)) return buffer;
		char* p=(char*)mp_alloc(n);
		if(!p) Exception::XBadAlloc(n);
		return p;
	}

	static container_type& append(container_type& b,const char* p,size_t n)
	{
		b.append(p,n);
		return b;
	}
};

// BContainerPolicy2 container B has get_uninitialized_buffer/return_initialized_buffer
template<typename B>
class BContainerPolicy2
{
public:

	typedef B container_type;

	static container_type& return_initialized_buffer(container_type& b,size_t n,char* p)
	{
		b.return_initialized_buffer(n);
		return b;
	}

	static char* get_uninitialized_buffer(container_type& b,size_t n)
	{
		return (char*)b.get_uninitialized_buffer(n);
	}

	static container_type& append(container_type& b,const char* p,size_t n)
	{
		b.append(p,n);
		return b;
	}
};

DLLIMPEXP_EWA_BASE int variant_snprintf(char* p,size_t n,const char* s,const Variant& v);

template<template<typename> class C>
class BFormatPolicy : public FormatPolicy
{
public:

	template<typename B,typename G>
	static B& on_sprint_error(B& b,size_t n,const char* s,G v)
	{
		EW_UNUSED(v);
		EW_UNUSED(n);
		b.append(s,std::char_traits<char>::length(s));
		return b;
	}

	template<typename B,typename G>
	static B& format(B& b, size_t n, const char* s, const G& v)
	{

		C<B> cb;
		EW_UNUSED(cb);
		n = std::max(FormatPolicy::width(v), n);

		char* p = cb.get_uninitialized_buffer(b, n);

		int nd = FormatHandler<G>::printf_s(p, s, v);

		if (nd >= 0)
		{
			cb.return_initialized_buffer(b, nd, p);
		}
		else
		{
			cb.return_initialized_buffer(b, 0, p);
			on_sprint_error(b, n, s, v);
		}
		return b;
	}

	template<typename B,typename G>
	static B& format(B& b, size_t n, FormatState0& s, const G& v)
	{

		C<B> cb;
		EW_UNUSED(cb);
		n = std::max(FormatPolicy::width(v), n);

		char* p = cb.get_uninitialized_buffer(b, n);

		int nd = FormatHandler<G>::printf_s(p, s, v);

		if (nd >= 0)
		{
			cb.return_initialized_buffer(b, nd, p);
		}
		else
		{
			cb.return_initialized_buffer(b, 0, p);
			on_sprint_error(b, n, s.f1, v);
		}
		return b;
	}


	template<typename B,typename G>
	static B& format_integer(B& b,G& o)
	{
		char buf[64];
		char* p2=buf+63;
		char* p1=FormatPolicy::do_format_integer(p2,o);
		return C<B>::append(b,p1,p2-p1);
	}

	template<typename B>
	static B& append(B& b,const char* p)
	{
		return C<B>::append(b,p,std::char_traits<char>::length(p));
	}

	template<typename B>
	static B& append(B& b,const char* p,size_t n)
	{
		return C<B>::append(b,p,n);
	}

	template<typename B>
	static B& append(B& b,const detail::StringHolder& o)
	{
		return C<B>::append(b,o.c_str(),o.size());
	}
};

class FormatPolicy1 : public BFormatPolicy<BContainerPolicy1>
{
public:

};

class FormatPolicy2 : public BFormatPolicy<BContainerPolicy2>
{
public:

};


template<typename B,typename P=FormatPolicy1>
class FormatHelper
{
public:

	typedef P Policy;
	B& operator<<(bool v){return P::append(_fmt_container(),v?"true":"false");}
	B& operator<<(char v){	return P::append(_fmt_container(),&v,1);}
	B& operator<<(int32_t v){return P::format_integer(_fmt_container(),v);}
	B& operator<<(int64_t v){return P::format_integer(_fmt_container(),v);}
	B& operator<<(uint32_t v){return P::format_integer(_fmt_container(),v);}
	B& operator<<(uint64_t v){return P::format_integer(_fmt_container(),v);}

#ifdef EW_WINDOWS
	B& operator<<(long v){return P::format_integer(_fmt_container(),v);}
#endif

	B& operator<<(float v){return P::format(_fmt_container(),64,"%g",double(v));}
	B& operator<<(double v){return P::format(_fmt_container(),64,"%g",v);}
	B& operator<<(long double v){ return P::format(_fmt_container(), 64, "%Lg", v); }
	B& operator<<(const char* v){return P::append(_fmt_container(),v);}
	B& operator<<(const unsigned char* v){return P::append(_fmt_container(),(const char*)v);}
	B& operator<<(const void* v){return P::format(_fmt_container(),16,"%p",v);}
	B& operator<<(const std::basic_string<char>& v){return P::append(_fmt_container(),v.c_str(),v.size());}


	B& operator<<(wchar_t v);
	B& operator<<(const wchar_t* v);

	B& operator<<(const Variant& v);
	B& operator<<(const String& v);
	B& operator<<(const StringBuffer& v);


	template<typename T>
	B& operator<<(const tiny_cpx<T>& v)
	{
		auto& os(*this);

		if (v.imag() == T(0))
		{
			os << v.real();
		}
		else if (v.real() == T(0))
		{
			os << v.imag()<<"i";
		}
		else
		{
			os << "(" << v.real();
			P::format(_fmt_container(), 64, "%+g", double(v.imag()));
			os << "i)";
		}

		return _fmt_container();
	}

private:
	EW_FORCEINLINE B& _fmt_container(){return static_cast<B&>(*this);}
};


class DLLIMPEXP_EWA_BASE StringDetail
{
public:

	typedef char char_type;

	static inline char_type* str_empty()
	{
		return (char_type*)const_empty_buffer;
	}

	static inline char_type* str_alloc(size_t s)
	{
		EW_ASSERT(s+1!=0);
		char_type* p=(char_type*)mp_alloc(s+1);
		if (p == NULL)
		{
			Exception::XBadAlloc(s + 1);
		}
		p[s]=0;
		return p;
	}

	static inline void str_free(const char_type* p)
	{
		if(p!=const_empty_buffer)
		{
			mp_free((void*)p);
		}
	}

	static inline char_type* str_dup(const char_type* s)
	{
		size_t n=std::char_traits<char_type>::length(s);
		return str_dup(s,n);
	}

	static inline char_type* str_dup(const char_type* s,size_t n)
	{
		if(n==0)
		{
			return str_empty();
		}
		char_type* dst=str_alloc(n);
		memcpy(dst,s,sizeof(char_type)*n);
		return dst;
	}

	static inline char_type* str_cat(const char_type* p1,const char_type* p2)
	{
		size_t n1=std::char_traits<char_type>::length(p1);
		size_t n2=std::char_traits<char_type>::length(p2);
		return str_cat(p1,n1,p2,n2);
	}

	static inline char_type* str_cat(const char_type* p1,size_t n1,const char_type* p2,size_t n2)
	{
		size_t n=n1+n2;
		char_type* dst=str_alloc(n);
		memcpy(dst,p1,sizeof(char_type)*n1);
		memcpy(dst+n1,p2,sizeof(char_type)*n2);
		return dst;
	}

	static void str_reset(String& str, const char* p);

};


#define STRING_FORMATER_FORMAT_FUNCS_4(X,Y,Z,D)\
template<\
	typename T0,\
	typename T1,\
	typename T2=tl::nulltype,\
	typename T3=tl::nulltype,\
	typename T4=tl::nulltype,\
	typename T5=tl::nulltype,\
	typename T6=tl::nulltype,\
	typename T7=tl::nulltype,\
	typename T8=tl::nulltype,\
	typename T9=tl::nulltype\
>\
X(\
	D\
	const T0& v0,\
	const T1& v1,\
	const T2& v2=T2(),\
	const T3& v3=T3(),\
	const T4& v4=T4(),\
	const T5& v5=T5(),\
	const T6& v6=T6(),\
	const T7& v7=T7(),\
	const T8& v8=T8(),\
	const T9& v9=T9()\
)\
{\
	Y fb(v0);\
	StringFormater::Format(fb,v1,v2,v3,v4,v5,v6,v7,v8,v9);\
	Z;\
}\
template<typename T0> X(D const T0& v0)\
{\
	Y fb(v0);\
	Z;\
}

#define STRING_FORMATER_FORMAT_FUNCS_SB(X,Z) STRING_FORMATER_FORMAT_FUNCS_4(X,FormatStateSb,Z,)


EW_LEAVE
#endif
