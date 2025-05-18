
#ifndef __H_EW_DOMDATA_DOBJECT__
#define __H_EW_DOMDATA_DOBJECT__


#include "ewa_base/basic/object.h"
#include "ewa_base/math/math_def.h"
#include "ewa_base/scripting/variant.h"

#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_table.h"

#include "ewa_base/domdata/xcolor.h"



#define DECLARE_DOMDATA_INFO(TYPE,INFO) DECLARE_OBJECT_INFO_REAL(TYPE,INFO,2)


EW_ENTER

class VariantTable;
class Executor;
class DLLIMPEXP_EWA_BASE DState;
class DLLIMPEXP_EWA_BASE DContext;
class DLLIMPEXP_EWA_BASE XCtxChildrenState;
class DLLIMPEXP_EWA_BASE TableSerializer;

template<typename T>
class get_this_ref_heler;



class DLLIMPEXP_EWA_BASE DFontStyle
{
public:

	DFontStyle();
	DFontStyle(float size);

	String sname;
	float nsize;
	BitFlags flags;
	DColor color;

	bool operator==(const DFontStyle& rhs) const;
	bool operator!=(const DFontStyle& rhs) const;

	enum
	{
		STYLE_VERTICAL = 1 << 0,
		STYLE_ITALIC = 1 << 1,
		STYLE_UNDERLINE = 1 << 2,
		STYLE_STRIDE = 1 << 3,
	};

	void Serialize(SerializerHelper sh);

private:
	void _init();
};


class DLLIMPEXP_EWA_BASE DLineStyle
{
public:

	enum
	{
		LINE_NONE,
		LINE_SOLID,
		LINE_DOT1,
		LINE_DOT2,
		LINE_DASH1,
		LINE_DASH2,
		LINE_DASH3,
		LINE_DASH4,
	};

	DLineStyle();
	DLineStyle(double w);

	void set(int t, float w, const DColor& c);
	void set(int t, float w = 1.0);

	int ntype;
	float nsize;
	DColor color;

	void Serialize(SerializerHelper sh);
};


class DLLIMPEXP_EWA_BASE DMarkerStyle
{
public:

	enum
	{
		MAKER_STYLE_NONE,
		MAKER_STYLE_QUAD,
		MAKER_STYLE_CIRCLE,
		MAKER_STYLE_TRIANGLE,
		MAKER_STYLE_HL_QUAD,
		MAKER_STYLE_HL_CIRCLE,
		MAKER_STYLE_HL_TRIANGLE,
		MAKER_STYLE_STAR,
	};

	DMarkerStyle();
	DMarkerStyle(double w);

	void set(int t, float w, const DColor& c);
	void set(int t, float w = 1.0);

	int ntype;
	float nsize;
	DColor color;

	void Serialize(SerializerHelper sh);
};



class DLLIMPEXP_EWA_BASE DLight
{
public:
	DLight();
	vec4f v4ambient;
	vec4f v4diffuse;
	vec4f v4specular;
	vec4f v4position;

	int light_index;

	void Serialize(SerializerHelper sh);
};


class DLLIMPEXP_EWA_BASE DExprItem
{
public:
	String name,value,desc;

	DExprItem(const String& n, const String& v, const String& d = "");
	DExprItem();

	void SerializeVariant(Variant& v,int dir);
	void Serialize(SerializerHelper sh);

	bool operator==(const DExprItem& rhs) const;
	bool operator!=(const DExprItem& rhs) const;
};



template<>
class hash_t<DExprItem>
{
public:
	inline uint32_t operator()(const DExprItem& val)
	{
		hash_t<String> h2;
		return h2(val.name)^h2(val.value)^h2(val.desc);
	}
};

template<>
class hash_t<DColor> : public hash_pod<DColor>{};

template<>
class hash_t<RGBValue> : public hash_pod<RGBValue>{};


DEFINE_OBJECT_NAME(DExprItem,"expritem");




class DLLIMPEXP_EWA_BASE DMatrixBox
{
public:

	DMatrixBox();
	DMatrixBox(const mat4d& m);


	mat4d m4;
	box3d b3;

	void Serialize(SerializerHelper sh);
};



