#ifndef __EWA_MATRIX_OP_H__
#define __EWA_MATRIX_OP_H__


#include "ewa_base/matrix/dense.h"
#include "ewa_base/matrix/sparse.h"


EW_ENTER

namespace matops
{

	template<typename T1,typename X,typename T2,typename T3>
	void mult_mat_Mv(const matrix_rcv_t<T1,X>& mm, const T2* vv, T3* result)
	{

		size_t m = mm.get_rows();
		auto pd = mm.value.data();

		for (size_t i = 0; i < m; i++)
		{
			T3 val(0);
			auto it1 = pd + mm.r_pos[i];
			auto it2 = pd + mm.r_pos[i + 1];

			for (auto it = it1; it != it2; it++)
			{
				val += (*it).value*vv[(*it).col];
			}
			result[i] = val;
		}
	
	}


	template<typename T1, typename X, typename T2, typename T3>
	void mult_inv_Lv(const matrix_rcv_t<T1, X>& mm, const T2* vv, T3* result)
	{

		size_t m = mm.get_rows();

		auto pd = mm.value.data();

		result[0] = vv[0] / mm.value[0].value;

		for (size_t i = 1; i < m; i++)
		{
			T3 val(vv[i]);
			auto it1 = pd + mm.r_pos[i];
			auto it2 = pd + mm.r_pos[i + 1] - 1;

			for (auto it = it1; it != it2; it++)
			{
				val -= (*it).value*result[(*it).col];
			}
			result[i] = val / (*it2).value;
		}
	}

	template<typename T1, typename X, typename T2, typename T3>
	void mult_inv_Uv(const matrix_rcv_t<T1, X>& mm, const T2* vv, T3* result)
	{
		size_t m = mm.get_rows();

		auto pd = mm.value.data();

		for (size_t j = 0; j < m; j++)
		{
			size_t i = m - j - 1;

			auto it1 = pd + mm.r_pos[i] + 1;
			auto it2 = pd + mm.r_pos[i + 1];

			EW_ASSERT(it1 <= it2);

			T3 val(vv[i]);

			for (auto it = it1; it != it2; it++)
			{
				val -= (*it).value*result[(*it).col];
			}

			result[i] = val / it1[-1].value;
		}
	}

	template<typename T>
	void mult_inv_Pv(const pivot_matrix& mm, const T* vv, T* result)
	{
		for (size_t i = 0; i < mm.get_rows(); i++)
		{
			result[mm.value[i]] = vv[i];
		}
	}

	template<typename T>
	void mult_mat_Pv(const pivot_matrix& mm, const T* vv, T* result)
	{
		size_t n = mm.get_rows();
		for (size_t i = 0; i < n; i++)
		{
			result[i] = vv[mm.value[i]];
		}
	}


	template<typename T>
	void print_matrix(matrix_rcv_t<T>& M, const char* p)
	{
		Console::Write("%s=\n", p);
		for (size_t i = 0; i < M.get_rows(); i++)
		{
			for (size_t j = 0; j < M.get_cols(); j++)
			{
				Console::Write(variant_cast<String>(M(i, j)));
				Console::Write(" ");
			}
			Console::Write("\n");
		}
		Console::Write("\n\n");

	}


	class matchekcer
	{
	public:

		double eps;

		matchekcer()
		{
			eps = 1.0e-9;
		}

		bool is_equal(double v1, double v2)
		{
			double dv = ::fabs(v1 - v2);
			return dv < eps || dv / (::fabs(v1) + ::fabs(v2)) < eps;
		}

		bool is_equal(dcomplex v1, dcomplex v2)
		{
			return is_equal(v1.real(), v2.real()) && is_equal(v1.imag(), v2.imag());
		}

		template<typename T, typename X>
		bool is_equal(const matrix_rcv_t<T, X>& lhs, const matrix_rcv_t<T, X>& rhs)
		{
			if (lhs.get_rows() != rhs.get_rows()) return false;
			if (lhs.get_cols() != rhs.get_cols()) return false;

			matrix_rcv_t<T, X> delta = lhs - rhs;
			for (auto it = delta.value.begin(); it != delta.value.end(); ++it)
			{
				if (!is_equal((*it).value, T(0)))
				{
					return false;
				}
			}
			return true;
		}

