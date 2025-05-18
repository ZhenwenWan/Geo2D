#ifndef __EWA_MATRIX_SPARSE_H__
#define __EWA_MATRIX_SPARSE_H__

#
#include "ewa_base/matrix/helper.h"


EW_ENTER



template<typename T,typename X = element_rcv_helper_t<T,uint32_t> >
class matrix_rcv_t
{
public:

	typedef X helper;
	typedef typename X::elem_type elem_type;
	typedef typename X::size_type size_type;


	arr_1t<elem_type> value;
	arr_1t<size_type> r_pos;
	size_type n_row;
	size_type n_col;

	size_type get_rows() const { return n_row; }
	size_type get_cols() const { return n_col; }

	bool is_square() const { return n_row==n_col; }

	matrix_rcv_t();
	matrix_rcv_t(const matrix_rcv_t& v);
	matrix_rcv_t(matrix_rcv_t&& v);

	matrix_rcv_t& operator=(const matrix_rcv_t& v);
	matrix_rcv_t& operator=(matrix_rcv_t&& v);

	template<typename T2,typename A2>
	matrix_rcv_t(const arr_xt<T2,A2>& v);

	void swap(matrix_rcv_t& v);

	const T& operator()(size_type i, size_type j) const;

	void transpose();

	template<typename T2>
	static matrix_rcv_t diag(const T2* p, size_t n);

	static matrix_rcv_t identity(size_type n);

	void zero_non_main_sub_matrix();

	void reset(size_type r, size_type c);


	matrix_rcv_t& mult_r(const pivot_matrix& rhs);

	matrix_rcv_t& mult_l(const pivot_matrix& lhs);

	template<typename T2,typename A2>
	void write(arr_xt<T2,A2>& val);
};



template<typename T,typename X>
matrix_rcv_t<T,X>::matrix_rcv_t()
{
	n_row = n_col = 0;
}

template<typename T,typename X>
matrix_rcv_t<T,X>::matrix_rcv_t(const matrix_rcv_t& v) :value(v.value), r_pos(v.r_pos), n_row(v.n_row), n_col(v.n_col)
{

}

template<typename T,typename X>
matrix_rcv_t<T,X>& matrix_rcv_t<T,X>::operator=(const matrix_rcv_t& v)
{
	value = v.value;
	r_pos = v.r_pos;
	n_row = v.n_row;
	n_col = v.n_col;
	return *this;
}

template<typename T,typename X>
matrix_rcv_t<T,X>::matrix_rcv_t(matrix_rcv_t&& v)
{
	swap(v);
}
template<typename T,typename X>
matrix_rcv_t<T,X>& matrix_rcv_t<T,X>::operator=(matrix_rcv_t&& v)
{
	swap(v);
	return *this;
}

template<typename T,typename X>
template<typename T2,typename A2>
matrix_rcv_t<T,X>::matrix_rcv_t(const arr_xt<T2,A2>& v)
{
	reset(v.size(0), v.size(1));

	for (size_t i = 0; i < n_row; i++)
	{
		for (size_t j = 0; j < n_col; j++)
		{
			T val(v(i, j));
			if (!helper::is_zero(val))
			{
				value.push_back(elem_type(i, j, val));
			}
		}
		r_pos[i + 1] = value.size();
	}
}

template<typename T,typename X>
void matrix_rcv_t<T,X>::reset(size_type r, size_type c)
{
	n_row = r;
	n_col = c;
	r_pos.resize(r + 1);
}


template<typename T,typename X>
void matrix_rcv_t<T,X>::swap(matrix_rcv_t& v)
{
	value.swap(v.value);
	r_pos.swap(v.r_pos);
	std::swap(n_row, v.n_row);
	std::swap(n_col, v.n_col);
}

template<typename T,typename X>
const T& matrix_rcv_t<T,X>::operator()(size_type i, size_type j) const
{
	EW_ASSERT(i < n_row && j < n_col);

	const elem_type* p1 = value.data() + r_pos[i];
	const elem_type* p2 = value.data() + r_pos[i + 1];

	const elem_type* p = std::lower_bound(p1, p2, j, helper::less_col);
	if (p < p2 && p->col == j)
	{
		return p->value;
	}

	return helper::sm_value_0;
}

template<typename T,typename X>
void matrix_rcv_t<T,X>::transpose()
{
	std::swap(n_row, n_col);
	for (size_t i = 0; i < value.size(); i++)
	{
		std::swap(value[i].row,value[i].col);
	}
	radix_sort(value.begin(), value.end(), &helper::get_row);
	helper::adjust_r_pos(value, r_pos, n_row);
}