class DLLIMPEXP_EWA_BASE DAttribute : public CallableObject
{
public:

	class Item
	{
	public:

		Item() :state(1){}

		String type;
		String value;
		int state;

		void set(int v){ type = "int"; value = String::Format("%d",v); state = 1; }
		void set(double v){type = "double";value = String::Format("%.17g", v);; state = 1;}
		void set(const String& v){type = "string";value = v; state = 1;}

		void set(float v){set(double(v));}
		void set(DColor v){ set(v.cast_to_string()); }
		void set(BitFlags v){set(v.val());}

		void get(int& v)
		{
			if (state == 1) value.ToNumber(&v);
		}
		void get(float& v)
		{
			if (state == 1) value.ToNumber(&v);
		}
		void get(double& v)
		{
			if (state == 1) value.ToNumber(&v);
		}
		void get(String& v)
		{
			if (state == 1) v = value;
		}
		void get(DColor& v)
		{
			if (state == 1) v.set_by_string(value);
		}
		void get(BitFlags& v)
		{
			if (state == 1) value.ToNumber((int*)&v);
		}

		void Serialize(Serializer& ar){ ar & type & value & state; }
	};

	typedef indexer_map<String, Item> map_type;

	DAttribute() :n_data_version(0){}
	DAttribute(const DAttribute& o) :values(o.values),n_data_version(0){}

	map_type values;
	int n_data_version;

	virtual int __getindex(Executor&, const String&);

	static String MakeName(const String& name, const String& sub);

	void SetValue(const String& name, int value);
	void SetValue(const String& name, float value);
	void SetValue(const String& name, double value);
	void SetValue(const String& name, const String& value);
	void SetValue(const String& name, DColor value);
	void SetValue(const String& name, BitFlags value);
	void SetValue(const String& name, const DLineStyle& value);
	void SetValue(const String& name, const DMarkerStyle& value);
	void SetValue(const String& name, const DFontStyle& value);

	void SetValue(const String& name, const vec3d& value);
	void SetValue(const String& name, const box3d& value);


	void GetValue(const String& name, int& value);
	void GetValue(const String& name, float& value);
	void GetValue(const String& name, double& value);
	void GetValue(const String& name, String& value);
	void GetValue(const String& name, DColor& value);
	void GetValue(const String& name, BitFlags& value);
	void GetValue(const String& name, DLineStyle& value);
	void GetValue(const String& name, DMarkerStyle& value);
	void GetValue(const String& name, DFontStyle& value);

	void GetValue(const String& name, vec3d& value);
	void GetValue(const String& name, box3d& value);


	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DAttribute, ObjectInfo)

};


class DLLIMPEXP_EWA_BASE DObjectInfo : public ObjectInfo
{
public:
	DObjectInfo(const String& s = "");
	~DObjectInfo();

	enum
	{
		FLAG_PROPMAP_ENABLED =1<<0,
	};

	VariantTable props;
	bst_map<String, String> propmap;
	bst_map < String, DataPtrT <ObjectData> > helper;
	BitFlags flags;

	virtual Object* GetHelperObject(const String&);

};


enum
{
	DPHASE_NIL = 0,
	DPHASE_VAR = 1,
	DPHASE_SHP = 2,
	DPHASE_ACT = 3,
	DPHASE_VAL = 4,
	DPHASE_CHK = 5,

	DPHASE_MIN = DPHASE_VAR,
	DPHASE_MAX = DPHASE_CHK,
};

class DLLIMPEXP_EWA_BASE ObjectVisitorState;
class DLLIMPEXP_EWA_BASE DNodeInfoCtx;
class DLLIMPEXP_EWA_BASE XDocumentData;
class DLLIMPEXP_EWA_BASE SymbolManager;

class DLLIMPEXP_EWA_BASE DEstimateCtx : public Object
{
public:
	DEstimateCtx(const vec3i &v, size_t n, size_t d);

	vec3i v3_grid_size;
	size_t n_time_step;
	size_t n_floating_size;

	double n_face_size;

	double estimate_memory;
	double estimate_time;
};


class XCtxSuffix
{
public:

	XCtxSuffix(const String& s)
	{
		set_suffix(s);
	}

	String suffix;

