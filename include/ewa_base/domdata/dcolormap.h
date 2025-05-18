#ifndef __H_EW_DOMDATA_DCOLORMAP__
#define __H_EW_DOMDATA_DCOLORMAP__

#include "ewa_base/domdata/dobject.h"

EW_ENTER




template<typename T>
inline void linpl(T &res, T &v1, T &v2, double p1)
{
	res = v1*p1 + v2*(1.0 - p1);
}


template<>
inline void linpl(DColor &res, DColor &v1, DColor &v2, double p1)
{
	double p2 = 1.0 - p1;
	res.r = (unsigned char)(p1*v1.r + p2*v2.r);
	res.g = (unsigned char)(p1*v1.g + p2*v2.g);
	res.b = (unsigned char)(p1*v1.b + p2*v2.b);
}

template<typename T1, typename T2>
class mapper_t
{
public:

	typedef std::pair<T1, T2> nodetype;

	typedef typename arr_1t<nodetype>::iterator iterator;

	mapper_t()
	{
		vc = 0.0;
		wd = 1.0;
		dipole = true;
	}

	inline bool get(T1 val, void *p)
	{
		return get(val, *(T2 *)p);
	}

	inline bool get(T1 v, T2 &color)
	{

		if (!dipole && v<T1(0))
		{
			v = -v;
		}

		T1 val = (v - vc) / wd;

		size_t size = keynodes.size();
		if (size == 0) return false;


		nodetype *p = &keynodes[0];

		if (val < p->first)
		{
			color = p->second;
			return true;
		}

		for (unsigned i = 1; i < size; i++)
		{
			p++;
			if (val < p->first)
			{

				double r1 = ((p->first - val) / (p->first - (p - 1)->first));
				linpl(color, (p - 1)->second, p->second, r1);
				return true;
			}
		}
		color = (p)->second;
		return true;
	}

	inline T2 get(T1 val)
	{
		T2 v;
		get(val, v);
		return v;
	}

	void insert(T1 val, T2 color)
	{
		keynodes.push_back(nodetype(val, color));
	}

	iterator begin()
	{
		return keynodes.begin();
	}

	iterator end()
	{
		return keynodes.end();
	}

	void remove(iterator it)
	{
		keynodes.remove(it);
	}

	void scale(double d)
	{
		wd *= d;
	}


	T1 vc;
	T1 wd;
	bool dipole;

	void swap(mapper_t& other)
	{
		other.keynodes.swap(keynodes);
		std::swap(vc, other.vc);
		std::swap(wd, other.wd);
		std::swap(dipole, other.dipole);
	}

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		ar & keynodes & vc & wd & dipole;
	}

	arr_1t<nodetype> keynodes;

};


template<>
mapper_t<double, DColor>::mapper_t();

typedef mapper_t<double, DColor> XColorMap;

class XColorItem
{
public:
	XColorItem();
	XColorItem(const std::pair<double, DColor>& item);

	double value;
	String color;

	void Serialize(SerializerHelper sh);


	void SerializeVariant(Variant& v, int dir);
};
 
class DLLIMPEXP_EWA_BASE DColorMap : public DObjectBoxMovable
{
public:
	typedef DObjectBoxMovable basetype;

	enum
	{
		FLAG_MIN			= basetype::FLAG_MAX,
		FLAG_SYMMETRY		= FLAG_MIN << 1,
		FLAG_COLORMAP_AUTO	= FLAG_MIN << 2,

		FLAG_MAX			= FLAG_MIN << 4,
	};


	enum
	{
		COLORMAP_MODE_AUTO,
		COLORMAP_MODE_MANUAL,
		COLORMAP_MODE_CUSTOM,
	};

	DColorMap(const String& name = "");

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	bool IsAuto() const;

	int mode;
	double tmp_min, tmp_max;
	arr_1t<XColorItem> tmp_arr;

	int m_nPositionMode;

	XColorMap colormap;

	uint32_t m_nDataVersion;

	void OnDataUpdated();

	enum
	{
		MODE_FIXED_BBOX_RIGHT,
		MODE_FLY,
	};


	DECLARE_DOMDATA_INFO(DColorMap, DObjectInfo);

};


EW_LEAVE

namespace std 
{
	template<typename T1, typename T2>
	void swap(ew::mapper_t<T1,T2>& v1, ew::mapper_t<T1,T2>& v2)
	{
		v1.swap(v2);
	}
}

#endif
