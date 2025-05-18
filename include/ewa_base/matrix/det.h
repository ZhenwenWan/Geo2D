#ifndef __EWA_MATRIX_DET_H__
#define __EWA_MATRIX_DET_H__

#include "ewa_base/matrix/lu.h"

EW_ENTER

double det(const pivot_matrix& P)
{
	return 1.0; // TODO: -1?
}

namespace detail
{

	class det_abs
	{
	public:

		template<typename T>
		static double g(const T& v)
		{
			return double(v);
		}

		static double g(const dcomplex& v)
		{
			return v.length();
		}
	};

}


template<typename T, typename X>
double rcv_det(const matrix_rcv_t<T, X>& A)
{
	if (!A.is_square())
	{
		Exception::XError("matrix is not square");
	}

	matrix_rcv_t<T, X> L, U;
	pivot_matrix P;

	if (!rcv_lu(A, L, U, P))
	{
		return 0.0;
	}

	size_t n = A.get_rows();
	double v = det(P);

	for (size_t i = 0; i < n; i++)
	{
		uint32_t n1 = U.r_pos[i];
		uint32_t n2 = U.r_pos[i + 1];
		if (n1 == n2 || U.value[n1].row != U.value[n1].col)
		{
			return 0.0;
		}
		v *= detail::det_abs::g(U.value[n1].value);
	}

	return v;

}



EW_LEAVE

#endif