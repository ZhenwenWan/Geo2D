#ifndef __MODULE_MATH_PATCH_HPP__
#define __MODULE_MATH_PATCH_HPP__

#include "ewa_base/scripting.h"
#include "ewa_base/scripting/pl_dispatch.h"
#include "ewa_base/math/math_op.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/scripting/variant.h"

EW_ENTER
// down casting function (defined in "module_math.cpp")
int down_cast_variant_type(Variant& v);

template<typename P>
class PL1_func : public CallableFunction
{
public:
	typedef typename pl1_call<P>::lk lk;

	static int variant_call(Executor &ks, Variant& v1)
	{
		int n = down_cast_variant_type(v1);
		if (lk::cmap[n] == &PL1_func<P>::variant_call)
		{
			return pl1_dispatch_base<P>::value(ks, v1);
		}
		return lk::test(n)(ks, v1);
	}

	PL1_func() :CallableFunction(P::info().name, P::info().flag | CallableFunction::FLAG_IS_META)
	{
		int v1 = type_flag<arr_xt<Variant> >::value;
		lk::cmap[v1] = variant_call;
	}

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 1);
		//Variant &v(*ks.ci1.nsp);
		return pl1_call<P>::g(ks);
	}
};

template<typename P /* function policy */, class H /* check and lift */, bool S = true /* static method or closure ( simulated by c++ functor ) */ >
class PL1_func_ext : public PL1_func < P >
{
public:
	typedef PL1_func<P> basetype;

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 1);

		if (H::check(ks.ci0.nbx[1]))
		{
			H::lift(ks.ci0.nbx[1]);
			return pl1_call<P>::g(ks);
		}
		else
		{
			return pl1_call<P>::g(ks);
		}
	}
};

template<typename P, class H>
class PL1_func_ext<P, H, false> : public PL1_func < P >
{
public:
	typedef PL1_func<P> basetype;

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 1);

		H h;

		if (h.check(ks.ci0.nbx[1]))
		{
			h.lift(ks.ci0.nbx[1]);
			return pl1_call<P>::g(ks);
		}
		else
		{
			return pl1_call<P>::g(ks);
		}
	}
};

// Composed Policy with both C (Checker) and L (Lifter)
template<class C, class L>
struct LiftPolicyT : public C, public L
{
	typedef LiftPolicyT<C, L> this_type;

	template<typename T>
	static bool check_arr(const arr_xt<T>& arr)
	{
		for (size_t i = 0; i < arr.size(); ++i)
		{
			if (C::check(arr[i]))
			{
				return true;
			}
		}
		return false;
	}

	static bool check_arr(const arr_xt<Variant>& arr)
	{
		for (size_t i = 0; i < arr.size(); ++i)
		{
			if (this_type::check(arr[i]))
			{
				return true;
			}
		}
		return false;
	}

	static bool check(Variant v)
	{
		switch (v.type())
		{
		case type_flag<int64_t>::value: return C::check(v.get<int64_t>());
		case type_flag<double>::value: return C::check(v.get<double>());
		case type_flag<dcomplex>::value: return C::check(v.get<dcomplex>());
		case type_flag<arr_xt<int64_t> >::value: return check_arr(v.get<arr_xt<int64_t> >());
		case type_flag<arr_xt<double> >::value: return check_arr(v.get<arr_xt<double> >());
		case type_flag<arr_xt<dcomplex> >::value: return check_arr(v.get<arr_xt<dcomplex> >());
		case type_flag<arr_xt<Variant> >::value: return check_arr(v.get<arr_xt<Variant> >());
		default:
			return false;
		}
	}

	static bool check(Variant v1, Variant v2)
	{
		return check(v1) || check(v2);
	}
};


template<typename P>
class PL2_func : public CallableFunction
{
public:

	typedef typename pl2_call<P>::lk lk;

