#ifndef __H_EW_SCRIPTING_VARIANT_TABLE__
#define __H_EW_SCRIPTING_VARIANT_TABLE__

#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/scripting/callable_wrap.h"
#include "ewa_base/collection/indexer_map.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE VariantTable : public indexer_map<String,Variant>
{
public:

	typedef indexer_map<String, Variant> basetype;

	bool operator==(const VariantTable& v2) const;
	bool operator!=(const VariantTable& v2) const;

	VariantTable& operator= (const VariantTable& o)
	{
		this->impl = o.impl;
		return *this;
	}

	VariantTable() {}
	VariantTable(const VariantTable& o)
	{
		this->impl = o.impl;
	}

#ifdef EW_C11

	VariantTable(VariantTable&& o)
	{
		swap(o);
	}

	VariantTable& operator= (VariantTable&& o)
	{
		swap(o);
		return *this;
	}
#endif



	CallableData* get_ptr_data(const String& k) const throw();

	template<typename T>
	const T* get_ptr(const String& k) const throw()
	{
		index_type idx = this->find1(k);
		if (idx >= 0)
		{
			return this->get(idx).second.ptr<T>();
		}
		else
		{
			return NULL;
		}
	}

	template<typename T>
	T* get_ptr(const String& k) throw()
	{
		index_type idx = this->find1(k);
		if (idx >= 0)
		{
			return this->get(idx).second.ptr<T>();
		}
		else
		{
			return NULL;
		}
	}

	template<typename T>
	bool get_val(const String& k, T& val) const throw()
	{
		index_type idx = this->find1(k);
		if (idx < 0)
		{
			return false;
		}
		else
		{
			return this->get(idx).second.get(val);
		}
	}

	bool get_variant(const String& k, Variant& val) const throw()
	{
		index_type idx = this->find1(k);
		if (idx < 0)
		{
			return false;
		}
		else
		{
			val=this->get(idx).second;
			return true;
		}
	}

	template<typename X>
	bool get_var(Variant& val,X it1,X it2) const throw()
	{
		if (it1 == it2) return false;

		Variant tmp;
		const VariantTable* ptable = this;

		for (X it = it1; it!=it2; it++)
		{
			if (!ptable)
			{
				return false;
			}

			index_type idx = ptable->find1(*it);
			if (idx < 0)
			{
				return false;
			}

			tmp=this->get(idx).second;
			ptable = tmp.ptr<VariantTable>();
	
		}

		val = tmp;
		return true;

	}

	bool get_var_2(Variant& val, const String& it1, const String& it2) const throw()
	{
		index_type idx = this->find1(it1);
		if (idx < 0)
		{
			return false;
		}

		const VariantTable* ptable = const_cast<Variant&>(this->get(idx).second).get_table_ptr();
		if (!ptable)
		{
			return false;
		}

		idx = ptable->find1(it2);
		if (idx < 0)
		{
			return false;
		}

		val = ptable->get(idx).second;
		return true;
	}


	void Serialize(SerializerHelper sh);

};
DEFINE_OBJECT_NAME(VariantTable, "table");

class DLLIMPEXP_EWA_BASE ClassMetaTable : public VariantTable
{
public:

	template<typename T>
	void add_func(const String& name)
	{
		T* p = T::sm_info.CreateObject();
		(*this)[name].kptr(p);
	}
};

class DLLIMPEXP_EWA_BASE CallableTableOperators : public CallableObject
{
public:
	static int __do_setindex(Executor& ks,VariantTable& tb,const String& si);
	static int __do_getindex(Executor& ks,VariantTable& tb,const String& si);
	static int __do_getarray(Executor& ks,VariantTable& tb,int pm);
	static int __do_setarray(Executor& ks,VariantTable& tb,int pm);
	static void __do_get_iterator(Executor& ks,VariantTable& tb,int nd);

	static bool __do_test_dims(VariantTable& tb,arr_xt_dims& dm,int op);
	static int __do_getarray_index_range(Executor& ks,VariantTable& tb,int pm);
};


template<>
class DLLIMPEXP_EWA_BASE CallableWrapT<VariantTable> : public CallableDataBaseT<VariantTable>
{
public:
	typedef CallableDataBaseT<VariantTable> basetype;
	CallableWrapT():basetype(){}
	CallableWrapT(const VariantTable& v):basetype(v){}

	CallableWrapT(const CallableWrapT& o) 
	{
		this->value = o.value;
	}

#ifdef EW_C11
	CallableWrapT(CallableWrapT&& o) 
	{
		this->value.swap(o.value);
	}
	CallableWrapT(VariantTable&& o)
	{
		this->value.swap(o);
	}
#endif


	virtual CallableWrapT<VariantTable>* ToTable() {return this;}

	virtual CallableData* DoClone(ObjectCloneState& cs)
	{
		if(cs.type==0) return this;
		return new CallableWrapT<VariantTable>(*this);
	}

	bool DoGetChildren(XCtxChildrenState& cs);

	VariantTable* get_table_ptr()
	{
		return &value;
	}

	int __setindex(Executor& ks,const String& si){return CallableTableOperators::__do_setindex(ks,value,si);}
	int __getindex(Executor& ks,const String& si){return CallableTableOperators::__do_getindex(ks,value,si);}
	int __getarray(Executor& ks,int pm){return CallableTableOperators::__do_getarray(ks,value,pm);}
	int __setarray(Executor& ks,int pm){return CallableTableOperators::__do_setarray(ks,value,pm);}

