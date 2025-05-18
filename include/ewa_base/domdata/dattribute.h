#ifndef __H_EW_DOMDATA_DATTRIBUTE__
#define __H_EW_DOMDATA_DATTRIBUTE__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/table_serializer.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE AttributeUpdator;

class DLLIMPEXP_EWA_BASE DAttributeManager : public DObject
{
public:

	DAttributeManager();
	indexer_map<String, DataPtrT<DAttribute> > m_mapAttributes;

	void SetStyle(DataPtrT<DAttribute>& p);

	int n_data_version;

	void Serialize(SerializerHelper sh);

	static DataPtrT<DAttribute> MakeStyle(const String& name);

	DECLARE_DOMDATA_INFO(DAttributeManager, DObjectInfo);

protected:
	static DataPtrT<DAttribute> MakeWhiteBackgroundStyle();
	static DataPtrT<DAttribute> MakeBlackBackgroundStyle();
	static DataPtrT<DAttribute> MakeBackgroundStyle(DColor bg_color,DColor fg_color);


	int n_last_version;
};

class GLStyleData : public ObjectData
{
public:
	GLStyleData()
	{
		line_index = -1;
		data_count = 0;
	}

	int line_index;
	int data_count;
	DLineStyle line_style;
	DMarkerStyle marker_style;
	DataPtrT<DAttribute> p_attribute;
	BitFlags flags;
};


class DLLIMPEXP_EWA_BASE XDataStyle
{
public:
	XDataStyle();

	void Serialize(SerializerHelper sh);

	String name;
	int32_t size;
	BitFlags flags;
	int32_t marker_mode;
};


class DLLIMPEXP_EWA_BASE DStyleUpdator : public ObjectData
{
public:
	virtual void Update(GLStyleData* pstyle, size_t, XDataStyle&);

	static DStyleUpdator& get(const String& name);
	static arr_1t<String> get_names();
};



class DLLIMPEXP_EWA_BASE AttributeUpdator
{
public:

	void SetManager(DAttributeManager* pmgr);
	void SetObject(DObject* pobj);

	bool TestVersion(int& v);

	void Update(const String& name, String& value);
	void Update(const String& name, int& value);
	void Update(const String& name, double& value);
	void Update(const String& name, float& value);
	void Update(const String& name, DColor& value);
	void Update(const String& name, DLineStyle& value);
	void Update(const String& name, DMarkerStyle& value);

	void Update(const String& name, DFontStyle& value);
	void Update(const String& name, vec3d& value);
	void Update(const String& name, box3d& value);


	void ResetIndex();
	void ResetVersion();

protected:

	void Update2(const String& name, DLineStyle& value,const String& style,size_t i,size_t n);
	void Update2(const String& name, DMarkerStyle& value,const String& style,size_t i,size_t n);

	int nIndex;

	void DoAppend(DAttribute* p);
	DataPtrT<DAttributeManager> m_pAttributeManager;

	void DoUpdateInternal(const String& name);
	arr_1t<indexer_map<String, DAttribute::Item>*> aLinks;


	template<typename T>
	void DoUpdateType(const String& name, T& value);


};



class DLLIMPEXP_EWA_BASE TableSerializer;

class DLLIMPEXP_EWA_BASE AttributeSerializer
{
public:

	TableSerializer& ar;
	DataPtrT<DAttribute> pattribute;

	AttributeSerializer(TableSerializer& a, DataPtrT<DAttribute>& p) :ar(a)
	{
		if (!p) p.reset(new DAttribute);
		pattribute = p;
	}


	template<typename T>
	void link(const String& n, const T& v)
	{
		auto& item(pattribute->values[n]);
		if (item.type.empty())
		{
			item.set(v);
			item.state = 0;
		}

		ar.link(n + ".value", item.value);
		ar.link(n + ".state", item.state);
	}

	void link_fontstyle(const String& name)
	{
		static DFontStyle style;
		link(DAttribute::MakeName(name, "nsize"), style.nsize);
		link(DAttribute::MakeName(name, "sname"), style.sname);
		link(DAttribute::MakeName(name, "color"), style.color);
		link(DAttribute::MakeName(name, "flags"), style.flags);
	}

	void link_linestyle(const String& name)
	{
		static DLineStyle style;
		link(DAttribute::MakeName(name, "nsize"), style.nsize);
		link(DAttribute::MakeName(name, "ntype"), style.ntype);
		link(DAttribute::MakeName(name, "color"), style.color);
	}

	void link_markerstyle(const String& name)
	{
		static DMarkerStyle style;
		link(DAttribute::MakeName(name, "nsize"), style.nsize);
		link(DAttribute::MakeName(name, "ntype"), style.ntype);
		link(DAttribute::MakeName(name, "color"), style.color);
	}

};

EW_LEAVE
#endif
