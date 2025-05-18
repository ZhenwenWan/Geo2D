
#ifndef __H_EW_DOMDATA_DEFINITION__
#define __H_EW_DOMDATA_DEFINITION__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/dstate.h"
#include "ewa_base/domdata/table_serializer.h"

EW_ENTER




class XDefinitionHelper
{
public:
	static void internal_init(VariantTable& tb1, Variant& fn, DObject* pobj);
	static bool call_function(VariantTable& tb1, const String& func, const Variant* p, size_t n);
	static bool get_children(XCtxChildrenState& cs,VariantTable& tb1,Variant& var, arr_xt<Variant>* parr);
	static bool lexer_compile(const String& filename, Variant& fn);
};


template<typename T>
class LocalNamedDObjectInfoT : public T::infobase
{
public:
    typedef typename T::infobase basetype;

    using basetype::m_sClassName;

	Variant fn;
	LocalNamedDObjectInfoT(const String& name, const Variant& fn_) : T::infobase(name)
	{
		fn = fn_;
	}

	LocalNamedDObjectInfoT(const String& name, const String& fp) :T::infobase(name)
	{
		if (!XDefinitionHelper::lexer_compile(fp,fn))
		{
			System::LogError("compile %s failed", fp);
		}
	}

	Object* CreateObject();
};

template<typename T>
class DDefinitionBaseT_internal_init
{
public:
	static void g(VariantTable&,T&){}
};

template<typename T>
class DDefinitionT;

template<typename T>
class DDefinitionBaseT : public T
{
protected:

    using T::flags;


	void _reset_object_info(LocalNamedDObjectInfoT<T>* p)
	{
		tb1.clear();
		_m_p_info = p;

		DDefinitionBaseT_internal_init<T>::g(tb1, *this);
		XDefinitionHelper::internal_init(tb1, _m_p_info->fn, this);
	}

	LocalNamedDObjectInfoT<T>* _m_p_info;

public:

	virtual DObjectInfo& GetObjectInfoVirtual()
	{
		return *_m_p_info;
	}

	typedef T basetype;
	typedef typename T::infobase infobase;

    using basetype::m_sId;

	virtual VariantTable& get_table()
	{
		return tb1;
	}

	VariantTable* get_tb1()
	{
		return &tb1;
	}


	DDefinitionBaseT(LocalNamedDObjectInfoT<T>& info)
		: _m_p_info(&info)
	{
		DDefinitionBaseT_internal_init<T>::g(tb1, *this);
		XDefinitionHelper::internal_init(tb1, _m_p_info->fn, this);
	}

