
#ifndef __H_EW_SCRIPTING_PL_DISPATCH__
#define __H_EW_SCRIPTING_PL_DISPATCH__

#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/executor.h"
#include "ewa_base/scripting/callable_class.h"

EW_ENTER

// N: type id of argument
// X: policy ID, 0 ~ p2p scattering mode for numerical functions (derived from P::g(scalar_type)
//               1 ~ non-scattering mode for numerical functions (defined by P::g(arr_type))
template<typename P,unsigned N,unsigned X>
class pl1_dispatch : public pl1_dispatch_base<P>{};

template<typename P,unsigned N>
class pl1_dispatch<P,N,1> : public pl1_dispatch_base<P>
{
public:
	// call by scripting
	template<typename T>
	static typename tl::enable_if<type_flag<T>::is_scr>::type g(Executor& ks,const T& v)
	{
		typedef typename P::template rebind<T>::type retx;
		retx y=P::g(v);
		ks.ci1.nsp[0].reset(y);
	}

	template<typename T>
	static typename tl::enable_if<type_flag<T>::is_arr>::type g(Executor& ks,const T& v)
	{
		typedef typename type_flag<T>::scalar type;
		typedef typename P::template rebind<type>::type retx_;
		const bool is_boolean = tl::is_same_type<bool, retx_>::value;
		typedef typename tl::meta_if<is_boolean, int64_t, retx_>::type retx;

		type_flag<T>::size_chk(v);

		arr_xt<retx> y;
		y.resize(type_flag<T>::size_ptr(v));
		size_t n=y.size();

		for (size_t i = 0; i < n; ++i)
		{
			y(i) = P::g(v(i));
		}
		ks.ci1.nsp[0].reset(y);
	}

	static int value(Executor& ks,Variant& v)
	{
		typedef typename flag_type<N>::type type;
		g(ks,variant_handler<type>::raw(v));
		return 1;
	}
};


template<typename P,unsigned N>
class pl1_dispatch<P,N,2> : public pl1_dispatch_base<P>
{
public:

	template<typename T>
	static void g(Executor& ks,const T& v)
	{
		typedef typename P::template rebind<T>::type retx;
		retx y=P::g(v);
		ks.ci1.nsp[0].reset(y);
	}

	static int value(Executor& ks,Variant& v)
	{
		typedef typename flag_type<N>::type type;
		g(ks,variant_handler<type>::g(v));
		return 1;
	}
};

template<typename P>
class pl1_call
{
public:
	template<unsigned N> class fk : public pl1_dispatch<P,N,flag_info1<P,N>::value>{};
	typedef int (*fn)(Executor&,Variant&);
	typedef lookup_table_4bit<fk,fn> lk;

	static EW_FORCEINLINE int g(Executor& ks)
	{
		return lk::test(ks.ci1.nsp[0].type())(ks,ks.ci1.nsp[0]);
	}

	static EW_FORCEINLINE int variant_call(Executor& ks, Variant& v)
	{
		return lk::test(v.type())(ks, v);
	}
};


template<typename P,unsigned N>
class flag_info2
{
public:

	static const unsigned N1=N>>4;
	static const unsigned N2=N&0xF;

	typedef typename flag_type<N1>::scalar scalar1;
	typedef typename flag_type<N1>::type type1;
	typedef typename flag_type<N2>::scalar scalar2;
	typedef typename flag_type<N2>::type type2;


	static const bool num_enable=P::template rebind<scalar1,scalar2>::value;
	static const bool arr_enable=P::template rebind<type1,type2>::value;

	static const int value=(P::dispatch_type==pl_base2::DISPATCH_NUM)?(num_enable?1:0):(arr_enable?2:0);

};

template<typename P>
class pl2_dispatch_base
{
public:
	static int value(Executor& ks,Variant&,Variant&)
	{
		return CallableMetatable::__metatable_call2(ks,P::info().op_name);
	}
};


template<typename P,unsigned N,unsigned X>
class pl2_dispatch : public pl2_dispatch_base<P>
{
public:


};

template<typename P,unsigned N>
class pl2_dispatch<P,N,1>: public pl2_dispatch_base<P>
{
public:

	static const unsigned N1=N>>4;
	static const unsigned N2=N&0xF;

	template<typename T1,typename T2>
	static inline typename tl::enable_if<type_flag<T1>::is_scr&&type_flag<T2>::is_scr>::type g(Executor& ks,const T1& v1,const T2& v2)
	{
		typedef typename P::template rebind<T1,T2>::type retx;
		retx y=P::g(v1,v2);
		(*--ks.ci1.nsp).reset(y);
	}