	int __getarray_index_range(Executor& ks,int pm){return CallableTableOperators::__do_getarray_index_range(ks,value,pm);}
	bool __test_dims(arr_xt_dims& dm,int op){return CallableTableOperators::__do_test_dims(value,dm,op);}
	void __get_iterator(Executor& ks,int nd){CallableTableOperators::__do_get_iterator(ks,value,nd);}

	bool ToValue(String& v,int) const;


	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableWrapT, ObjectInfo,1);

};


typedef CallableWrapT<VariantTable> CallableTableEx;


class DLLIMPEXP_EWA_BASE CallableTableWithLock : public CallableTableEx
{
public:

	AtomicMutex mutex;

	int __setindex(Executor& ks, const String& si) { LockGuard<AtomicMutex> lock(mutex); return CallableTableOperators::__do_setindex(ks, value, si); }
	int __getindex(Executor& ks, const String& si) { LockGuard<AtomicMutex> lock(mutex);return CallableTableOperators::__do_getindex(ks, value, si); }
	int __getarray(Executor& ks, int pm) { LockGuard<AtomicMutex> lock(mutex);return CallableTableOperators::__do_getarray(ks, value, pm); }
	int __setarray(Executor& ks, int pm) { LockGuard<AtomicMutex> lock(mutex);return CallableTableOperators::__do_setarray(ks, value, pm); }

	int __getarray_index_range(Executor& ks, int pm) { return CallableData::__getarray_index_range(ks, pm); }
	bool __test_dims(arr_xt_dims& dm, int op) { return CallableData::__test_dims(dm, op); }
	void __get_iterator(Executor& ks, int nd) { return CallableData::__get_iterator(ks, nd); }

	DECLARE_OBJECT_INFO(CallableTableWithLock, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableElementManager : public CallableTableEx
{
public:

	typedef CallableTableEx basetype;

	Variant extra;

	virtual void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(CallableElementManager, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableTableProxy : public CallableObject
{
public:

	static const int FLAG_READONLY = 1 << 0;
	static const int FLAG_SET_THIS = 1 << 1;

	VariantTable& value;
	DataPtrT<ObjectData> parent;

	BitFlags flags;

	CallableTableProxy(VariantTable& v, ObjectData* p = NULL,int f=0) :value(v), parent(p),flags(f){}

	int __getindex(Executor& ks, const String& si);
	int __setindex(Executor& ks, const String& si);
	int __getarray(Executor& ks, int pm);
	int __setarray(Executor& ks, int pm);

	int __getarray_index_range(Executor& ks, int pm){ return CallableTableOperators::__do_getarray_index_range(ks, value, pm); }
	bool __test_dims(arr_xt_dims& dm, int op){ return CallableTableOperators::__do_test_dims(value, dm, op); }
	void __get_iterator(Executor& ks, int nd){ CallableTableOperators::__do_get_iterator(ks, value, nd); }

	void Serialize(SerializerHelper sh);

	virtual CallableData* DoClone(ObjectCloneState& cs);

	virtual infobase& GetObjectInfo() const{ return CallableWrapT<VariantTable>::sm_info; }
	virtual const String& GetObjectName() const { return CallableWrapT<VariantTable>::sm_info.GetName(); }

};

class DLLIMPEXP_EWA_BASE CallableTableRo : public CallableTableEx
{
public:

	int __setindex(Executor& ks,const String&);
	int __setarray(Executor& ks,int pm);

	int __getindex(Executor& ks,const String& si);
	int __getarray(Executor& ks,int pm);


	virtual CallableData* DoClone(ObjectCloneState&){return this;}

	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableTableRo, ObjectInfo, 1);
};

class DLLIMPEXP_EWA_BASE CallableTableWrap : public CallableObject
{
public:

	typedef CallableObject basetype;

	CallableTableWrap(CallableData* p = NULL, int m = 0) :ptable(p), mode(m)
	{

	}

	virtual int __getindex(Executor& ks, const String& si);
	virtual int __getarray(Executor& ks, int pm);


	virtual CallableData* DoClone(ObjectCloneState&){return this;}

	DataPtrT<CallableData> ptable;
	int mode;

	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableTableWrap, ObjectInfo, 1);
};




class DLLIMPEXP_EWA_BASE ResultTreeNode
{
public:

	String value;
	arr_1t<ResultTreeNode> children;

	int32_t index;
	int32_t depth;

	ResultTreeNode(const String& v="");

	void find(arr_1t<int32_t>& res,String* p,int d);
	void addn(int idx,String* p,int d);

	void Serialize(SerializerHelper sh);

protected:
	void find_all(arr_1t<int32_t>& res,String* p,int d);
};



class DLLIMPEXP_EWA_BASE CallableResultGroup : public CallableWrapT<VariantTable>
{
public:
	typedef CallableWrapT<VariantTable> basetype;

	void addvar(Variant& var,const String& acc);

	ResultTreeNode strmap;

	int __getarray(Executor& ks,int pm);
	int __setarray(Executor& ks,int pm);

	int __setindex(Executor& ks, const String& si);
	int __getindex(Executor& ks, const String& si);

	virtual void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(CallableResultGroup,ObjectInfo)
};


template<>
class pl_cast_base<VariantTable>
{
public:
	typedef VariantTable type;

	static const type& g(const type& v){return v;}
	static type& g(type& v){return v;}

	template<typename O>
	static type g(const O&){Exception::XBadCast();return type();}

};


template<>
class hash_t<VariantTable>
{
public:
	inline uint32_t operator()(const VariantTable& o)
	{
		return hash_array<VariantTable::value_type>::hash(o.begin(),o.end());
	}
};


EW_LEAVE

#endif
