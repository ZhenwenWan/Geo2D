#ifndef __H_EW_SCRIPTING_VARIANT_OTHER__
#define __H_EW_SCRIPTING_VARIANT_OTHER__

#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/scripting/helpdata.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE CallableMetatable : public CallableTableEx
{
public:

	enum
	{
		FLAG_IS_LIBRARAY = 1 << 0,
		FLAG_IS_MODIFIED = 1 << 1,
	};

	VariantTable& table_meta;
	VariantTable table_self;
	VariantTable table_mfun;
	BitFlags flags;

	String m_sClassName;

	CallableMetatable(const String& name="");

	void Serialize(SerializerHelper sh);

	bool ToValue(String& v,int) const;


	int __setindex(Executor& ks,const String&);
	int __setarray(Executor& ks,int pm);

	virtual int __fun_call(Executor&,int);

	static int __metatable_call1(Executor&,const String&);
	static int __metatable_call2(Executor&,const String&);

	virtual CallableMetatable* ToMetatable(){return this;}

	virtual CallableData_helpdata* __get_helpdata();
	void __set_helpdata(const String& s);



	template<typename T>
	void add_meta_function()
	{
		_add_function<T>(table_meta);
	}

	template<typename T>
	void add_mfun_function()
	{
		_add_function<T>(table_mfun);
	}

	template<typename T>
	void add_function(int f)
	{
		if (f & 1)
		{
			_add_function<T>(table_meta);
		}

		if (f & 2)
		{
			_add_function<T>(table_mfun);
		}
	}


protected:
	DataPtrT<CallableData_helpdata> m_pHelp;


	template<typename T>
	static void _add_function(VariantTable& table)
	{
		T* p = T::sm_info.GetCachedInstance();
		_add_function(table, p);
	}

	static void _add_function(VariantTable& table, CallableFunction* p);


	DECLARE_OBJECT_INFO(CallableMetatable, ObjectInfo);
};



class DLLIMPEXP_EWA_BASE CallableModule : public CallableMetatable
{
public:

	CallableModule(const String name = "");

	virtual CallableModule* DoClone(ObjectCloneState&);

	virtual int __fun_call(Executor& ks, int pm);

	void Serialize(SerializerHelper sh);

	bool ToValue(String& v,int) const;

	virtual CallableModule* ToModule(){return this;}

protected:

	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableModule, ObjectInfo, 1);
};

template<typename T>
class CallableMetatableT : public CallableMetatable
{
public:
	typedef T type;

	CallableMetatableT();
	virtual int __fun_call(Executor& ks, int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableMetatableT, ObjectInfo);
};



IMPLEMENT_OBJECT_INFO_T1(CallableMetatableT, ObjectInfo);

class DLLIMPEXP_EWA_BASE CallableClass : public CallableObject
{
public:
	DataPtrT<CallableMetatable> metax;

	CallableClass(CallableMetatable* p=NULL){reset(p);}

	void reset(CallableMetatable* p = NULL);

	arr_1t<Variant> value;

	virtual CallableClass* ToClass() {return this;}
	virtual CallableMetatable* GetMetaTable(){return metax.get();}


	virtual int __getindex(Executor&,const String&); // top.s
	virtual int __setindex(Executor&,const String&); // top.s=val

	virtual int __getarray(Executor& ks,int pm);
	virtual int __setarray(Executor& ks,int pm);
	virtual int __getarray_index_range(Executor& ks,int pm);

	bool ToValue(String& v,int) const;
	
	virtual int __tostring(Executor&, int pm);

	void Serialize(SerializerHelper sh);

	virtual void __get_iterator(Executor&,int);

	DECLARE_OBJECT_INFO(CallableClass,ObjectInfo);
};

template<typename P>
class pl1_dispatch_base
{
public:
	static int value(Executor& ks, Variant&)
	{
		return CallableMetatable::__metatable_call1(ks, P::info().op_name);
	}
};


EW_LEAVE

#endif
