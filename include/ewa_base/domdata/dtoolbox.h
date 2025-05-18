
#ifndef __H_EW_DOMDATA_DTOOLBOX__
#define __H_EW_DOMDATA_DTOOLBOX__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_function.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DToolBox : public DObject
{
public:

	typedef DObject basetype;

	DToolBox();
	DToolBox(CallableData* p);
	~DToolBox();

	void Serialize(SerializerHelper sh);
	virtual void DoTransferData(TableSerializer& ar);

	bool DoCallFunction(const String& func, const Variant* p=NULL,int n=0);

	DataPtrT<CallableData> value;

	int __getindex(Executor& ks, const String& si);
	int __setindex(Executor& ks, const String& si);

	void GetIcon(XCtxGetImage& ctx) const;


	DECLARE_DOMDATA_INFO(DToolBox, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableToolBox_create : public CallableFunction
{
public:
	CallableToolBox_create();
	int __fun_call(Executor& ks, int pm);

	static Variant load_toolbox(const String& file, Variant* p, int n);

	DECLARE_OBJECT_INFO_CACHED(CallableToolBox_create, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableToolBox_sd_call : public CallableFunction
{
public:
	CallableToolBox_sd_call();
	int __fun_call(Executor& ks, int pm);


	DECLARE_OBJECT_INFO_CACHED(CallableToolBox_sd_call, ObjectInfo);
};


EW_LEAVE
#endif
