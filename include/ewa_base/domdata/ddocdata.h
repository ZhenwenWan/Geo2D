
#ifndef __H_EW_DOMDATA_DDOCDATA__
#define __H_EW_DOMDATA_DDOCDATA__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/dunit.h"
#include "ewa_base/domdata/dresults.h"
#include "ewa_base/domdata/dcommand.h"
#include "ewa_base/scripting/executor.h"

EW_ENTER


class DDocumentBase;


template<typename T>
class RefValueDataT : public ObjectData
{
public:

	virtual const T& get() = 0;

	virtual void set(const T&) = 0;

	virtual void swap(T& v)
	{
		T tmp = get();
		set(v);
		v = tmp;
	}
};

template<typename T>
class RefValueDataByRefT : public RefValueDataT<T>
{
public:

	T& ref;

	RefValueDataByRefT(T& v, DataPtrT<ObjectData> p = NULL) :ref(v) {}

	virtual const T& get()
	{
		return ref;
	}

	virtual void set(const T& v)
	{
		ref = v;
	}

	virtual void swap(T& v)
	{
		std::swap(ref, v);
	}
};



template<typename T>
class RefValueHolderT : public ObjectT< RefValueDataT<T> >
{
public:
	typedef ObjectT< RefValueDataT<T> > basetype;

	RefValueHolderT(RefValueDataT<T>* p = NULL)
	{
		m_refData.reset(p);
	}

	bool ok()
	{
		return m_refData;
	}

	void swap(T& val)
	{
		return m_refData->swap(val);
	}

	const String& get()
	{
		return m_refData->get();
	}

	virtual void set(const String& val)
	{
		m_refData->set(val);
	}
protected:
	using basetype::m_refData;
};


template<typename T>
class RefValueDataByTableT : public RefValueDataT<T>
{
public:

	RefValueDataByTableT(DataPtrT<CallableTableEx> p, const String& id)
		: ptable(p)
		, index(id)
	{

	}

	DataPtrT<CallableTableEx> ptable;
	const String index;

	const T& get()
	{
		return ptable->value[index].ref<T>();
	}

	void set(const T& val)
	{
		ptable->value[index].ref<T>() = val;
	}

	void swap(T& val)
	{
		std::swap(ptable->value[index].ref<T>(), val);
	}
};

class DLLIMPEXP_EWA_BASE CachedPtrManager : public Object
{
public:
	bst_map<void*, DataPtrT<ObjectData> > cached;

	template<typename T>
	void add(T* p)
	{
		return cached[&T::sm_info].reset(p);
	}

	template<typename T>
	T* get()
	{
		T* p = dynamic_cast<T*>(cached[&T::sm_info].get());
		if (p == NULL)
		{
			EW_NOOP();
		}
		return p;
	}

};


class DLLIMPEXP_EWA_BASE XDocumentData : public CallableObject
{
public:
	XDocumentData();

	String s_template;
	String s_app;
	int n_version;
	BitFlags flags;

	String s_doc_filename;
	String s_job_name;
	int n_doc_index;

	DataPtrT<DResultObject> p_result;
	DataPtrT<DObject> p_dispatcher;
	DataPtrT<SymbolManager> p_objmgr;
	LitePtrT<DDocumentBase> p_doc;

	CachedPtrManager ptrmgr;

	Executor lexer;
	AtomicSpin spin;

	void set_filename(const String& filename);


};

class DLLIMPEXP_EWA_BASE DTaskModifier : public DObject
{
public:
	VariantTable replaced_variables;
	indexer_map<String, DataPtrT<DObject> > replaced_objects;
	indexer_map<String, VariantTable> replaced_properties;
};

class DLLIMPEXP_EWA_BASE DTaskDataObject : public DObject
{
public:
	typedef DObject basetype;

	DTaskDataObject();

	virtual VariantTable& get_table();
	Variant props;

	static String get_task_type(const VariantTable& task);

