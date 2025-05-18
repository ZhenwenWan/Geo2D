#ifndef __EWA_MATRIX_HELPER_H__
#define __EWA_MATRIX_HELPER_H__

#include "ewa_base/math/math_op.h"
#include "ewa_base/util/radix_sort.h"

EW_ENTER


template<typename T>
class abstract_matrixt_t
{
public:
	typedef uint32_t size_type;
	typedef T value_type;

	size_type get_rows() const;
	size_type get_cols() const;
	bool is_square() const;
	const T& operator()(size_type i, size_type j) const;

};

class matrix_helper_base
{
public:

	template<typename M1, typename M2>
	static void check_matrix_size_for_mult(const M1& lhs, const M2& rhs)
	{
		if (lhs.get_cols() != rhs.get_rows())
		{
			Exception::XError("invalid matrix size");
		}
	}

	template<typename M1, typename M2>
	static void check_matrix_size_is_same(const M1& lhs, const M2& rhs)
	{
		if (lhs.get_rows() != rhs.get_rows() || lhs.get_cols() != rhs.get_cols())
		{
			Exception::XError("invalid matrix size");
		}
	}

	template<typename M1>
	static void check_matrix_size_is_square(const M1& mat)
	{
		if (mat.is_square())
		{
			Exception::XError("matrix is not square");
		}
	}
};




template<typename T, typename X = uint32_t>
class element_rcv_t
{
public:

	typedef X size_type;

	element_rcv_t() {}
	element_rcv_t(size_type i, size_type j, const T& v) :row(i), col(j), value(v) {}

	template<typename T2, typename X2>
	element_rcv_t(element_rcv_t<T2,X2>&& v) : row(v.row), col(v.col), value(std::forward<T2>(v.value)) {}

	template<typename T2, typename X2>
	element_rcv_t& operator=(element_rcv_t<T2, X2>&& rhs)
	{
		row = rhs.row;
		col = rhs.col;
		value = std::forward<T2>(rhs.value);
		return *this;
	}


	size_type row, col;
	T value;
};

template<typename T, typename X>
bool operator!=(const element_rcv_t<T, X>& lhs, const element_rcv_t<T, X>& rhs)
{
	return lhs.row != rhs.row || lhs.col != rhs.col || lhs.value != rhs.value;
}

template<typename T, typename X>
bool operator==(const element_rcv_t<T, X>& lhs, const element_rcv_t<T, X>& rhs)
{
	return lhs.row == rhs.row && lhs.col == rhs.col && lhs.value == rhs.value;
}



template<typename T, typename X>
class element_rcv_helper_t : public numeric_helper_t<T>
{
public:
	typedef element_rcv_t<T, X> elem_type;
	typedef X size_type;


	static bool less_elem(const elem_type& lhs, const elem_type& rhs)
	{
		if (lhs.row < rhs.row) return true;
		if (lhs.row > rhs.row) return false;
		return lhs.col < rhs.col;
	}

	static bool less_elem_row(const elem_type& lhs, const elem_type& rhs)
	{
		return lhs.row < rhs.row;
	}

	static bool less_elem_col(const elem_type& lhs, const elem_type& rhs)
	{
		return lhs.col < rhs.col;
	}

	static size_type get_row(const elem_type& item)
	{
		return item.row;
	}

	static size_type get_col(const elem_type& item)
	{
		return item.col;
	}

	static bool less_col(const elem_type& lhs, size_type col)
	{
		return lhs.col < col;
	}

	static void adjust_r_pos(arr_1t<elem_type>& value, arr_1t<size_type>& r_pos, size_type rows)
	{
		r_pos.resize(rows + 1);

		if (value.empty())
		{
			for (size_t i = 0; i <= rows; i++)
			{
				r_pos[i] = 0;
			}
			return;
		}


		size_t nd = 0;
		size_t sz = value.size();

		size_t row = value[nd].row;

		for (size_t r1 = 0; r1 < rows; )
		{
			while (r1 >= row)
			{
				if (++nd < sz)
				{
					row = value[nd].row;
				}
				else
				{
					row = rows;
					break;
				}
			}

			r_pos[++r1] = nd;
		}

	}

	static void adjust_value(arr_1t<elem_type>& value, arr_1t<size_type>& r_pos, size_type rows)
	{
		std::stable_sort(value.begin(), value.end(), &less_elem);
		adjust_r_pos(value, r_pos, rows);
	}

};


class pivot_matrix
{
public:

	pivot_matrix() {}
	pivot_matrix(size_t n) { reset(n); }
	pivot_matrix(pivot_matrix&& o) { swap(o); }
	pivot_matrix(const pivot_matrix& o) : value(o.value) {}

	typedef uint32_t size_type;
	arr_1t<size_type> value;

	size_t get_rows() const { return value.size(); }
	size_t get_cols() const { return value.size(); }
	bool is_square() const { return true; }

	double operator()(size_type i, size_type j) const
	{
		EW_ASSERT(i < value.size() && j < value.size());
		return value[i] == j ? 1.0 : 0.0;
	}

	void reset(size_t n)
	{
		value.resize(n);
		for (size_t i = 0; i < n; i++) value[i] = i;
	}

	void swap(pivot_matrix& o)
	{
		value.swap(o.value);
	}

	void transpose()
	{
		arr_1t<size_type> tmp;
		transpose(value, tmp);
		value.swap(tmp);
	}

	static void transpose(const arr_1t<size_type>& value, arr_1t<size_type>& result)
	{
		size_t n = value.size();
		result.resize(n);
		for (size_t i = 0; i < n; i++)
		{
			result[value[i]] = i;
		}
	}
};


inline pivot_matrix operator*(const pivot_matrix& lhs, const pivot_matrix& rhs)
{
	matrix_helper_base::check_matrix_size_for_mult(lhs, rhs);
	size_t n = lhs.value.size();

	pivot_matrix tmp;
	tmp.value.resize(n);

	for (size_t i = 0; i < n; i++)
	{
		tmp.value[i] = rhs.value[lhs.value[i]];
	}

	return EW_MOVE(tmp);
}


EW_LEAVE

#endif
