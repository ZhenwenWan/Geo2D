#ifndef __EW_SCRIPTING_VARIANT_ARRAY_H__
#define __EW_SCRIPTING_VARIANT_ARRAY_H__

//#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/scripting/callable_wrap.h"
#include "ewa_base/scripting/detail/idx_1t.h"


EW_ENTER


class Variant;

// 取子数组的引用
template<typename A, bool CONST_REF = false>
class DLLIMPEXP_EWA_BASE sub_xt
{
public:

	typedef typename tl::meta_if<CONST_REF, const typename A::value_type&, typename A::value_type&>::type reference;
	typedef const typename A::value_type& const_reference;
	typedef typename A::size_type size_type;
	typedef typename tl::meta_if<CONST_REF, const A&, A&>::type arr_ref_t;

	sub_xt(arr_ref_t a) :arr(a){}

	arr_ref_t arr;
	idx_1t idx[6];

	// 非const环境调用
	reference operator()(size_type s0)
	{
		return arr(idx[0](s0), idx[1](0), idx[2](0), idx[3](0), idx[4](0), idx[5](0));
	}

	reference operator()(size_type s0, size_type s1)
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](0), idx[3](0), idx[4](0), idx[5](0));
	}

	reference operator()(size_type s0, size_type s1, size_type s2)
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](0), idx[4](0), idx[5](0));
	}

	reference operator()(size_type s0, size_type s1, size_type s2, size_type s3)
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3), idx[4](0), idx[5](0));
	}

	reference operator()(size_type s0, size_type s1, size_type s2, size_type s3, size_type s4)
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3), idx[4](s4), idx[5](0));
	}

	reference operator()(size_type s0, size_type s1, size_type s2, size_type s3, size_type s4, size_type s5)
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3), idx[4](s4), idx[5](s5));
	}

	// const环境调用
	const_reference operator()(size_type s0) const
	{
		return arr(idx[0](s0));
	}

	const_reference operator()(size_type s0, size_type s1) const
	{
		return arr(idx[0](s0), idx[1](s1));
	}

	const_reference operator()(size_type s0, size_type s1, size_type s2, size_type s3) const
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3));
	}

	const_reference operator()(size_type s0, size_type s1, size_type s2, size_type s3, size_type s4) const
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3), idx[4](s4));
	}

	const_reference operator()(size_type s0, size_type s1, size_type s2, size_type s3, size_type s4, size_type s5) const
	{
		return arr(idx[0](s0), idx[1](s1), idx[2](s2), idx[3](s3), idx[4](s4), idx[5](s5));
	}

};

template<typename A, bool CONST_REF>
sub_xt<A, CONST_REF> make_subxt(typename sub_xt<A, CONST_REF>::arr_ref_t arr)
{
	return sub_xt<A, CONST_REF>(arr);
}

template<typename T>
class DLLIMPEXP_EWA_BASE CallableWrapT<arr_xt<T> > : public CallableDataBaseT<arr_xt<T> >
{
public:
	typedef CallableDataBaseT<arr_xt<T> > basetype;
	using basetype::value;

	typedef arr_xt<T> arr_type;

	CallableWrapT()
		:basetype()
	{
	}

#ifdef EW_C11
	CallableWrapT(arr_xt<T>&& v)
	{
		this->value.swap(v);
	}
#endif

	CallableWrapT(const arr_xt<T>& v)
		:basetype(v)
	{

	}

	void __get_iterator(Executor& ks,int d);

	virtual int __getarray(Executor& ks,int pm);
	virtual int __getarray_dot(Executor& ks,int pm);
	virtual int __setarray(Executor& ks,int pm);
	virtual int __getarray_index_range(Executor& ks,int pm);

	// op:
	//   0: 将数组的大小赋值给dm
	//   1: 将数组resize为dm
	//   2: 将数组当作1维数组赋值给dm
	virtual bool __test_dims(arr_xt_dims& dm,int op);

	template<typename A,typename X>
	void __arrset_xt(A& sub,const X& tmp);

	template<typename A,typename X>
	void __arrset_xt(A& sub,const arr_xt<X>& tmp);

	template<typename A,typename X>
	void __arrset_1t(A& sub,const X& tmp);

	template<typename A,typename X>
	void __arrset_1t(A& sub,const arr_xt<X>& tmp);

	virtual CallableData* DoClone(ObjectCloneState&);

	virtual CallableData* DoCloneObject();


	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableWrapT, ObjectInfo,1);

};






DEFINE_OBJECT_NAME_T(arr_1t,"arr_1t")
DEFINE_OBJECT_NAME_T(arr_xt,"arr_xt")

EW_LEAVE

#endif

