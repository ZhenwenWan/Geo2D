
#ifndef __H_EW_MATH_MATH_OP__
#define __H_EW_MATH_MATH_OP__

#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/math/math_def.h"


//extern "C" {
//	double boost_asinh(double);
//	double boost_acosh(double);
//	double boost_atanh(double);
//}

EW_ENTER

template<typename H, int N>
struct arr_promote_real_1t : public H {};

template<typename H>
struct arr_promote_real_1t<H, 1> : public H
{
	typedef typename H::type scalar;
	typedef arr_1t<typename H::type> promoted;
	typedef scalar type;
};

template<typename H>
struct arr_promote_real_1t<H, 2> : public H
{
	typedef typename H::type scalar;
	typedef arr_1t<typename H::type> promoted;
	typedef promoted type;
};

template<typename X, typename Y>
struct arr_promote_1t : public arr_promote_real_1t<vec_promote<X, Y>, vec_promote<X, Y>::value ? 1 : 0>{};

template<typename X, typename Y>
struct arr_promote_1t<arr_1t<X>, Y > : public arr_promote_real_1t<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};

template<typename X, typename Y>
struct arr_promote_1t<X, arr_1t<Y> > : public arr_promote_real_1t<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};

template<typename X, typename Y>
struct arr_promote_1t<arr_1t<X>, arr_1t<Y> > : public arr_promote_real_1t<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};



template<typename H, int N>
struct arr_promote_real_xt : public H {};

template<typename H>
struct arr_promote_real_xt<H, 1> : public H
{
	typedef typename H::type scalar;
	typedef arr_xt<typename H::type> promoted;
	typedef scalar type;
};

template<typename H>
struct arr_promote_real_xt<H, 2> : public H
{
	typedef typename H::type scalar;
	typedef arr_xt<typename H::type> promoted;
	typedef promoted type;
};

template<typename X, typename Y>
struct arr_promote_xt : public arr_promote_real_xt<vec_promote<X, Y>, vec_promote<X, Y>::value ? 1 : 0>{};

template<typename X, typename Y>
struct arr_promote_xt<arr_xt<X>, Y > : public arr_promote_real_xt<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};

template<typename X, typename Y>
struct arr_promote_xt<X, arr_xt<Y> > : public arr_promote_real_xt<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};

template<typename X, typename Y>
struct arr_promote_xt<arr_xt<X>, arr_xt<Y> > : public arr_promote_real_xt<vec_promote<X, Y>, vec_promote<X, Y>::value ? 2 : 0>{};


template<typename T>
class numeric_helper_t
{
public:

	static const T sm_value_0;
	static const T sm_value_1;

	static inline bool is_zero(const T& v)
	{
		return v == sm_value_0;
	}

	static inline bool is_zero(const T* p, size_t n)
	{
		if (!p)
		{
			return true;
		}
		for (size_t i = 0; i < n; i++)
		{
			if (p[i] != sm_value_0)
			{
				return false;
			}
		}
		return true;
	}


};

template<typename T>
const T numeric_helper_t<T>::sm_value_0(0);

template<typename T>
const T numeric_helper_t<T>::sm_value_1(1);

class DLLIMPEXP_EWA_BASE op_info
{
public:
	op_info(){}
	op_info(const String& s,int f=0);
	op_info(const String& s,const String& o,int f=0);

	String name;
	String op_name;
	int flag;
};

class DLLIMPEXP_EWA_BASE Variant;

struct DLLIMPEXP_EWA_BASE pl_base0
{
	enum
	{
		DISPATCH_NUM=1<<0,
		DISPATCH_ARR=1<<1,
	};

	static inline const std::complex<double>& k(const dcomplex& v){return v;}
	template<typename T> static inline T k(T v){return v;}

	static inline int metatable_call1_var(Variant&, Variant&){return 0;}
	static inline int metatable_call2_var(Variant&, Variant&, Variant&){return 0;}


	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_base1 : public pl_base0
{
	static const int dispatch_type=DISPATCH_NUM;
};

struct DLLIMPEXP_EWA_BASE pl_base2 : public pl_base0
{
	static const int dispatch_type=DISPATCH_NUM;
};

typedef tl::mk_typelist<int32_t,int64_t>::type TL_I;
typedef tl::mk_typelist<int32_t,int64_t,float,double>::type TL_1;
typedef tl::mk_typelist<int32_t,int64_t,float,double,tiny_cpx<float>,tiny_cpx<double> >::type TL_2;
typedef tl::mk_typelist<int32_t,int64_t,float,double,float,double>::type TL_S;

template<typename L,typename X,typename S>
struct swtich_tl_with_value : public tl::enable_tl<L,X,typename S::template at<L::template id<X>::value>::type>
{
	static const bool value=L::template id<X>::value>=0;
};

template<typename L,typename X,typename Y=X>
struct enable_tl_with_value : public tl::enable_tl<L,X,Y>
{
	static const bool value=L::template id<X>::value>=0;
};

template<typename V,typename Y>
struct enable_if_with_value : public tl::enable_if_c<V,Y>
{
	static const bool value=V::value;
};

// predicates
template<typename T>
struct DLLIMPEXP_EWA_BASE pl_base1_predT : public enable_tl_with_value < TL_2, T, bool > {};

// for general numeric function (1-airy)
template<typename T>
struct DLLIMPEXP_EWA_BASE pl_base1_numT : public enable_tl_with_value<TL_1,T,double>
{
	typedef T value_type;
	typedef double return_type;

