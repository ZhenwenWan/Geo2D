
#ifndef __H_EW_SCRIPTING_TYPEFLAG__
#define __H_EW_SCRIPTING_TYPEFLAG__

#include "ewa_base/basic/object.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE CG_Variable;

class DLLIMPEXP_EWA_BASE CallableData_helpdata;
class DLLIMPEXP_EWA_BASE CallableData;
class DLLIMPEXP_EWA_BASE CallableWrap;

template<typename T>
class DLLIMPEXP_EWA_BASE CallableWrapT;

class DLLIMPEXP_EWA_BASE CallableObject;
class DLLIMPEXP_EWA_BASE CallableMetatable;
class DLLIMPEXP_EWA_BASE CallableClass;
class DLLIMPEXP_EWA_BASE CallableFunction;
class DLLIMPEXP_EWA_BASE CallableClosure;
class DLLIMPEXP_EWA_BASE CallableCoroutine;
class DLLIMPEXP_EWA_BASE CallableCode;
class DLLIMPEXP_EWA_BASE CallableModule;

class DLLIMPEXP_EWA_BASE Variant;
class DLLIMPEXP_EWA_BASE Executor;
class DLLIMPEXP_EWA_BASE VariantTable;
class DLLIMPEXP_EWA_BASE VariantCloneState;


template<typename T> class hash_t<std::complex<T> > : public hash_pod<std::complex<T> >{};

template<typename X, typename Y> class hash_t<std::pair<X, Y> >
{
public:
	uint32_t operator()(const std::pair<X, Y> & o)
	{
		hash_t<X> h1; hash_t<Y> h2;
		return h1(o.first) - h2(o.second);
	}
};


typedef tl::mk_typelist<
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
>::type TL_VARIANT_TYPES;


template<typename T>
class flag_type_helper
{
public:
	typedef T type;
	typedef T scalar;

	static const bool is_arr = false;
	static const bool is_scr = true;
};

template<>
class flag_type_helper<tl::nulltype>
{
public:
	typedef CallableData* type;
	typedef CallableData* scalar;

	static const bool is_arr = false;
	static const bool is_scr = true;
};

template<typename T>
class flag_type_helper<arr_xt<T> >
{
public:
	typedef arr_xt<T> type;
	typedef T scalar;

	static const bool is_arr = true;
	static const bool is_scr = false;

	static inline arr_xt_dims size_ptr(const type& v){ return v.size_ptr(); }
	static inline void size_chk(const type& v){}
};


template<int N>
struct flag_type : public flag_type_helper<typename TL_VARIANT_TYPES::template at<N>::type>
{
	static const int value = N>=0 ? N : 15;
};

template<typename T>
struct type_flag : public flag_type<TL_VARIANT_TYPES::template id<T>::value>
{

};

template<typename T1, typename T2>
struct type_flag2
{
	static const int value = type_flag<T1>::value << 4 | type_flag<T2>::value;
};

template<int N1, int N2>
struct flag_type2
{
	typedef typename flag_type<N1>::type type1;
	typedef typename flag_type<N2>::type type2;
};



EW_LEAVE
#endif