	virtual int __getindex(Executor&, const String&); // top.s
	virtual int __setindex(Executor&, const String&); // top.s=val

	void set_task(Variant var, const String& pdis = "");

	void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(DTaskDataObject, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DTaskData : public DTaskDataObject
{
public:
	typedef DTaskDataObject basetype;

	DataPtrT<DUnit>		p_unit;
	DataPtrT<DObject>	p_variable;
	DataPtrT<DObject>	p_topgroup;
	DataPtrT<DObject>	p_workmode;

	DataPtrT<XDocumentData> p_doc;
	DataPtrT<DTaskModifier> pmodifier;

	int32_t last_version;

	void MakeTableUnique();

	DTaskData();

	SymbolManager& GetSymbolManager();

	void DoRender(DContext& dc);
	virtual void DoRender_real(DContext& dc);

	static bool real_link(DState& dp, const Variant& val, DataPtrT<DObject>& ptr);

	template<typename T1,typename T2>
	static bool real_link_t(DState& dp, const Variant& val, DataPtrT<T2>& ptr)
	{
		DataPtrT<DObject> ptr2;
		if (!real_link(dp, val, ptr2))
		{
			return false;
		}
		T1* p2 = dynamic_cast<T1*>(ptr2.get());
		return ptr.cast_and_set(p2);
	}


	bool UpdateValue(DTaskModifier* pmodifier, int f=-1, Executor* p = NULL);

	bool UpdateValue(int f=-1, Executor* p = NULL);
	bool UpdateValue(Executor& ks, int f);
	bool UpdateValue(DState& dp, int f = -1);

	virtual bool UpdateValue_real(DState& dp, int f);

	virtual bool InitTask(DState& dp);


};

class DLLIMPEXP_EWA_BASE DTaskDataPtr : public DTaskData
{
public:

	virtual bool UpdateValue_real(DState& dp, int f);

	virtual DataPtrT<DTaskData> GetRealTask(DState& dp) = 0;

};


class DLLIMPEXP_EWA_BASE DTaskData2 : public DTaskData
{
public:

	typedef DTaskData basetype;

	static String MakeName(const String& type);

	bool InitTask(DState& dp);

	virtual bool DoInitTask(DState& dp, const VariantTable& table_);

	int tmp_version;
};



class DTemplate : public DTaskData
{
public:

	typedef DTaskData basetype;

	DTemplate();

	bst_map<int, DataGrpT<DObject> > sub_objects;
	bst_map<String, DataPtrT<DObject> > named_objects;

	bool DoUpdateValue(DState& dp);

	virtual int GetTraitId();

	virtual String GetOutputFolder();

	virtual String GetOutputName();

	virtual bool DoGetChildrenTemplate(XCtxChildrenState&, int d);

	virtual bool IsObjectActivated(const String&);

	virtual RefValueHolderT<String> GetObjectNameReference(const String&);

	virtual bool GetTopGroupObjects(arr_1t<String>&);

	virtual DObject* GetTopGroup();

	virtual DObject* GetWorkmode();

	virtual DObject* GetVariable();

	virtual DTaskData* GetTaskData();

	virtual void DoRender(DContext&);

	virtual bool DoInitSubObjects(DObject* pdoc_);

	DataPtrT<DTaskData2> p_data;

	static indexer_map<String, indexer_set<String> >& GetInfoTypes(DObjectInfo* info);


	DECLARE_DOMDATA_INFO(DTemplate, DObjectInfo);

};



class DLLIMPEXP_EWA_BASE DTaskDataObjectTable : public ClassMetaTable
{
public:
	DTaskDataObjectTable();
};

class DLLIMPEXP_EWA_BASE CallableFunction_task_update : public CallableFunction
{
public:
	int __fun_call(Executor& ks, int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_task_update, ObjectInfo);
};



class XTaskDataHelper
{
public:

	static VariantTable& get_table(Executor& ks);

	static arr_1t<Functor<VariantTable* (CallableData*)> > fn_grp;

};

EW_LEAVE

#endif
