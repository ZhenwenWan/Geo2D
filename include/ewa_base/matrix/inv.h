#ifndef __EWA_MATRIX_INV_H__
#define __EWA_MATRIX_INV_H__


#include "ewa_base/matrix/lu.h"

EW_ENTER

template<typename T, typename X>
class inv_context_rcv : public context_rct_t<T,X>
{
public:

	arr_1t<RowType> rows;
	arr_1t<ColType> cols;

	matops::matchekcer ns_compare;


	void append(elem_type* p)
	{
		rows[p->row].append(p);
		cols[p->col].append(p);
	}

	void append(elem_type& p)
	{
		rows[p.row].append(&p);
		cols[p.col].append(&p);
	}

	template<typename IT>
	void append(IT p)
	{
		append(*p);
	}

	template<typename IT>
	void append(IT p1, IT p2)
	{
		for (auto it = p1; it != p2; ++it)
		{
			append(it);
		}
	}



	void inv_L_matrix(arr_1t<elem_type>& data,size_t n)
	{

		rows.clear();
		cols.clear();

		rows.resize(n);
		cols.resize(n);

		append(data.begin(),data.end());

		for (size_t i = 0; i < n; i++)
		{
			elem_type* p = _p_alloc->allocate();
			p->row = i;
			p->col = n + i;
			p->value = T(1);
			rows[i].append(p);	
		}

	
		for (size_t i = 0; i < n; i++)
		{
			auto it1 = cols[i].begin();
			auto it2 = cols[i].end();

			if (it1 == it2)
			{
				Exception::XError("inv matrix error");
			}

			T ukk = (*it1)->value;
			if (ukk != T(1))
			{
				if (pl_abs::g(ukk) < 1.0e-9)
				{
					Exception::XError("ukk is too small");
				}
				rows[i].mult(1.0 / ukk);
			}

			for (auto it = it1+1; it != it2; ++it)
			{
				elem_type* p = *it;
				rows[p->row].row_add(*_p_alloc, rows[i], -p->value, p->row);
			}

		}

		arr_1t<elem_type> tmp;
		for (size_t i = 0; i < n; i++)
		{
			for (auto it = rows[i].begin(); it != rows[i].end(); ++it)
			{
				auto p = *it;
				if (p->col < n)
				{
					continue;
				}

				p->row = i;
				p->col -= n;
				tmp.append(**it);
			}
		}

		data.swap(tmp);

		rows.clear();
		cols.clear();
	}

	void inv_L_matrix(matrix_rcv_t<T, X>& L)
	{
		inv_L_matrix(L.value, L.get_rows());
		helper::adjust_value(L.value, L.r_pos, L.get_rows());

	}

	void inv_U_matrix(matrix_rcv_t<T, X>& U)
	{
		for (auto it = U.value.begin(); it != U.value.end(); ++it)
		{
			std::swap((*it).row, (*it).col);
		}
		inv_L_matrix(U.value, U.get_rows());
		for (auto it = U.value.begin(); it != U.value.end(); ++it)
		{
			std::swap((*it).row, (*it).col);
		}
		helper::adjust_value(U.value, U.r_pos, U.get_rows());
	}

	bool apply(const matrix_rcv_t<T, X>& A, matrix_rcv_t<T, X>& B)
	{

		typedef matrix_rcv_t<T, X> tiny_matrix;

		if (!A.is_square())
		{
			return false;
		}

		size_t n = A.get_rows();

		tiny_matrix II= tiny_matrix::identity(n);

		tiny_matrix L, U;
		pivot_matrix P;

		if (!rcv_lu(A, L, U, P))
		{
			return false;
		}
		
		inv_L_matrix(L);
		inv_U_matrix(U);

		B = U*L*P;

		return true;
	}
};

template<typename T, typename X>
bool rcv_inv(const matrix_rcv_t<T, X>& A, matrix_rcv_t<T, X>& B)
{
	inv_context_rcv<T, X> ic;
	return ic.apply(A, B);

}


EW_LEAVE

#endif