	static int variant_call(Executor& ks, Variant& v1, Variant& v2)
	{
		int n = (down_cast_variant_type(v1) << 4) | down_cast_variant_type(v2);
		if (lk::cmap[n] == &PL2_func<P>::variant_call)
		{
			return pl2_dispatch_base<P>::value(ks, v1, v2);
		}
		return lk::test(n)(ks, v1, v2);
	}

	PL2_func() :CallableFunction(P::info().name, P::info().flag | CallableFunction::FLAG_IS_META)
	{
		int value = type_flag<arr_xt<Variant> >::value;
		for (int i = 0; i<16; i++)
		{
			int v1 = (value << 4) | i;
			int v2 = (i << 4) | value;
			lk::cmap[v1] = variant_call;
			lk::cmap[v2] = variant_call;
		}
	}

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 2);
		return pl2_call<P>::g(ks);
	}
};


template<typename P, class H, bool S = true>
class PL2_func_ext : public PL2_func < P >
{
public:
	typedef PL2_func<P> basetype;

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 2);

		// 考虑被指令调用的可能性
		Variant& v1(ks.ci1.nsp[-1]);
		Variant& v2(ks.ci1.nsp[-0]);

		if (H::check(v1,v2))
		{
			H::lift(v1,v2);
			return pl2_call<P>::g(ks);
		}
		else
		{
			return pl2_call<P>::g(ks);
		}
	}
};

template<typename P, class H>
class PL2_func_ext < P, H, false > : public PL2_func < P >
{
public:
	typedef PL2_func<P> basetype;

	virtual int __fun_call(Executor& ks, int n)
	{
		ks.check_pmc(this, n, 2);
		H h;

		if (h.check(ks.ci0.nbx[1], ks.ci0.nbx[2]))
		{
			h.lift(ks.ci0.nbx[1], ks.ci0.nbx[2]);
			return pl2_call<P>::g(ks);
		}
		else
		{
			return pl2_call<P>::g(ks);
		}
	}
};

template<typename P>
class PLX_func : public tl::meta_if<tl::is_convertible<P, pl_base1>::value, PL1_func<P>, PL2_func<P> >::type
{
public:
	DECLARE_OBJECT_INFO_CACHED(PLX_func, ObjectInfo);

};

template<typename P, class H, bool S = true>
class PLX_func_ext : public tl::meta_if<tl::is_convertible<P, pl_base1>::value, PL1_func_ext<P, H, S>, PL2_func_ext<P, H, S> >::type
{
public:
	typedef typename tl::meta_if<tl::is_convertible<P, pl_base1>::value, PL1_func_ext<P, H, S>, PL2_func_ext<P, H, S> >::type type;
	static int g(Executor& ks, int n)
	{
		static type instance;
		return instance.__fun_call(ks, n);
	}
	DECLARE_OBJECT_INFO_CACHED(PLX_func_ext, ObjectInfo);
};

template<typename P> typename PLX_func<P>::infotype PLX_func<P>::sm_info(String("EWSL_PLX_func#") + P::info().name);
template<typename P, class H, bool S> typename PLX_func_ext<P, H, S>::infotype PLX_func_ext<P, H, S>::sm_info(String("EWSL_PLX_func#") + P::info().name);

// Math function patch policies of check and lift
namespace detail
{
	template<typename T>
	struct _Lift_by_T
	{
		template<typename T1>
		static bool check(const T1) { return false; }
		static bool check(const T) { return true; }

		template<typename T1, typename T2>
		static bool check(const T1 x, const T2 y) { return check(x)||check(y); }
	};

	struct _Lift_When_Negative
	{
		static bool check(const int64_t x) { return x < 0; }
		static bool check(const double x) { return x < 0; }
		static bool check(const dcomplex&) { return false; }
	};

	struct _Lift_When_Inverse_Trig
	{
		static bool check(const int64_t x) { return x < -1 || x > 1; }
		static bool check(const double x) { return x < -1 || x > 1; }
		static bool check(const dcomplex&) { return false; }
	};

