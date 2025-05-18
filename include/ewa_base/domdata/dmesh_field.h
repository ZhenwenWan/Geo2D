#ifndef __H_EW_DOMDATA_DMESH_FIELD__
#define __H_EW_DOMDATA_DMESH_FIELD__

#include "ewa_base/domdata/dfigdata.h"
#include "ewa_base/domdata/dmesh.h"
#include "ewa_base/domdata/dmesh_split.h"
#include "ewa_base/scripting/callable_function.h"
#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/executor.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE VolumePositionData : public ObjectData
{
public:

	arr_1t<double> data;

	int gmin;
	int gmax;
	double vmin;
	double vmax;
	double dmin;
	double dmax;


	void set(const arr_1t<double>& v,int d);

	double f2g_d(double val);

	int get_index_n(double val, int& idx);

	void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(VolumePositionData, ObjectInfo);
};



class DLLIMPEXP_EWA_BASE VolumePositionItem : public DataPtrT<VolumePositionData>
{
public:
	typedef DataPtrT<VolumePositionData> basetype;

	int get_index_n(double val, int& idx)
	{
		return m_ptr->get_index_n(val, idx);
	}

	void set(const arr_1t<double>& v,int d)
	{
		reset(new VolumePositionData);
		m_ptr->set(v,d);
	}

	double f2g_d(double d)
	{
		return m_ptr->f2g_d(d);
	}

	double operator[](size_t i) const
	{
		return m_ptr->data[i-m_ptr->gmin];
	}

	void Serialize(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		ar & (basetype&)(*this);
	}
};

class DLLIMPEXP_EWA_BASE VolumePosition : public DObject
{
public:

	VolumePositionItem dpos[3];

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(VolumePosition, DObjectInfo);

};

class DLLIMPEXP_EWA_BASE DenseVolumePosition : public VolumePosition
{
public:
	typedef VolumePosition basetype;

	typedef tiny_vec<int, 3> index_type;

	template<typename T>
	struct rebind
	{
		typedef arr_xt<T> array_type;
	};

	template<typename T>
	void get_v(arr_xt<T>& data, T& val, index_type& idx, double& rat)
	{
		auto& v(data(idx[0], idx[1], idx[2]));
		if (v != T(0))
		{
			val += v;
			rat += 1.0;
		}
	}

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DenseVolumePosition, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE SparseVolumePosition : public VolumePosition
{
public:

	typedef VolumePosition basetype;
	typedef vec3i index_type;

	indexer_set<index_type> vpos;

	template<typename T>
	struct rebind
	{
		typedef arr_1t<T> array_type;
	};

	template<typename T>
	bool sum_v(arr_1t<T>& data, T& val, index_type& idx, double rat)
	{
		int n = vpos.find1(idx);
		if (n < 0)
		{
			return false;
		}

		auto& v(data[n]);
		if (v == T(0))
		{
			return false;
		}

		val += v*rat;
		return true;
	}


	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(SparseVolumePosition, DObjectInfo);
};



template<typename T>
class VolumeData : public Object
{
public:
	virtual T get(const vec3d&) = 0;
	virtual void SerializeData(SerializerHelper sh) = 0;
};


template<typename T, typename P>
class VolumeDataT : public VolumeData<T>
{
public:



	typedef T value_type;
	typedef P position_type;
	typedef typename P::index_type index_type;
	typedef typename P::template rebind<T>::array_type array_type;


	DataPtrT<P> p_position;
	array_type data;

	void Mult(const T& value)
	{
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			(*it) *= value;
		}
	}

	virtual void SerializeData(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		ar & data;
	}

	void Serialize(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		ar & p_position & data;

		EW_NOOP();
	}

	class ValueContext
	{
	public:
		ValueContext(const vec3d& p) :pos(p), val(0), cnt(0.0){}

		const vec3d& pos;
		T val;
		double cnt;
		index_type idx;
	};

	static inline void on_d_zero()
	{
		// 给了个不在网格区域内的坐标，不太可能来到这里
	}

