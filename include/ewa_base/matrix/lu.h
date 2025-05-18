#ifndef __EWA_MATRIX_LU_H__
#define __EWA_MATRIX_LU_H__


#include "ewa_base/matrix/matrix_op.h"

EW_ENTER


template<typename T>
class fixed_size_temp_allocator : public ObjectData
{
public:

	static const size_t N = 1024;

	T* copy(const T& e)
	{
		T* p = allocate();
		*p = e;
		return p;
	}

	T* allocate()
	{
		if (_p_beg >= _p_end)
		{
			T* p = new T[N];
			_v_mems.push_back(p);
			_p_beg = p;
			_p_end = p + N;
		}
		return _p_beg++;
	}

	fixed_size_temp_allocator()
	{
		_p_beg = _p_end = NULL;
	}

	~fixed_size_temp_allocator()
	{
		for (size_t i = 0; i < _v_mems.size(); i++)
		{
			delete[] _v_mems[i];
		}
	}

private:
	arr_1t<T*> _v_mems;
	T* _p_beg;
	T* _p_end;
};

template<typename T,typename X>
class context_rct_t
{
public:
	typedef X helper;
	typedef typename X::elem_type elem_type;
	typedef typename X::size_type size_type;

	DataPtrT<fixed_size_temp_allocator<elem_type> > _p_alloc;

	context_rct_t()
	{
		_p_alloc.reset(new fixed_size_temp_allocator<elem_type>);
	}

	T* allocate()
	{
		return _p_alloc->allocate();
	}

	class PtrArray : public arr_1t<elem_type*>
	{
	public:
		template<typename Y>
		void mult(const Y& val)
		{
			for (auto it = this->begin(); it != this->end(); ++it)
			{
				(**it).value *= val;
			}
		}

	};

	class RowType : public PtrArray
	{
	public:

		template<typename P>
		void row_add(P& ctx, RowType& rhs, const T& kk,size_type rr)
		{
			RowType& lhs(*this);

			auto a1 = lhs.begin();
			auto a2 = lhs.end();
			auto b1 = rhs.begin();
			auto b2 = rhs.end();

			RowType result;

			while (1)
			{
				if (a1 == a2)
				{
					for (auto it = b1; it != b2; ++it)
					{
						elem_type* p = ctx.allocate();

						p->row = rr;
						p->col = (*it)->col;
						p->value = kk*(*it)->value;
						result.append(p);
					}
					break;
				}
				if (b1 == b2)
				{
					for (auto it = a1; it != a2; ++it)
					{
						elem_type* p = *it;
						result.append(p);
					}
					break;
				}

				size_t c1 = (*a1)->col;
				size_t c2 = (*b1)->col;
				if (c1 < c2)
				{
					result.append(*a1++);
				}
				else if (c1 > c2)
				{
					elem_type* p = ctx.allocate();
					p->row = rr;
					p->col = (*b1)->col;
					p->value = kk*(*b1)->value;
					result.append(p);

					b1++;
				}
				else
				{

					(*a1)->value += kk*(*b1)->value;
					if ((*a1)->value != T(0))
					{
						result.append(*a1);
					}

					a1++;
					b1++;
				}

			}
			lhs.swap(result);
		}

	};

	class ColType : public PtrArray
	{
	public:

	};

	class RowMap
	{
	public:

		arr_1t<uint32_t> idx2row;
		arr_1t<uint32_t> row2idx;

		size_t min_idx;
		size_t min_row;
		size_t num;

		uint32_t operator[](uint32_t row) const
		{
			return row2idx[row];
		}

		void resize(size_t n)
		{
			idx2row.resize(n, -1);
			row2idx.resize(n, -1);
			min_idx = min_row = 0;
			num = 0;
		}

		void set(uint32_t idx, uint32_t row)
		{
			if (row == -1)
			{
				while (row2idx[min_row] != -1) min_row++;
				row = min_row;
			}
			idx2row[idx] = row;
			row2idx[row] = idx;
			num++;
		}

		void update()
		{
			while (num < idx2row.size())
			{
				while (idx2row[min_idx] != -1) min_idx++;
				set(min_idx, -1);
			}
		}

	};



};

template<typename T,typename X>
class lu_context_rct_t : public context_rct_t<T,X>
{
public:
	typedef context_rct_t<T,X> basetype;
	typedef typename basetype::RowType RowType;
	typedef typename basetype::ColType ColType;
	typedef typename basetype::RowMap RowMap;
	typedef typename basetype::elem_type elem_type;

    using basetype::_p_alloc;
    typedef typename basetype::helper helper;

	arr_1t<RowType> rows;
	arr_1t<ColType> cols;
	RowMap rmap;

	const matrix_rcv_t<T, X>& A;
	matrix_rcv_t<T, X>& L;
	matrix_rcv_t<T, X>& U;

	lu_context_rct_t(
		const matrix_rcv_t<T, X>& A_,
		matrix_rcv_t<T, X>& L_,
		matrix_rcv_t<T, X>& U_
	):A(A_),L(L_),U(U_)
	{

	}

	RowType result;