	template<typename T1,typename T2>
	static typename tl::enable_if<type_flag<T1>::is_arr&&type_flag<T2>::is_scr>::type g(Executor& ks,const T1& v1,const T2& v2)
	{
		typedef typename P::template rebind<typename type_flag<T1>::scalar,T2>::type retx_;
		const bool is_boolean = tl::is_same_type<bool, retx_>::value;
		typedef typename tl::meta_if<is_boolean, int64_t, retx_>::type retx;

		arr_xt<retx> y;
		y.resize(type_flag<T1>::size_ptr(v1));

		size_t n=y.size();
		for(size_t i=0;i<n;i++)
		{
			y(i)=P::g(v1(i),v2);
		}

		(*--ks.ci1.nsp).reset(y);
	}
	template<typename T1,typename T2>
	static typename tl::enable_if<type_flag<T1>::is_scr&&type_flag<T2>::is_arr>::type g(Executor& ks,const T1& v1,const T2& v2)
	{

		typedef typename P::template rebind<T1,typename type_flag<T2>::scalar>::type retx_;
		const bool is_boolean = tl::is_same_type<bool, retx_>::value;
		typedef typename tl::meta_if<is_boolean, int64_t, retx_>::type retx;
		arr_xt<retx> y;
		y.resize(type_flag<T2>::size_ptr(v2));

		size_t n=y.size();
		for(size_t i=0;i<n;i++)
		{
			y(i)=P::g(v1,v2(i));
		}

		(*--ks.ci1.nsp).reset(y);
	}

	template<typename T1,typename T2>
	static typename tl::enable_if<type_flag<T1>::is_arr&&type_flag<T2>::is_arr>::type g(Executor& ks,const T1& v1,const T2& v2)
	{
		typedef typename P::template rebind<typename type_flag<T1>::scalar,typename type_flag<T2>::scalar>::type retx_;
		const bool is_boolean = tl::is_same_type<bool, retx_>::value;
		typedef typename tl::meta_if<is_boolean, int64_t, retx_>::type retx;

		arr_xt<retx> y;
		y.resize(type_flag<T1>::size_ptr(v1));

		size_t n=y.size();
		if(n!=type_flag<T2>::size_ptr(v2).size())
		{
			Exception::XError(_hT("array_size_error"));
		}

		for(size_t i=0;i<n;i++)
		{
			y(i)=P::g(v1(i),v2(i));
		}

		(*--ks.ci1.nsp).reset(y);
	}


	static inline int value(Executor& ks,Variant& v1,Variant& v2)
	{
		typedef typename flag_type<N1>::type type1;
		typedef typename flag_type<N2>::type type2;

		g(ks,variant_handler<type1>::raw(v1),variant_handler<type2>::raw(v2));

		return 1;
	}
};



template<typename P,unsigned N>
class pl2_dispatch<P,N,2>: public pl2_dispatch_base<P>
{
public:

	template<typename T>
	static inline void s(Variant& r,const T& v)
	{
		r.reset(v);
	}

	template<typename T>
	static inline void s(Variant& r,const arr_xt<T>& v)
	{
		if(v.size()==1)
		{
			r.reset(v[0]);
		}
		else
		{
			r.reset(v);
		}
	}

	template<typename T1,typename T2>
	static inline void g(Executor& ks,const T1& v1,const T2& v2)
	{
		typedef typename P::template rebind<T1,T2>::type retx;
		retx y=P::g(v1,v2);
		s((*--ks.ci1.nsp),y);
	}

	static inline int value(Executor& ks,Variant& v1,Variant& v2)
	{
		static const unsigned N1=N>>4;
		static const unsigned N2=N&0xF;

		typedef typename flag_type<N1>::type type1;
		typedef typename flag_type<N2>::type type2;

		g(ks,variant_handler<type1>::raw(v1),variant_handler<type2>::raw(v2));

		return 1;
	}
};


template<typename P>
class pl2_call
{
public:

	template<unsigned N> class fk : public pl2_dispatch<P,N,flag_info2<P,N>::value>{};

	typedef int (*fn)(Executor&,Variant&,Variant&);
	typedef lookup_table_8bit<fk,fn> lk;

	static EW_FORCEINLINE int g(Executor& ks)
	{
		Variant& v1(ks.ci1.nsp[-1]);
		Variant& v2(ks.ci1.nsp[-0]);
		unsigned n=(v1.type()<<4)|v2.type();
		return lk::test(n)(ks,v1,v2);
	}

	static EW_FORCEINLINE int variant_call(Executor& ks, Variant& v1, Variant& v2)
	{
		unsigned n = (v1.type() << 4) | v2.type();
		return lk::test(n)(ks, v1, v2);
	}

	static EW_FORCEINLINE int k(Executor& ks)
	{
		typedef int64_t fast_type;
		static const unsigned fast_flag=(type_flag<fast_type>::value<<4)|type_flag<fast_type>::value;

		Variant& v1(ks.ci1.nsp[-1]);
		Variant& v2(ks.ci1.nsp[-0]);
		unsigned n=(v1.type()<<4)|v2.type();
		if(n==fast_flag)
		{
			(*--ks.ci1.nsp).reset(P::g(variant_handler<fast_type>::raw(v1),variant_handler<fast_type>::raw(v2)));
			return 1;
		}

		return lk::test(n)(ks,v1,v2);
	}
};


template<typename P>
void Executor::call1()
{
    int ret=pl1_call<P>::g(*this);
	_vm_check_call_ret(ret);
}

template<typename P>
void Executor::call2()
{
    int ret=pl2_call<P>::g(*this);
	if (ret == 1) return;
	_vm_check_call_ret(ret);
}


EW_LEAVE
#endif

