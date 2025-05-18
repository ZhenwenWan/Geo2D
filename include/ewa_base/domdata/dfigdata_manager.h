
#ifndef __H_EW_DOMDATA_DFIGDATA_MANAGER__
#define __H_EW_DOMDATA_DFIGDATA_MANAGER__

#include "ewa_base/domdata/dfigdata.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DFigDataManager : public DObjectGroupT<DFigData, DObject>
{
public:
	typedef DObjectGroupT<DFigData, DObject> basetype;

	DFigDataManager(const String& name = _kT("datamgr"));

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DFigDataManager, DObjectInfo);
};

EW_LEAVE

#endif