	void adjust_row(size_t rr, RowType& rhs, const T& ratio,size_t cc)
	{
		RowType& lhs(rows[rr]);

		auto a1 = lhs.begin();
		auto a2 = lhs.end();

		auto b1 = rhs.begin();
		auto b2 = rhs.end();

		result.clear();

		while (1)
		{
			if (b1 == b2)
			{
				for (auto it = a1; it != a2; ++it)
				{
					result.append(*it);
				}
				break;
			}

			if (a1 == a2)
			{
				for (auto it = b1; it != b2; ++it)
				{

					uint32_t c2 = (*it)->col;
					if (c2 == cc) continue;

					elem_type* p = _p_alloc->allocate();
					p->row = rr;
					p->col = c2;
					p->value = ratio*(*it)->value;
					result.append(p);

					cols[c2].append(p);

				}
				break;
			}


			size_t c1 = (*a1)->col;
			size_t c2 = (*b1)->col;
			if (c1 < c2)
			{
				result.append(*a1++);
			}
			else if (c1 > c2)
			{
				if (c2!=cc)
				{
					elem_type* p = _p_alloc->allocate();
					p->row = rr;
					p->col = c2;
					p->value = ratio*(*b1)->value;
					result.append(p);

					cols[c2].append(p);
				}

				b1++;
			}
			else
			{
				if (c2!=cc)
				{
					(*a1)->value += ratio*(*b1)->value;
					if ((*a1)->value != T(0))
					{
						result.append(*a1);
					}
				}

				a1++;
				b1++;
			}

		}

		lhs.swap(result);

	}

	void handle_col(size_t k)
	{
		ColType& col(cols[k]);

		auto it1 = col.begin();
		auto it2 = col.end();
		auto itp = it2;

		double v_max = 0.0;
		double v;

		while (it1 != it2)
		{
			elem_type *p = *it1;
			if (rmap[p->row] != -1 || (v = pl_abs::g(p->value))==0.0)
			{
				std::swap(*it1, *--it2);
				continue;
			}
			if (v > v_max)
			{
				v_max = v;
				itp = it1;
			}
			it1++;
		}



		if (itp>=it2)
		{
			rmap.set(k, -1);
		}
		else
		{

			std::swap(*itp, *--it2);
			rmap.set(k, (*it2)->row);

			RowType& row(rows[(*it2)->row]);

			for(auto it=col.begin();it!=it2;++it)
			{
				elem_type *p = *it;
				p->value /= (*it2)->value;
				adjust_row(p->row, row, -p->value, k);
				it1++;
			}
		}

		col.resize(it2 - col.begin());


	}

	void apply_real_col()
	{

		size_t m = A.get_rows();
		size_t n = A.get_cols();

		rows.resize(m);
		cols.resize(n);

		rmap.resize(m);

		for (auto it = A.value.begin(); it != A.value.end(); ++it)
		{
			elem_type* p = _p_alloc->copy(*it);
			rows[p->row].append(p);
			cols[p->col].append(p);
		}

		size_t d = std::min(m, n);


		for (size_t k = 0; k < d; k++)
		{
			handle_col(k);
		}


		rmap.update();


		if (m <= n)
		{
			L.reset(m, m);
			U.reset(m, n);
			L.value.clear();
			U.value.clear();

			for (size_t i = 0; i < d; i++)
			{

				for (auto it = cols[i].begin(); it != cols[i].end(); ++it)
				{
					elem_type item(**it);
					item.row =rmap[item.row];
					if (item.row <= item.col)
					{
						continue;
					}
					L.value.push_back(item);
				}
			}

			for (size_t i = 0; i < m; i++)
			{
				L.value.push_back(elem_type(i, i, T(1)));
			}

			for (size_t i = 0; i < m; i++)
			{
				for (auto it = rows[i].begin(); it != rows[i].end(); ++it)
				{
					elem_type item(**it);
					item.row = rmap[item.row];
					U.value.push_back(item);
				}
			}
		}
		else // n<m
		{

			L.reset(m, n);
			U.reset(n, n);
			L.value.clear();
			U.value.clear();

			for (size_t i = 0; i < d; i++)
			{
				for (auto it = cols[i].begin(); it != cols[i].end(); ++it)
				{
					elem_type item(**it);
					item.row = rmap[item.row];
					L.value.push_back(item);
				}
			}

			for (size_t i = 0; i < n; i++)
			{
				L.value.push_back(elem_type(i, i, T(1)));
			}

			for (size_t i = 0; i < m; i++)
			{
				if (rmap[i] >= n)
				{
					continue;
				}
				for (auto it = rows[i].begin(); it != rows[i].end(); ++it)
				{
					elem_type item(**it);
					item.row = rmap[item.row];
					U.value.push_back(item);
				}
			}

		}

		helper::adjust_value(U.value, U.r_pos, U.get_rows());
		helper::adjust_value(L.value, L.r_pos, L.get_rows());

	}



	bool apply(pivot_matrix& P)
	{
		apply_real_col();
		P.value.swap(rmap.idx2row);
		return true;

	}


};


template<typename T, typename X>
bool rcv_lu(const matrix_rcv_t<T, X>& A, matrix_rcv_t<T, X>& L, matrix_rcv_t<T, X>& U, pivot_matrix& P)
{
	lu_context_rct_t<T, X> lc(A, L, U);
	return lc.apply(P);
}


EW_LEAVE
#endif