	void set_suffix(const String& s)
	{
		suffix = s;
		if (!s.empty() && s.c_str()[0] != '#')
		{
			suffix = "#" + suffix;
		}
	}

	String apply(const String& val) const
	{
		return val + suffix;
	}
};


class DLLIMPEXP_EWA_BASE DExtraInfoCtx;

class DLLIMPEXP_EWA_BASE DBindExtraCtx
{
public:
	Variant fn_extra;
	mat4d m4_extra;
};

class DLLIMPEXP_EWA_BASE DDecorateCtx
{
public:

	DDecorateCtx() 
	{
		_init();
	}

	DDecorateCtx(const mat4d& m) :m4(m) 
	{
		_init();
	}

	mat4d m4;
	BitFlags flags;
	double fmin;
	double fmax;

	void _init()
	{
		fmin = fmax = -1.0;
	}

};

class DLLIMPEXP_EWA_BASE DVersionCtx
{
public:

	DVersionCtx()
	{
		version = 0;
		cached = -1;
	}

	void update()
	{
		version++;
	}

	bool is_updated()
	{
		return cached != version;
	}

	bool set_flag(bool f)
	{
		cached = version;
		flag = f;
		return flag;
	}

	int version;
	int cached;
	bool flag;
};

#define EW_NEED_VERSION_CTX()	\
DVersionCtx version_ctx;\
void OnDataUpdated() { version_ctx.update(); basetype::OnDataUpdated();}

class DLLIMPEXP_EWA_BASE XSmUpgradeCtx
{
public:

	XSmUpgradeCtx(const String& prjfile);
	XSmUpgradeCtx(int vsrc, int vdst);

	int src_version;
	int dst_version;

	String fn_folder;
	String fn_extra;

	void set_project(const String& project);

	String copy_data_file(const String& sid, const String& src);

	VariantTable props;
	SymbolManager* p_mgr;
};


class DLLIMPEXP_EWA_BASE DObject : public CallableObject
{
public:

	DObject(const String& s = "");

#ifdef EW_C11
	DObject(const DObject&) = default;
#endif

	enum
	{
		FLAG_MIN		=1<<0,
		FLAG_UPDATED	=FLAG_MIN<<0,
		FLAG_ERROR		=FLAG_MIN<<1,
		FLAG_READONLY	=FLAG_MIN<<2,
		FLAG_SHOWABLE	=FLAG_MIN<<3,
		FLAG_NO_CMDPROC	=FLAG_MIN<<4,
		FLAG_IS_VISIBLE	=FLAG_MIN<<5,
		FLAG_TRANSLATE	=FLAG_MIN<<6,
		FLAG_TRANSLATE2	=FLAG_MIN<<7,
		FLAG_MAX		=FLAG_MIN<<8,


		FLAG_INIT_CLOSE	=1<<27,
		FLAG_INIT_EXPAND=1<<28,
		FLAG_ICON_NAME	=1<<29,
		FLAG_REF_UPDATED=1<<30,

	};

	String m_sId;
	mutable DataPtrT<DAttribute> m_pAttribute;
	BitFlags flags;
	int n_dp_level;

	virtual void SetId(const String& s);
	virtual const String& GetId() const;

	virtual String MakeLabel(int hint) const;

	void Serialize(SerializerHelper sh);

	virtual bool DoUpdateValue(DState&);
	virtual bool DoCheckParam(DState&);


	// 2018.02.22 handwd: DoTransferData 返回值由bool改为void，使用异常报错
	virtual void DoTransferData(TableSerializer& ar);

	virtual void DoRender(DContext&);
	virtual bool DoGetChildren(XCtxChildrenState&);
	virtual bool DoAppendChild(DataPtrT<DObject>);

	virtual void OnUpgrade(XSmUpgradeCtx& ctx);

	virtual DObject* DecorateWithM4(const DDecorateCtx&);

	virtual DObject* BindExtra(DBindExtraCtx&);

	virtual String GetPrefix() const;

	virtual DObject* GetRealObject();

	virtual void AcceptObject(ObjectVisitorState& vs);

	// 输出元件的附加信息（适合用户阅读的内容条目，元件的核心参数）
	virtual void GetSymbolInfo(DNodeInfoCtx& nc);