template<typename T, typename X>
matrix_rcv_t<T, X>& matrix_rcv_t<T, X>::mult_r(const pivot_matrix& rhs)
{
	auto& lhs(*this);

	matrix_helper_base::check_matrix_size_for_mult(lhs, rhs);

	size_t rows = lhs.get_rows();
	size_t cols = rhs.get_cols();

	const arr_1t<uint32_t>& pivot(rhs.value);

	for (size_t i = 0; i < value.size(); i++)
	{
		auto& item(value[i]);
		item.col = pivot[item.col];
	}

	auto pd = value.data();

	for (size_t i = 0; i < rows; i++)
	{
		std::sort(pd + r_pos[i], pd + r_pos[i + 1], helper::less_elem_col);
	}

	return *this;
}
template<typename T, typename X>
matrix_rcv_t<T, X>& matrix_rcv_t<T, X>::mult_l(const pivot_matrix& lhs)
{
	auto& rhs(*this);

	matrix_helper_base::check_matrix_size_for_mult(lhs, rhs);

	size_t rows = lhs.get_rows();
	size_t cols = rhs.get_cols();

	const arr_1t<uint32_t>& pivot(lhs.value);

	arr_1t<elem_type> tmp;
	arr_1t<size_type> pos;
	pos.resize(rows + 1);

	auto pd = value.data();

	for (size_t k = 0; k < rows; k++)
	{
		size_t r = pivot[k];
		auto p1 = pd + r_pos[r];
		auto p2 = pd + r_pos[r + 1];
		for (auto p = p1; p < p2; p++)
		{
			tmp.push_back(elem_type(k, (*p).col, (*p).value));
		}
		pos[k + 1] = tmp.size();
	}

	value.swap(tmp);
	r_pos.swap(pos);

	return *this;
}
template<typename T, typename X>
template<typename T2, typename A2>
void matrix_rcv_t<T, X>::write(arr_xt<T2, A2>& val)
{
	arr_xt<T2, A2> arr;
	arr.resize(n_row, n_col);
	for (size_t i = 0; i < value.size(); i++)
	{
		auto& item(value[i]);
		arr(item.row, item.col) = item.value;
	}
	val.swap(arr);
}


template<typename T,typename X>
template<typename T2>
matrix_rcv_t<T,X> matrix_rcv_t<T,X>::diag(const T2* p, size_t n)
{
	matrix_rcv_t<T,X> result;

	result.reset(n, n);
	result.value.reserve(n);

	for (size_t i = 0; i < n; i++)
	{
		T val(p[i]);
		if (!helper::is_zero(val))
		{
			result.value.push_back(elem_type(i, i, val));
		}
		result.r_pos[i + 1] = result.value.size();
	}

	return EW_MOVE(result);
}

template<typename T,typename X>
matrix_rcv_t<T,X> matrix_rcv_t<T,X>::identity(size_type n)
{
	matrix_rcv_t<T,X> result;
	result.reset(n, n);
	result.value.resize(n);

	for (size_type i = 0; i < n; i++)
	{
		result.value[i] = elem_type(i, i, T(1));
		result.r_pos[i + 1] = i + 1;
	}

	return EW_MOVE(result);
}

template<typename T,typename X>
void matrix_rcv_t<T,X>::zero_non_main_sub_matrix()
{

	matrix_helper_base::check_matrix_size_is_square(*this);

	arr_1t<int> dmn;
	dmn.resize(n_row);

	int id = 0;

	arr_1t<int> tmp;
	tmp.reserve(n_row);

	for (int i = 0; i < n_row; i++)
	{
		if (dmn[i] != 0) continue;
		if (r_pos[i + 1] - r_pos[i] == 0) continue;

		tmp.push_back(i);

		id++;

		for (size_t j = tmp.size() - 1; j<tmp.size(); j++)
		{

			int row = tmp[j];
			if (dmn[row] != 0) continue;

			dmn[row] = id;
			auto p1 = value.data() + r_pos[row];
			auto p2 = value.data() + r_pos[row + 1];

			for (auto p = p1; p != p2; ++p)
			{
				int col = (*p).col;
				if (dmn[col] != 0) continue;
				tmp.push_back(col);
			}
		}

	}


	if (id <= 1) return;

	arr_1t<int> cnt; cnt.resize(id + 1);

	for (size_t i = 0; i < n_row; i++)
	{
		cnt[dmn[i]]++;
	}

	int max_cnt = 0;
	int max_idx = 0;

	for (size_t i = 1; i <= id; i++)
	{
		if (cnt[i] > max_cnt)
		{
			max_idx = i;
			max_cnt = cnt[i];
		}
	}

	size_t j = 0;

	for (size_t i = 0; i < value.size(); i++)
	{
		if (dmn[value[i].row] == max_idx)
		{
			value[j++] = value[i];
		}
	}

	value.resize(j);
	helper::adjust_r_pos(value, r_pos, n_row);

}



EW_LEAVE

#endif
