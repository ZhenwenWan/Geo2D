
#ifndef __H_EW_DOMDATA_DFIGDATA__
#define __H_EW_DOMDATA_DFIGDATA__

#include "ewa_base/domdata/dobject.h"

EW_ENTER



class DLLIMPEXP_EWA_BASE FigDataAxis
{
public:

	void reset()
	{
		bbox.load_min();
		radius.load_min();
		values.load_min();
	}

	void test_point(double x, double y)
	{
		bbox.add_x(x);
		bbox.add_y(y);
	}

	void test_point(double x, double y, double z)
	{
		bbox.add_x(x);
		bbox.add_y(y);
		bbox.add_z(z);
	}

	void test_point(const vec3d& p)
	{
		bbox.add(p);
	}
	void test_radius(double r)
	{
		radius.add_x(r);
	}

	void test_value(double v)
	{
		values.add_x(v);
	}

	box3d bbox;
	tiny_box<double, 1> radius;
	tiny_box<double, 1> values;

};

class DLLIMPEXP_EWA_BASE DTipData : public DObject
{
public:

	typedef DObject basetype;

	enum
	{
		FLAG_MIN		= basetype::FLAG_MAX,
		FLAG_SHOW_MARKER= FLAG_MIN << 0,
		FLAG_SHOW_LINE	= FLAG_MIN << 1,
		FLAG_MAX		= FLAG_MIN << 5,
	};

	String value;
	vec3d v3pos;
	vec3d v3pxl;
	vec3d v3shf;


	DTipData();

	void Serialize(SerializerHelper sh);
	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DTipData, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DTipDataLine : public DTipData
{
public:

	typedef DTipData basetype;

	vec2d v2pos1, v2pos2;
	vec3d v3pos1, v3pos2;

	void DoTransferData(TableSerializer& ar);

	DTipDataLine();

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DTipDataLine, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DTipDataText : public DTipData
{
public:

	typedef DTipData basetype;

	vec2d v2pos;

	void DoTransferData(TableSerializer& ar);

	DTipDataText();

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DTipDataText, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DTipDataWithIndex : public DTipData
{
public:

	typedef DTipData basetype;

	int index;
	String fmt;

	void Serialize(SerializerHelper sh);

	DTipDataWithIndex();

	DECLARE_DOMDATA_INFO(DTipDataWithIndex, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DDataTipManager : public DObject
{
public:


	DDataTipManager();


	DataGrpT<DTipData> m_aTips;
	DataPtrT<DTipDataWithIndex> m_pTmpTip;
	DataPtrT<CallableData> m_pTipFunc;


	virtual bool DoGetChildren(XCtxChildrenState& cs);

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DDataTipManager, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DDataTipGroup : public DObject
{
public:

	DDataTipGroup();

	DataGrpT<DTipData> m_aTips;
	DataPtrT<CallableData> m_pTipFunc;

	virtual bool DoGetChildren(XCtxChildrenState& cs);

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DDataTipGroup, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DFigData : public DObject
{
public:

	typedef DObject basetype;

	enum
	{
		TYPE_NONE,
		TYPE_PLOT,
		TYPE_POLAR,
		TYPE_IMAGESC,
		TYPE_POLAR3D,
		TYPE_FIELD,
		TYPE_MESH,
	};

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_UNIT_DEG = FLAG_MIN << 0,
		FLAG_NO_LEGEND = FLAG_MIN << 1,
		FLAG_MAX =FLAG_MIN << 4,
	};

	DFigData(const String& name="data");

	uint32_t m_nDataType;
	uint32_t m_nDataVersion;
	VariantTable m_tValue;

	String m_sCwd_hint;

	DataPtrT<DDataTipManager> m_pTipManager;

	void OnDataUpdated();

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	virtual bool ExportFigData(Stream& sb);

	virtual bool get_max_v(double& max_v);

	static bool do_get_max_v(VariantTable& table, double& max_v);

	DECLARE_DOMDATA_INFO(DFigData, DObjectInfo);
};




template<typename B>
class DLLIMPEXP_EWA_BASE DFigDataCpxT : public B
{
public:
	typedef B basetype;
	DFigDataCpxT(const String& name = "data", int m = 0);
	
	arr_xt<dcomplex> m_aCvalues;
	int m_nCpxMode;

	void DoTransferData(TableSerializer& ar);

	void Serialize(SerializerHelper sh);
};



class TimeSerialDataItem
{
public:

	TimeSerialDataItem();
	TimeSerialDataItem(const String& fp,double t);

	String filename;
	double tp_time;
	uint32_t index1;
	uint32_t index2;
	double scale;
	BitFlags flags;

	void Serialize(SerializerHelper sh);

};

template<typename B>
class DLLIMPEXP_EWA_BASE DFigDataTimeT : public B
{
public:
	typedef B basetype;
	DFigDataTimeT(const String& name = "data");

	
	arr_1t<TimeSerialDataItem> m_aDatas;
	String m_sFolder;
	double tp_scaling;
	double tp_scaling_user;
	
	void DoTransferData(TableSerializer& ar);

	void Serialize(SerializerHelper sh);
};


class DLLIMPEXP_EWA_BASE PositionAndIndex
{
public:

	PositionAndIndex(){}
	PositionAndIndex(double v) :pos(v){}
	double pos;
	size_t idx;

	bool operator==(const PositionAndIndex& other) const;
};

class DLLIMPEXP_EWA_BASE PositionAndIndexArray : public arr_1t<PositionAndIndex>
{
public:
	void reset(const arr_xt<double>& arr, double lo, double hi);
};


EW_LEAVE

#endif
