#ifndef __H_EW_DOMDATA_DUNIT__
#define __H_EW_DOMDATA_DUNIT__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/symm.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE KUnit
{
public:
	KUnit();

	double ul, ut, uf;

	BitFlags flags;

	KUnit& operator/=(const KUnit& ku)
	{
		ul /= ku.ul;
		ut /= ku.ut;
		uf /= ku.uf;

		return *this;
	}

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		ar & ul & ut & uf & flags;
	}
};

class DLLIMPEXP_EWA_BASE XUnitType
{
public:
	arr_1t < std::pair<double, String> > choices;

	XUnitType();

	static XUnitType& get(const String& t);

};


class DLLIMPEXP_EWA_BASE XUnit
{
public:
	XUnit(const String& u) :type(0), udata(XUnitType::get(u)){}

	void Serialize(Serializer& ar)
	{
		ar & type & expr;
	}

	String expr;
	int type;

	XUnitType& udata;
};

class DLLIMPEXP_EWA_BASE DUnit : public DObject
{
public:

	typedef DObject basetype;

	DUnit();

	XUnit ul, ut, uf;

	BitFlags flags;

	KUnit ku;

	virtual bool DoUpdateValue(DState& dp);
	virtual void DoTransferData(TableSerializer& ar);

	String GetPrefix() const {return "unit"; }

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DUnit, DObjectInfo)

};



EW_LEAVE

#endif
