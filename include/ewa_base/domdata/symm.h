#ifndef __H_EW_DOMDATA_SYMM__
#define __H_EW_DOMDATA_SYMM__


#include "ewa_base/domdata/dobject.h"
#include "ewa_base/scripting/callable_table.h"


EW_ENTER


class DLLIMPEXP_EWA_BASE SymbolItem
{
public:
	SymbolItem(const String& n = "");

	String name;
	arr_1t<Variant> prop;

	void SerializeVariant(Variant& v, int dir);

	void Serialize(SerializerHelper sh);

	bool operator==(const SymbolItem& rhs) const;
	bool operator!=(const SymbolItem& rhs) const;
};


template<>
class hash_t<SymbolItem>
{
public:
	uint32_t operator()(const SymbolItem& item);
};

DEFINE_OBJECT_NAME(SymbolItem, "SymbolItem")



template<>
class DLLIMPEXP_EWA_BASE CallableWrapT<arr_1t<SymbolItem> > : public CallableDataBaseT<arr_1t<SymbolItem> >
{
public:
	typedef arr_1t<SymbolItem> type;
	typedef CallableDataBaseT<type> basetype;

	CallableWrapT() :basetype(){}
	CallableWrapT(const type& v) :basetype(v){}
	virtual CallableData* DoClone(ObjectCloneState&){ return new CallableWrapT(value); }
	virtual CallableData* DoCloneObject(){ return new CallableWrapT(value); }
};

class DLLIMPEXP_EWA_BASE SymbolManager : public CallableData
{
protected:
	typedef CallableData basetype;

	class ObjState
	{
	public:
		ObjState(const String& n="",const String& t=""):name(n),type(t){}

		String name;
		String type;
		DataPtrT<DObject> symptr;
		VariantTable value;
	};


	indexer_map<String,DataPtrT<DObject> > m_aSymbol;
	arr_1t<ObjState> m_aStack;


public:

	SymbolManager(const SymbolManager& o);
	SymbolManager& operator=(const SymbolManager& o);

	void Serialize(SerializerHelper sh);

	void clear();

	bool swap_obj(DataPtrT<DObject>& p1);

	void swap(SymbolManager& o);

	void ref(SymbolManager& o);

	size_t depth();

	size_t size();

	DObject* get_item(size_t i);

	ObjectCreator factory;

	SymbolManager(){}

	bool UpdateValue();
	bool DoUpdateValue(DState& dp);
	bool DoUpdateValue(DState& dp,const String& name);

	void Upgrade(XSmUpgradeCtx& ctx);

	void gp_add(const String& s);

	void gp_set(const String& s, const String& v);

	void gp_set(const String& s, const Variant& v);

	Variant& gp_get(const String& s);

	void append(DObject* p);

	void update(const String& id);

	void update(DataPtrT<DObject> p);

	void remove(DObject* p);
	void remove(const String& s);

	void gp_add(DObject* p);

	void gp_beg(const String& s,const String& t="");
	void gp_end();

	void append(const String& s, const String& t);

	DObject* get_item(const String& s);
	DObject* get_item(const Variant& v);

	template<typename T>
	T* get_item_t(const String& s)
	{
		return dynamic_cast<T*>(get_item(s));
	}
	template<typename T>
	T* get_item_t(size_t i)
	{
		return dynamic_cast<T*>(get_item(i));
	}

	String make_name(const String& pre);

	void apply_variable_modifier(const VariantTable& tb);

	virtual int __getindex(Executor& ks, const String& si);
};


template<>
class get_this_ref_heler<SymbolManager>
{
public:
	static SymbolManager* g(Executor& ks);
	static void init(VariantTable& fn);
};

class DLLIMPEXP_EWA_BASE DExtraInfoCtx : public Object
{
public:
	DataPtrT<SymbolManager> pmgr;
	LitePtrT<XDocumentData> pdoc;
};

//class DLLIMPEXP_EWA_BASE DObjectReplaced : public DObject
//{
//public:
//	DataPtrT<DObject> p_origin;
//	DataPtrT<DObject> p_replaced;
//};



class DLLIMPEXP_EWA_BASE NameGenerator
{
public:
	NameGenerator();
	NameGenerator(SymbolManager * p);

	DataPtrT<SymbolManager> psymmgr;
	bst_set<String> names_bad;
	bst_set<String> names_ok;

	void clear();

	bool test(const String& name);

	String name(const String& prefix);
	String name2(const String& name);

	void update(DObject* p);

	void name(DObject* p);
	void init(DObject* p);

};



EW_LEAVE
#endif
