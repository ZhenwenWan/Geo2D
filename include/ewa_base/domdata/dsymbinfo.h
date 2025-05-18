#ifndef __H_EW_DOMDATA_DSYMBINFO__
#define __H_EW_DOMDATA_DSYMBINFO__

#include "ewa_base/domdata/dobject.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DSymbolInfo : public DObjectBoxMovable
{
public:

	typedef DObjectBoxMovable basetype;

	DSymbolInfo(const String& name = "symbol_info");

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DSymbolInfo, DObjectInfo);
};



EW_LEAVE

#endif