	// 2018.09.03 handwd: 这个对象的内部数据已经被更新后调用，主要用于通过设置特殊标识的方式通知界面等刷新缓存数据（如果有的话）
	virtual void OnDataUpdated();


	virtual void DoEstimate(DEstimateCtx& dm, const mat4d& m4);

	virtual void DoGetExtraInfo(DExtraInfoCtx& ctx);


	virtual bool DoCallFunction(const String& func, const Variant* p, size_t n);

	bool CallFunction(const String& func);
	bool CallFunction(const String& func, const Variant& var);
	bool CallFunction(const String& func, const String& v);
	bool CallFunction(const String& func, int v);

	virtual DObject* DoGetActiveItem();

	virtual bool IsDecorator() const;
	virtual bool IsGroup() const;
	virtual bool IsType(const String& type) const;
	virtual void GetIcon(XCtxGetImage& ctx) const;

	virtual int __getindex(Executor&, const String&); // top.s
	virtual int __setindex(Executor& ks, const String& id);

	virtual int GetRenderMode() const;

	virtual DObjectInfo& GetObjectInfoVirtual();

	virtual VariantTable* get_tb1();

	DECLARE_OBJECT_INFO_REAL(DObject, DObjectInfo,2);
};




class DLLIMPEXP_EWA_BASE XCtxChildrenState
{
public:


	XCtxChildrenState()
	{
		m_parray = &m_internal;
	}

	XCtxChildrenState(const XCtxChildrenState& o) :parent(o.parent), pdoc(o.pdoc), pextra(o.pextra)
	{
		m_parray = &m_internal;
	}

	typedef arr_1t<DataPtrT<DObject> > grp_type;

	void assign(grp_type::iterator t1, grp_type::iterator t2)
	{
		m_parray->assign(t1, t2);
		ptable = NULL;
	}

	grp_type::iterator begin()
	{
		return m_parray->begin();
	}

	grp_type::iterator end()
	{
		return m_parray->end();
	}

	void append(DObject* p)
	{
		if (!p) return;
		m_parray->append(p);
		ptable = NULL;
	}

	template<typename T>
	void append(const DataPtrT<T>& p)
	{
		if (!p) return;
		m_parray->append((T*)p.get());
		ptable = NULL;
	}

	size_t size() const
	{
		return m_parray->size();
	}

	bool empty() const
	{
		return m_parray->empty();
	}

	DataPtrT<DObject>& operator[](size_t i)
	{
		return (*m_parray)[i];
	}

	void clear()
	{
		m_internal.clear();
		m_parray = &m_internal;
		ptable.reset(NULL);
	}

	template<typename T>
	void set_array(arr_1t<DataPtrT<T> >& p)
	{
		m_parray = (grp_type*)&p;
		ptable = NULL;
	}

	void set_array(grp_type& p)
	{
		m_parray = &p;
		ptable = NULL;
	}

	void set_item(DObject* p)
	{
		clear();
		m_internal.append(p);
		ptable = NULL;
	}

	bool test(grp_type& arr)
	{
		auto& src(*m_parray);
		if (arr != src)
		{
			arr = src;
			return true;
		}
		return false;
	}

	void set_filter(const String& name,int flag=0);

	bool filter(const String& name);

	LitePtrT<DObject> parent;
	DataPtrT<DObject> pdoc;
	DataPtrT<ObjectData> pextra;
	LitePtrT<VariantTable> ptable;

private:
	grp_type* m_parray;
	grp_type m_internal;

};

class DLLIMPEXP_EWA_BASE DGroup : public DObject
{
public:

	typedef DObject basetype;

	enum
	{
		FLAG_MIN = basetype::FLAG_MAX,
		FLAG_HIDE_EMPTY = FLAG_MIN << 1,
		FLAG_MAX = FLAG_MIN << 5,
	};

	DGroup(const String& name = "");

	DGroup(const DGroup&);

	bool DoGetChildren(XCtxChildrenState& cs);

	virtual int __setindex(Executor& ks, const String& id);

	DataGrpT<DObject> values;
	String image;

	void GetIcon(XCtxGetImage& ctx) const;

