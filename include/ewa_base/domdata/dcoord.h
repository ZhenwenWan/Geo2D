
#ifndef __H_EW_DOMDATA_DCOORD__
#define __H_EW_DOMDATA_DCOORD__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/dtext.h"
#include "ewa_base/domdata/dlegend.h"
#include "ewa_base/domdata/dcolormap.h"
#include "ewa_base/domdata/daxis_d.h"
#include "ewa_base/domdata/dfigdata.h"
#include "ewa_base/domdata/dfigdata_manager.h"
#include "ewa_base/domdata/daxis_symb.h"
#include "ewa_base/domdata/dunit.h"
#include "ewa_base/domdata/dsymbinfo.h"
#include "ewa_base/domdata/dattribute.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DCoord : public DObjectGroupT<DObject, DObjectBox>
{
public:

	typedef DObjectGroupT<DObject, DObjectBox> basetype;

	enum
	{
		FLAG_MIN			= basetype::FLAG_MAX,
		FLAG_DYNAMIC_FBOX	= FLAG_MIN << 0,
		FLAG_AUTO_PLAY		= FLAG_MIN << 1,
		FLAG_MAX			= FLAG_MIN << 8,
	};


	DCoord();

	bool DoAppendChild(DataPtrT<DObject> p);
	virtual bool DoReplaceLastChild(DataPtrT<DObject> p);

	DataPtrT<DUnit> p_unit;

	DataPtrT<DFigDataManager> m_pDataManager;
	DataPtrT<DText> m_pTitle;
	DataPtrT<DText> m_pXlabel;
	DataPtrT<DText> m_pYlabel;
	DataPtrT<DAxis> m_pAxis;
	DataPtrT<DColorMap> m_pColorMap;
	DataPtrT<DLegend> m_pLegend;
	DataPtrT<DAxisBox> m_pAxisBox;
	DataPtrT<DFigInfoManager> m_pInfoManager;

	box3d b3axis0, b3axis1;

	XDataStyle datastyle;


	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);
	

	DECLARE_DOMDATA_INFO(DCoord, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DCoord2D : public DCoord
{
public:

	typedef DCoord basetype;

	DCoord2D();


	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DCoord2D, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DCoord3D : public DCoord
{
public:

	typedef DCoord basetype;

	enum
	{
		FLAG_MIN		= basetype::FLAG_MAX,

		FLAG_MAX		= FLAG_MIN << 1,
	};

	int mode;

	DCoord3D(int m=0);

	void Serialize(SerializerHelper sh);

	virtual bool DoGetChildren(XCtxChildrenState& cs);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DCoord3D, DObjectInfo);


};


class DLLIMPEXP_EWA_BASE DVisualizer : public DCoord3D
{
public:
	DVisualizer();

	virtual int __getindex(Executor& ks, const String& id);

	static DataPtrT<DVisualizer> wrap(DObject* p);


	String m_sMenu;

	DECLARE_DOMDATA_INFO(DVisualizer, DObjectInfo);
};

template<typename T>
class DVisualizerT : public DVisualizer
{
public:
	T& Target;
	DVisualizerT(T& t) :Target(t){}
	virtual DObject* GetRealObject()
	{
		return &Target;
	}
};


EW_LEAVE
#endif