	void get_x(ValueContext& vc, double rt1)
	{
		static const int N = 0;

		int d = p_position->dpos[N].get_index_n(vc.pos[N], vc.idx[N]);

		if (d==2)
		{

			double p1 = p_position->dpos[N][vc.idx[N]];
			double p2 = p_position->dpos[N][vc.idx[N] + 1];
			double rt = (vc.pos[N] - p1) / (p2 - p1);

			double k1 = (1.0 - rt)*rt1;
			if (p_position->sum_v(data, vc.val, vc.idx, k1))
			{
				vc.cnt += k1;
			}

			vc.idx[N]++;
			double k2 = rt*rt1;
			if (p_position->sum_v(data, vc.val, vc.idx, k2))
			{
				vc.cnt += k2;
			}

		}
		else if (d==1)
		{
			if (p_position->sum_v(data, vc.val, vc.idx, rt1))
			{
				vc.cnt += rt1;
			}
		}
		else
		{
			on_d_zero();
		}

	}

	void get_xy(ValueContext& vc,double rt2)
	{
		static const int N = 1;

		int d = p_position->dpos[N].get_index_n(vc.pos[N], vc.idx[N]);

		if (d==2)
		{

			double p1 = p_position->dpos[N][vc.idx[N]];
			double p2 = p_position->dpos[N][vc.idx[N] + 1];
			double rt = (vc.pos[N] - p1) / (p2 - p1);

			get_x(vc, (1.0 - rt)*rt2);

			vc.idx[N]++;
			get_x(vc, rt*rt2);

		}
		else if (d==1)
		{
			get_x(vc, rt2);
		}
		else
		{
			on_d_zero();
		}
	}


	void get_xyz(ValueContext& vc)
	{
		static const int N = 2;
		int d = p_position->dpos[N].get_index_n(vc.pos[N], vc.idx[N]);

		if (d == 2)
		{
			double p1 = p_position->dpos[N][vc.idx[N]];
			double p2 = p_position->dpos[N][vc.idx[N] + 1];
			double rt = (vc.pos[N] - p1) / (p2 - p1);

			get_xy(vc, 1.0 - rt);

			vc.idx[N]++;
			get_xy(vc, rt);
		}
		else if (d==1)
		{
			get_xy(vc,1.0);
		}
		else
		{
			on_d_zero();
		}

	}


	virtual T get(const vec3d& p3)
	{
		ValueContext vc(p3);
		get_xyz(vc);

		if (vc.cnt > 0.0)
		{
			return vc.val / vc.cnt;
		}
		else
		{
			return vc.val;
		}
	}
};


template<typename T>
class DenseVolumeData : public VolumeDataT <T, DenseVolumePosition>
{
public:


};

template<typename T>
class SparseVolumeData : public VolumeDataT <T, SparseVolumePosition>
{
public:

};


class DLLIMPEXP_EWA_BASE VolumeFunction : public DObject
{
public:
	typedef DObject basetype;

	const int type;

	VolumeFunction(int t) :type(t)
	{

	}

	enum
	{
		TYPE_DOUBLE,
		TYPE_VECTOR,
		TYPE_COMPLEX,
		TYPE_COMPLEX_VECTOR
	};

	virtual void SerializeData(SerializerHelper sh);
	void Serialize(SerializerHelper sh);



	VariantTable table;
};

template<typename T> struct VolumFunctionType;
template<> struct VolumFunctionType<double> : public tl::int2type <VolumeFunction::TYPE_DOUBLE>
{
	typedef double scalar_type;
	static const int VECTORLENGTH = 1;
};

template<> struct VolumFunctionType<vec3d> : public tl::int2type <VolumeFunction::TYPE_VECTOR>
{
	typedef double scalar_type;
	static const int VECTORLENGTH = 3;
};
template<> struct VolumFunctionType<dcomplex> : public tl::int2type <VolumeFunction::TYPE_COMPLEX>
{
	typedef dcomplex scalar_type;
	static const int VECTORLENGTH = 1;
};

template<> struct VolumFunctionType<vec3dc> : public tl::int2type <VolumeFunction::TYPE_COMPLEX_VECTOR>
{
	typedef dcomplex scalar_type;
	static const int VECTORLENGTH = 3;
};


template<typename T>
class VolumeFunctionT : public VolumeFunction
{
public:
	typedef VolumeFunction basetype;
	typedef T value_type;

	VolumeFunctionT() :basetype(VolumFunctionType<T>::value){}

	virtual T get(const vec3d& p3) = 0;