	struct _Lift_When_Inverse_acosh
	{
		static bool check(const int64_t x) { return x < 1; }
		static bool check(const double x) { return x < 1; }
		static bool check(const dcomplex&) { return false; }
	};

	struct _Lift_When_Inverse_atanh
	{
		static bool check(const int64_t x) { return x < -1 || x > 1; }
		static bool check(const double x) { return x < -1 || x > 1; }
		static bool check(const dcomplex&) { return false; }
	};

	struct _lift_int2double
	{
		static void lift(Variant& v)
		{
			if (!variant_helper::is_num(v))
			{
				return;
			}

			Variant p = v;
			int type = v.type();
			if (type == type_flag<int64_t>::value)
			{
				v.ref<double>() = variant_cast<double>(p);
			}
			else if (type == type_flag<arr_xt<int64_t> >::value)
			{
				v.ref<arr_xt<double> >() = variant_cast<arr_xt<double> >(p);
			}
		}

		static void lift(Variant& v1, Variant& v2)
		{
			lift(v1);
			lift(v2);
		}
	};

	struct _lift2complex
	{
		static void lift(Variant& v)
		{
			if (!variant_helper::is_num(v))
			{
				return;
			}

			Variant p = v;
			if (variant_helper::is_scr(v))
			{
				v.ref<dcomplex>() = variant_cast<dcomplex>(p);
			}
			else if (variant_helper::is_arr(v))
			{
				variant_cast<arr_xt<dcomplex> >(p).swap(v.ref<arr_xt<dcomplex> >());
			}
		}
	};
}

struct Negative_lift2complex : public LiftPolicyT<detail::_Lift_When_Negative, detail::_lift2complex> {};
struct InverseTrig_lift2complex : public LiftPolicyT<detail::_Lift_When_Inverse_Trig, detail::_lift2complex> {};
struct InverseHyperTrig_acosh_lift2complex : public LiftPolicyT < detail::_Lift_When_Inverse_acosh, detail::_lift2complex > {};
struct InverseHyperTrig_atanh_lift2complex : public LiftPolicyT < detail::_Lift_When_Inverse_atanh, detail::_lift2complex > {};

struct Integral_lift2double : public LiftPolicyT < detail::_Lift_by_T<int64_t>, detail::_lift_int2double > {};


namespace detail
{
	namespace lift
	{
		// 提升级别1：当底数和指数都是整数，且底数>0时，如果指数<0，则将底数提升为浮点数
		template<typename T1, typename T2> // T1 = int64_t, T2 = int64_t
		struct _level1
		{
			static bool check(T1 _base, T2 _power)
			{
				return _base != 0 && _power < 0;
			}

			static void lift(Variant& _base, Variant& _power)
			{
				EW_UNUSED(_power);
				Variant p = _base;
				_base.ref<double>() = p.get<T1>();
			}
		};

		// 提升级别2：当底数是整数或者浮点数，且底数<0时，如果指数不是实质性整数，则将底数提升为复数
		template<typename T1, typename T2> // T1 = int64_t | double, T2 = int64_t | double | dcomplex
		struct _level2
		{
			static bool check(T1 _base, T2 _power)
			{
				return _base < 0 && !variant_helper::is_integer(_power);
			}

			static void lift(Variant& _base, Variant& _power)
			{
				EW_UNUSED(_power);
				Variant p = _base;
				_base.ref<dcomplex>() = p.get<T1>();
			}
		};

		template<typename T1, typename T2>
		struct impl;

		template<typename T1, typename T2>
		struct impl_basic;

		template<typename T1, typename T2>
		struct impl_var;

		template<>
		struct impl_basic<int64_t, int64_t> : public _level1 < int64_t, int64_t > {};
		template<>
		struct impl_basic<int64_t, double> : public _level2 < int64_t, double > {};
		template<>
		struct impl_basic<double, int64_t> : public _level2 < double, int64_t > {};
		template<>
		struct impl_basic<double, double> : public _level2 < double, double > {};

