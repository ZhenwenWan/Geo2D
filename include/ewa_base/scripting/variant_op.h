
#ifndef __H_EW_SCRIPTING_VARIANT_OP__
#define __H_EW_SCRIPTING_VARIANT_OP__


#include "ewa_base/basic/exception.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/math/math_def.h"
#include "ewa_base/domdata/xcolor.h"


EW_ENTER



class Variant;
class DObject;
class CallableData;

template<typename T, typename P>
class vv_cast;

#ifdef EW_UOS
template<typename T, typename P>
class vv_cast : public P
{
public:

	using P::g;

	template<unsigned N>
	struct fk
	{
		static T value(const Variant& v);
	};

	static inline T g(const Variant& v);

};
#endif


template<>
class pl_cast_base<bool>
{
public:
	typedef bool type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	static type g(const VariantTable&){ return true; }

	static type g(const type& v){ return v; }
	static type g(int32_t v){ return v != 0; }
	static type g(int64_t v){ return v != 0; }
	static type g(float v){ return v != 0; }
	static type g(double v){ return v != 0; }
	static type g(const String& v){ return !v.empty(); }
	static type g(CallableData* d){ return d != NULL; }

	template<typename T>
	static type g(const std::complex<T>& o)
	{
		return g(o.real()) || g(o.imag());
	}

	template<typename T>
	static type g(const tiny_cpx<T>& o)
	{
		return g(o.real()) || g(o.imag());
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		if (v.size() == 1) return g(v(0));
		Exception::XBadCast();
		return type();
	}

};


template<>
class pl_cast_base<VariantTable>;


template<>
class pl_cast_base<DataPtrT<CallableData> >
{
public:
	typedef DataPtrT<CallableData> type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

};



template<typename T>
class kany_cast_number
{
public:
	typedef T type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

#if !defined(LONG_IS_INT)
	static type g(long v){ return (type)v; }
#endif

	static type g(bool v){ return (type)v; }
	static type g(char v){ return (type)v; }
	static type g(int8_t v){ return (type)v; }
	static type g(uint8_t v){ return (type)v; }
	static type g(int16_t v){ return (type)v; }
	static type g(uint16_t v){ return (type)v; }
	static type g(int32_t v){ return (type)v; }
	static type g(int64_t v){ return (type)v; }
	static type g(uint32_t v){ return (type)v; }
	static type g(uint64_t v){ return (type)v; }
	static type g(void* v){ return (type)(uintptr_t)v; }

	static type g(float v){ return (type)v; }
	static type g(double v){ return (type)v; }

	static type g(CallableData* v);

	static type g(const String& v)
	{
		if (tl::is_same_type<type, double>::value || tl::is_same_type<type, float>::value)
		{
			double k; if (v.ToNumber(&k)) return (type)k;
			Exception::XBadCast(); return type();
		}
		else
		{
			double k; if (v.ToNumber(&k)) return (type)k;
			Exception::XBadCast(); return type();
		}
	}

	static type g(const char* v)
	{
		return g(String(v));
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		if (v.size() == 1) return g(v(0));
		Exception::XBadCast();
		return type();
	}

	template<typename O>
	static type g(const std::complex<O>& v)
	{
		if (v.imag() == type(0)) return g(v.real());
		Exception::XBadCast();
		return type();
	}

	template<typename O>
	static type g(const tiny_cpx<O>& v)
	{
		if (v.imag() == type(0)) return g(v.real());
		Exception::XBadCast();
		return type();
	}


};

template<> class pl_cast_base<uint32_t> : public kany_cast_number<uint32_t>{};
template<> class pl_cast_base<uint64_t> : public kany_cast_number<uint64_t>{};
template<> class pl_cast_base<int32_t> : public kany_cast_number<int32_t>{};
template<> class pl_cast_base<int64_t> : public kany_cast_number<int64_t>{};
template<> class pl_cast_base<float> : public kany_cast_number<float>{};
template<> class pl_cast_base<double> : public kany_cast_number<double>{};

template<typename T>
class pl_cast_base<tiny_cpx<T> >
{
public:
	typedef tiny_cpx<T> type;
	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	static type g(bool v){ return type(v, 0); }
	static type g(int32_t v){ return type(v, 0); }
	static type g(int64_t v){ return type(v, 0); }
	static type g(float v){ return type(v, 0); }
	static type g(double v){ return type(v, 0); }
	static const type& g(const type& v){ return v; }

	static type g(const String& v) /* v should be stripped first */
	{
		return g(v.c_str());
	}

	static type g(const char* p_)
	{
		type val;
		const char* p = p_;
		ScannerHelper<const char*>::read_number_ex(p, val);
		return val;
	}

	template<typename O>
	static type g(const arr_1t<O>& v)
	{
		if (v.size() == 1) return g(v[0]);
		Exception::XBadCast();
		return type();
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		if (v.size() == 1) return g(v(0));
		Exception::XBadCast();
		return type();
	}
};

template<typename T>
class pl_cast_base<std::complex<T> >
{
public:
	typedef std::complex<T> type;

