
#ifndef __H_EW_DOMDATA_DDOCBASE__
#define __H_EW_DOMDATA_DDOCBASE__

#include "ewa_base/domdata/ddocdata.h"
#include "ewa_base/domdata/symm.h"
#include "ewa_base/domdata/table_serializer.h"
#include "ewa_base/util/strlib.h"


EW_ENTER


class DFilter : public DObject
{
public:

	typedef DObject basetype;

	DFilter();

	virtual String MakeLabel(int) const;
};

class DFilterByType : public DFilter
{
public:
	typedef DFilter basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_CAN_ACTIVATE = FLAG_MIN << 1,


	};

	DFilterByType();

	bool DoGetChildren(XCtxChildrenState& cs);

	indexer_set<String> types;


	DECLARE_DOMDATA_INFO(DFilterByType, DObjectInfo)
};




class DDocSerializer;
class PCreatorParam;

class DDocumentBase : public DTaskDataPtr
{
public:

	typedef DTaskDataPtr basetype;

	DDocumentBase();
	DDocumentBase(const DDocumentBase& o);
	~DDocumentBase();

	virtual void DoTransferData(TableSerializer& ar);
	virtual bool DoGetChildren(XCtxChildrenState&);

	virtual bool IsObjectActivated(const String&id, DFilterByType* pfilter);

	virtual RefValueHolderT <String> GetObjectNameReferenceByFilter(DFilterByType* pfilter);

	RefValueHolderT<String> GetObjectNameReference(const String& id);

	DataPtrT<DTaskData> GetRealTask(DState& dp);

	void Upgrade();
	void Upgrade(XSmUpgradeCtx& ctx);

	virtual void DoGetExtraInfo(DExtraInfoCtx& ctx);

	template<typename T>
	void ptr_add(T* p)
	{
		p_doc->ptrmgr.add(p);
	}

	template<typename T>
	T* ptr_get()
	{
		return p_doc->ptrmgr.get<T>();
	}

	virtual int __getindex(Executor& ks, const String& si);
	virtual int __setindex(Executor& ks, const String& si);

	DataGrpT<DObject> children;

	virtual bool DoSave(Stream& fp);
	virtual bool DoLoad(Stream& fp);

	virtual void HandleSerialize(DDocSerializer& vis);

	DECLARE_DOMDATA_INFO(DDocumentBase, DObjectInfo)

protected:

	virtual bool InternalMoveCopy(DDocumentBase* pdoc);
};



class DDocSerializer
{
public:

	DDocSerializer(Stream& fp_);


	void WriteSymbol(DObject* pobj);
	void WriteVariant(const Variant& v);
	void WriteString(const String& s);
	void WriteArray(const arr_xt<Variant>& v);
	void WriteObject(const Variant& v);


	void WriteType(const String& s);

	void WriteValue(const String& s);

	void WriteTab();

	void WriteProperty(const String& s, const Variant& v);

	void WriteNameValue(const String& name, const Variant& value);

	void WritePropertys(DObject* pobj);

	void SymbolEnter(DObject* pobj);

	void SymbolLeave();


	String td;
	Stream& fp;
	arr_1t<char> nb;
	indexer_map<DObject*, DataPtrT<DObject> > objmap;

	int n_ddoc_array_critical_size;


};



class SymbolObjectInternalParam
{
public:

	enum
	{
		FLAG_POBJ_OK = 1 << 0,
		FLAG_DATA_OK = 1 << 1,
	};

	DataPtrT<CallableData> pdata;
	String sdata;


	int ncode;
	BitFlags flags;

	SymbolObjectInternalParam();

	void reset_pdata(CallableData* p);
	void reset_sdata(const String& s);

	bool ensure_pobj();
	bool ensure_data();

	static DataPtrT<CallableData> decode(const String& s, int d);
	static String encode(const DataPtrT<CallableData>& p, int d);


};



EW_LEAVE

#endif