	static double k(value_type v){return double(v);}

	static double sqrt(value_type x){return ::sqrt(k(x));}
	static double log(value_type x){return ::log(k(x));}
	static double log10(value_type x){return ::log10(k(x));}

	static double logdb(value_type x)
	{
		double v = k(x);
		if (v <= 1.0e-100)
		{
			return -1000.0;
		}
		else
		{
			return 10.0*::log10(v);
		}
	}

	static double exp(value_type x){return ::exp(k(x));}

	static double deg2rad(value_type x){ return x*M_DEG2RAD; }
	static double rad2deg(value_type x){ return x*M_RAD2DEG; }

	static double ceil(value_type x){return ::ceil(k(x)); }
	static double floor(value_type x){return ::floor(k(x)); }
	static double round(value_type x){return ::round(k(x)); }
	static double trunc(value_type x){return ::trunc(k(x)); }
	static double fix(value_type x){ return (x >= 0) ? ::floor(x) : (::ceil(x)); }
};

template<typename T>
struct DLLIMPEXP_EWA_BASE pl_base1_numT<tiny_cpx<T> > : public enable_tl_with_value<TL_1,T,dcomplex>
{
	typedef tiny_cpx<T> value_type;
	typedef dcomplex return_type;

	static const std::complex<double>& k(const dcomplex& v){return v;}
	static std::complex<double> k(const fcomplex& v){return std::complex<double>(v.real(),v.imag());}

	static return_type sqrt(value_type x){return std::sqrt(k(x));}
	static return_type log(value_type x){return std::log(k(x));}
	static return_type log10(value_type x){return std::log10(k(x));}
	static return_type exp(value_type x){return std::exp(k(x));}

	static return_type deg2rad(value_type x){ return x*M_DEG2RAD; }
	static return_type rad2deg(value_type x){ return x*M_RAD2DEG; }

	static return_type logdb(value_type x)
	{
		if (x.imag() != 0.0)
		{
			Exception::XError("calling of logdb witch complex");
		}
		return pl_base1_numT<T>::logdb(x.real());
	}

	// ceil(a+bi)=ceil(a)+ceil(b)i, floor(a+bi)=floor(a)+floor(b)i
	static return_type ceil(value_type x){ return return_type(::ceil(pl_base1_numT<T>::k(x.real())), ::ceil(pl_base1_numT<T>::k(x.imag()))); }
	static return_type floor(value_type x){ return return_type(::floor(pl_base1_numT<T>::k(x.real())), ::floor(pl_base1_numT<T>::k(x.imag()))); }
	static return_type round(value_type x){ return return_type(::round(pl_base1_numT<T>::k(x.real())), ::round(pl_base1_numT<T>::k(x.imag()))); }
	static return_type trunc(value_type x){ return return_type(::trunc(pl_base1_numT<T>::k(x.real())), ::trunc(pl_base1_numT<T>::k(x.imag()))); }
	static return_type fix(value_type x){ return return_type(pl_base1_numT<T>::fix(x.real()), pl_base1_numT<T>::fix(x.imag())); }
};

struct DLLIMPEXP_EWA_BASE pl_base1_pred : public pl_base1
{
	static const int dispatch_type = DISPATCH_NUM;