	template<typename O>
	static type g(const O& o)
	{
		return pl_cast_base<tiny_cpx<T> >::g(o);
	}

	static type g(bool v){ return type(v, 0); }
	static type g(int32_t v){ return type(v, 0); }
	static type g(int64_t v){ return type(v, 0); }
	static type g(float v){ return type(v, 0); }
	static type g(double v){ return type(v, 0); }
	static const type& g(const type& v){ return v; }

};

template<>
class pl_cast_base<mat4d>
{
public:
	typedef mat4d type;

	template<typename O>
	static type g(const O&) { Exception::XBadCast(); return type(); }

	static const type& g(const type& v){ return v; }

	template<typename O>
	static type g(const arr_xt<O>& v);

};

template<>
class pl_cast_base<String>
{
public:
	typedef String type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	template<typename O>
	static type g(O* p){ return String::Format("addr:0x%p", p); }

	static const type& g(const type& v){ return v; }
	static type g(bool v){ return v ? "true" : "false"; }
	static type g(int32_t v){ type t; t << v; return t; }
	static type g(int64_t v){ type t; t << v; return t; }
	static type g(float v){ type t; t << v; return t; }
	static type g(double v){ type t; t << v; return t; }
	static type g(DColor v){ return v.cast_to_string(); }
	static type g(const VariantTable& o)
	{
		return String::Format("table:0x%p", &o);
	}

	template<typename O>
	static type g(const tiny_cpx<O>& v){ type t; t << "complex(" << v.real() << "," << v.imag() << ")"; return t; }

	template<typename O>
	static type g(const std::complex<O>& v){ type t; t << "complex(" << v.real() << "," << v.imag() << ")"; return t; }

	static type g(CallableData* v);

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		auto& sz(v.size_ptr());
		String s0, s1;

		for (int i = 0; i < sz.MAX_DIM; i++)
		{
			if (i == 0)
			{
				s1 << sz[i];
			}
			else
			{
				s1 << 'x' << sz[i];
			}

			if (sz[i] != 1 || s0.empty())
			{
				s0 = s1;
			}
		}

		String type;

		if (tl::is_same_type<O, double>::value)
		{
			type = "array_double";
		}
		else if (tl::is_same_type<O, int64_t>::value)
		{
			type = "array_integer";
		}
		else if (tl::is_same_type<O, tiny_cpx<double> >::value)
		{
			type = "array_complex";
		}
		else if (tl::is_same_type<O, Variant >::value)
		{
			type = "array_variant";
		}
		else
		{
			type = "array";
		}

		return String::Format("[%s:size(%s)]", type, s0);
	}

};

template<>
class pl_cast_base<Variant>;



template<typename T>
class pl_cast_base<arr_1t<T> >
{
public:
	typedef arr_1t<T> type;