		// if T1 or T2 is Variant or arr_xt<Variant>, it is dispatched to impl_var, else is dispatched to impl_basic
		template<typename T1, typename T2>
		struct impl : public tl::meta_if<
			tl::is_same_type<T1, Variant>::value ||
			tl::is_same_type<T2, Variant>::value ||
			tl::is_same_type<T1, arr_xt<Variant> >::value ||
			tl::is_same_type<T2, arr_xt<Variant> >::value,
			impl_var<T1, T2>,
			impl_basic<T1, T2> >::type
		{};

		template<typename T1, typename T2>
		struct impl_basic
		{
			static bool check(T1, T2) { return false; }
			static void lift(Variant&, Variant&) { return; }
		};

		template<typename T1, typename T2>
		struct impl_var : public impl_basic < T1, T2 > {};

		template<typename T1, typename T2>
		struct impl_basic < T1, arr_xt<T2> >
		{
			static bool check(const T1& _base, const arr_xt<T2>& _power)
			{
				// Note: this loop can be optimized by just check _base once
				for (size_t i = 0; i < _power.size(); ++i)
				{
					if (impl<T1, T2>::check(_base, _power[i]))
					{
						return true;
					}
				}
				return false;
			}

			static void lift(Variant& _base, Variant& _power)
			{
				Variant p = _base;

				if (variant_helper::is_integer(_base) && variant_helper::is_integer(_power))
				{
					_base.ref<double>() = variant_cast<double>(p);
				}
				else
				{
					_base.ref<dcomplex>() = variant_cast<dcomplex>(p);
				}
			}
		};

		template<typename T1, typename T2>
		struct impl_basic < arr_xt<T1>, T2 >
		{
			static bool check(arr_xt<T1> _base, const T2& _power)
			{
				for (size_t i = 0; i < _base.size(); ++i)
				{
					if (impl<T1, T2>::check(_base[i], _power))
					{
						return true;
					}
				}
				return false;
			}

			static void lift(Variant& _base, Variant& _power)
			{
				EW_UNUSED(_power);
				Variant p = _base;

				if (variant_helper::is_integer(_base) && variant_helper::is_integer(_power))
				{
					//_base.ref<arr_xt<double> >().swap(variant_cast<arr_xt<double> >(p));
					variant_cast<arr_xt<double> >(p).swap(_base.ref<arr_xt<double> >());
				}
				else
				{
					variant_cast<arr_xt<dcomplex> >(p).swap(_base.ref<arr_xt<dcomplex> >());
				}
			}
		};

		template<typename T1, typename T2>
		struct impl_basic < arr_xt<T1>, arr_xt<T2> >
		{
			static bool check(const arr_xt<T1>& _base, const arr_xt<T2>& _power)
			{
				// Note: array size should match
				EW_ASSERT(_base.size() == _power.size());
				for (size_t i = 0; i < _base.size(); ++i)
				{
					if (impl<T1, T2>::check(_base[i], _power[i]))
					{
						return true;
					}
				}
				return false;
			}
			static void lift(Variant& _base, Variant& _power)
			{
				EW_UNUSED(_power);
				Variant p = _base;

				if (variant_helper::is_integer(_base) && variant_helper::is_integer(_power))
				{
					variant_cast<arr_xt<double>>(p).swap(_base.ref<arr_xt<double> >());
				}
				else
				{
					variant_cast<arr_xt<dcomplex>>(p).swap(_base.ref<arr_xt<dcomplex> >());
				}
			}
		};


