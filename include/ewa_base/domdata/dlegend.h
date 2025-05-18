#ifndef __H_EW_DOMDATA_DLEGEND__
#define __H_EW_DOMDATA_DLEGEND__

#include "ewa_base/domdata/dobject.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE DLegend : public DObjectBoxMovable
{
public:
	typedef DObjectBoxMovable basetype;

	enum
	{
		FLAG_MIN				= basetype::FLAG_MAX,
		FLAG_LG_HIDE_INVISIBLE	= FLAG_MIN << 0,
		FLAG_MAX				= FLAG_MIN << 1,
	};

	DLegend(const String& name = "");

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DLegend, DObjectInfo);

};


EW_LEAVE
#endif
