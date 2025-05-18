#ifndef __H_EW_BASIC_SCANNER_BASE__
#define __H_EW_BASIC_SCANNER_BASE__

#include "ewa_base/config.h"
#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/lookuptable.h"
#include "ewa_base/math/tiny_cpx.h"

EW_ENTER


template<typename T>
class char_pos_t
{
public:

	typedef T type;
	typedef const T* pointer;

	char_pos_t(pointer p = NULL){ reset(p); }

	void advance()
	{
		if (pcur[0] == '\n')
		{
			++line;
			cpos = 1;
		}
		else
		{
			++cpos;
		}
		++pcur;
	}

	void advance(int n)
	{
		if (n < 0)
		{
			Exception::XError("invalid_operation",false);
		}

		for (int i = 0; i < n; i++)
		{
			advance();
		}
	}

	void reset(pointer p){pcur = p;line = 1;cpos = 1;}

	char_pos_t& operator=(pointer v){ reset(v); return *this; }
	char_pos_t& operator++(){ advance(); return *this; }
	char_pos_t& operator+=(int n){ advance(n); return *this; }
	pointer operator++(int){ pointer p = pcur; advance(); return p; }

	operator pointer(){ return pcur; }

	size_t line;
	size_t cpos;
	pointer pcur;
};



template<typename B>
class ScannerHelperBase
{
public:
	typedef typename B::type type;
	typedef typename B::pointer pointer;
};

template<typename T>
class ScannerHelperBase<const T*>
{
public:
	typedef T type;
	typedef const T* pointer;
};


namespace detail
{

	template<size_t N,size_t V>
	class max_digit_count;

	template<size_t N>
	class max_digit_count<N, 0>
	{
	public:
		static const size_t value = 0;
	};

	template<size_t N,size_t V>
	class max_digit_count
	{
	public:
		static const size_t value = max_digit_count<N, V / N>::value + 1;
	};
}






template<typename B>
class DLLIMPEXP_EWA_BASE ScannerHelper : public ScannerHelperBase<B>
{
public:

	typedef ScannerHelperBase<B> basetype;
	typedef typename basetype::type type;
	typedef typename basetype::pointer pointer;

	template<template<unsigned> class H, unsigned U>
	class rebind
	{
	public:
		typedef typename tl::meta_if<sizeof(typename basetype::type) == 1, lookup_table<H>, lookup_table_u<H, U> >::type type;
	};


	template<template<unsigned> class H,unsigned U=0>
	static inline void skip(B& p)
	{
		typedef typename rebind<H, U>::type lktable;
		while (lktable::test(*p)) ++p;
	}


	template<typename T,int N>
	static void read_uint_t(B &p, T& v_)
	{
		typedef typename rebind<lkt_number16b,16>::type lktable;
		unsigned char w;

		typedef typename integer_type<sizeof(T)>::unsigned_type unsigned_type;
		unsigned_type& v(*(unsigned_type*)&v_);

		
		size_t n = detail::max_digit_count<N, unsigned_type(-1)>::value;

		v = 0;
		for (size_t i = 1; i < n; i++)
		{
			w = lktable::test(p[0]);
			if (w >= N)
			{
				return;
			}
			v = v*N + w;
			++p;
		}

		while (1)
		{
			w = lktable::test(p[0]);
			if (w >= N)
			{
				return;
			}
			unsigned_type tmp = v*N + w;
			if (v > tmp || (tmp - w) / v != N)
			{
				OnIntegerOverflow();
			}
			v = tmp;
			++p;
		}		

	}

	template<typename T>
	static void read_uint(B &p, T& v)
	{
		v = 0;

		char ch = p[0];
		if (ch != '0')
		{
			read_uint_t<T,10>(p, v);
			return;
		}

		typedef typename rebind<lkt2uppercase,0>::type rebind_type;
		ch = rebind_type::test(*++p);
		if (ch == 'X')
		{
			read_uint_t<T,16>(++p, v);
		}
		else if (ch == 'B')
		{
			read_uint_t<T,2>(++p, v);
		}
		else if (ch >= '0'&&ch <= '9')
		{
			read_uint_t<T,8>(p, v);
		}
		else if (ch == 0)
		{
			EW_NOOP();
		}
		else
		{
			EW_NOOP();
		}
	}

	static bool read_sign(B &p);
	static bool read_number(B &p, double& v);
	static bool read_number(B &p, int64_t& v);
	static bool read_number_ex(B &p, dcomplex& v);

	static int read_number_2(B &p, int64_t& v1,double& v2);

	static void skip_comment_1(B &p);
	static void skip_comment_2(B &p);
	static void skip_line(B &p);
	static void skip_blank(B &p);
	static void skip_blank_inline(B &p);
	static void skip_blank_and_comment(B &p);

	static void read_chunk(B& p, String& name);
	static void read_chunk_raw(B& p, String& name);
	static void read_string(B& p, String& name, bool unescape_value);

	static bool match_str(B& p, pointer x,size_t n);

};


EW_LEAVE
#endif
