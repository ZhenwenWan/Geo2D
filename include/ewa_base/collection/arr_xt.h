#ifndef __H_EW_COLLECTION_ARR_XT__
#define __H_EW_COLLECTION_ARR_XT__


#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/math/tiny_vec.h"
#include "ewa_base/math/tiny_mat.h"

#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/string.h"

EW_ENTER

template<typename T,typename A=def_allocator>
class arr_xt : public containerB< arr_container<T,typename A::template rebind<T>::other,tl::is_pod<T>::value> >
{
protected:
	typedef containerB< arr_container<T,typename A::template rebind<T>::other,tl::is_pod<T>::value> > basetype;

public:
	static const size_t MAX_DIM=arr_xt_dims::MAX_DIM;

	typedef typename basetype::size_type size_type;
	typedef typename basetype::value_type value_type;
	typedef typename basetype::pointer pointer;
	typedef typename basetype::const_pointer const_pointer;
	typedef typename basetype::reference reference;
	typedef typename basetype::const_reference const_reference;
	typedef typename basetype::iterator iterator;
	typedef typename basetype::const_iterator const_iterator;
	typedef typename basetype::reverse_iterator reverse_iterator;
	typedef typename basetype::const_reverse_iterator const_reverse_iterator;

	inline arr_xt(){}
	inline arr_xt(const arr_xt& o):basetype(o),dims(o.dims){}

	template<typename T1, typename A1>
	inline arr_xt(const arr_1t<T1,A1>& o)
	{
		(*this) = o;
	}

	template<typename T1, typename A1>
	inline arr_xt(const arr_xt<T1,A1>& o)
	{
		(*this) = o;
	}

	template<typename T1, int R, int C>
	inline arr_xt(const tiny_mat<T1, R, C>& m)
	{
		this->resize(R, C);

		for (int j = 0; j < C; ++j) for (int i = 0; i < R; ++i)
		{
			(*this)(i, j) = m(i, j);
		}
	}

	inline explicit arr_xt(const A& al):basetype(al){}

	inline explicit arr_xt(size_type k0, size_type k1 = 1, size_type k2 = 1, size_type k3 = 1, size_type k4 = 1, size_type k5 = 1)
	{
		resize(k0, k1, k2, k3, k4, k5);
	}

	inline explicit arr_xt(const arr_xt_dims& d,size_t n=0)
	{
		resize(d,n);
	}


	template<typename T1,typename A1>
	inline arr_xt& operator=(const arr_1t<T1,A1>& o)
	{
		impl.assign(o.begin(), o.end());
		dims.resize(impl.size());
		return *this;
	}

	template<typename T1,typename A1>
	inline arr_xt& operator=(const arr_xt<T1,A1>& o)
	{
		impl.assign(o.begin(), o.end());
		dims = o.get_dims();
		return *this;
	}

	template<typename T1, int R, int C>
	inline arr_xt& operator= (const tiny_mat<T1, R, C>& m)
	{
		arr_xt<T>(m).swap(*this);
		return (*this);
	}

	inline void swap(arr_xt& o)
	{
		static_cast<basetype&>(o).swap(impl);
		std::swap(dims,o.dims);
	}

	inline arr_xt& operator=(const arr_xt& o)
	{
		impl = o.impl;
		dims = o.dims;
		return *this;
	}

#if defined(EW_C11)

	inline arr_xt(arr_xt&& o)
	{
		static_cast<basetype&>(o).swap(impl);
		dims = o.dims;
	}

	inline arr_xt(arr_1t<T,A>&& o)
	{
		static_cast<basetype&>(o).swap(impl);
		dims.resize(impl.size());
	}

	inline arr_xt& operator=(arr_xt&& o)
	{
		static_cast<basetype&>(o).swap(impl);
		dims = o.dims;
		return *this;
	}

	inline arr_xt& operator=(arr_1t<T, A>&& o)
	{
		static_cast<basetype&>(o).swap(impl);
		dims.resize(impl.size());
		return *this;
	}

#endif

	void assign(size_type count_,const T& val_)
	{
		impl.assign(count_,val_);
		dims.resize(count_);
	}

	template<typename It>
	void assign(It first_,size_type count_)
	{
		impl.assign(first_,count_);
		dims.resize(count_);
	}

	template<typename It>
	void assign(It first_,It last_)
	{
		impl.assign(first_,last_);
		dims.resize(impl.size());
	}

	inline iterator append(const T& val_)
	{
		return impl.append(val_);
	}

	template<typename It>
	void append(It first_,size_type count_)
	{
		impl.append(first_,count_);
	}

	template<typename It>
	void append(It first_,It last_)
	{
		impl.append(first_,last_);
	}

	inline void push_back(const T& val_)
	{
		impl.append(val_);
	}

