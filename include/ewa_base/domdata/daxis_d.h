#ifndef __H_EW_DOMDATA_DAXIS_D__
#define __H_EW_DOMDATA_DAXIS_D__

#include "ewa_base/domdata/daxisunit.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DAxis : public DObjectGroupT<DObject, DObject>
{
public:

	typedef DObjectGroupT<DObject, DObject> basetype;

	class Item
	{
	public:

		enum
		{
			FLAG_AUTO	=1<<0,
			FLAG_LOG10	=1<<1,
		};

		enum
		{
			TYPE_NORMAL,
			TYPE_ANGLE,
			TYPE_RADIUS,
		};

		Item()
		{
			type = TYPE_NORMAL;
			flags.add(FLAG_AUTO);
			vmin = -1.0;
			vmax = +1.0;
		}

		int type;
		BitFlags flags;
		String func;
		double vmin, vmax;

		void Serialize(SerializerHelper sh)
		{
			Serializer& ar(sh.ref(0));
			ar & type & flags & vmin & vmax & func;
		}


		void reset()
		{
			if (!flags.get(FLAG_AUTO)) return;
			vmin = +std::numeric_limits<double>::max();
			vmax = -std::numeric_limits<double>::max();
		}

		void testd(double v)
		{
			if (!flags.get(FLAG_AUTO)) return;
			if (vmin > v) vmin = v;
			if (vmax < v) vmax = v;
		}

		void testd(double* p, size_t n)
		{
			if (!flags.get(FLAG_AUTO)) return;
			for (size_t i = 0; i<n; i++)
			{
				double v = p[i];
				if (vmin > v) vmin = v;
				if (vmax < v) vmax = v;
			}
		}

		bool testv(double v)
		{
			if (flags.get(FLAG_AUTO)) return true;
			return v >= vmin && v <= vmax;
		}

		bool is_auto()
		{
			return flags.get(FLAG_AUTO) || !(vmax >= vmin);
		}

		bool is_valid()
		{
			return vmax >= vmin;
		}

	};

	class Data
	{
	public:
		Item x, y, z, r, v, t;

		void Serialize(SerializerHelper sh)
		{
			Serializer& ar(sh.ref(0));
			ar & x & y & z & r & v & t;
		}

		Item& operator[](size_t i)
		{
			EW_ASSERT(i < 6);
			return (&x)[i];
		}

		void reset()
		{
			for (size_t i = 0; i < 6; i++)
			{
				(*this)[i].reset();
			}
		}

		void testd(const vec3d& v)
		{
			if (x.testv(v[0]) && y.testv(v[1]))
			{
				z.testd(v[2]);
			}
			if (x.testv(v[0]) && z.testv(v[2]))
			{
				y.testd(v[1]);
			}
			if (z.testv(v[2]) && y.testv(v[1]))
			{
				x.testd(v[0]);
			}
		}

		void testd(const vec2d& v)
		{
			if (x.testv(v[0]))
			{
				y.testd(v[1]);
			}
			if (y.testv(v[1]))
			{
				x.testd(v[0]);
			}
		}

		void testp(double rr, double)
		{
			r.testd(rr);
		}

		void testp(double rr, double,double)
		{
			r.testd(rr);
		}

	};

	Data xyzrvt;

	DAxis(const String& name=_kT("axis"));


	virtual bool DoGetChildren(XCtxChildrenState& cs);

	bool IsEqual();

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);
	void DoTransferData_coord(TableSerializer& ar);

	void DoTransferData_item(TableSerializer& ar, Item& item, const String& suffix);

	int GetMode(){ return pdata->m_nAxisMode; }
	void SetMode(int d){ pdata->m_nAxisMode = d; }

	DataPtrT<DSharedAxisData> pdata;

	DataGrpT<DObject> sub_objects;

protected:

	void SetCurrentMode(int mode);
	int m_nCurrentMode;


	DECLARE_DOMDATA_INFO(DAxis, DObjectInfo);
};

class DAxisBox : public DObject
{
public:
	typedef DObject basetype;
	DAxisBox(const String& name = "");
	DECLARE_DOMDATA_INFO(DAxisBox, DObjectInfo);
};

EW_LEAVE

#endif