	template<typename T>
	struct rebind : public pl_base1_predT <T>{};
};

struct DLLIMPEXP_EWA_BASE pl_isinf : public pl_base1_pred
{
	static bool g(bool) { return false; }
	static bool g(int32_t) { return false; }
	static bool g(int64_t) { return false; }
	static bool g(float x) { return std::isinf(x); }
	static bool g(double x) { return std::isinf(x); }
	static bool g(const fcomplex& x) { return std::isinf(x.real()) || std::isinf(x.imag()); }
	static bool g(const dcomplex& x) { return std::isinf(x.real()) || std::isinf(x.imag()); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_isfinite : public pl_base1_pred
{
	static bool g(bool) { return true; }
	static bool g(int32_t) { return true; }
	static bool g(int64_t) { return true; }
	static bool g(float x) { return std::isfinite(x); }
	static bool g(double x) { return std::isfinite(x); }
	static bool g(const fcomplex& x) { return std::isfinite(x.real()) || std::isfinite(x.imag()); }
	static bool g(const dcomplex& x) { return std::isfinite(x.real()) || std::isfinite(x.imag()); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_islogical : public pl_base1_pred
{
	static bool g(bool) { return true; }
	static bool g(int32_t x) { return x == 0 || x == 1; }
	static bool g(int64_t x) { return x == 0 || x == 1; }
	static bool g(float x) { return x == 0 || x == 1; }
	static bool g(double x) { return x == 0 || x == 1; }
	static bool g(const fcomplex& x) { return x.imag() == 0 && g(x.real()); }
	static bool g(const dcomplex& x) { return x.imag() == 0 && g(x.real()); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_isnan : public pl_base1_pred
{
	static bool g(bool) { return false; }
	static bool g(int32_t) { return false; }
	static bool g(int64_t) { return false; }
	static bool g(float x) { return std::isnan(x); }
	static bool g(double x) { return std::isnan(x); }
	static bool g(const fcomplex& x) { return std::isnan(x.real()) || std::isnan(x.imag()); }
	static bool g(const dcomplex& x) { return std::isnan(x.real()) || std::isnan(x.imag()); }
	
	static const op_info& info();
};

// used in 'math.find' function, not exported
struct DLLIMPEXP_EWA_BASE pl_iszero : public pl_base1_pred
{
	static bool g(bool x) { return !x; }
	static bool g(int32_t x) { return x == 0; }
	static bool g(int64_t x) { return x == 0; }
	static bool g(float x) { return x == 0.0f; }
	static bool g(double x) { return x == 0.0; }

	static bool g(const fcomplex& x) { return x.real() == 0 && x.imag() == 0; }
	static bool g(const dcomplex& x) { return x.real() == 0 && x.imag() == 0; }
};

struct DLLIMPEXP_EWA_BASE pl_isreal : public pl_base1_pred
{
	template<typename T>
	static bool g(const T& v) { return true; }
	
	template<typename T>
	static bool g(const tiny_cpx<T>& v) { return v.imag() == 0; }
	
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_iscomplex : public pl_base1_pred
{
	template<typename T>
	static bool g(const T& v) { return true; }

	template<typename T>
	static bool g(const tiny_cpx<T>& v) { return v.imag() != 0; }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_base1_num : public pl_base1
{
	template<typename T>
	struct rebind : public pl_base1_numT<T>{};
};


template<typename T>
struct dbl_promote
{
	typedef T type;
};

template<>
struct dbl_promote<int32_t>
{
	typedef double type;
};

template<>
struct dbl_promote<int64_t>
{
	typedef double type;
};

template<>
struct dbl_promote<arr_xt<int64_t> >
{
	typedef arr_xt<double> type;
};

template<>
struct dbl_promote<arr_xt<int32_t> >
{
	typedef arr_xt<double> type;
};



struct DLLIMPEXP_EWA_BASE pl_base2_num : public pl_base2
{
	template<typename T1, typename T2>
	struct rebind : public cpx_promote<T1, T2>{};

	template<typename T1, typename T2>
	struct rebind_double : public cpx_promote<typename dbl_promote<T1>::type, typename dbl_promote<T2>::type>{};
};




struct DLLIMPEXP_EWA_BASE pl_sqrt : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_numT<T>::sqrt(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_exp : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_numT<T>::exp(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_deg2rad : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_numT<T>::deg2rad(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_rad2deg : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_numT<T>::rad2deg(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_log : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_numT<T>::log(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_log10 : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_numT<T>::log10(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_logdb : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_numT<T>::logdb(v);}

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_ceil : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_numT<T>::ceil(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_floor : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_numT<T>::floor(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_round : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v) { return pl_base1_numT<T>::round(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_trunc : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v) { return pl_base1_numT<T>::trunc(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_fix : public pl_base1_num
{
	template<typename T>
	static typename rebind<T>::type g(const T& v) { return pl_base1_numT<T>::fix(v); }

	static const op_info& info();
};

#if defined(EW_MSVC) && EW_MSVC>1600
#define EW_HAS_TRI_H 1
#else
#define EW_HAS_TRI_H 0
#endif

template<typename T>
struct DLLIMPEXP_EWA_BASE pl_base1_triT : public enable_tl_with_value<TL_1,T,double>
{
	typedef T value_type;
	typedef double return_type;

	static double k(value_type v){return double(v);}

	static return_type sin(value_type x){return ::sin(k(x));}
	static return_type cos(value_type x){return ::cos(k(x));}
	static return_type tan(value_type x){return ::tan(k(x));}

	static return_type asin(value_type x){return ::asin(k(x));}
	static return_type acos(value_type x){return ::acos(k(x));}
	static return_type atan(value_type x){return ::atan(k(x));}

	static return_type sindeg(value_type x){return ew::sindeg(k(x));}
	static return_type cosdeg(value_type x){return ew::cosdeg(k(x));}
	static return_type tandeg(value_type x){return ew::tandeg(k(x));}

	static return_type sinc(value_type x) { return (x == 0) ? 1 : sin(x) / x; }

	static return_type sinh(value_type x){ return std::sinh(x); }
	static return_type cosh(value_type x){ return std::cosh(x); }
	static return_type tanh(value_type x){ return std::tanh(x); }

#if EW_HAS_TRI_H

	// C++ 11
	static return_type asinh(value_type x){ return std::asinh(x); }
	static return_type acosh(value_type x){ return std::acosh(x); }
	static return_type atanh(value_type x){ return std::atanh(x); }
#else
	// use boost version
	static return_type asinh(value_type x){ return boost_asinh(x); }
	static return_type acosh(value_type x){ return boost_acosh(x); }
	static return_type atanh(value_type x){ return boost_atanh(x); }
#endif

};

template<typename T>
struct DLLIMPEXP_EWA_BASE pl_base1_triT<tiny_cpx<T> > : public enable_tl_with_value<TL_1,T,dcomplex>
{
	typedef const tiny_cpx<T>& value_type;
	typedef dcomplex return_type;

	static const std::complex<double>& k(const dcomplex& v){return v;}
	static std::complex<double> k(const fcomplex& v){return std::complex<double>(v.real(),v.imag());}

	static const std::complex<double> g(const dcomplex& v){return v*M_DEG2RAD;}
	static std::complex<double> g(const fcomplex& v){return std::complex<double>(v.real()*M_DEG2RAD,v.imag()*M_DEG2RAD);}

	static return_type sin(value_type x){return std::sin(k(x));}
	static return_type cos(value_type x){return std::cos(k(x));}
	static return_type tan(value_type x){return std::tan(k(x));}

	static return_type sindeg(value_type x){ return std::sin(g(x)); }
	static return_type cosdeg(value_type x){ return std::cos(g(x)); }
	static return_type tandeg(value_type x){ return std::tan(g(x)); }

	static return_type asin(value_type x);
	static return_type acos(value_type x);
	static return_type atan(value_type x);


	static return_type sinc(value_type x) { return (x == 0) ? 1 : sin(x) / x; }

#if EW_HAS_TRI_H
	static return_type sinh(value_type x){return std::sinh(k(x));}
	static return_type cosh(value_type x){return std::cosh(k(x));}
	static return_type tanh(value_type x){return std::tanh(k(x));}

	static return_type asinh(value_type x){return std::asinh(k(x));}
	static return_type acosh(value_type x){return std::acosh(k(x));}
	static return_type atanh(value_type x){return std::atanh(k(x));}
#endif

};



struct DLLIMPEXP_EWA_BASE pl_base1_tri : public pl_base1
{
	template<typename T>
	struct rebind : public pl_base1_triT<T>{};
};

struct DLLIMPEXP_EWA_BASE pl_sin : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::sin(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_cos : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::cos(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_tan : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::tan(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_cot : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::cot(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_sindeg : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::sindeg(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_cosdeg : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::cosdeg(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_tandeg : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::tandeg(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_asin : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::asin(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_acos : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::acos(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_atan : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::atan(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_atan2 : public pl_base2_num
{
	static double g(int64_t v1, int64_t v2) { return ::atan2(v1, v2); }
	static double g(int64_t v1, double v2) { return ::atan2(v1, v2); }
	static double g(double v1, int64_t v2) { return ::atan2(v1, v2); }
	static double g(double v1, double v2) { return ::atan2(v1, v2); }
	
	template<typename T1, typename T2>
	static double g(const T1&, const T2&) { Exception::XInvalidArgument(); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_minmax : public pl_base2_num
{
	template<typename T1, typename T2>
	struct rebind : public scr_promote < T1, T2 > {};

	template<typename T1, typename T2>
	static typename rebind<T1, T2>::type max(const T1& v1, const T2& v2) { return std::max(v1, v2); }
	template<typename T1, typename T2>
	static typename rebind<T1, T2>::type min(const T1& v1, const T2& v2) { return std::min(v1, v2); }
};

struct DLLIMPEXP_EWA_BASE pl_max : public pl_minmax
{
	template<typename T1, typename T2>
	static typename rebind<T1, T2>::type g(const T1& v1, const T2& v2) { return pl_minmax::max(v1, v2); }
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_min : public pl_minmax
{
	template<typename T1, typename T2>
	static typename rebind<T1, T2>::type g(const T1& v1, const T2& v2) { return pl_minmax::min(v1, v2); }
	static const op_info& info();
};

#if EW_HAS_TRI_H
struct DLLIMPEXP_EWA_BASE pl_sinc : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v) { return pl_base1_triT<T>::sinc(v); }

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_sinh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::sinh(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_cosh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::cosh(v); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_tanh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){ return pl_base1_triT<T>::tanh(v); }

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_asinh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::asinh(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_acosh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::acosh(v);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_atanh : public pl_base1_tri
{
	template<typename T>
	static typename rebind<T>::type g(const T& v){return pl_base1_triT<T>::atanh(v);}

	static const op_info& info();
};
#endif


struct DLLIMPEXP_EWA_BASE pl_neg : public pl_base1
{

	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2,T>{};

	static int64_t g(int64_t a){return -a;}
	static double g(double a){return -a;}
	static dcomplex g(const dcomplex& a){return -a;}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_abs : public pl_base1
{
	template<typename T>
	struct rebind : public swtich_tl_with_value<TL_2,T,TL_S>{};

	static int64_t g(int64_t a){return a>0?a:-a;}
	static double g(double a){return a>0?a:-a;}
	static double g(const dcomplex& a){return a.length();}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_abs2 : public pl_base1
{
	template<typename T>
	struct rebind : public swtich_tl_with_value<TL_2,T,TL_S>{};

	static int64_t g(int64_t a){ return a*a; }
	static double g(double a){ return a*a; }
	static double g(const dcomplex& a) { return a.real()*a.real() + a.imag()*a.imag(); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_sign : public pl_base1
{
	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2, T>{};

	static int64_t g(int64_t a) { return a > 0 ? +1 : (a < 0 ? -1 : 0); }
	static double g(double a) { return a > 0 ? +1 : (a < 0 ? -1 : 0); }
	static dcomplex g(const dcomplex& a){ return a / a.length(); }

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_arg : public pl_base1
{

	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2,T,double>{};

	static double g(int64_t x){ return (x >= 0) ? 0 : M_PI; }
	static double g(double x){ return (x >= 0) ? 0 : M_PI; }
	static double g(const dcomplex& a){ return ::atan2(a.imag(),a.real());}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_bw_not : public pl_base1
{
	template<typename T> struct rebind : public enable_tl_with_value<TL_I,T>{};

	template<typename T>
	static typename rebind<T>::type g(T v1){return ~v1;}

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_conj : public pl_base1
{
	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2,T>{};

	static int64_t g(int64_t a){return a;}
	static double g(double a){return a;}
	static dcomplex g(const dcomplex& a){return dcomplex(a.real(),-a.imag());}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_real : public pl_base1
{

	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2,T,double>{};

	static double g(int64_t a){return a;}
	static double g(double a){return a;}
	static double g(const dcomplex& a){return a.real();}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_imag : public pl_base1
{

	template<typename T>
	struct rebind : public enable_tl_with_value<TL_2,T,double>{};

	static double g(int64_t){return 0.0;}
	static double g(double){return 0.0;}
	static double g(const dcomplex& a){return a.imag();}

	static const op_info& info();
};



struct DLLIMPEXP_EWA_BASE pl_cpx : public pl_base2
{
	template<typename T1,typename T2>
	struct rebind : public enable_if_with_value<cpx_promote<T1,T2>,dcomplex>{};

	static dcomplex g(double v1,double v2){return dcomplex(v1,v2);}
	static dcomplex g(const dcomplex& v1,double v2){return dcomplex(v1.real(),v1.imag()+v2);}
	static dcomplex g(double v1,const dcomplex& v2){return dcomplex(v1-v2.imag(),v2.real());}
	static dcomplex g(const dcomplex& v1,const dcomplex& v2){return dcomplex(v1.real()-v2.imag(),v2.real()-v1.imag());}

	static const op_info& info();
};



struct DLLIMPEXP_EWA_BASE pl_base2_mat : public pl_base2
{
	static const int dispatch_type=DISPATCH_ARR;

	template<typename T1,typename T2>
	struct rebind : public arr_promote_xt<T1,T2>{};

	template<typename T1,typename T2>
	struct rebind_double : public arr_promote_xt<typename dbl_promote<T1>::type,typename dbl_promote<T2>::type>{};

};

struct DLLIMPEXP_EWA_BASE pl_add : public pl_base2_num
{
	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1+v2;}

	static inline uint64_t g(uint64_t v1, uint64_t v2) { return v1 + v2; }

	template<typename T, int N>
	static inline tiny_vec<T, N> g(const tiny_vec<T, N>& v1, const tiny_vec<T, N>& v2)
	{
		return v1 + v2;
	}

	template<typename T1>
	static inline T1 g1(const T1 v)
	{
		return v;
	}

	template<typename T2>
	static inline T2 g2(const T2 v)
	{
		return v;
	}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_sub : public pl_base2_num
{
	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1-v2;}

	template<typename T, int N>
	static inline tiny_vec<T, N> g(const tiny_vec<T, N>& v1, const tiny_vec<T, N>& v2)
	{
		return v1 - v2;
	}

	template<typename T1>
	static inline T1 g1(const T1 v)
	{
		return v;
	}

	template<typename T2>
	static inline T2 g2(const T2 v)
	{
		return -v;
	}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_mul : public pl_base2_num
{
	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1*v2;}


	template<typename T1>
	static inline T1 g1(const T1 v)
	{
		return 0.0;
	}

	template<typename T2>
	static inline T2 g2(const T2 v)
	{
		return 0.0;
	}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_arr_mul : public pl_mul
{
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_mat_mul : public pl_base2_mat
{

	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1*v2;}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,T2>::type g(const arr_xt<T1>& v1,const T2& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		size_t sz=v1.size();
		result.resize(v1.size_ptr(),sz);
		for(size_t i=0;i<sz;i++) result[i]=v1[i]*v2;
		return EW_MOVE(result);
	}

	template<typename T1,typename T2>
	static typename rebind<T1,arr_xt<T2> >::type g(const T1& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		size_t sz=v2.size();
		result.resize(v2.size_ptr(),sz);
		for(size_t i=0;i<sz;i++) result[i]=v1*v2[i];
		return EW_MOVE(result);
	}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,arr_xt<T2> >::type g(const arr_xt<T1>& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;

		const arr_xt_dims& s1=v1.size_ptr();
		const arr_xt_dims& s2=v2.size_ptr();

		size_t k3=s1[2]*s1[3]*s1[4]*s1[5];

		if(s1[1]!=s2[0]||k3!=s2[2]*s2[3]*s2[4]*s2[5])
		{
			Exception::XMatrixSizeNotMatch();
		}

		result.resize(s1[0],s2[1],s1[2],s1[3],s1[4],s1[5]);

		if (k3 == 1)
		{
			/* 备注：两个1000 x 1000 矩阵, 下列代码大约1s，Octave (Open BLAS) 大概0.08s */
			// 常规矩阵乘法
			// 缓存V1的行向量
			arr_1t<T1> _a;
			_a.resize(s1[1]);
			T1* a = &_a[0];

			for (size_t i = 0; i < s1[0]; i++)
			{
				for (size_t n = 0; n < s1[1]; ++n)
				{
					a[n] = v1(i, n);
				}

				for (size_t j = 0; j < s2[1]; j++)
				{
					type tmp(0);
					const T2* b = &v2(0, j);
					size_t m = size_t(s1[1] / 8) * 8;

					for (size_t n = 0; n < m; n += 8)
					{
						tmp += a[n] * b[n]
							+  a[n + 1] * b[n + 1]
							+  a[n + 2] * b[n + 2]
							+  a[n + 3] * b[n + 3]
							+  a[n + 4] * b[n + 4]
							+  a[n + 5] * b[n + 5]
							+  a[n + 6] * b[n + 6]
							+  a[n + 7] * b[n + 7];
					}

					for (size_t n = m; n < s1[1]; ++n)
					{
						tmp += a[n] * b[n];
					}
					
					result(i, j) = tmp;
				}
			}
		}
		else
		{
			// 层叠矩阵乘法？
			for (size_t k = 0; k < k3; k++) for (size_t i = 0; i < s1[0]; i++) for (size_t j = 0; j < s2[1]; j++)
			{
				for (size_t n = 0; n < s1[1]; n++)
				{
					result(i, j, k) += v1(i, n, k)*v2(n, j, k);
				}
			}
		}

		return EW_MOVE(result);
	}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_div : public pl_base2_num
{
	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1/v2;}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_mat_div : public pl_base2_mat
{
	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return v1/v2;}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,T2>::type g(const arr_xt<T1>& v1,const T2& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		size_t sz=v1.size();
		result.resize(v1.size_ptr(),sz);
		for(size_t i=0;i<sz;i++) result[i]=v1[i]/v2; // do not use g<T1,T2>
		return EW_MOVE(result);

	}

	template<typename T1,typename T2>
	static typename rebind<T1,arr_xt<T2> >::type g(const T1& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		Exception::XError(_hT("matrix_div_not_implement"));
		return EW_MOVE(result);
	}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,arr_xt<T2> >::type g(const arr_xt<T1>& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		Exception::XError(_hT("matrix_div_not_implement"));
		return EW_MOVE(result);
	}

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_arr_div : public pl_div
{
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_pow : public pl_base2_num
{

	template<typename T1,typename T2>
	struct rebind : public pl_base2_num::rebind_double<T1,T2>
	{
		
	};

	static double k(int64_t v){return double(v);}
	static double k(double v){return double(v);}
	static const std::complex<double>& k(const dcomplex& v){return (const std::complex<double>&)(v);}

	template<typename T1>
	static typename rebind<T1,int64_t>::type g(const T1& v1,int64_t v2)
	{
		return std::pow(k(v1),int32_t(v2));
	}

	template<typename T1>
	static typename rebind<T1,int64_t>::type g(const T1& v1,int32_t v2)
	{
		return std::pow(k(v1),int32_t(v2));
	}

	template<typename T1>
	static typename rebind<T1,double>::type g(const T1& v1,double v2)
	{
		return std::pow(k(v1),v2);
	}

	template<typename T1>
	static typename rebind<T1,dcomplex>::type g(const T1& v1,const dcomplex& v2)
	{
		return std::pow(k(v1),k(v2));
	}


	static const op_info& info();
};



struct DLLIMPEXP_EWA_BASE pl_arr_pow : public pl_pow
{
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_mat_pow : public pl_base2_mat
{

	template<typename T1, typename T2>
	struct rebind : public pl_base2_mat::rebind_double<T1,T2>
	{
		
	};


	template<typename T1,typename T2>
	static typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_pow::g(v1,v2);}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,T2>::type g(const arr_xt<T1>& v1,const T2& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		arr_xt<type> result;
		Exception::XError(_hT("matrix_pow_not_implement"));
		return EW_MOVE(result);

	}
	template<typename T1,typename T2>
	static typename rebind<T1,arr_xt<T2> >::type g(const T1& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		size_t n = v2.size();
		arr_xt<type> result(v2.size_ptr());
		for (size_t i = 0; i < n; i++)
		{
			result[i] = pl_pow::g(v1, v2[i]);
		}
		return EW_MOVE(result);
	}

	template<typename T1,typename T2>
	static typename rebind<arr_xt<T1>,arr_xt<T2> >::type g(const arr_xt<T1>& v1,const arr_xt<T2>& v2)
	{
		typedef typename rebind<T1,T2>::type type;
		size_t n = v1.size();
		if (n != v2.size())
		{
			Exception::XArraySizeNotMatch();
		}
		arr_xt<type> result(v1.size_ptr());
		for (size_t i = 0; i < n; i++)
		{
			result[i] = pl_pow::g(v1[i], v2[i]);
		}
		return EW_MOVE(result);
	}

	static const op_info& info();
};


struct DLLIMPEXP_EWA_BASE pl_rem : public pl_base2_num
{
	template<typename T1, typename T2> struct rebind : public scr_promote<T1, T2>{};
	
	// static int64_t g(int64_t v1, int64_t v2) { return (v1>=0)?(v1%abs(v2)):(-(-v1)%abs(v2)); }
	static int64_t g(int64_t v1, int64_t v2) { return v1 % v2; }
	static double g(double v1, int64_t v2) { return v1 - v2 * pl_fix::g(v1 / v2); }
	static double g(int64_t v1, double v2) { return v1 - v2 * pl_fix::g(v1 / v2); }
	static double g(double v1, double v2) { return v1 - v2 * pl_fix::g(v1 / v2); }


	static const op_info& info();
};



struct DLLIMPEXP_EWA_BASE pl_mod : public pl_base2_num
{
	template<typename T1,typename T2> struct rebind : public scr_promote<T1,T2>{};

	
	static double g(double v1,double v2){ return v1-::floor(v1/v2)*v2;}
	static double g(int64_t v1,double v2){ return v1-::floor(v1/v2)*v2;}
	static double g(double v1,int64_t v2){ return v1-::floor(v1/v2)*v2;}
	static int64_t g(int64_t v1, int64_t v2)
	{
		return ((v1>=0) ^ (v2>=0))?(v1%v2+v2):(v1%v2);
	}
	



	static const op_info& info();
};


template<typename T1,typename T2>
struct DLLIMPEXP_EWA_BASE pl_base2_relT
{
	typedef typename scr_promote<T1,T2>::type scalar;
	typedef bool return_type;

	static inline return_type lt(const T1& v1,const T2& v2){return scalar(v1)< scalar(v2);}
	static inline return_type le(const T1& v1,const T2& v2){return scalar(v1)<=scalar(v2);}
	static inline return_type gt(const T1& v1,const T2& v2){return scalar(v1)> scalar(v2);}
	static inline return_type ge(const T1& v1,const T2& v2){return scalar(v1)>=scalar(v2);}
	static inline return_type eq(const T1& v1,const T2& v2){return scalar(v1)==scalar(v2);}
	static inline return_type ne(const T1& v1,const T2& v2){return scalar(v1)!=scalar(v2);}
};

template<typename T1, typename T2>
struct DLLIMPEXP_EWA_BASE pl_base2_relT < tiny_cpx<T1>, T2 >
{
	typedef tiny_cpx<T1> type1;
	typedef T2 type2;
	typedef typename scr_promote<T1,T2>::type scalar;
	typedef bool return_type;

	static inline return_type lt(const type1& v1,const type2& v2){return scalar(v1.real())< scalar(v2);}
	static inline return_type le(const type1& v1,const type2& v2){return scalar(v1.real())<=scalar(v2);}
	static inline return_type gt(const type1& v1,const type2& v2){return scalar(v1.real())> scalar(v2);}
	static inline return_type ge(const type1& v1,const type2& v2){return scalar(v1.real())>=scalar(v2);}
	static inline return_type eq(const type1& v1,const type2& v2){return v1.imag()==0 && scalar(v1.real())==scalar(v2);}
	static inline return_type ne(const type1& v1,const type2& v2){return v1.imag()!=0 || scalar(v1.real())!=scalar(v2);}
};

template<typename T1, typename T2>
struct DLLIMPEXP_EWA_BASE pl_base2_relT < T1, tiny_cpx<T2> >
{
	typedef T1 type1;
	typedef tiny_cpx<T2> type2;
	typedef typename scr_promote<T1,T2>::type scalar;
	typedef bool return_type;

	static inline return_type lt(const type1& v1,const type2& v2){return scalar(v1)< scalar(v2.real());}
	static inline return_type le(const type1& v1,const type2& v2){return scalar(v1)<=scalar(v2.real());}
	static inline return_type gt(const type1& v1,const type2& v2){return scalar(v1)> scalar(v2.real());}
	static inline return_type ge(const type1& v1,const type2& v2){return scalar(v1)>=scalar(v2.real());}
	static inline return_type eq(const type1& v1,const type2& v2){return v2.imag()==0 && scalar(v2.real())==scalar(v1);}
	static inline return_type ne(const type1& v1,const type2& v2){return v2.imag()!=0 || scalar(v2.real())!=scalar(v1);}
};

template<typename T1>
struct DLLIMPEXP_EWA_BASE pl_base2_relT<T1,T1>
{
	typedef T1 scalar;
	typedef scalar T2;
	typedef bool return_type;

	static inline return_type lt(const T1& v1,const T2& v2){return (v1)< (v2);}
	static inline return_type le(const T1& v1,const T2& v2){return (v1)<=(v2);}
	static inline return_type gt(const T1& v1,const T2& v2){return (v1)> (v2);}
	static inline return_type ge(const T1& v1,const T2& v2){return (v1)>=(v2);}
	static inline return_type eq(const T1& v1,const T2& v2){return (v1)==(v2);}
	static inline return_type ne(const T1& v1,const T2& v2){return (v1)!=(v2);}
};

template<typename T1>
struct DLLIMPEXP_EWA_BASE pl_base2_relT<tiny_cpx<T1>,tiny_cpx<T1> >
{
	typedef tiny_cpx<T1> type1;
	typedef tiny_cpx<T1> type2;
	typedef T1 scalar;
	typedef bool return_type;

	static inline return_type lt(const type1& v1,const type2& v2){return v1.real()< v2.real();}
	static inline return_type le(const type1& v1,const type2& v2){return v1.real()<=v2.real();}
	static inline return_type gt(const type1& v1,const type2& v2){return v1.real()> v2.real();}
	static inline return_type ge(const type1& v1,const type2& v2){return v1.real()>=v2.real();}
	static inline return_type eq(const type1& v1,const type2& v2){return (v1)==(v2);}
	static inline return_type ne(const type1& v1,const type2& v2){return (v1)!=(v2);}
};

struct DLLIMPEXP_EWA_BASE pl_base2_rel : public pl_base2
{
	//static const int dispatch_type=DISPATCH_ARR; // 不把数组展开分发
	static const int dispatch_type = DISPATCH_NUM;  // 相当于dot_xxx

	template<typename T1,typename T2>
	struct rebind : public enable_if_with_value<scr_promote<T1,T2>,bool>{};
	template<typename T1, typename T2>
	struct rebind<tiny_cpx<T1>, T2> : public enable_if_with_value < cpx_promote<T1, T2>, bool > {};
	template<typename T1, typename T2>
	struct rebind<T1, tiny_cpx<T2> > : public enable_if_with_value < cpx_promote<T1, T2>, bool > {};
	template<typename T1, typename T2>
	struct rebind<tiny_cpx<T1>, tiny_cpx<T2> > : public enable_if_with_value < cpx_promote<T1, T2>, bool > {};

	template<typename T1>
	struct rebind1
	{
		typedef bool type;
		static const bool value = true;
	};
	template<typename T1,typename T2>
	struct rebind2 : public tl::select_base < tl::is_same_type<T1, T2>::value, rebind1<T1>, rebind<T1, T2> >{};

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_eq : public pl_base2_rel
{
	static const int dispatch_type=DISPATCH_ARR;

	template<typename T1, typename T2>
	struct rebind : public rebind2<T1, T2>{};


	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::eq(v1,v2);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_ne : public pl_base2_rel
{
	static const int dispatch_type = DISPATCH_ARR;

	template<typename T1, typename T2>
	struct rebind : public rebind2<T1, T2>{};

	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::ne(v1,v2);}

	static const op_info& info();
};



struct DLLIMPEXP_EWA_BASE pl_arr_eq : public pl_eq
{
	static const int dispatch_type = DISPATCH_NUM;  // 相当于dot_xxx
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_arr_ne : public pl_ne
{
	static const int dispatch_type = DISPATCH_NUM;  // 相当于dot_xxx
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_gt : public pl_base2_rel
{
	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::gt(v1,v2);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_ge : public pl_base2_rel
{
	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::ge(v1,v2);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_lt : public pl_base2_rel
{
	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::lt(v1,v2);}

	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_le : public pl_base2_rel
{
	template<typename T1,typename T2>
	static inline typename rebind<T1,T2>::type g(const T1& v1,const T2& v2){return pl_base2_relT<T1,T2>::le(v1,v2);}

	static const op_info& info();
};

template<typename T1,typename T2> struct pl2_bw_rebind : public tl::value_type<false>{};
template<> struct pl2_bw_rebind<int64_t,int64_t> : public tl::value_type<true>{typedef int64_t type;};

struct DLLIMPEXP_EWA_BASE pl_base2_bw : public pl_base2
{
	template<typename T1,typename T2>
	struct rebind : public pl2_bw_rebind<T1,T2>{};
};

struct DLLIMPEXP_EWA_BASE pl_bw_and : public pl_base2_bw
{
	static int64_t g(int64_t x,int64_t y){return x&y;}
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_bw_or : public pl_base2_bw
{
	static int64_t g(int64_t x,int64_t y){return x|y;}
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_bw_xor : public pl_base2_bw
{
	static int64_t g(int64_t x,int64_t y){return x^y;}
	static const op_info& info();
};

struct DLLIMPEXP_EWA_BASE pl_bw_shl : public pl_base2_bw
{
	static int64_t g(int64_t x,int64_t y){return x<<y;}
	static const op_info& info();

};

struct DLLIMPEXP_EWA_BASE pl_bw_shr : public pl_base2_bw
{
	static int64_t g(int64_t x,int64_t y){return x>>y;}
	static const op_info& info();

};


EW_LEAVE

#endif