	String m_sMenu;

	void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO_REAL(DGroup, DObjectInfo, 2);
};


class DGroupTable : public DObject
{
public:
	typedef DObject basetype;

	DGroupTable(const String& name = "");

	void Serialize(SerializerHelper sh);

	VariantTable& get_table();


	VariantTable* get_table_ptr();

	bool DoGetChildren(XCtxChildrenState&);

	virtual int __setindex(Executor& ks, const String& id);
	virtual int __getindex(Executor& ks, const String& id);

	virtual void __get_iterator(Executor&, int);

	String image;
	Variant value;

	void GetIcon(XCtxGetImage& ctx) const;

	DECLARE_OBJECT_INFO_REAL(DGroupTable, DObjectInfo, 2);
};


class DLLIMPEXP_EWA_BASE DFigInfoManager : public DGroup
{
public:

	typedef DGroup basetype;

	DFigInfoManager();

	DFigInfoManager(const DFigInfoManager&);


	virtual bool DoGetChildren(XCtxChildrenState& cs);

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DFigInfoManager, DObjectInfo);
};


class BoxData
{
public:
	vec3d v3pos;
	vec3d v3pxl;
	vec3d v3shf;
	box3d b3margin;
	vec3d v3size;

	int ntype;

	BoxData();

	void Serialize(SerializerHelper sh);
};

class DLLIMPEXP_EWA_BASE DObjectBox : public DObject
{
public:
	typedef DObject basetype;

	enum
	{
		FLAG_MIN			= basetype::FLAG_MAX,
		FLAG_SHOW_BBOX		= FLAG_MIN << 1,	// 显示BBOX边框
		FLAG_FILL_BBOX		= FLAG_MIN << 2,	// 填充BBOX
		FLAG_MAX			= FLAG_MIN << 6,
	};

	DObjectBox(const String& name = "");

	uint32_t m_nDataVersion;

	BoxData bd;

	virtual void OnDataUpdated();

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	DECLARE_OBJECT_INFO_REAL(DObjectBox, DObjectInfo, 2);
};

class DLLIMPEXP_EWA_BASE DObjectBoxMovable : public DObjectBox
{
public:
	typedef DObjectBox basetype;

	enum
	{
		FLAG_MIN		= basetype::FLAG_MAX,
		FLAG_VPOS_REAL	= FLAG_MIN << 1,
		FLAG_MAX		= FLAG_MIN << 4,
	};

	DObjectBoxMovable(const String& name = "");


	void DoTransferData(TableSerializer& ar);
	void Serialize(SerializerHelper sh);

};



class DTransformItem
{
public:
	int type;
	String vx, vy, vz, ag;

	DTransformItem() { type = TRANSFORM_TRANSLATE; }
	DTransformItem(int t, const String& x, const String& y, const String& z, const String& a = "") :type(t), vx(x), vy(y), vz(z), ag(a) {}

	void SerializeVariant(Variant& v, int dir);

	void Serialize(Serializer& ar);

	enum
	{
		TRANSFORM_TRANSLATE,
		TRANSFORM_SCALE,
		TRANSFORM_ROTATE,

		TRANSFORM_SET_COL =10,
		TRANSFORM_SET_ROW =20,

	};
};

bool operator==(const DTransformItem& lhs, const DTransformItem& rhs);
bool operator!=(const DTransformItem& lhs, const DTransformItem& rhs);

template<>
class hash_t<DTransformItem>
{
public:
	uint32_t operator()(const DTransformItem& val);
};

DEFINE_OBJECT_NAME(DTransformItem, "transform_item")


template<typename T, typename B>
class DObjectGroupT : public B
{
public:
	typedef B basetype;

	DObjectGroupT(const String& name = "") :basetype(name){}

