#ifndef __H_EW_DOMDATA_DTABLE__
#define __H_EW_DOMDATA_DTABLE__

#include "ewa_base/domdata/dobject.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DFigArray : public DObjectGroupT<DObject, DObject>
{
public:
	typedef DObjectGroupT<DObject, DObject> basetype;

	DFigArray(const String& name = "") :basetype(name){}

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DFigArray, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DGridSizer : public DFigArray
{
public:

	typedef DFigArray basetype;

	DGridSizer(const String& name = "", const vec2i& g = vec2i(-1, 1));

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	vec2i grid;

	DECLARE_DOMDATA_INFO(DGridSizer, DObjectInfo);

};


EW_LEAVE

#endif