	inline void reshape_to_col_vector()
	{
		size_t d=impl.size();
		dims.resize(d,1);
	}
	inline void reshape_to_row_vector()
	{
		size_t d=impl.size();
		dims.resize(1,d);
	}

	inline void pop_back_and_reshape_to_row_vector(size_t n)
	{
		if(impl.size()<n) Exception::XError(_hT("cannot_pop_from_empty_container"));
		size_t d=impl.size()-n;
		impl.resize(d);
		dims.resize(1,d);
	}

	inline void pop_back_unsafe(){impl.pop_back();}
	inline void pop_back_unsafe(reference val){ val = EW_MOVE(impl.back()); impl.pop_back(); }

	void reshape(size_type k0,size_type k1=1,size_type k2=1,size_type k3=1,size_type k4=1,size_type k5=1);
	void reshape(const arr_xt_dims& kn);

	void resize(size_type k0,size_type k1=1,size_type k2=1,size_type k3=1,size_type k4=1,size_type k5=1);
	void resize(const arr_xt_dims& kn,size_t rs=0);

	template<typename X,int N>
	void resize(const tiny_vec<X, N>& sz);


	void clear()
	{
		impl.clear();
		dims.resize(0);
	}

	inline T& operator()(const tiny_vec<int, 3>& sz)
	{
		return impl[dims(sz[0], sz[1], sz[2])];
	}
	inline const T& operator()(const tiny_vec<int, 3>& sz) const
	{
		return impl[dims(sz[0], sz[1], sz[2])];
	}

	inline T& operator()(size_type k0)
	{
		return impl[dims(k0)];
	}
	inline T& operator()(size_type k0,size_type k1)
	{
		return impl[dims(k0,k1)];
	}
	inline T& operator()(size_type k0,size_type k1,size_type k2)
	{
		return impl[dims(k0,k1,k2)];
	}
	inline T& operator()(size_type k0,size_type k1,size_type k2,size_type k3)
	{
		return impl[dims(k0,k1,k2,k3)];
	}
	inline T& operator()(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4)
	{
		return impl[dims(k0,k1,k2,k3,k4)];
	}
	inline T& operator()(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4,size_type k5)
	{
		return impl[dims(k0,k1,k2,k3,k4,k5)];
	}


	inline const T& operator()(size_type k0) const
	{
		return impl[dims(k0)];
	}
	inline const T& operator()(size_type k0,size_type k1) const
	{
		return impl[dims(k0,k1)];
	}
	inline const T& operator()(size_type k0,size_type k1,size_type k2) const
	{
		return impl[dims(k0,k1,k2)];
	}
	inline const T& operator()(size_type k0,size_type k1,size_type k2,size_type k3) const
	{
		return impl[dims(k0,k1,k2,k3)];
	}
	inline const T& operator()(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4) const
	{
		return impl[dims(k0,k1,k2,k3,k4)];
	}
	inline const T& operator()(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4,size_type k5) const
	{
		return impl[dims(k0,k1,k2,k3,k4,k5)];
	}

	inline reference front() { return impl.front(); }
	inline const_reference front() const { return impl.front(); }
	inline reference back() { return impl.back(); }
	inline const_reference back() const { return impl.back(); }

	inline size_type size() const
	{
		return impl.size();
	}
	inline size_type size(int n) const
	{
		return dims[n];
	}

	inline const arr_xt_dims& size_ptr() const
	{
		return dims;
	}

	inline pointer data()
	{
		return impl.data();
	}

	inline const_pointer data() const
	{
		return impl.data();
	}

	inline T& operator[](size_type n){return impl[n];}
	inline const T& operator[](size_type n) const {return impl[n];}

	const arr_xt_dims& get_dims() const { return dims; }

	bool is_scalar() const;
	bool is_vector() const;
	bool is_row() const;
	bool is_col() const;
	bool is_matrix() const;
	bool is_square() const;

	static arr_xt<T, A> eye(int m, int n);
	static arr_xt<T, A> values(const T& _val, int m, int n);
	static arr_xt<T, A> zeros(int m, int n) { return values(0, m, n); }
	static arr_xt<T, A> ones(int m, int n) { return values(1, m, n); }


protected:
	using basetype::impl;
	arr_xt_dims dims;
};

template<typename T,typename A>
template<typename X,int N>
void arr_xt<T, A>::resize(const tiny_vec<X, N>& sz)
{
	EW_STATIC_ASSERT(N <= arr_xt_dims::MAX_DIM);

	arr_xt_dims _dims(sz[0]);
	for (int i = 1; i < N; i++)
	{
		_dims[i] = sz[i];
	}
	resize(_dims);
}

