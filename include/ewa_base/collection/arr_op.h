#ifndef __H_EW_COLLECTION_ARR_OP__
#define __H_EW_COLLECTION_ARR_OP__

#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/math/tiny_vec.h"
#include "ewa_base/math/tiny_mat.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/math/math_op.h"

EW_ENTER


namespace arrops1
{
	// type promoters
	template<typename T> struct type_wrapper { typedef T type; };

	template<typename T1, typename T2>
	struct type_promote;

	template<typename T>
	struct type_promote<T, T> : public type_wrapper < T > {};

#define __DEFINE_TYPE_PROMOTE__(T1, T2, T3)\
	template<> struct type_promote<T1, T2> : public type_wrapper < T3 > {}; \
	template<> struct type_promote<T2, T1> : public type_wrapper < T3 > {};

	__DEFINE_TYPE_PROMOTE__(int, int64_t, int64_t);
	__DEFINE_TYPE_PROMOTE__(int, float, float);
	__DEFINE_TYPE_PROMOTE__(int, double, double);
	__DEFINE_TYPE_PROMOTE__(int64_t, float, double);
	__DEFINE_TYPE_PROMOTE__(int64_t, double, double);
	__DEFINE_TYPE_PROMOTE__(float, double, double);

#undef __DEFINE_TYPE_PROMOTE__

	// complex promoter
	template<typename T1, typename T2>
	struct type_promote<tiny_cpx<T1>, T2> : public type_wrapper < tiny_cpx<typename type_promote<T1, T2>::type> > {};

	template<typename T1, typename T2>
	struct type_promote<T1, tiny_cpx<T2> > : public type_wrapper < tiny_cpx<typename type_promote<T1, T2>::type > > {};

	template<typename T1, typename T2>
	struct type_promote<tiny_cpx<T1>, tiny_cpx<T2> > : public type_wrapper < tiny_cpx<typename type_promote<T1, T2>::type > > {};

	template<typename T>
	struct type_promote<tiny_cpx<T>, tiny_cpx<T> > : public type_wrapper < tiny_cpx<T> > {};

	// arr_1t promoter
	template<typename T1, typename T2>
	struct type_promote<arr_1t<T1>, T2> : public type_wrapper < arr_1t<typename type_promote<T1, T2>::type> > {};

	template<typename T1, typename T2>
	struct type_promote<T1, arr_1t<T2> > : public type_wrapper < arr_1t<typename type_promote<T1, T2>::type > > {};

	template<typename T1, typename T2>
	struct type_promote<arr_1t<T1>, arr_1t<T2> > : public type_wrapper < arr_1t<typename type_promote<T1, T2>::type > > {};

	template<typename T>
	struct type_promote<arr_1t<T>, arr_1t<T> > : public type_wrapper < arr_1t<T> > {};

	// arr_xt promoter
	template<typename T1, typename T2>
	struct type_promote<arr_xt<T1>, T2> : public type_wrapper < arr_xt<typename type_promote<T1, T2>::type> > {};

	template<typename T1, typename T2>
	struct type_promote<T1, arr_xt<T2> > : public type_wrapper < arr_xt<typename type_promote<T1, T2>::type > > {};

	template<typename T1, typename T2>
	struct type_promote<arr_xt<T1>, arr_xt<T2> > : public type_wrapper < arr_xt<typename type_promote<T1, T2>::type > > {};

	template<typename T>
	struct type_promote<arr_xt<T>, arr_xt<T> > : public type_wrapper < arr_xt<T> > {};

	// tiny_vec
	template<typename T1, typename T2, int N>
	struct type_promote<tiny_vec<T1, N>, T2> : public type_wrapper < tiny_vec<typename type_promote<T1, T2>::type, N> > {};

	template<typename T1, typename T2, int N>
	struct type_promote<T1, tiny_vec<T2, N> > : public type_wrapper < tiny_vec<typename type_promote<T1, T2>::type, N> > {};

