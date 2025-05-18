#ifndef __H_EW_DOMDATA_DNODE_INFO__
#define __H_EW_DOMDATA_DNODE_INFO__

#include "ewa_base/domdata/dobject.h"

EW_ENTER

class DUnit;

class DLLIMPEXP_EWA_BASE DNodeInfoCtx : public Object
{
public:
	typedef Object basetype;

	DNodeInfoCtx();

	enum
	{
		UNIT_L,
		UNIT_F,
		UNIT_T
	};

	virtual void Begin() {};
	virtual void End() {};
	virtual void Commit() {};

	virtual void Add(const String& k, const String& v,const String& u=empty_string) {};

	virtual void SetUnit(DUnit* p);


	//virtual vec3s GetUnit();
	//String GetUnit(int n);

	bst_map<String, String> units;

	DataPtrT<DUnit> punit;
};


EW_LEAVE

#endif
