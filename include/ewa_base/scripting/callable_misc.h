#ifndef __H_EW_SCRIPTING_CALLABLE_MISC__
#define __H_EW_SCRIPTING_CALLABLE_MISC__


#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/scripting/callable_array.h"
#include "ewa_base/scripting/callable_function.h"
#include "ewa_base/util/str_macro.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE CallableStrMacro : public CallableData
{
public:

	typedef CallableData basetype;

	CallableStrMacro();
	CallableStrMacro(StrMacro& o);
	CallableStrMacro(const CallableStrMacro& o);
	~CallableStrMacro();

	StrMacro value;

	void Serialize(SerializerHelper sh);

	virtual int __getindex(Executor&, const String&);


	DECLARE_OBJECT_INFO(CallableStrMacro, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableArray_length : public CallableFunction
{
public:

	CallableArray_length(const String& name="length",int v=0);

	virtual int __fun_call(Executor& ks, int pm);
	static int do_apply(Executor& ks, Variant& v);

	DECLARE_OBJECT_INFO_CACHED(CallableArray_length, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableArray_rows : public CallableFunction
{
public:
	CallableArray_rows(const String& name = "rows", int v = 0);
	virtual int __fun_call(Executor& ks, int pm);
	static int do_apply(Executor& ks, Variant& v);
	DECLARE_OBJECT_INFO_CACHED(CallableArray_rows, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableArray_columns : public CallableFunction
{
public:
	CallableArray_columns(const String& name = "columns", int v = 0);
	virtual int __fun_call(Executor& ks, int pm);
	static int do_apply(Executor& ks, Variant& v);
	DECLARE_OBJECT_INFO_CACHED(CallableArray_columns, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableArray_ind2sub : public CallableFunction
{
public:
	CallableArray_ind2sub(const String& name = "ind2sub", int v = 0);

	virtual int __fun_call(Executor& ks, int pm);
	static int do_apply(Executor& ks, Variant& v, Variant& vind);
	DECLARE_OBJECT_INFO_CACHED(CallableArray_ind2sub, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableArray_sub2ind : public CallableFunction
{
public:
	// 输入下标要求是1维数组，或者是2维数组
	CallableArray_sub2ind(const String& name = "sub2ind", int v = 0);

	virtual int __fun_call(Executor& ks, int pm);
	static int do_apply(Executor& ks, Variant& v, Variant& vsub);
	DECLARE_OBJECT_INFO_CACHED(CallableArray_sub2ind, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_unique : public CallableFunction
{
public:
	// 去重
	typedef CallableFunction basetype;
	CallableFunction_unique();
	virtual int __fun_call(Executor&, int);
	DECLARE_OBJECT_INFO_CACHED(CallableFunction_unique, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_find : public CallableFunction
{
public:
	// 搜索数组内容，返回非零元的下标
	typedef CallableFunction basetype;
	CallableFunction_find();
	virtual int __fun_call(Executor& ks, int pmc);
	DECLARE_OBJECT_INFO_CACHED(CallableFunction_find, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_reverse : public CallableFunction
{
public:
	CallableFunction_reverse(const String& name="reverse");
	static int do_apply(Executor& ks,Variant& var);
	virtual int __fun_call(Executor& ks,int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_reverse, ObjectInfo);
};


class CallableFunction_sort : public CallableFunction
{
public:
	CallableFunction_sort();
	static int do_apply(Executor& ks, Variant& var);
	virtual int __fun_call(Executor& ks, int pm);
	DECLARE_OBJECT_INFO_CACHED(CallableFunction_sort, ObjectInfo);
};

// 选定一列，以此为标志按升序或降序排列所有行
class CallableFunction_sortrows : public CallableFunction
{
public:
	CallableFunction_sortrows();
	virtual int __fun_call(Executor& ks, int pm);
	DECLARE_OBJECT_INFO_CACHED(CallableFunction_sortrows, ObjectInfo);

};

class DLLIMPEXP_EWA_BASE CallableFunction_pack : public CallableFunction
{
public:
	CallableFunction_pack();
	virtual int __fun_call(Executor& ks,int pm);
	DECLARE_OBJECT_INFO_CACHED(CallableFunction_pack, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_unpack : public CallableFunction
{
public:
	CallableFunction_unpack(const String& name="unpack");

	static int do_apply(Executor& ks,Variant& var);
	virtual int __fun_call(Executor& ks,int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_unpack, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableFunction_resize : public CallableFunction
{
public:
	CallableFunction_resize(const String& name="math.resize", int f=1);

	static void update_dims(arr_xt_dims& dims,Variant* nbp,int pm);
	static int do_apply(Executor& ks, Variant& var, Variant* nbp, int pm);
	virtual int __fun_call(Executor& ks,int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_resize, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_reshape : public CallableFunction
{
public:
	CallableFunction_reshape(const String& name="math.reshape", int f=1);

	static int do_apply(Executor& ks, Variant& var, Variant* nbp, int pm);
	virtual int __fun_call(Executor& ks,int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_reshape, ObjectInfo);
};


class XMemoryRecordItem : public CallableData
{
public:
	typedef CallableData basetype;
	TimePoint tp;
	int64_t used;
	int64_t avail;
	String phase;

	void Serialize(SerializerHelper sh);

	int __getindex(Executor& ks, const String& si);

	bool ToJson(JsonWriter&);

	DECLARE_OBJECT_INFO(XMemoryRecordItem, ObjectInfo);
};

class CallableObjectGroupBase : public CallableObject
{
public:
	typedef CallableData basetype;
	int __getindex(Executor& ks, const String& si);
	int __getarray(Executor& ks, int pm) = 0;
	void __get_iterator(Executor& ks, int nd);
	int __getarray_index_range(Executor& ks, int pm);
};

template<typename T>
class CallableObjectGroupT : public CallableObjectGroupBase
{
public:
	typedef CallableObjectGroupBase basetype;

	DataGrpT<T> value;

	int __getarray(Executor& ks, int pm)
	{
		ks.check_pmc(this, pm, 1);
		kCallableWrapT_getarray_1t<Variant,false>::g(value, *ks.ci1.nsp);
		return 1;
	}

	void Serialize(SerializerHelper sh)
	{
		Serializer& ar(sh.ref(0));
		basetype::Serialize(ar);
		ar& value;
	}
};

class XMemoryRecordGroup : public CallableObjectGroupT<XMemoryRecordItem>
{
public:
	typedef CallableObjectGroupT<XMemoryRecordItem> basetype;

	size_t handle(MemoryCtxData& ctx, const char* phase);

	static XMemoryRecordGroup& get(const String& name);

	int __getindex(Executor& ks, const String& si);

	DECLARE_OBJECT_INFO(XMemoryRecordGroup, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableFunction_memory_recorder: public CallableFunction
{
public:

	CallableFunction_memory_recorder();

	virtual int __fun_call(Executor& ks, int pm);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_memory_recorder, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_multilayer : public CallableFunction
{
public:
	// 多层介质透反射
	CallableFunction_multilayer() : CallableFunction("misc.multilayer") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_multilayer, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_fld2cur : public CallableFunction
{
public:
	// 近场电磁场转换为电流，提高计算效率
	// 电流值直接乘以反射系数的比例即可调节其结果
	CallableFunction_fld2cur() : CallableFunction("misc.fld2cur") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_fld2cur, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_epm : public CallableFunction
{
public:
	// 近-远场等效原理，Equivalance Principle Method
	// 备注：先输入网格参数和远场方位，构造从表面场到远场的线性关系（即等效关系）
	CallableFunction_epm() : CallableFunction("misc.epm") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_epm, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableEPM: public CallableData
{
public:
	// 等效原理方法操作对象
	// 0 - 初始化，包括单位、频率范围等
	// state = 0
	// epm.init('unit', 'SI')
	// epm.init('unit', table('ul', 1e-3, 'uf', 1e9, 'ut', 1e-9))
	// state = 1 
	// epm.prep(vertex, normal)
	// 2 - 填充总场
	// state = 2
	// epm.fill( e,  h, '-field')    // 填充输入电磁场
	// epm.fill(jm, je, '-current')  // 填充输入电磁场
	// 3 - 外推
	// state = 3
	// epm.ntff(kfar, r) // 计算远场
	// epm.ntff(kfar, r, np) // 计算远场
	// epm.ntff(kfar, r, opt) // 计算远场
	//     >>>> opt.phase_center
	//     >>>> opt.np
	// epm.ntnf(r)       // 计算准远场
	// epm.ntnf(r, np)   // 计算准远场
	// epm.ntnf(r, opt)   // 计算准远场
	//     >>>> opt.np

	typedef CallableData basetype;

	enum {
		EPM_INIT = 0,    // 初始化环境设置
		EPM_PREP = 1,    // 前处理，载入模型、处理网格等
		EPM_FILL = 2,    // 填充场量
		EPM_NTFF = 3     // 远场外推
	};

	enum {
		INPUT_FIELD = 0,  // 输入电磁场
		INPUT_CURRENT = 1 // 输出电磁流
	};

	CallableEPM();

	virtual int __getindex(Executor& ks, const String&);

	void Serialize(SerializerHelper);

	DECLARE_OBJECT_INFO(CallableEPM, ObjectInfo);

protected:
	int state;

	double ul;
	double uf;
	double ut;

	arr_1t<vec3d> vertex;
	arr_1t<vec3d> normal; // 注意：只有一个元素时按常数处理

	arr_1t<vec3dc> jm, je;

public:
	void init(const String& name, Variant val);
	void prep(const arr_1t<vec3d>& _vertex, const arr_1t<vec3d>& _normal);
	void fill(const arr_1t<vec3dc>& e_or_jm, const arr_1t<vec3dc>& h_or_je, int mode);

	// 状态模型接口
	int current_state() const { return state; }
	size_t size() const { return vertex.size(); }


	// 计算波长
	double f2lambda(double f) const
	{
		return M_LIGHTSPEED / (f * uf) / ul;
	}

	double f2lambda(double f, double uf1) const
	{
		return M_LIGHTSPEED / (f * uf1) / ul;
	}

	// 将频率转换为k
	double f2k(double f) const
	{
		return M_PI*2 / f2lambda(f);
	}

	double f2k(double f, double uf1) const
	{
		return M_PI * 2 / f2lambda(f, uf1);
	}

	bool next(); // 前进
	void prev(); // 后退

	bool valid() const; // 验证数据，要求

	// 外推功能（当且仅当state = 3时有效）
	// 长度单位为ul
	// 波矢量单位为ul^-1
	// 注意：E/H会清空结果
	void ntff(const vec3d& kfar, double r, vec3dc& E, vec3dc& H, const vec3d& phase_center) const;
	void ntnf(double k0, const vec3d& r, vec3dc& E, vec3dc& H) const;
	
	// 区块计算
	void ntnf(double k0, const vec3d& r, vec3dc& E, vec3dc& H, int tid, int numTask) const;
};

namespace detail
{
	template<typename T>
	struct vec3xt_op1
	{
		typedef T type;

		static void normalize_ref(arr_xt<T>& v)
		{
			EW_ASSERT(v.size(1) == 3);
			type len, x, y, z;

			for (size_t i = 0; i < v.size(0); ++i)
			{
				x = v(i, 0);
				y = v(i, 1);
				z = v(i, 2);

				len = sqrt(x * x + y * y + z * z);

				v(i, 0) /= len;
				v(i, 1) /= len;
				v(i, 2) /= len;
			}
		}

		static arr_xt<T> normalize(const arr_xt<T>& v)
		{
			arr_xt<T> ret;
			ret.resize(v.size_ptr());
			type len, x, y, z;
			for (size_t i = 0; i < v.size(0); ++i)
			{
				x = v(i, 0);
				y = v(i, 1);
				z = v(i, 2);
				len = sqrt(x * x + y * y + z * z);

				ret(i, 0) = x / len;
				ret(i, 1) = y / len;
				ret(i, 2) = z / len;
			}

			return EW_MOVE(ret);
		}
	};

	template<typename T1, typename T2>
	struct vec3xt_op2
	{
		typedef typename cpx_promote<T1, T2>::type rtype;

		typedef T1 type1;
		typedef T2 type2;

		static arr_xt<rtype> dot(const arr_xt<type1>& v1, const arr_xt<type2>& v2)
		{
			arr_xt<rtype> ret;

			size_t nn = std::max(v1.size(0), v2.size(0));
			ret.resize(nn, 1);

			if (v1.size(0) == v2.size(0))
			{
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = v1(i, 0) * v2(i, 0) + v1(i, 1) * v2(i, 1) + v1(i, 2) * v2(i, 2);
				}
			}
			else if (v1.size(0) == 1)
			{
				type1 x, y, z;
				x = v1(0, 0);
				y = v1(0, 1);
				z = v1(0, 2);
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = x * v2(i, 0) + y * v2(i, 1) + z * v2(i, 2);
				}
			}
			else
			{
				EW_ASSERT(v2.size(0) == 1);
				type2 x, y, z;
				x = v2(0, 0);
				y = v2(0, 1);
				z = v2(0, 2);
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = v1(i, 0) * x + v1(i, 1) * y + v1(i, 2) * z;
				}
			}
			return EW_MOVE(ret);
		}

		static arr_xt<rtype> cross(const arr_xt<type1>& v1, const arr_xt<type2>& v2)
		{
			arr_xt<rtype> ret;

			size_t nn = std::max(v1.size(0), v2.size(0));
			ret.resize(nn, 3);

			if (v1.size(0) == v2.size(0))
			{
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = v1(i, 1) * v2(i, 2) - v1(i, 2) * v2(i, 1);
					ret(i, 1) = v1(i, 2) * v2(i, 0) - v1(i, 0) * v2(i, 2);
					ret(i, 2) = v1(i, 0) * v2(i, 1) - v1(i, 1) * v2(i, 0);
				}
			}
			else if (v1.size(0) == 1)
			{
				type1 x, y, z;
				x = v1(0, 0);
				y = v1(0, 1);
				z = v1(0, 2);
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = y * v2(i, 2) - z * v2(i, 1);
					ret(i, 1) = z * v2(i, 0) - x * v2(i, 2);
					ret(i, 2) = x * v2(i, 1) - y * v2(i, 0);
				}
			}
			else
			{
				EW_ASSERT(v2.size(0) == 1);
				type2 x, y, z;
				x = v2(0, 0);
				y = v2(0, 1);
				z = v2(0, 2);
				for (size_t i = 0; i < nn; ++i)
				{
					ret(i, 0) = v1(i, 1) * z - v1(i, 2) * y;
					ret(i, 1) = v1(i, 2) * x - v1(i, 0) * z;
					ret(i, 2) = v1(i, 0) * y - v1(i, 1) * x;
				}
			}

			return EW_MOVE(ret);
		}
	};
}

class DLLIMPEXP_EWA_BASE CallableFunction_sample_rect : public CallableFunction
{
public:
	// 对四边形面片进行采样
	typedef CallableFunction basetype;

	CallableFunction_sample_rect() : basetype("misc.sample_rect") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_sample_rect, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_precalcrefl : public CallableFunction
{
public:
	// 计算面表面反射场中间信息：镜像k矢量和TE基矢量和TM基矢量
	// e_TE, e_TM = calcrefl(ne, ke)
	// 要求：ne, ke必须是单位矢量，此处不再归一化处理
	// 注意：入射坐标系和反射坐标系之间的差别主要是e_TM，需要将e_TM进行镜像翻转
	typedef CallableFunction basetype;
	CallableFunction_precalcrefl() : basetype("misc.precalcrefl") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_precalcrefl, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_fdecomp : public CallableFunction
{
public:
	// 场量分解函数，注意：只提供一个基上的投影计算函数
	// eu和ev为归一化矢量坐标系
	// au = fdecomp(eu, Ei) dot(eu, Ei)
	// av = fdecomp(ev, Ei)
	typedef CallableFunction basetype;
	CallableFunction_fdecomp() : basetype("misc.fdecomp") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_fdecomp, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_calcmirror : public CallableFunction
{
public:
	// 将入射的k计算为反射的k'，按法向进行变换
	// 镜像反射：入射的k对n投影，然后反向；水平部分不变
	// k', dot(n, k) = calcmirror(n, k)
	typedef CallableFunction basetype;

	CallableFunction_calcmirror() : basetype("misc.calcmirror") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_calcmirror, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_pomat_pec : public CallableFunction
{
public:
	// PEC材料反射系数
	// rTe, rTM = pomat.pec(...)
	// rTe, rTM = pomat.pec(cos_theta)
	// 若输入参数为空，则返回-1,+1
	// 若输入参数为ne, ke，则返回数组-1,+1，其大小与其size(0)匹配
	typedef CallableFunction basetype;

	CallableFunction_pomat_pec() : basetype("misc.pomat.pec") {}

	virtual int __fun_call(Executor&, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_pomat_pec, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_pomat_simple : public CallableFunction
{
public:
	// 单界面（半无限大平面）材料反射系数
	// rTE, rTM = pomat.simple(eps, mu, cos_theta)
	typedef CallableFunction basetype;
	CallableFunction_pomat_simple() : basetype("misc.pomat.simple") {}

	virtual int __fun_call(Executor& ks, int pmc);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_pomat_simple, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableFunction_calcpower : public CallableFunction
{
public:
	// 计算能流积分：(1/2)*sum(Eu x Hv^*)
	// P = 0.5*sum(Eu.*Hv)*(du*dv)
	// 注意1：如果计算-(1/2)*sum(Ev x Hu^*)注意取个负号
	// 注意2：网格匹配仅考虑计算垂直分量（此时对偶场的水平坐标对齐）
	// 注意3：建议使用pputil.slicedata.adjust函数对输入的场数据预先转换维度
	CallableFunction_calcpower() : CallableFunction("misc.calcpower") {}

	virtual int __fun_call(Executor& ks, int);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_calcpower, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableFunction_loadex : public CallableFunction
{
public:
	// 导入一些外部的数据格式
	// 返回一个变量表，按照某些规则映射到变量上
	// load(filename, '-fluent-dat') // 导入Fluent文件的dat

	CallableFunction_loadex() : CallableFunction("misc.loadex") {}

	virtual int __fun_call(Executor& ks, int);

	typedef Variant(*loader_t)(StringBuffer&);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_loadex, ObjectInfo);

protected:
	Variant load(Executor& ks, loader_t func, const String& fn);

public:
	// 载入Fluent-Data数据（试验功能）
	static Variant load_fluent(StringBuffer& sb);
	// 载入Abaqus Inp文件
	static Variant load_inp(StringBuffer& sb);
};


class CallableTableDelayed : public CallableObject
{
public:
	typedef CallableObject basetype;
	CallableTableDelayed();

	SerializerReader reader;
	DataPtrT<CallableTableEx> ptable;
	int state;


	void step1_prepare();
	void step2_loaddata();
	void step3_loadtable();

	CallableTableEx* get_table();

	Variant load_item(const String& id);

	int __getindex(Executor&, const String&);
	int __getarray(Executor&, int);
};

EW_LEAVE
#endif
