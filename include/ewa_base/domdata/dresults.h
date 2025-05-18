#ifndef __EW_EWA_MODEL_TDDOC_RESULTS_H__
#define __EW_EWA_MODEL_TDDOC_RESULTS_H__

#include "ewa_base/domdata/dobject.h"


EW_ENTER




class DResultFile : public DObject
{
public:
	DResultFile(const String& fp = empty_string);

	String s_file;
	TimePoint tp;

	DECLARE_DOMDATA_INFO(DResultFile, DObjectInfo)
};


class DResultObject : public DObject
{
public:

	typedef DObject basetype;

	DResultObject(const String& id = empty_string);

	String s_filename;

	bst_map<String, TimePoint> a_real_results;
	bst_map<String, TimePoint> a_results;

	void AddResult(const String& f);

	virtual String MakeLabel(int) const;

	bool updated;

	bool DoGetChildren(XCtxChildrenState& cs);

	String GetFolder() const;
	void SetFilename(const String& s);

	virtual int __fun_call(Executor&, int);	// top(...)

	bool Sync(int dir);
	bool SyncEx(int dir);

	bool DeleteResult(const String& s);
	bool DeleteResult(DResultFile& f);

	void ClearResults();


	DataGrpT<DResultFile> children;

	DECLARE_DOMDATA_INFO(DResultObject, DObjectInfo);
};



class DObjectResultRoot : public DObject
{
public:

	DObjectResultRoot();

	DObjectResultRoot(const VariantTable& t);

	Variant result;

	static String make_str(const String& val, const String& pre);

	void handle(const VariantTable& t, const String& pre = "");

	virtual int __fun_call(Executor&, int);	// top(...)


	virtual bool DoGetChildren(XCtxChildrenState& cs);

	DataGrpT<DObject> children;

	DECLARE_DOMDATA_INFO(DObjectResultRoot, DObjectInfo);
};


class DResultVisualizer : public DGroup
{
public:

	String s_wnd_target;

	DResultVisualizer();

	size_t Update(CallableData* ptable,Executor* plexer=NULL);

	DataGrpT<DObject> fallback_values;

	virtual int __fun_call(Executor&, int);	// top(...)

	DECLARE_DOMDATA_INFO(DResultVisualizer, DObjectInfo);
};



EW_LEAVE

#endif