	template<typename T1, typename T2, int N>
	struct type_promote<tiny_vec<T1, N>, tiny_vec<T2, N> > : public type_wrapper < tiny_vec<typename type_promote<T1, T2>::type, N> > {};

	template<typename T, int N>
	struct type_promote<tiny_vec<T, N>, tiny_vec<T, N> > : public type_wrapper < tiny_vec<T, N> > {};

	// tiny_mat
	template<typename T1, typename T2, int R, int C>
	struct type_promote < tiny_mat<T1, R, C>, T2 >
		: public type_wrapper<tiny_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T1, typename T2, int R, int C>
	struct type_promote < T1, tiny_mat<T2, R, C> >
		: public type_wrapper<tiny_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T1, typename T2, int R, int C>
	struct type_promote < tiny_mat<T1, R, C>, tiny_mat<T2, R, C> >
		: public type_wrapper < tiny_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T, int R, int C>
	struct type_promote < tiny_mat<T, R, C>, tiny_mat<T, R, C> >
		: public type_wrapper < tiny_mat<T, R, C> > {};

	// type_mat
	template<typename T1, typename T2, int R, int C>
	struct type_promote < type_mat<T1, R, C>, T2 >
		: public type_wrapper<type_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T1, typename T2, int R, int C>
	struct type_promote < T1, type_mat<T2, R, C> >
		: public type_wrapper<type_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T1, typename T2, int R, int C>
	struct type_promote < type_mat<T1, R, C>, type_mat<T2, R, C> >
		: public type_wrapper < type_mat<typename type_promote<T1, T2>::type, R, C> >::type{};

	template<typename T, int R, int C>
	struct type_promote < type_mat<T, R, C>, type_mat<T, R, C> >
		: public type_wrapper < type_mat<T, R, C> > {};

	// Numeric Operator Definitions
	template<typename T1, typename T2>
	struct num_op
	{
		typedef typename type_promote<T1, T2>::type type;
		typedef T1 type1;
		typedef T2 type2;

		static type add(const type1 x, const type2 y)
		{
			return x * y;
		}

		static type sub(const type1 x, const type2 y)
		{
			return x - y;
		}

		static type mul(const type1 x, const type2 y)
		{
			return x * y;
		}

		static type div(const type1 x, const type2 y)
		{
			return x / y;
		}
	};

	// arr_xt
	template<typename T1, typename T2>
	struct num_op < arr_xt<T1>, T2 >
	{
		typedef typename type_promote<arr_xt<T1>, T2>::type type;
		typedef arr_xt<T1> type1;
		typedef T2 type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1& x, const type2 y)
		{
			type z(x.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x[i], y);
			return EW_MOVE(z);
		}

		static type sub(const type1& x, const type2 y)
		{
			type z(x.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x[i], y);
			return EW_MOVE(z);
		}

		static type mul(const type1& x, const type2 y)
		{
			type z(x.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x[i], y);
			return EW_MOVE(z);
		}