	typedef vv_cast<T, pl_cast_base<T> > pl_type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	static type g(bool v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(int32_t v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(int64_t v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(float v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(double v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(dcomplex v){ type t(1); t[0] = pl_type::g(v); return t; }
	static type g(const String& v){ type t(1); t[0] = pl_type::g(v); return t; }

	static const type& g(const type& v){ return v; }

	template<typename O>
	static type g(const arr_1t<O>& v)
	{
		type t(v.size());
		for (size_t i = 0; i<v.size(); i++) t[i] = pl_type::g(v[i]);
		return EW_MOVE(t);
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		type t(v.size());
		for (size_t i = 0; i < v.size(); i++) t[i] = pl_type::g(v(i));
		return EW_MOVE(t);
	}

};

template<typename T>
class pl_cast_base < arr_xt<T> >
{
public:
	typedef arr_xt<T> type;

	typedef vv_cast<T, pl_cast_base<T>> pl_type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	static type g(bool v){ type t(1); t(0) = pl_type::g(v); return t; }
	static type g(int32_t v){ type t(1); t(0) = pl_type::g(v); return t; }
	static type g(int64_t v){ type t(1); t(0) = pl_type::g(v); return t; }
	static type g(float v){ type t(1); t(0) = pl_type::g(v); return t; }
	static type g(double v){ type t(1); t(0) = pl_type::g(v); return t; }
	static type g(dcomplex v) { type t(1); t(0) = pl_type::g(v); return t; }
	static type g(const String& v){ type t(1); t(0) = pl_type::g(v); return t; }

	static type g(const mat4d& v)
	{
		type t;
		t.resize(4, 4);
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				t(i,j)= pl_type::g(v(i,j));
			}
		}
		return EW_MOVE(t);
	}

	static const type& g(const type& v){ return v; }

	template<typename O>
	static type g(const arr_1t<O>& v)
	{
		type t(v.size());
		for (size_t i = 0; i<v.size(); i++) t[i] = pl_type::g(v[i]);
		return EW_MOVE(t);
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		type t(v.size_ptr(), v.size());
		for (size_t i = 0; i < v.size(); i++) t(i) = pl_type::g(v(i));
		return EW_MOVE(t);
	}

};

template<typename T, int N>
class pl_cast_base < tiny_vec<T, N> >
{
public:
	typedef tiny_vec<T, N> type;

	template<typename O>
	static type g(const O&){ Exception::XBadCast(); return type(); }

	template<typename O>
	static type g(const arr_1t<O>& v)
	{
		type t;
		for (size_t i = 0; i < N && i < v.size(); ++i)
		{
			t[i] = vv_cast<T, pl_cast_base<T> >::g(v[i]);
		}
		return t;
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		type t;
		size_t n = std::min<size_t>(N, v.size());
		for (size_t i = 0; i < n; ++i)
		{
			t[i] = vv_cast<T, pl_cast_base<T> >::g(v[i]);
		}
		return t;
	}
};



template<typename X, int N>
class pl_cast_base < arr_xt<tiny_vec<X, N> > >
{
public:

	typedef arr_xt<tiny_vec<X, N> > type;

	typedef vv_cast<tiny_vec<X, N>, pl_cast_base<tiny_vec<X, N> >> pl_type;
	typedef vv_cast<X, pl_cast_base<X>> pl_type_x;

	template<typename O>
	static type g(const O&) { Exception::XBadCast(); return type(); }

	static const type& g(const type& v) { return v; }

	template<typename O>
	static type g(const arr_1t<O>& v)
	{
		type t(v.size());
		for (size_t i = 0; i < v.size(); i++) t[i] = pl_type::g(v[i]);
		return EW_MOVE(t);
	}

	template<typename O>
	static type g(const arr_xt<O>& v)
	{
		size_t n = v.size();
		if (n % N != 0) Exception::XBadCast();
		type t;
		t.resize(n / N);
		X* p1 = (X*)t.data();
		O* p2 = (O*)v.data();
		for (size_t i = 0; i < n; i++) p1[i] = pl_type_x::g(p2[i]);
		return EW_MOVE(t);
	}

	template<typename O, int M>
	static type g(const arr_xt<tiny_vec<O, M> >& v)
	{
		if (N != M) Exception::XBadCast();
		size_t n = v.size();
		type t;
		t.resize(n);
		for (size_t i = 0; i < n; i++) t(i) = pl_type::g(v(i));
		return EW_MOVE(t);
	}


};


#ifndef EW_UOS
template<typename T, typename P>
class vv_cast : public P
{
public:

	using P::g;

	template<unsigned N>
	struct fk
	{
		static T value(const Variant& v);
	};

	static inline T g(const Variant& v);

};
#endif


template<typename O>
mat4d pl_cast_base<mat4d>::g(const arr_xt<O>& v)
{
	size_t sz = v.size(0);
	if (sz != v.size(1))
	{
		Exception::XBadCast(); return type();
	}

	if (tl::is_same_type<O, double>::value && sz == 4)
	{
		return *(type*)v.data();
	}

	typedef vv_cast<double, pl_cast_base<double> > pl_type;
	//typedef pl_cast_base<double> pl_type;

	mat4d m4;
	if (sz == 3)
	{
		m4(0, 0) = pl_type::g(v(0, 0));
		m4(1, 0) = pl_type::g(v(1, 0));
		m4(2, 0) = pl_type::g(v(2, 0));
		m4(0, 1) = pl_type::g(v(0, 1));
		m4(1, 1) = pl_type::g(v(1, 1));
		m4(2, 1) = pl_type::g(v(2, 1));
		m4(0, 2) = pl_type::g(v(0, 2));
		m4(1, 2) = pl_type::g(v(1, 2));
		m4(2, 2) = pl_type::g(v(2, 2));
	}
	else if (sz == 4)
	{
		m4(0, 0) = pl_type::g(v(0, 0));
		m4(1, 0) = pl_type::g(v(1, 0));
		m4(2, 0) = pl_type::g(v(2, 0));
		m4(3, 0) = pl_type::g(v(3, 0));
		m4(0, 1) = pl_type::g(v(0, 1));
		m4(1, 1) = pl_type::g(v(1, 1));
		m4(2, 1) = pl_type::g(v(2, 1));
		m4(3, 1) = pl_type::g(v(3, 1));
		m4(0, 2) = pl_type::g(v(0, 2));
		m4(1, 2) = pl_type::g(v(1, 2));
		m4(2, 2) = pl_type::g(v(2, 2));
		m4(3, 2) = pl_type::g(v(3, 2));
		m4(0, 3) = pl_type::g(v(0, 3));
		m4(1, 3) = pl_type::g(v(1, 3));
		m4(2, 3) = pl_type::g(v(2, 3));
		m4(3, 3) = pl_type::g(v(3, 3));

	}
	else
	{
		Exception::XBadCast();
	}

	return m4;
}

template<typename T>
class pl_cast : public vv_cast<T, pl_cast_base<T> >
{
public:

	typedef vv_cast<T, pl_cast_base<T> > basetype;

	using basetype::g;

	static inline T g(const char* v)
	{
		return basetype::g(v);
	}

	template<typename X>
	static inline T g(const X& v, const T& d)
	{
		try
		{
			return g(v);
		}
		catch (...)
		{
			return d;
		}
	}

	static T k(const Variant& v);

};

template<>
class pl_cast < Variant > ;

template<>
class pl_cast < DObject* > ;





EW_LEAVE

#endif
