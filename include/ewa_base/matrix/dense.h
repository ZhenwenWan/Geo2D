#ifndef __EWA_MATRIX_DENSE_H__
#define __EWA_MATRIX_DENSE_H__

#include "ewa_base/math/math_op.h"
#include "ewa_base/util/radix_sort.h"

EW_ENTER


template<typename T,typename A = def_allocator>
class matrix_arr_t : public arr_xt<T,A>
{
public:

	size_t get_rows() const { return this->size(0); }
	size_t get_cols() const { return this->size(1); }
	bool is_square() const { return this->size(0) == this->size(1); }

	void transpose()
	{
		size_t rows = this->size(0);
		size_t cols = this->size(1);
		arr_xt<T> tmp; tmp.resize(cols, rows);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				tmp(i, j) = (*this)(j, i);
			}
		}
		this->swap(tmp);
	}
};

EW_LEAVE

#endif