	DataGrpT<T> m_aItems;

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		ar & m_aItems;
		basetype::Serialize(sh);
	}

	virtual bool DoAppendChild(DataPtrT<DObject> p)
	{
		if (T* pd = dynamic_cast<T*>(p.get()))
		{
			m_aItems.append(pd);
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool DoGetChildren(XCtxChildrenState& cs)
	{
		cs.set_array(m_aItems);
		return true;
	}
};


template<typename T>
class NamedReferenceT : public DataPtrT<T>
{
public:
	String name;

	typedef DataPtrT<T> basetype;

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		if (!ar.flags.get(Serializer::FLAG_NO_USR_DATA))
		{
			ar & name;
		}		
		if (!ar.flags.get(Serializer::FLAG_NO_BIN_DATA))
		{
			ar & static_cast<basetype&>(*this);
		}
	}

	void clear()
	{
		name = empty_string;
		basetype::reset(NULL);
	}

	bool IsEmpty() const { return this->get() == NULL; }
};


class DLLIMPEXP_EWA_BASE ObjectVisitorState : public Object
{
public:


	ObjectVisitorState();

	uint32_t count;
	BitFlags flags;

	virtual void handle(String&);

	virtual void handle(DObject* p);

	virtual void handle(arr_1t<String>& arr);

	virtual void handle(DataGrpT<DObject>& grp);

	virtual void handle(String& name, DObject* p);

	virtual void handle(arr_1t<String>& name, DataGrpT<DObject>& grp);


	template<typename T>
	void visit(arr_1t<String>& name, DataGrpT<T>& grp)
	{
		handle(name,(DataGrpT<DObject>&)grp);
	}

	void visit(String& name, DObject* p)
	{
		handle(name, p);
	}


	template<typename T>
	ObjectVisitorState& operator&(NamedReferenceT<T>& ref)
	{
		handle(ref.name,ref.get());
		return *this;
	}

	template<typename T>
	ObjectVisitorState& operator&(DataGrpT<T>& grp)
	{
		handle((DataGrpT<DObject>&)grp);
		return *this;
	}


	ObjectVisitorState& operator&(arr_1t<String>& names)
	{
		handle(names);
		return *this;
	}

	ObjectVisitorState& operator&(String& name)
	{
		handle(name);
		return *this;
	}

};


class DLLIMPEXP_EWA_BASE CallableVisitorState : public CallableObject
{
public:
	typedef CallableObject basetype;

	ObjectVisitorState& vis;
	Variant tb;

	CallableVisitorState(ObjectVisitorState& s,const Variant& v);

	int __getindex(Executor&, const String&); // top.s

};



class DLLIMPEXP_EWA_BASE ObjectReplaceState : public ObjectVisitorState
{
public:

	ObjectReplaceState(DObject* pold, DObject* pnew);

	virtual void handle(DObject* p);

	virtual void handle(String& name);

	bool b_same;
	DataPtrT<DObject> p_old;
	DataPtrT<DObject> p_new;

};



class DLLIMPEXP_EWA_BASE InvokeDObjectHandler : public NonCopyableAndNonNewable
{
public:

	virtual void DoHandle(DObject*) = 0;

	InvokeDObjectHandler();
	void EnsureHandled();

private:
	bool m_bInited;
};


class DLLIMPEXP_EWA_BASE InvokeParamDObject : public InvokeParam
{
public:
	arr_1t<InvokeDObjectHandler*> handlers;
	void OnInvoke(ObjectInfo* p);
	static InvokeParamDObject& current();
};

////

/* 2017/06/28 Dai.Weifeng
* DWhen 设置计算体系的起止时间步和间隔时间步
* 默认构造参数：t_begin = 0, t_end = 0, t_step = 1
* 文档对象设定时，事实上由各个对象自身给出DWhen参数的“用户建议设置”
* t_begin == -1, t_end == -1, t_step == -1均认为是“默认设置”
* 默认设置如何转换为前后处理对象和引擎级别对象的真实DWhen参数，则由监视器的前后处理对象的函数autowhen来修正
* bool test(int) const函数用于测试某时间步是否处于需要处理，true为需要处理，false为不需要
* 由于计算提前终止导致真实的截至时间步和用户设定的截至时间步不同时由引擎级别对象或前后处理对象进行处理
*/
class DLLIMPEXP_EWA_BASE DWhen : public Object
{
public:
	int t_begin, t_end, t_step;
	bool test(int t_cur) const;
	DWhen();
	DWhen& operator= (const DWhen& _when);

	virtual void Serialize(SerializerHelper sh);

	virtual void DoTransferData(TableSerializer& ar, const String& suffix);