		template<typename M1, typename M2>
		bool is_equal(const M1& lhs, const M2& rhs)
		{
			if (lhs.get_rows() != rhs.get_rows()) return false;
			if (lhs.get_cols() != rhs.get_cols()) return false;

			size_t rows = lhs.get_rows();
			size_t cols = lhs.get_cols();
			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					if (!is_equal(lhs(i, j), rhs(i, j)))
					{
						return false;
					}
				}
			}

			return true;
		}

		template<typename T, typename X>
		static bool is_L_matrix(const matrix_rcv_t<T, X>& val)
		{
			for (auto it = val.value.begin(); it != val.value.end(); ++it)
			{
				if ((*it).row < (*it).col && (*it).value != T(0))
				{
					return false;
				}
			}
			return true;
		}

		template<typename T, typename X>
		static bool is_U_matrix(const matrix_rcv_t<T, X>& val)
		{
			for (auto it = val.value.begin(); it != val.value.end(); ++it)
			{
				if ((*it).row > (*it).col && (*it).value != T(0))
				{
					return false;
				}
			}
			return true;
		}

	};


}


template<typename T>
class tmp_sparse_vector
{
public:
	tmp_sparse_vector()
	{
		head.next = pfree = NULL;
	}

	class Item
	{
	public:
		Item* next;
		T value;
		uint32_t idx;
	};


	Item head;
	Item* pfree;


	template<typename X,typename Y>
	void append(X it1, X it2, Y ratio)
	{
		Item* lhs = head.next;
		Item* dst = &head;

		while (1)
		{
			if (it1 == it2)
			{
				dst->next = lhs;
				return;
			}

			if (!lhs)
			{
				for (auto it = it1; it != it2; ++it)
				{
					Item* p = allocate();
					p->value = (*it).value*ratio;
					p->idx = (*it).col;
					dst->next = p;
					dst = p;
				}
				return;

			}

			uint32_t c1 = lhs->idx;
			uint32_t c2 = (*it1).col;

			if (c1 > c2)
			{
				do
				{
					Item* p = allocate();
					p->value = (*it1).value*ratio;
					p->idx = (*it1).col;
					dst->next = p;
					dst = p;
					it1++;

				} while (it1 != it2 && c1 > (*it1).col);

			}
			else if (c1 < c2)
			{
				dst->next = lhs;
				dst = lhs;

				while (dst->next && dst->next->idx < c2)
				{
					dst = dst->next;
				}

				lhs = dst->next;
				dst->next = NULL;
				
			}
			else
			{
				lhs->value += (*it1).value*ratio;
				dst->next = lhs;
				dst = lhs;
				lhs = lhs->next;

				it1++;
			}

		}


	}

	Item* allocate()
	{
		Item* p = pfree;
		if (p)
		{
			pfree = p->next;
		}
		else
		{
			p = new Item;
		}
		p->next = NULL;
		return p;
	}

	void clear()
	{
		destroy(pfree);
		pfree = head.next;
		head.next = NULL;
	}

	~tmp_sparse_vector()
	{
		destroy(pfree);
		destroy(head.next);
	}

	void destroy(Item* p)
	{
		while (p)
		{
			Item* p2 = p;
			p = p2->next;
			delete p2;
		}
	}


};


