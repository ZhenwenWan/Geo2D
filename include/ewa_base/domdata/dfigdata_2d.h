
#ifndef __H_EW_DOMDATA_DFIGDATA_2D__
#define __H_EW_DOMDATA_DFIGDATA_2D__

#include "ewa_base/domdata/dfigdata.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DFigData2D : public DFigData
{
public:

	typedef DFigData basetype;

	DFigData2D(const String& name = "data");


	DECLARE_DOMDATA_INFO(DFigData2D, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DFigData2DFunction : public DFigData2D
{
public:

	typedef DFigData2D basetype;

	DFigData2DFunction(const String& name = "data");


	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DFigData2DFunction, DObjectInfo);
};




class DLLIMPEXP_EWA_BASE DFigData2DSimple : public DFigData2D
{
public:

	typedef DFigData2D basetype;

	DFigData2DSimple(const String& name = "data");

	arr_xt<double> m_aXvalues;
	arr_xt<double> m_aVvalues;
	arr_xt<uint32_t> m_aMarker;


	virtual bool DoGetChildren(XCtxChildrenState& cs);


	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	virtual bool ExportFigData(Stream& sb);

	DECLARE_DOMDATA_INFO(DFigData2DSimple, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigData2DSimpleCpx : public DFigDataCpxT < DFigData2DSimple >
{
public:
	typedef DFigDataCpxT < DFigData2DSimple > basetype;

	DFigData2DSimpleCpx(const String& name = "data");

	void DoTransferData(TableSerializer& ar);

	virtual bool ExportFigData(Stream& sb);

	DECLARE_DOMDATA_INFO(DFigData2DSimpleCpx, DObjectInfo);
};

EW_LEAVE

#endif
