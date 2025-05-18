
#ifndef __H_EW_DOMDATA_DFIGDATA_3D__
#define __H_EW_DOMDATA_DFIGDATA_3D__

#include "ewa_base/domdata/dfigdata.h"
#include "ewa_base/domdata/dmesh.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DFigData3D : public DFigData
{
public:

	typedef DFigData basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,

		FLAG_MAX = FLAG_MIN << 4,
	};

	DFigData3D(const String& name = "data");

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DFigData3D, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigData3DSimple : public DFigData3D
{
public:

	typedef DFigData3D basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_ENABLE_HEIGHT = FLAG_MIN<<0,

		FLAG_MAX = FLAG_MIN << 4,
	};

	DFigData3DSimple(const String& name = "data");

	arr_xt<double> m_aXvalues;
	arr_xt<double> m_aYvalues;
	arr_xt<double> m_aZvalues;
	arr_xt<double> m_aVvalues;

	double n_scale;
	double n_shift;
	double n_z_pos;
	
	mat4d m4;

	box2d b2box;

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	virtual bool ExportFigData(Stream& sb);

	DECLARE_DOMDATA_INFO(DFigData3DSimple, DObjectInfo);
};



class DLLIMPEXP_EWA_BASE DFigData3DBar : public DFigData3D
{
public:

	typedef DFigData3D basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_ENABLE_HEIGHT = FLAG_MIN << 0,

		FLAG_MAX = FLAG_MIN << 4,
	};

	DFigData3DBar(const String& name = "data");

	arr_xt<vec3d> m_aDatas;
	arr_xt<double> m_aWidth;
	
	mat4d m4;

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DFigData3DBar, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigData3DVector : public DFigData3D
{
public:

	typedef DFigData3D basetype;

	DFigData3DVector(const String& name = "data");

	arr_1t<vec3d> vertexs;
	arr_1t<vec3d> a_value;
	double ref_length;


	DECLARE_DOMDATA_INFO(DFigData3DVector, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigData3DSimpleCpx : public DFigDataCpxT < DFigData3DSimple >
{
public:
	typedef DFigDataCpxT < DFigData3DSimple > basetype;

	DFigData3DSimpleCpx(const String& name = "data");

	virtual bool ExportFigData(Stream& sb);

	DECLARE_DOMDATA_INFO(DFigData3DSimpleCpx, DObjectInfo);

};


class DLLIMPEXP_EWA_BASE DFigData3DSimpleTime : public DFigDataTimeT < DFigData3DSimple >
{
public:
	typedef DFigDataTimeT < DFigData3DSimple > basetype;

	DFigData3DSimpleTime(const String& name = "data");
	virtual bool ExportFigData(Stream& sb);

	DECLARE_DOMDATA_INFO(DFigData3DSimpleTime, DObjectInfo);

};



class DLLIMPEXP_EWA_BASE DFigData3DGroup : public DObjectGroupT<DObject, DFigData3D>
{
public:
	typedef DObjectGroupT<DObject, DFigData3D> basetype;
	DFigData3DGroup(const String& name = "coord3d");

	DECLARE_DOMDATA_INFO(DFigData3DGroup, DObjectInfo);
};

class DUnit;
class DLLIMPEXP_EWA_BASE DFigDataModel : public DFigData3D
{
public:

	typedef DFigData3D basetype;

	DFigDataModel();
	DFigDataModel(const String& id,DataPtrT<DObject> p,int m);

	void Serialize(SerializerHelper sh);
	bool DoGetChildren(XCtxChildrenState& cs);
	void DoTransferData(TableSerializer& ar);

	virtual DUnit* GetUnit();

	String MakeLabel(int) const;

	DataPtrT<DObject> m_pRealObject;
	DataPtrT<DObject> m_pUserObject;

	mat4d m4;

	int attr_type;

	double alpha;

	virtual bool UpdateRealObject();

	DECLARE_DOMDATA_INFO(DFigDataModel, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigDataMesh : public DFigData3D
{
public:
	typedef DFigData3D basetype;

	DataPtrT<DStlModel> p_model;

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DFigDataMesh, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DFigDataFunction : public DFigDataMesh
{
public:

	typedef DFigDataMesh basetype;

	DataPtrT<CallableData> m_pFunction;
	arr_1t<vec2d> m_aOutline;

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DFigDataFunction, DObjectInfo);

};


EW_LEAVE

#endif