	// 脚本借口
	virtual int __getindex(Executor& ks, const String& si)
	{
		if (si == "get")
		{
			// 注：为了简单不再弄外部缓存的实例
			ks.ci1.nbp[StackState1::SBASE_THIS].kptr(this);
			ks.ci1.nsp[0].kptr(new VolumeFunctionT<T>::CallableFunction_getdata());
			return STACK_BALANCED;
		}
		else
		{
			ks.kerror("invalid_member");
			return INVALID_CALL;
		}
	}

private:
	class CallableFunction_getdata : public CallableFunction
	{
	public:
		virtual int __fun_call(Executor& ks, int pmc)
		{
			VolumeFunctionT<T>* pfunc = dynamic_cast<VolumeFunctionT<T>*>(ks.ci1.nsp[StackState1::SBASE_THIS].kptr());
			if (pfunc == NULL)
			{
				ks.kerror(_hT("invalid_this_pointer"));
			}

			ks.check_pmc(this, pmc, 1);

			arr_xt<double> mesh = variant_cast<arr_xt<double> >(ks.ci0.nbx[1]);

			if (mesh.size(1) < 3)
			{
				ks.kerror(_hT("invalid_mesh_array_size"));
			}

			int np = mesh.size(0);

			typedef typename VolumFunctionType<T>::scalar_type scalar_type;
			const int N = VolumFunctionType<T>::VECTORLENGTH;

			arr_xt<scalar_type> fdata;
			fdata.resize(np, N);

			for (int i = 0; i < np; ++i)
			{
				vec3d p(mesh(i, 0), mesh(i, 1), mesh(i, 2));
				T value = pfunc->get(p);
				for (int j = 0; j < N; ++j)
				{
					fdata(i, j) = value[j];
				}
			}

			return 1;
		}
	};
};

/*
template<typename T>
class VolumeFunctionGrpT : public VolumeFunctionT < T >
{
public:

	class Item
	{
	public:
		DataPtrT<VolumeFunctionT < T > > pdata;
		double ratio;
	};

	arr_1t<Item> items;

	virtual T get(const vec3d& p3)
	{
		T val(0);
		for (auto it = items.begin(); it != items.end(); ++it)
		{
			val += (*it).pdata->get(p3);
		}
		return val;
	}
};
*/


template<typename T, template<typename> class H>
class DLLIMPEXP_EWA_BASE SpacialFieldData : public VolumeFunctionT<tiny_vec<T,3> >
{
public:

	typedef VolumeFunctionT<tiny_vec<T, 3> > basetype;
	typedef typename basetype::value_type value_type;

	typedef H <T> Item;


	Item items[3];

	SpacialFieldData()
	{

	}

	void Mult(const T& value)
	{
		if (value == T(1.0)) return;
		for (size_t i = 0; i < 3; i++)
		{
			items[i].Mult(value);
		}
	}


	void Serialize(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		basetype::Serialize(ar);
		items[0].Serialize(ar);
		items[1].Serialize(ar);
		items[2].Serialize(ar);
	}

	tiny_vec<T, 3> get(const vec3d& p3)
	{
		tiny_vec<T, 3> val;
		val[0] = items[0].get(p3);
		val[1] = items[1].get(p3);
		val[2] = items[2].get(p3);
		return val;
	}

};



class DLLIMPEXP_EWA_BASE SparseFieldData : public SpacialFieldData<tiny_cpx<double>, SparseVolumeData>
{
public:

	typedef SpacialFieldData<tiny_cpx<double>, SparseVolumeData> basetype;

	void Serialize(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		EW_UNUSED(ar);
		basetype::Serialize(sh);
	}

	// 直接当函数调用
	// 输入坐标数组Nx3，返回Nx3的复数矢量
	int __fun_call(Executor& ks, int pmc);

