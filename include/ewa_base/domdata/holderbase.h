#ifndef __H_EW_DOMDATA_HOLDERBASE__
#define __H_EW_DOMDATA_HOLDERBASE__


#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/symm.h"
#include "ewa_base/domdata/ddocbase.h"
#include "ewa_base/scripting/callable_table.h"


EW_ENTER



class HolderBase : public Object
{
public:

	virtual HolderBase& beg(const String& s, const String& t = "");
	virtual HolderBase& end();

	virtual HolderBase& add(const String& s, const Variant& v);
	virtual HolderBase& set(const String& s, const Variant& v);

	virtual HolderBase& err(const String& s);
	virtual HolderBase& chk();

	void SetData(DDocumentBase* d);
	DDocumentBase* GetData();
	SymbolManager& GetSymbolManager();


	BitFlags flags;

	DataGrpT<DDocumentBase> adocs;

	void add_symbol(DataPtrT<DObject> p);
	DObject* get_symbol(const String& name);

protected:

	virtual void _init(DDocumentBase* pdoc);


	LitePtrT<SymbolManager> psmap;
	DataGrpT<DDocumentBase> asmap;

};

EW_LEAVE
#endif