	void TransferData(TableSerializer& ar, const String& suffix);

	virtual bool DoUpdateValue(DState&)
	{
		return true;
	}
	virtual bool DoCheckParam(DState&)
	{
		return true;
	}
};

// 2018/01/18 Dai.Weifeng
// 增强版本的DWhen，支持时域参数设定、频域参数设定进行计算
// 前后处理对象PObjectT<S>里提供一个专门接口获得基本的DWhen
/* MODE_AUTOTIME             模式由记录器自己判断，默认值
 * MODE_INTEGER_STEP         兼容旧版模式，整数时间步
 * MODE_PHYSICAL_TIME_COLON  采用begin:delta:end的方式来选取间隔
 * MODE_PHYSICAL_TIME_SERIES 采用数组序列指定时间点
 * MODE_FREQUENCY_WIDTH      给定一个频宽来计算合理的dt，默认值为freq.max * 2
 */
class DLLIMPEXP_EWA_BASE DWhenEx : public DWhen
{
public:
	typedef DWhen basetype;

	typedef enum {
		MODE_AUTOTIME             = 0,
		MODE_INTEGER_STEP         = 1,
		MODE_PHYSICAL_TIME_COLON  = 2,
		MODE_PHYSICAL_TIME_SERIES = 3,
		MODE_FREQUENCY_WIDTH      = 4
	} enumDWhenMode;

	DWhenEx& operator= (const DWhen& _when);
	DWhenEx& operator= (const DWhenEx& _when);

	bool IsTimeSeries() const { return mode == MODE_PHYSICAL_TIME_SERIES; }

	int mode;

/*

应该是其他问题引起的

#ifndef EW_NDEBUG
#define __PADDING_MEM__
#endif

#ifdef __PADDING_MEM__
	// Debugger Visualizer 有问题，用pad一段内存来解决
	int __pad[8];
#endif
*/
	String s_begin, s_end, s_delta_time, s_freq_width;
	double   begin,   end,   delta_time,   freq_width;

	String s_time_series;
	arr_1t<double> time_series;

	DWhenEx();
	DWhenEx(const DWhen& _when);
	DWhenEx(const DWhenEx& _when);

	void Serialize(SerializerHelper sh);

	virtual void DoTransferData(TableSerializer& ar, const String& suffix);

	virtual bool DoUpdateValue(DState&);
	virtual bool DoCheckParam(DState&);
};


class XWhenTester
{
public:

	XWhenTester();

	class Data : public ObjectData
	{
	public:
		arr_1t<int> value;
	};

	void get_record_e(arr_1t<double>& value, double dt, int tmax);

	void get_record_h(arr_1t<double>& value, double dt, int tmax);

	void set(DataPtrT<Data> p);

	template<typename IT>
	void set(IT beg, IT end, double delta)
	{
		DataPtrT<Data> p = new Data;
		p->value.push_back(-std::numeric_limits<int>::max());
		for (auto it = beg; it != end; ++it)
		{
			p->value.push_back((*it) / delta);
		}
		p->value.push_back(std::numeric_limits<int>::max());
		set(p);
	}

	template<typename IT>
	void set(IT beg, IT end)
	{
		DataPtrT<Data> p = new Data;
		p->value.push_back(-std::numeric_limits<int>::max());
		p->value.append(beg, end);
		p->value.push_back(std::numeric_limits<int>::max());
		set(p);
	}

	void set(int beg, int end, int stp);


	bool test(int step);

	mutable int* p_pos;
	int* p_beg;
	int* p_end;
	int n_beg;
	int n_stp;
	int n_end;
	DataPtrT<ObjectData> p_data;
};


template<>
class pl_cast<DObject*>
{
public:

	template<typename T>
	static inline Variant g(const T& v)
	{
		Exception::XBadCast();
	}

	static inline DObject* g(const Object* v)
	{
		return dynamic_cast<DObject*>((Object*)v);
	}

	static inline DObject* g(const DObject* v)
	{
		return (DObject*)v;
	}

	static inline DObject* g(const Variant& v)
	{
		return dynamic_cast<DObject*>((DObject*)v.kptr());
	}
};

EW_LEAVE
#endif