	DECLARE_DOMDATA_INFO(SparseFieldData, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE SparseFieldData2 : public SpacialFieldData<double, SparseVolumeData>
{
public:

	typedef SpacialFieldData<double, SparseVolumeData> basetype;

	void Serialize(SerializerHelper sh)
	{
		auto& ar(sh.ref(0));
		EW_UNUSED(ar);
		basetype::Serialize(sh);
	}

	// 直接当函数调用
	// 输入坐标数组Nx3，返回Nx3的复数矢量
	int __fun_call(Executor& ks, int pmc);

	DECLARE_DOMDATA_INFO(SparseFieldData2, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE FieldIndexParam
{
public:

	FieldIndexParam();

	void Serialize(SerializerHelper sh);

	uint32_t index;
	uint32_t field;
	uint32_t ntype;



};

bool operator==(const FieldIndexParam& lhs, const FieldIndexParam& rsh);


class DLLIMPEXP_EWA_BASE VolumeFunctionField : public ObjectData
{
public:

	enum
	{
		FIELD_T,
		FIELD_S,
		FIELD_A,
		FIELD_X,
		FIELD_Y,
		FIELD_Z,
		FIELD_U,
		FIELD_V,
		FIELD_N
	};

	virtual DataPtrT<DStlModel> remesh(DataPtrT<DStlModel> p,double hint);
	virtual bool update(DataPtrT<DStlModel> p, const FieldIndexParam& param) = 0;
};

class DLLIMPEXP_EWA_BASE DStlModelField : public DStlModel
{
public:
	typedef DStlModel basetype;

	DStlModelField(const String& name="field");

	FieldIndexParam param;

	mat4d m4grp;

	bool get_max_v(double& max_v);


	DataGrpT<VolumeFunctionT<vec3dc> > a_cvalues;


	virtual int __getindex(Executor&, const String&);

	void Serialize(SerializerHelper sh);


	DECLARE_DOMDATA_INFO(DStlModelField, DObjectInfo);
};

/*
class DStlModelFieldTime : public DFigDataTimeT<DStlModel>
{
public:
	typedef DFigDataTimeT<DStlModel> basetype;

	DStlModelFieldTime(const String& name="field");

	FieldIndexParam param;
	mat4d m4grp;


	DataPtrT<VolumeFunctionT<vec3d> > p_value;


	virtual int __getindex(Executor&, const String&);

	void Serialize(SerializerHelper sh);


	DECLARE_DOMDATA_INFO(DStlModelFieldTime, DObjectInfo);
};
*/


class CallableFunction_get_fdata : public CallableFunction
{
public:

	int __fun_call(Executor& ks, int pmc);

	DECLARE_OBJECT_INFO_CACHED(CallableFunction_get_fdata, ObjectInfo)
};

template<typename T>
class StlMeshRefinePolicyField : public StlMeshRefinePolicy
{
public:

	StlMeshRefinePolicyField()
	{
		min_d = 10;
		max_v = 1.0;
		scale = 1.0;
		field = VolumeFunctionField::FIELD_T;
	}

	int min_d;
	double max_v;

	mat4d m4grp;
	mat4d m4inv;
	mat3d m3fld;

	T get(const vec3d& v)
	{
		T vv = pdata->get(v);
		return vv*scale;
	}


	DataPtrT<VolumeFunctionT<T> > pdata;

	int field;
	typename T::value_type scale;



	DataPtrT<DMeshDomainStl> get_mesh_stl(DMeshDomainVisitorMesher& ctx);
};


template<typename T>
class FieldContext : public Object
{
public:

	FieldContext(DataGrpT<DMeshDomain>& t, DataPtrT<VolumeFunctionT<T> > p);

	DataGrpT<DMeshDomain>& items;
	DataPtrT<VolumeFunctionT<T> > pdata;

	mat4d m4grp;
	mat4d m4inv;

	mat4d m4stl;
	vec3d n3stl;
	vec3d v3cnt;

	void handle(DStlModel* p,const mat4d& m4);

	virtual double vfunc(const tiny_vec<tiny_cpx<double>, 3>& v);

	virtual double get(const vec3d& vc)
	{
		return vfunc(pdata->get(m4stl*vc));
	}


};

template<typename T>
class StlModelFieldHandler
{
public:

	FieldContext<T>& context;
	bool b_is_fvalue;
	double n_largest_edge_hint_2;
	double n_largest_edge_hint_1;

	StlModelFieldHandler(FieldContext<T>& ctx) :context(ctx)
	{
		b_is_fvalue = true;
		set_edge_hint(2.0);
	}

	void set_edge_hint(double v)
	{
		n_largest_edge_hint_1 = v;
		n_largest_edge_hint_2 = n_largest_edge_hint_1*n_largest_edge_hint_1;
	}


	void handle(DataGrpT<DMeshDomain>& children, const mat4d& m)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			handle(children[i], m);
		}
	}

	arr_1t<vec3i> tmp_triangles;
	indexer_set<vec3d> tmp_vertexs;


	void handle_triangle(const vec3d& p1, const vec3d& p2, const vec3d& p3)
	{

		double L1 = (p2 - p3).length2();
		double L2 = (p3 - p1).length2();
		double L3 = (p1 - p2).length2();


		if (L1 >= L2 && L1 >= L3)
		{
			if (L1 > n_largest_edge_hint_2)
			{
				vec3d cc(0.5*(p2 + p3));
				handle_triangle(p2, cc, p1);
				handle_triangle(cc, p3, p1);
				return;
			}
		}
		else if (L2 >= L3)
		{
			if (L2 > n_largest_edge_hint_2)
			{
				vec3d cc(0.5*(p3 + p1));
				handle_triangle(p3, cc, p2);
				handle_triangle(cc, p1, p2);
				return;
			}
		}
		else
		{
			if (L3 > n_largest_edge_hint_2)
			{
				vec3d cc(0.5*(p1 + p2));
				handle_triangle(p1, cc, p3);
				handle_triangle(cc, p2, p3);
				return;
			}
		}


		vec3i ti;
		ti[0] = tmp_vertexs.find2(p1);
		ti[1] = tmp_vertexs.find2(p2);
		ti[2] = tmp_vertexs.find2(p3);
		tmp_triangles.push_back(ti);


	}





	DMeshDomainStl* meshstl_real(DMeshDomainStl* pstl)
	{
		size_t n = pstl->triangles.size();
		vec3d* pv = pstl->pvertex->vertexs.data();

		tmp_vertexs.insert(pstl->pvertex->vertexs.begin(), pstl->pvertex->vertexs.end());
		for (size_t i = 0; i < n; i++)
		{
			auto& ti(pstl->triangles[i]);

			const vec3d& p1(pv[ti[0]]);
			const vec3d& p2(pv[ti[1]]);
			const vec3d& p3(pv[ti[2]]);

			handle_triangle(p1, p2, p3);
		}

		pstl = new DMeshDomainStl;
		tmp_triangles.swap(pstl->triangles);
		tmp_vertexs.swap_array(pstl->pvertex->vertexs);
		return pstl;
	}

	DMeshDomainStl* meshstl(DMeshDomainStl* pstl)
	{
		size_t n = pstl->triangles.size();
		vec3d* pv = pstl->pvertex->vertexs.data();

		for (size_t i = 0; i < n; i++)
		{
			auto& ti(pstl->triangles[i]);

			const vec3d& p1(pv[ti[0]]);
			const vec3d& p2(pv[ti[1]]);
			const vec3d& p3(pv[ti[2]]);

			if ((p2 - p1).length2() > n_largest_edge_hint_2 || (p3 - p1).length2() > n_largest_edge_hint_2 || (p3 - p2).length2() > n_largest_edge_hint_2)
			{
				return meshstl_real(pstl);
			}
		}

		return pstl;
	}


	void handle(DataPtrT<DMeshDomain>& pitem, const mat4d& m4)
	{

		if (DMeshDomainGrp* pgrp = dynamic_cast<DMeshDomainGrp*>(pitem.get()))
		{
			DMeshDomainGrp* p = new DMeshDomainGrp(*pgrp);
			pitem.reset(p);
			handle(p->children, m4*p->m4tmp);
			return;
		}

		DMeshDomainStl* pstl = dynamic_cast<DMeshDomainStl*>(pitem.get());
		if (!pstl)
		{
			return;
		}

		pstl = meshstl(pstl);
		pitem.reset(pstl);


		pstl->GenerateFaceNormals();
		pstl->pvalue.reset(new DMeshValue(DMeshValue::VALUE_FVALUE));

		size_t n = pstl->triangles.size();
		vec3d* pv = pstl->pvertex->vertexs.data();
		vec3d* pn = pstl->normals.data();

		arr_xt<double>& arr(pstl->pvalue->value.ref<arr_xt<double> >());
		arr.resize(n);

		mat4d m4g = context.m4grp*m4;

		context.m4stl = m4g;
		context.v3cnt = m4g*vec3d(0, 0, 0);



		for (size_t i = 0; i < n; i++)
		{
			auto& ti(pstl->triangles[i]);
			const vec3d& p1(pv[ti[0]]);
			const vec3d& p2(pv[ti[1]]);
			const vec3d& p3(pv[ti[2]]);
			vec3d vc = (p1 + p2 + p3) / 3.0;

			context.n3stl = pn[i];
			//arr[i] = context.vfunc(context.pdata->get(m4g*vc));
			arr[i] = context.get(vc);
		}

	}
};




EW_LEAVE
#endif
