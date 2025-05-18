#ifndef __H_EW_DOMDATA_DAXIS_SYMB__
#define __H_EW_DOMDATA_DAXIS_SYMB__

#include "ewa_base/domdata/daxisunit.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DAxisSymbol : public DObjectBoxMovable
{
public:

	typedef DObjectBoxMovable basetype;

	DAxisSymbol(const String& name = "");

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	vec2d lo;
	vec2d hi;

	DECLARE_DOMDATA_INFO(DAxisSymbol, DObjectInfo);
};



EW_LEAVE

#endif