
#ifndef __H_EW_MATH_INTERP__
#define __H_EW_MATH_INTERP__

#include "ewa_base/config.h"

EW_ENTER


template<typename V>
inline void interp_update(size_t& n1,size_t& n2,double& r1,V it1, V it2, double t)
{
	auto itx = std::upper_bound(it1, it2, t);
	if (itx == it1)
	{
		r1 = 0.0;
		n1 = n2 = 0;
		return;
	}
	if (itx == it2)
	{
		r1 = 0.0;
		n1 = n2 = it2 - it1 - 1;
		return;
	}

	r1 = (*itx - t)/(*itx -*(itx-1));
	n2 = itx - it1;
	n1 = n2 - 1;
}

template<typename T>
class interp_1d_linear
{
public:

	T operator()(double t1)
	{
		EW_ASSERT(!tvalues.empty());
		EW_ASSERT(tvalues.size() == xvalues.size());
		double ur;
		size_t u1, u2;
		interp_update(u1,u2,ur, xvalues.begin(), xvalues.end(), t1);
		return tvalues[u1] * ur + tvalues[u2] * (1.0 - ur);

	}

	arr_1t<T> tvalues;
	arr_1t<double> xvalues;
};


template<typename T>
class interp_2d_linear
{
public:

	T operator()(double t1,double t2)
	{
		EW_ASSERT(!tvalues.empty());
		EW_ASSERT(tvalues.size(0) == xvalues.size());
		EW_ASSERT(tvalues.size(1) == yvalues.size());

		double ur;
		size_t u1, u2;
		interp_update(u1, u2, ur, xvalues.begin(), xvalues.end(), t1);

		double vr;
		size_t v1, v2;
		interp_update(v1, v2, vr, yvalues.begin(), yvalues.end(), t2);

		T k1= tvalues(u1,v1) * ur + tvalues(u2,v1) * (1.0 - ur);
		T k2= tvalues(u1,v2) * ur + tvalues(u2,v2) * (1.0 - ur);

		return k1 * vr + k2 * (1.0 - vr);

	}

	arr_xt<T> tvalues;
	arr_1t<double> xvalues;
	arr_1t<double> yvalues;
};






EW_LEAVE
#endif