		static type div(const type1& x, const type2 y)
		{
			type z(x.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x[i], y);
			return EW_MOVE(z);
		}
	};

	template<typename T1, typename T2>
	struct num_op < T1, arr_xt<T2> >
	{
		typedef typename type_promote<T1, arr_xt<T2> >::type type;
		typedef T1 type1;
		typedef arr_xt<T2> type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1 x, const type2& y)
		{
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x, y[i]);
			return EW_MOVE(z);
		}

		static type sub(const type1 x, const type2& y)
		{
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x, y[i]);
			return EW_MOVE(z);
		}

		static type mul(const type1 x, const type2& y)
		{
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x, y[i]);
			return EW_MOVE(z);
		}

		static type div(const type1 x, const type2& y)
		{
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x, y[i]);
			return EW_MOVE(z);
		}
	};

	template<typename T1, typename T2>
	struct num_op < arr_xt<T1>, arr_xt<T2> >
	{
		typedef typename type_promote<arr_xt<T1>, arr_xt<T2> >::type type;
		typedef arr_xt<T1> type1;
		typedef arr_xt<T2> type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type sub(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type mul(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type div(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size_ptr());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x[i], y[i]);
			return EW_MOVE(z);
		}
	};


	// arr_1t
	template<typename T1, typename T2>
	struct num_op < arr_1t<T1>, T2 >
	{
		typedef typename type_promote<arr_1t<T1>, arr_1t<T2> >::type type;
		typedef arr_1t<T1> type1;
		typedef T2 type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1& x, const type2 y)
		{
			type z(x.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x[i], y);
			return EW_MOVE(z);
		}

		static type sub(const type1& x, const type2 y)
		{
			type z(x.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x[i], y);
			return EW_MOVE(z);
		}

		static type mul(const type1& x, const type2 y)
		{
			type z(x.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x[i], y);
			return EW_MOVE(z);
		}

		static type div(const type1& x, const type2 y)
		{
			type z(x.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x[i], y);
			return EW_MOVE(z);
		}
	};

	template<typename T1, typename T2>
	struct num_op < T1, arr_1t<T2> >
	{
		typedef typename type_promote<T1, arr_1t<T2> >::type type;
		typedef T1 type1;
		typedef arr_1t<T2> type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1 x, const type2& y)
		{
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x, y[i]);
			return EW_MOVE(z);
		}

		static type sub(const type1 x, const type2& y)
		{
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x, y[i]);
			return EW_MOVE(z);
		}

		static type mul(const type1 x, const type2& y)
		{
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x, y[i]);
			return EW_MOVE(z);
		}

		static type div(const type1 x, const type2& y)
		{
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x, y[i]);
			return EW_MOVE(z);
		}
	};

	template<typename T1, typename T2>
	struct num_op < arr_1t<T1>, arr_1t<T2> >
	{
		typedef typename type_promote<arr_1t<T1>, arr_1t<T2> >::type type;
		typedef arr_1t<T1> type1;
		typedef arr_1t<T2> type2;

		typedef num_op<T1, T2> scalar_op;

		static type add(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::add(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type sub(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::sub(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type mul(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::mul(x[i], y[i]);
			return EW_MOVE(z);
		}

		static type div(const type1& x, const type2& y)
		{
			EW_ASSERT(x.size() == y.size());
			type z(y.size());
			for (size_t i = 0; i < z.size(); ++i) z[i] = scalar_op::div(x[i], y[i]);
			return EW_MOVE(z);
		}
	};

	// tiny_vec
	template<typename T1, typename T2, int N>
	struct num_op < tiny_vec<T1, N>, T2 >
	{
		typedef typename type_promote<tiny_vec<T1, N>, T2>::type type;
		typedef tiny_vec<T1, N> type1;
		typedef T2 type2;

		static type add(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x[i], y);
			return z;
		}

		static type sub(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::sub(x[i], y);
			return z;
		}

		static type mul(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x[i], y);
			return z;
		}

		static type div(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::div(x[i], y);
			return z;
		}
	};

	template<typename T1, typename T2, int N>
	struct num_op < T1, tiny_vec<T2, N> >
	{
		typedef typename type_promote<T1, tiny_vec<T2, N> >::type type;
		typedef T1 type1;
		typedef tiny_vec<T2, N> type2;

		static type add(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x, y[i]);
			return z;
		}

		static type sub(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::sub(x, y[i]);
			return z;
		}

		static type mul(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x, y[i]);
			return z;
		}

		static type div(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::div(x, y[i]);
			return z;
		}
	};

	template<typename T1, typename T2, int N>
	struct num_op < tiny_vec<T1, N>, tiny_vec<T2, N> >
	{
		typedef typename type_promote<tiny_vec<T1, N>, tiny_vec<T2, N> >::type type;
		typedef tiny_vec<T1, N> type1;
		typedef tiny_vec<T2, N> type2;

		static type add(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x[i], y[i]);
			return z;
		}

		static type sub(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::sub(x[i], y[i]);
			return z;
		}

		static type mul(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::mul(x[i], y[i]);
			return z;
		}

		static type div(const type1& x, const type2& y)
		{
			type z;
			for (int i = 0; i < N; ++i) z[i] = num_op<T1, T2>::div(x[i], y[i]);
			return z;
		}
	};



	// function dispatchers
	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type mul(const T1& x, const T2& y)
	{
		return num_op<T1, T2>::mul(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type div(const T1& x, const T2& y)
	{
		return num_op<T1, T2>::div(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type add(const T1& x, const T2& y)
	{
		return num_op<T1, T2>::add(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type sub(const T1& x, const T2& y)
	{
		return num_op<T1, T2>::sub(x, y);
	}

	// operators
	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type operator+ (const T1& x, const T2& y)
	{
		return num_op<T1, T2>::add(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type operator- (const T1& x, const T2& y)
	{
		return num_op<T1, T2>::sub(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type operator* (const T1& x, const T2& y)
	{
		return num_op<T1, T2>::mul(x, y);
	}

	template<typename T1, typename T2>
	inline typename num_op<T1, T2>::type operator/ (const T1& x, const T2& y)
	{
		return num_op<T1, T2>::div(x, y);
	}


}

namespace arrops
{
	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator + (const arr_xt<X> &lhs, const Y rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] + rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator + (const X lhs, const arr_xt<Y> &rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(rhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator + (const arr_xt<X> &lhs, const arr_xt<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] + rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator - (const arr_xt<X> &lhs, const Y rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] - rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator - (const X lhs, const arr_xt<Y> &rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(rhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator - (const arr_xt<X> &lhs, const arr_xt<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator * (const arr_xt<X> &lhs, const Y rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] * rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator * (const X lhs, const arr_xt<Y> &rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(rhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs * rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator * (const arr_xt<X> &lhs, const arr_xt<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] * rhs[i];
		return result;
	}


	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator / (const arr_xt<X> &lhs, const Y rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] / rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator / (const X lhs, const arr_xt<Y> &rhs)
	{
		typename arr_promote_xt<X,Y>::promoted result(rhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs / rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_xt<X, Y>::promoted operator / (const arr_xt<X> &lhs, const arr_xt<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_xt<X,Y>::promoted result(lhs.size_ptr());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] / rhs[i];
		return result;
	}


	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator + (const arr_1t<X> &lhs, const Y rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] + rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator + (const X lhs, const arr_1t<Y> &rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(rhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator + (const arr_1t<X> &lhs, const arr_1t<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] + rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator - (const arr_1t<X> &lhs, const Y rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(rhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator - (const X lhs, const arr_1t<Y> &rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(rhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator - (const arr_1t<X> &lhs, const arr_1t<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] - rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator * (const arr_1t<X> &lhs, const Y rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] * rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator * (const X lhs, const arr_1t<Y> &rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(rhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs * rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator * (const arr_1t<X> &lhs, const arr_1t<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] * rhs[i];
		return result;
	}


	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator / (const arr_1t<X> &lhs, const Y rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] / rhs;
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator / (const X lhs, const arr_1t<Y> &rhs)
	{
		typename arr_promote_1t<X,Y>::promoted result(rhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs / rhs[i];
		return result;
	}

	template<typename X, typename Y>
	inline typename arr_promote_1t<X, Y>::promoted operator / (const arr_1t<X> &lhs, const arr_1t<Y> &rhs)
	{
		if (lhs.size() != rhs.size()) Exception::XInvalidArgument();
		typename arr_promote_1t<X,Y>::promoted result(lhs.size());
		for (size_t i = 0; i < result.size(); i++) result[i] = lhs[i] / rhs[i];
		return result;
	}
};

EW_LEAVE

#endif