template<typename T1, typename X1, typename T2, typename X2>
matrix_rcv_t<typename cpx_promote<T1, T2>::type> operator*(const matrix_rcv_t<T1, X1>& lhs, const matrix_rcv_t<T2, X2>& rhs)
{

	matrix_helper_base::check_matrix_size_for_mult(lhs, rhs);

	typedef typename cpx_promote<T1, T2>::type value_type;
	typedef matrix_rcv_t<value_type> result_type;

	typedef typename result_type::elem_type elem_type;

	matrix_rcv_t<value_type> result;

	result.reset(lhs.n_row, rhs.n_col);
	result.value.reserve(4 + std::min(lhs.n_row, rhs.n_col));

	auto pd_lhs = lhs.value.data();
	auto pd_rhs = rhs.value.data();

	bst_map<uint32_t, value_type> tmp1;
	tmp_sparse_vector<value_type> tmp2;

	for (uint32_t i = 0; i < lhs.n_row; i++)
	{
		auto p1 = pd_lhs + lhs.r_pos[i];
		auto p2 = pd_lhs + lhs.r_pos[i + 1];

		//if (p2 - p1 < 16)
		//{
		//	for (auto p = p1; p<p2; p++)
		//	{
		//		size_t row_id = p->col;
		//		auto v1 = pd_rhs + rhs.r_pos[row_id];
		//		auto v2 = pd_rhs + rhs.r_pos[row_id + 1];

		//		for (auto v = v1; v<v2; v++)
		//		{
		//			tmp1[(*v).col] += (*p).value*(*v).value;
		//		}
		//	}

		//	for (auto it = tmp1.begin(); it != tmp1.end(); it++)
		//	{
		//		if (!result_type::helper::is_zero((*it).second))
		//		{
		//			result.value.push_back(elem_type(i, (*it).first, (*it).second));
		//		}
		//	}

		//	tmp1.clear();
		//}
		//else
		{
			for (auto p = p1; p<p2; p++)
			{
				size_t row_id = p->col;
				auto v1 = pd_rhs + rhs.r_pos[row_id];
				auto v2 = pd_rhs + rhs.r_pos[row_id + 1];

				tmp2.append(v1, v2, (*p).value);

			}

			for (auto it = tmp2.head.next;it;it=it->next)
			{
				if (!result_type::helper::is_zero((*it).value))
				{
					result.value.push_back(elem_type(i, (*it).idx, (*it).value));
				}
			}

			tmp2.clear();
		}

		result.r_pos[i + 1] = result.value.size();
	}

	return EW_MOVE(result);

}


template<typename T1, typename X1, typename T2, typename X2>
arr_1t<typename cpx_promote<T1, T2>::type> operator*(const matrix_rcv_t<T1, X1>& lhs, const arr_1t<T2, X2>& rhs)
{

	if (lhs.get_cols() != rhs.size())
	{
		Exception::XError("matrix.cols!=vector.rows");
	}

	typedef typename cpx_promote<T1, T2>::type value_type;
	arr_1t<value_type> result;

	result.resize(lhs.get_rows());
	matops::mult_mat_Mv(lhs, rhs.data(), result.data());
	
	return EW_MOVE(result);

}



template<typename T1, typename X1, typename T2, typename X2>
arr_1t<typename cpx_promote<T1, T2>::type> operator*(const matrix_arr_t<T1, X1>& lhs, const arr_1t<T2, X2>& rhs)
{

	if (lhs.get_cols() != rhs.size())
	{
		Exception::XError("matrix.cols!=vector.rows");
	}

	typedef typename cpx_promote<T1, T2>::type value_type;

	size_t m = lhs.get_rows();
	size_t n = lhs.get_cols();

	arr_1t<value_type> result;
	result.resize(m);


	for(size_t i=0;i<m;i++)
	{
		value_type val(0);
		for (size_t j = 0; j < n; j++)
		{
			val += lhs(i, j) * rhs[j];
		}
		result[i] = val;
	}

	return EW_MOVE(result);

}


template<typename T2, typename X2>
arr_1t<T2> operator*(const pivot_matrix& lhs, const arr_1t<T2, X2>& rhs)
{

	if (lhs.get_cols() != rhs.size())
	{
		Exception::XError("matrix.cols!=vector.rows");
	}

	arr_1t<T2> result;
	result.resize(lhs.get_rows());

	matops::mult_mat_Pv(lhs, rhs.data(), result.data());

	return EW_MOVE(result);

}


class matrix_op_policy_add
{
public:
	template<typename M,typename T>
	static inline void append_lhs(M& result,const T& v)
	{
		result.value.push_back(v);
	}
	template<typename M,typename T>
	static inline void append_lhs_range(M& result,T it1,T it2)
	{
		while (it1 < it2) result.value.push_back(*it1++);
	}
	template<typename M,typename T>
	static inline void append_rhs(M& result,const T& val)
	{
		result.value.push_back(val);
	}
	template<typename M,typename T>
	static inline void append_rhs_range(M& result,T it1,T it2)
	{
		while (it1 < it2) result.value.push_back(*it1++);
	}
	template<typename M,typename T1,typename T2>
	static inline void append_val(M& result,const T1& v1,const T2& v2)
	{
		typedef typename M::elem_type elem_type;
		auto val = v1.value + v2.value;
		if (!M::helper::is_zero(val))
		{
			result.value.push_back(elem_type(v1.row, v1.col, val));
		}
	}
};