	DDefinitionBaseT(const DDefinitionBaseT& o)
		: basetype(o)
		, _m_p_info(o._m_p_info)
	{
		DDefinitionBaseT_internal_init<T>::g(tb1, *this);
		XDefinitionHelper::internal_init(tb1, _m_p_info->fn, this);
		TableSerializerEx::sync(*this, const_cast<DDefinitionBaseT&>(o));
	}

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		basetype::Serialize(ar);
		ar & tb1;
	}


	const String& GetObjectName() const
	{
		return _m_p_info->GetName();
	}

	virtual void DoTransferData(TableSerializer& ar)
	{
		basetype::DoTransferData(ar);
		LockState<DataPtrT<CallableTableEx> > lock(ar.ptable, tb1["wmuser"].kptr_t<CallableTableEx>());
		Variant var(&ar);
		if (!this->DoCallFunction("transfer", &var, 1))
		{
			Exception::XError("invalid transfer function");
		}
	}

	virtual void DoRender(DContext& dc)
	{
		Variant var((CallableData*)&dc);
		if (!this->DoCallFunction("render", &var, 1))
		{
			Exception::XError("invalid transfer function");
		}
	}

	void AcceptObject(ObjectVisitorState& vs)
	{
		basetype::AcceptObject(vs);
		Variant var(new CallableVisitorState(vs, tb1["wmuser"]));
		if (!this->DoCallFunction("accept_vis", &var, 1))
		{
			Exception::XError("invalid transfer function");
		}
	}

	virtual bool DoCheckParam(DState& dp)
	{
		LoggerEx logger;

		if (!basetype::DoCheckParam(dp))
		{
			return false;
		}

		Variant var;
		var.kptr(&dp);

		if (!this->DoCallFunction("check", &var, 1))
		{
			return false;
		}

		return !logger.has_new_error_only();
	}

	virtual bool DoUpdateValue(DState& dp)
	{
		LoggerEx logger;

		if (!basetype::DoUpdateValue(dp))
		{
			return false;
		}

		Variant var;
		var.kptr(&dp);

		if (!this->DoCallFunction("update", &var, 1))
		{
			return false;
		}

		return !logger.has_new_error_only();
	}

	bool DoCallFunction(const String& func, const Variant* p, size_t n)
	{
		if (XDefinitionHelper::call_function(tb1, func, p, n))
		{
			return true;
		}
		this_logger().LogError("%s function error @ %s", func, m_sId);
		return false;
	}


	virtual bool DoGetChildren(XCtxChildrenState& cs)
	{

		Variant var = tb1.get_ptr_data("children");
		if (!var.kptr())
		{
			return basetype::DoGetChildren(cs);
		}

		arr_xt<Variant> *parr = var.ptr<arr_xt<Variant> >();
		if (!parr)
		{
			if (basetype::DoGetChildren(cs))
			{
				var.ref<arr_xt<Variant> >().assign(cs.begin(), cs.end());
			}
			else
			{
				var.clear();
			}
		}
		return XDefinitionHelper::get_children(cs, tb1, var, parr);
	}

	virtual int __getindex(Executor& ks, const String& si)
	{

		ks.ci1.nbp[StackState1::SBASE_THIS].kptr(this);

		if (si == "idata")
		{
			ks.ci1.nsp[0] = tb1["wmdata"];
		}
		else if (si == "udata")
		{
			ks.ci1.nsp[0] = tb1["wmuser"];
		}
		else
		{
			return basetype::__getindex(ks, si);
		}

		return CallableData::STACK_BALANCED;

	}

	String GetPrefix() const
	{

		if (const VariantTable* ptable = tb1["wmbase"].ptr<VariantTable>())
		{
			const String* pv = ptable->get_ptr<String>("prefix");
			if (pv && !pv->empty())
			{
				return *pv;
			}
		}
		return basetype::GetPrefix();
	}


	DDefinitionBaseT<T>* DoClone(ObjectCloneState& cs);

	VariantTable tb1;

};

template<typename T>
class DDefinitionT : public DDefinitionBaseT<T>
{
public:
	typedef DDefinitionBaseT<T> basetype;

	DDefinitionT(LocalNamedDObjectInfoT<T>& info) :basetype(info)
	{

	}

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		basetype::Serialize(ar);
	}

};

template<typename T>
DDefinitionBaseT<T>* DDefinitionBaseT<T>::DoClone(ObjectCloneState& cs)
{
	DDefinitionBaseT<T>* pdst = new  DDefinitionT<T>(*(DDefinitionT<T>*)this);
	return pdst;
}

template<typename T>
Object* LocalNamedDObjectInfoT<T>::CreateObject()
{
	return new DDefinitionT<T>(*this);
}



typedef ObjectInfo* (*object_info_creator_func_t)(const String&,const String&);

//void RegisterDObjectsByEwsl_real(const String& name, object_info_creator_func_t func,String path);


template<typename T>
ObjectInfo* object_creator_func(const String& type,const String& file)
{
	return new LocalNamedDObjectInfoT<T>(type, file);
}


class DObjectsRegisterHelper
{
public:

	template<typename T>
	static void apply_t(const String& prefix=empty_string,const String& path=empty_string)
	{
		DObjectsRegisterHelper obj;
		obj.add<T>(prefix);
		obj.apply(path);
	}

	indexer_map<String, object_info_creator_func_t> objmap;

	template<typename T>
	void add(const String& prefix = empty_string)
	{
		String name = prefix.empty() ? T::sm_info.GetName() : prefix;
		objmap[name]=&object_creator_func<T>;
	}

	DObjectsRegisterHelper(const String& name_, object_info_creator_func_t func_);
	DObjectsRegisterHelper();

	void apply(const String& path_=empty_string, size_t depth = 2);

private:

	void apply_file(const String& filename, const String& folder);
	void apply_path(const String& path, size_t depth);
	String s_ext;
};

EW_LEAVE

#endif