		template<>
		struct impl_var < Variant, Variant >
		{
			static bool check(const Variant& _base, const Variant& _power)
			{
				switch (_base.type())
				{
				case type_flag<int64_t>::value: return impl<int64_t, Variant>::check(_base.get<int64_t>(), _power);
				case type_flag<double>::value: return impl<double, Variant>::check(_base.get<double>(), _power);
				case type_flag<arr_xt<int64_t> >::value: return impl<arr_xt<int64_t>, Variant>::check(_base.get<arr_xt<int64_t> >(), _power);
				case type_flag<arr_xt<double> >::value: return impl<arr_xt<double>, Variant>::check(_base.get<arr_xt<double> >(), _power);
				case type_flag<arr_xt<Variant> >::value:
					return variant_helper::is_num(_base) && impl_var<arr_xt<Variant>, Variant>::check(_base.get<arr_xt<Variant> >(), _power);
				default:
					return false;
				}
			}

			static void lift(Variant& _base, Variant& _power)
			{
				switch (_base.type())
				{
				case type_flag<int64_t>::value: impl<int64_t, Variant>::lift(_base, _power); return;
				case type_flag<double>::value: impl<int64_t, Variant>::lift(_base, _power); return;
				case type_flag<arr_xt<int64_t> >::value: impl<int64_t, Variant>::lift(_base, _power); return;
				case type_flag<arr_xt<double> >::value: impl<int64_t, Variant>::lift(_base, _power); return;
				case type_flag<arr_xt<Variant> >::value: impl_var<arr_xt<Variant>, Variant>::lift(_base, _power); return;
				}
			}
		};

		template<typename T1> // T1 is in { int64_t, double, arr_xt<int64_t>, arr_xt<double> }
		struct impl_var < T1, Variant >
		{
			static bool check(const T1& _base, const Variant& _power)
			{
				switch (_power.type())
				{
				case type_flag<int64_t>::value: return impl<T1, int64_t>::check(_base, _power.get<int64_t>());
				case type_flag<double>::value: return impl<T1, double>::check(_base, _power.get<double>());
				case type_flag<arr_xt<int64_t> >::value: return impl<T1, arr_xt<int64_t> >::check(_base, _power.get<arr_xt<int64_t> >());
				case type_flag<arr_xt<double> >::value: return impl<T1, arr_xt<double> >::check(_base, _power.get<arr_xt<double> >());
				case type_flag<arr_xt<Variant> >::value:
					return variant_helper::is_num(_power) && impl_var<T1, arr_xt<Variant> >::check(_base, _power.get<arr_xt<Variant> >());
				default:
					return false;
				}
			}

			static void lift(Variant& _base, Variant& _power)
			{
				switch (_power.type())
				{
				case type_flag<int64_t>::value: impl<T1, int64_t>::lift(_base, _power); return;
				case type_flag<double>::value: impl<T1, double>::lift(_base, _power); return;
				case type_flag<arr_xt<int64_t> >::value: impl<T1, arr_xt<int64_t> >::lift(_base, _power); return;
				case type_flag<arr_xt<double> >::value: impl<T1, arr_xt<double> >::lift(_base, _power); return;
				case type_flag<arr_xt<Variant> >::value: impl_var<T1, arr_xt<Variant> >::lift(_base, _power); return;
				}
			}
		};

		template<typename T2>
		struct impl_var < arr_xt<Variant>, T2 > : impl_basic < arr_xt<Variant>, T2 > {};

		template<>
		struct impl_var < arr_xt<Variant>, arr_xt<Variant> > : impl_basic < arr_xt<Variant>, arr_xt<Variant> > {};

		//template<>
		//struct impl_var < arr_xt<Variant>, Variant > : impl_basic < arr_xt<Variant>, Variant > {};
	}
}


struct Power_lift
{


	static bool check(const Variant& _base, const Variant& _power)
	{
		return detail::lift::impl<Variant, Variant>::check(_base, _power);
	}

	static void lift(Variant& _base, Variant& _power)
	{
		detail::lift::impl<Variant, Variant>::lift(_base, _power);
	}

};

EW_LEAVE
#endif