class matrix_op_policy_sub
{
public:
	template<typename M,typename T>
	static inline void append_lhs(M& result,const T& val)
	{
		result.value.push_back(val);
	}
	template<typename M,typename T>
	static inline void append_lhs_range(M& result,T it1,T it2)
	{
		while (it1 < it2) result.value.push_back(*it1++);
	}
	template<typename M,typename T>
	static inline void append_rhs(M& result,const T& val)
	{
		typedef typename M::elem_type elem_type;
		result.value.push_back(elem_type(val.row,val.col,-val.value));
	}
	template<typename M,typename T>
	static inline void append_rhs_range(M& result,T it1,T it2)
	{
		while (it1 < it2) append_rhs(result,*it1++);
	}
	template<typename M, typename T1, typename T2>
	static inline void append_val(M& result,const T1& v1,const T2& v2)
	{
		typedef typename M::elem_type elem_type;
		auto val = v1.value - v2.value;
		if (!M::helper::is_zero(val))
		{
			result.value.push_back(elem_type(v1.row,v1.col,val));
		}
	}
};


template<typename P>
class matrix_dot_op
{
public:

	template<typename M3,typename M1,typename M2>
	static void apply(M3& result, const M1& lhs, const M2& rhs)
	{
		matrix_helper_base::check_matrix_size_is_same(lhs, rhs);

		result.n_row = lhs.n_row;
		result.n_col = rhs.n_col;
		result.value.resize(0);

		auto it1 = lhs.value.begin();
		auto it2 = rhs.value.begin();

		auto it1_end = lhs.value.end();
		auto it2_end = rhs.value.end();

		while (1)
		{
			if (it1 == it1_end)
			{
				P::append_rhs_range(result, it2, it2_end);
				break;
			}
			else if (it2 == it2_end)
			{
				P::append_lhs_range(result, it1, it1_end);
				break;
			}

			if ((*it1).row < (*it2).row)
			{
				P::append_lhs(result, *it1++);
			}
			else if ((*it1).row > (*it2).row)
			{
				P::append_rhs(result, *it2++);
			}
			else if ((*it1).col < (*it2).col)
			{
				P::append_lhs(result, *it1++);
			}
			else if ((*it1).col > (*it2).col)
			{
				P::append_rhs(result, *it2++);
			}
			else
			{
				P::append_val(result, *it1++, *it2++);
			}
		}
		M3::helper::adjust_r_pos(result.value, result.r_pos, result.n_row);
	}
};



template<typename T1, typename X1, typename T2, typename X2>
matrix_rcv_t<typename cpx_promote<T1, T2>::type> operator+(const matrix_rcv_t<T1, X1>& lhs, const matrix_rcv_t<T2, X2>& rhs)
{
	typedef typename cpx_promote<T1, T2>::type value_type;
	typedef matrix_rcv_t<value_type> result_type;
	matrix_rcv_t<value_type> result;
	matrix_dot_op<matrix_op_policy_add>::apply(result, lhs, rhs);
	return EW_MOVE(result);
}


template<typename T1, typename X1, typename T2, typename X2>
matrix_rcv_t<typename cpx_promote<T1, T2>::type> operator-(const matrix_rcv_t<T1, X1>& lhs, const matrix_rcv_t<T2, X2>& rhs)
{
	typedef typename cpx_promote<T1, T2>::type value_type;
	typedef matrix_rcv_t<value_type> result_type;
	matrix_rcv_t<value_type> result;
	matrix_dot_op<matrix_op_policy_sub>::apply(result, lhs, rhs);
	return EW_MOVE(result);

}

template<typename T, typename X>
matrix_rcv_t<T, X> operator*(const matrix_rcv_t<T, X>& lhs, const pivot_matrix& rhs)
{
	matrix_rcv_t<T, X> tmp(lhs);
	tmp.mult_r(rhs);
	return EW_MOVE(tmp);
}

template<typename T, typename X>
matrix_rcv_t<T, X> operator*(const pivot_matrix& lhs, const matrix_rcv_t<T, X>& rhs)
{
	matrix_rcv_t<T, X> tmp(rhs);
	tmp.mult_l(lhs);
	return EW_MOVE(tmp);
}


EW_LEAVE

#endif

