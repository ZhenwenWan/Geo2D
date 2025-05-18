#ifndef __H_EW_DOMDATA_DTEXT__
#define __H_EW_DOMDATA_DTEXT__

#include "ewa_base/domdata/dobject.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE DText : public DObjectBoxMovable
{
public:
	typedef DObjectBoxMovable basetype;


	DText(const String& name = "");

	void Serialize(SerializerHelper sh);
	void DoTransferData(TableSerializer& ar);


	String m_sText;

	DECLARE_DOMDATA_INFO(DText, DObjectInfo);

};



EW_LEAVE

#endif