template<typename T,typename A>
void arr_xt<T,A>::resize(const arr_xt_dims& kn,size_t rs)
{

	size_t _newsize=kn.checked_size(this->get_allocator().max_size());
	if(_newsize<rs) _newsize=rs;

	arr_xt tmp;
	tmp.impl.resize(_newsize);
	tmp.dims=kn;

	arr_xt& _Eold(*this);
	arr_xt& _Enew(tmp);

	if(_newsize>0)
	{
		for(size_type a0=0; a0<std::min(_Eold.dims[0],_Enew.dims[0]); a0++)
			for(size_type a1=0; a1<std::min(_Eold.dims[1],_Enew.dims[1]); a1++)
				for(size_type a2=0; a2<std::min(_Eold.dims[2],_Enew.dims[2]); a2++)
					for(size_type a3=0; a3<std::min(_Eold.dims[3],_Enew.dims[3]); a3++)
						for(size_type a4=0; a4<std::min(_Eold.dims[4],_Enew.dims[4]); a4++)
							for(size_type a5=0; a5<std::min(_Eold.dims[5],_Enew.dims[5]); a5++)
							{
								tmp(a0,a1,a2,a3,a4,a5)=(*this)(a0,a1,a2,a3,a4,a5);
							}
	}

	swap(tmp);
}

template<typename T,typename A>
void arr_xt<T,A>::resize(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4,size_type k5)
{
	resize(arr_xt_dims(k0,k1,k2,k3,k4,k5));
}

template<typename T,typename A>
void arr_xt<T,A>::reshape(size_type k0,size_type k1,size_type k2,size_type k3,size_type k4,size_type k5)
{
	arr_xt_dims _dms(k0,k1,k2,k3,k4,k5);
	reshape(_dms);
}

template<typename T,typename A>
void arr_xt<T,A>::reshape(const arr_xt_dims& _dms)
{
	size_t n = _dms.checked_size(this->get_allocator().max_size());
	if(n!=impl.size())
	{
		impl.resize(_dms.size());
	}
	dims=_dms;
}

template<typename T,typename A1,typename A2>
bool operator==(const arr_xt<T,A1>& lhs,const arr_xt<T,A2>& rhs)
{
	typedef size_t size_type;
	if(lhs.size()!=rhs.size()) return false;

	for(size_type i=0; i<lhs.MAX_DIM; i++)
	{
		if(lhs.size(i)!=rhs.size(i)) return false;
	}

	for(size_type i=0; i<lhs.size(); i++)
	{
		if(lhs[i]!=rhs[i]) return false;
	}

	return true;
}

template<typename T,typename A1,typename A2>
bool operator!=(const arr_xt<T,A1>& lhs,const arr_xt<T,A2>& rhs)
{
	return !(lhs==rhs);
}


template<typename T,typename A> class hash_t<arr_xt<T,A> >
{
public:
	typedef arr_xt<T,A> type;
	uint32_t operator()(const arr_xt<T,A>& o)
	{
		return hash_array<T>::hash(o.data(),o.size())^hash_array<typename type::size_type>::hash(o.size_ptr(),type::MAX_DIM);
	}
};


template<typename T, typename A>
arr_xt<T, A> arr_xt<T, A>::eye(int m, int n)
{
	EW_ASSERT(m >= 0 && n >= 0);

	arr_xt<T, A> ret(m, n);
	T v(1);

	for (int i = 0; i < std::min(m, n); ++i)
	{
		ret(i, i) = v;
	}
	return EW_MOVE(ret);
}

template<typename T, typename A>
arr_xt<T, A> arr_xt<T, A>::values(const T& _val, int m, int n)
{
	EW_ASSERT(m >= 0 && n >= 0);

	arr_xt<T, A> ret(m, n);
	ret.assign(ret.size(), _val);

	return EW_MOVE(ret);
}

template<typename T, typename A>
bool arr_xt<T, A>::is_scalar() const
{
	return size() == 1;
}

template<typename T, typename A>
bool arr_xt<T, A>::is_vector() const
{
	size_t n = size();
	if (n == 0)
	{
		return false;
	}
	for (int d = 0; d < 6; ++d)
	{
		if (size(d) == n)
		{
			return true;
		}
	}
	return false;
}

template<typename T, typename A>
bool arr_xt<T, A>::is_matrix() const
{
	int n = size();
	return n > 0 && n == size(0) * size(1);
}

template<typename T, typename A>
bool arr_xt<T, A>::is_square() const
{
	return this->is_matrix() && (size(0) == size(1));
}

template<typename T, typename A>
bool arr_xt<T, A>::is_col() const
{
	int n = size();
	return n > 0 && n == size(0);
}

template<typename T, typename A>
bool arr_xt<T, A>::is_row() const
{
	int n = size();
	return n > 0 && n == size(1);
}

EW_LEAVE


namespace std
{
	template<typename T, typename A>
	void swap(ew::arr_xt<T, A>& v1, ew::arr_xt<T, A>& v2)
	{
		v1.swap(v2);
	}
}

#endif
