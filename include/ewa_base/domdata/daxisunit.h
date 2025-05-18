
#ifndef __H_EW_DOMDATA_DAXISUNIT__
#define __H_EW_DOMDATA_DAXISUNIT__

#include "ewa_base/domdata/dobject.h"


EW_ENTER


class DLLIMPEXP_EWA_BASE XAxisTick
{
public:
	enum
	{
		TICK_MAIN = 1,
	};
	BitFlags flags;

	XAxisTick();
	XAxisTick(double v);

	void set(double v);

	double m_nValue;
	String m_sLabel;

	int m_nType;

	void Serialize(SerializerHelper sh);
};

inline bool operator<(const XAxisTick& lhs, const XAxisTick& rhs)
{
	return lhs.m_nValue < rhs.m_nValue;
}

class DSharedAxisData : public DObject
{
public:
	
	enum
	{
		MODE_NONE,
		MODE_2D_BOX,
		MODE_2D_POLAR,
		MODE_3D_BOX,
		MODE_CYLINDER,
		MODE_3D_DIR,
		MODE_SPHERE,
	};

	enum
	{
		STYLE_MIN			= 1,
		STYLE_SHOW_LINE		= STYLE_MIN << 1,
		STYLE_SHOW_DIRS		= STYLE_MIN << 2,
		STYLE_AXIS_EQUAL		= STYLE_MIN << 3,
		STYLE_CUT_MINUS		= STYLE_MIN << 4,
		STYLE_R_LOG10		= STYLE_MIN << 5,
		STYLE_P_MINUS		= STYLE_MIN << 6,
		STYLE_P_MIRROR		= STYLE_MIN << 7,
		STYLE_MAX			= STYLE_MIN << 8,
	};

	int m_nAxisMode;

	BitFlags style;
	double rmin;
	double rdeg;
	double amin;

	DSharedAxisData();

	inline double adj_angle(double a)
	{
		if (style.get(DSharedAxisData::STYLE_P_MIRROR))
		{
			return 360.0 - a + amin;
		}
		else
		{
			return a + amin;
		}
	}

	inline double adj_radius(double r)
	{

		if (style.get(STYLE_R_LOG10))
		{
			if (r > 0.0)
			{
				r = 10.0*::log10(r);
			}
			else
			{
				r = -1000.0;
			}
		}

		r -= rmin;

		if (r < 0.0 && style.get(STYLE_CUT_MINUS))
		{
			r = 0.0;
		}
		return r;
	}

	DECLARE_DOMDATA_INFO(DSharedAxisData, DObjectInfo);

private:
	BitFlags flags;
};


class DLLIMPEXP_EWA_BASE DAxisItem : public DObject
{
public:
	typedef DObject basetype;

	DAxisItem(const String& name);

	DataPtrT<DSharedAxisData> pdata;

};

// 标尺
class DLLIMPEXP_EWA_BASE DAxisScale : public DAxisItem
{
public:
	DAxisScale(const String& id = empty_string) : DAxisItem(id) {}
	DECLARE_DOMDATA_INFO(DAxisScale, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DAxisLine : public DAxisItem
{
public:
	typedef DAxisItem basetype;
	DAxisLine(const String& name = "");
	DECLARE_DOMDATA_INFO(DAxisLine, DObjectInfo);
};



class DLLIMPEXP_EWA_BASE DAxisUnit : public DAxisItem
{
public:

	typedef DAxisItem basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_SHOW_MESH_MAIN = FLAG_MIN << 0,
		FLAG_SHOW_MESH_USER = FLAG_MIN << 1,
		FLAG_MAX			= FLAG_MIN << 3,
	};

	DAxisUnit(const String& name = "");


	arr_1t<XAxisTick> ticks;

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);
};

class DLLIMPEXP_EWA_BASE DAxisBasic : public DAxisUnit
{
public:
	typedef DAxisUnit basetype;


	DAxisBasic(const String& name="",int d=0);
	int m_nDirection;

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_LABEL_DIR_MAX = FLAG_MIN << 0,
		FLAG_MAX = FLAG_MIN << 1,
	};

	DECLARE_DOMDATA_INFO(DAxisBasic, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DAxisSphere : public DAxisUnit
{
public:
	typedef DAxisUnit basetype;

	DAxisSphere(const String& name = "");
	void Serialize(SerializerHelper sh);
	void DoTransferData(TableSerializer& ar);


	DECLARE_DOMDATA_INFO(DAxisSphere, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DAxisPolar : public DAxisUnit
{
public:

	typedef DAxisUnit basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_MINUS_ANGLE = FLAG_MIN << 0,
		FLAG_MAX = FLAG_MIN << 3,
	};


	DAxisPolar(const String& name="");


	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DAxisPolar, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DAxisRadius : public DAxisUnit
{
public:

	typedef DAxisUnit basetype;

	DAxisRadius(const String& name = "");

	double m_nRadiusTextAngle; //deg

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_DOMDATA_INFO(DAxisRadius, DObjectInfo);
};



EW_LEAVE

#endif
