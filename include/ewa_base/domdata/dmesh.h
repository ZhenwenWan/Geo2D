
#ifndef __H_EW_DOMDATA_DMESH__
#define __H_EW_DOMDATA_DMESH__

#include "ewa_base/domdata/dfigdata.h"

EW_ENTER


enum
{
	//NX_NOOP,
	//NX_TRIANGLES,
	//NX_TRIANGLE_STRIP,
	//NX_QUADS,
	//NX_QUAD_STRIP,
	//NX_TRIANGLE_FAN,
	//NX_POINTS,
	//NX_LINES,
	//NX_LINE_LOOP,

	NX_POINTS			= 0,
	NX_LINES			= 1,
	NX_LINE_LOOP		= 2,
	NX_LINE_STRIP		= 3,
	NX_TRIANGLES		= 4,
	NX_TRIANGLE_STRIP	= 5,
	NX_TRIANGLE_FAN		= 6,
	NX_QUADS			= 7,
	NX_QUAD_STRIP		= 8,
	NX_POLYGON			= 9,
	NX_NOOP				= 10,


	//添加额外的快速绘图标识
	//NX_BEGIN_ARROW = (1 << 4),
	//NX_END_ARROW = (2 << 4),
	//NX_BOTH_ARROW = (3 << 4),
	//NX_ARROW_MASK = (3 << 4),

	//NX_BEGIN_LABEL = (1 << 6),
	//NX_END_LABEL = (2 << 6),
	//NX_MIDDLE_LABEL = (3 << 6),
	//NX_LABEL_MASK = (3 << 6),

	NX_MASK = (1 << 10) - 1,
	NX_DEFAULT = 1 << 10,

};

class DLLIMPEXP_EWA_BASE DMeshVertex : public DObject
{
public:
	typedef DObject basetype;

	arr_1t<vec3d> vertexs;
	arr_1t<vec3d> normals;

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DMeshVertex, DObjectInfo);
};


class DLLIMPEXP_EWA_BASE DMeshValue : public DObject
{
public:
	enum
	{
		VALUE_NONE,
		VALUE_COLOR,
		VALUE_FVALUE,
		VALUE_VVALUE,
	};

	int type;
	Variant value;

	DMeshValue(int t = VALUE_NONE) :type(t){}
	DMeshValue(int t, Variant _value) :type(t), value(_value){}

	typedef DObject basetype;
	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DMeshValue, DObjectInfo);
};


class DMeshDomain;
class DMeshDomainStl;
class DMeshDomainGrp;

class DLLIMPEXP_EWA_BASE DMeshDomainVisitor : public Object
{
public:

	DMeshDomainVisitor();

	mat4d m4tmp;

	virtual void visit(DMeshDomain*);
	virtual void visit(DMeshDomainGrp*);

	virtual void visit(DMeshDomainStl*);

	virtual void visit_triangle(const vec3d& p1, const vec3d& p2, const vec3d& p3);

	virtual void handle(DMeshDomain* p, mat4d m4 = mat4d::id);


};

class DLLIMPEXP_EWA_BASE DMeshDomain : public DObject
{
public:
	typedef DObject basetype;

	enum
	{
		DOMAIN_NONE,
		DOMAIN_STL,
		DOMAIN_GRP,
		DOMAIN_STL_EX,
	};

	enum
	{
		EXTRA_FACES	=1<<1,
	};

	DMeshDomain(int t);

	const int type;
	BitFlags extra;

	mat4d m4tmp;

	DataPtrT<DObject> matptr;

	void Serialize(SerializerHelper sh);

	virtual void PrepareForVisualizer();

	virtual void accept(DMeshDomainVisitor& v);

	virtual size_t get_triangle_size();

};

class DLLIMPEXP_EWA_BASE DMeshDomainStl : public DMeshDomain
{

protected:
	DMeshDomainStl(int t);

public:
	typedef DMeshDomain basetype;

	DMeshDomainStl();
	DMeshDomainStl(const String& name);

	bool GenerateFaceNormals();

	virtual void PrepareForVisualizer();

	void InvertNormal();

	DataPtrT<DMeshVertex> pvertex;

	arr_1t<vec3i> triangles;
	arr_1t<vec3d> normals;

	DataPtrT<DMeshValue> pvalue;

	AtomicMutex mutex;

	void DoRender(DContext&);
	void SetValue(int type, Variant val);

	void Serialize(SerializerHelper sh);

	virtual size_t get_triangle_size();

	virtual void accept(DMeshDomainVisitor& v);

	DECLARE_DOMDATA_INFO(DMeshDomainStl, DObjectInfo);
};



class DLLIMPEXP_EWA_BASE DMeshDomainStlEx : public DMeshDomainStl
{
public:
	typedef DMeshDomainStl basetype;

	DMeshDomainStlEx();

	arr_1t<int> vertexs;
	arr_1t<vec2i> edges;

	void DoRender(DContext&);
	void SetValue(int type, Variant val);

	void Serialize(SerializerHelper sh);

	DECLARE_DOMDATA_INFO(DMeshDomainStlEx, DObjectInfo);
};



class DLLIMPEXP_EWA_BASE DMeshDomainGrp : public DMeshDomain
{
public:

	typedef DMeshDomain basetype;

	DMeshDomainGrp();

	DataGrpT<DMeshDomain> children;

	void swap(DMeshDomainGrp& other);

	void DoRender(DContext&);

	bool DoGetChildren(XCtxChildrenState& cs);

	void Serialize(SerializerHelper sh);

	virtual void PrepareForVisualizer();

	virtual void accept(DMeshDomainVisitor& v);

	virtual size_t get_triangle_size();

	DECLARE_DOMDATA_INFO(DMeshDomainGrp, DObjectInfo);
};

class DLLIMPEXP_EWA_BASE DStlModel : public DMeshDomainGrp
{
public:

	typedef DMeshDomainGrp basetype;

	DStlModel();
	DStlModel(const String& name);

	bool LoadStl(const String& file);
	bool LoadStlBlock(Stream& is, int64_t st, int64_t len);
	bool LoadData(const arr_xt<double>& data);

	bool SaveStl(const String& file,int type);
	bool SaveStl(Stream ss,int type);


	static DataPtrT<DMeshDomainStl> GetUnitObject(const String& s, int d = -1);

	void Serialize(SerializerHelper sh);

	void DoRender(DContext& dc);

	void Split(const vec3d& dir, const arr_1t<double>& pos);

	void MergeAllDomains();

	virtual int __fun_call(Executor& ks, int pm);
	virtual int __getindex(Executor& ks, const String& name);
	virtual int __setindex(Executor& ks, const String& name);

	DECLARE_DOMDATA_INFO(DStlModel, DObjectInfo);


};



template<typename X>
struct DMeshHandlerT
{
	static void HandleV(X&, size_t){}
	static void HandleF(X&, size_t){}
};

template<typename X>
struct DMeshNormalHandlerF : public DMeshHandlerT<X>
{
	DMeshNormalHandlerF(vec3d* p) :pvalue(p){}
	vec3d* pvalue;
	void HandleF(X& dc, size_t i){ dc.Normal(pvalue[i]); }
};

template<typename X>
struct DMeshNormalHandlerV : public DMeshHandlerT<X>
{
	DMeshNormalHandlerV(vec3d* p) :pvalue(p){}
	vec3d* pvalue;
	void HandleV(X& dc, size_t i){ dc.Normal(pvalue[i]); }
};

template<typename X>
struct DMeshValueHandlerF : public DMeshHandlerT<X>
{
	DMeshValueHandlerF(double* p) :pvalue(p){}
	double* pvalue;
	void HandleF(X& dc, size_t i){ dc.Value(pvalue[i]); }
};

template<typename X>
struct DMeshValueHandlerV : public DMeshHandlerT<X>
{
	DMeshValueHandlerV(double* p) :pvalue(p){}
	double* pvalue;
	void HandleV(X& dc, size_t i){ dc.Value(pvalue[i]); }
};



template<typename X>
class DMeshDomainHandler
{
public:

	template<typename D>
	static bool HandleDomain(D& dc, DMeshDomain* p)
	{
		if (!p) return false;
		DMeshDomainHandler<X> handler(dc);
		return handler.HandleDomain_T(*p);
	}

private:

	X& dc;

	DMeshDomainHandler(X& d) :dc(d){}

	template<typename NHandler, typename VHandler>
	bool HandleDomain_V(DMeshDomainStl& dmn, NHandler nhandler, VHandler vhandler = VHandler())
	{
		auto& triangle(dmn.triangles);
		auto& vertex(dmn.pvertex->vertexs);

		dc.Begin();
		for (size_t i = 0; i < triangle.size(); i++)
		{
			nhandler.HandleF(dc, i);
			vhandler.HandleF(dc, i);

			auto& t(triangle[i]);

			nhandler.HandleV(dc, t[0]);
			vhandler.HandleV(dc, t[0]);
			dc.Vertex(vertex[t[0]]);

			nhandler.HandleV(dc, t[1]);
			vhandler.HandleV(dc, t[1]);
			dc.Vertex(vertex[t[1]]);

			nhandler.HandleV(dc, t[2]);
			vhandler.HandleV(dc, t[2]);
			dc.Vertex(vertex[t[2]]);

		}
		dc.End();

		return true;
	}

	template<typename NHandler>
	bool HandleDomain_N(DMeshDomainStl& dmn, NHandler nhandler = NHandler())
	{

		int type = dmn.pvalue ? dmn.pvalue->type : DMeshValue::VALUE_NONE;

		if (type == DMeshValue::VALUE_VVALUE)
		{
			auto& zvalue(dmn.pvalue->value.ref<arr_xt<double> >());
			auto& vertex(dmn.pvertex->vertexs);

			if (zvalue.size() != vertex.size())
			{
				return false;
			}

			return HandleDomain_V<NHandler, DMeshValueHandlerV<X> >(dmn, nhandler, zvalue.data());

		}
		else if (type == DMeshValue::VALUE_FVALUE)
		{
			auto& zvalue(dmn.pvalue->value.ref<arr_xt<double> >());
			auto& triangle(dmn.triangles);

			if (zvalue.size() != triangle.size())
			{
				return false;
			}
			return HandleDomain_V<NHandler, DMeshValueHandlerF<X> >(dmn, nhandler, zvalue.data());
		}
		else
		{
			if (type == DMeshValue::VALUE_COLOR)
			{
				dc.Color(DColor(dmn.pvalue->value.ref<int64_t>()));
			}
			return HandleDomain_V<NHandler, DMeshHandlerT<X> >(dmn, nhandler);

		}
	}

	bool HandleDomain_T(DMeshDomainGrp& dmn)
	{
		for (size_t i = 0; i < dmn.children.size(); i++)
		{
			DMeshDomain* p = dmn.children[i].get();
			if (p && !HandleDomain_T(*p))
			{
				return false;
			}
		}
		return true;
	}

	bool HandleDomain_T(DMeshDomainStlEx& dmn)
	{
		if (!HandleDomain_T((DMeshDomainStl&)dmn))
		{
			return false;
		}

		bool flag = dc.Light(false);

		const vec3d* pvec=dmn.pvertex->vertexs.data();
		dc.Begin(NX_POINTS);
		for (size_t i = 0; i < dmn.vertexs.size(); i++)
		{
			dc.Vertex(pvec[dmn.vertexs[i]]);
		}
		dc.End();
		dc.Begin(NX_LINES);
		for (size_t i = 0; i < dmn.edges.size(); i++)
		{
			dc.Vertex(pvec[dmn.edges[i][0]]);
			dc.Vertex(pvec[dmn.edges[i][1]]);
		}
		dc.End();

		dc.Light(flag);

		return true;
	}

	bool HandleDomain_T(DMeshDomainStl& dmn)
	{

		if (!dmn.pvertex)
		{
			return false;
		}

		if (dmn.normals.size() == dmn.triangles.size())
		{
			return HandleDomain_N<DMeshNormalHandlerF<X> >(dmn, dmn.normals.data());
		}
		else if (dmn.pvertex->normals.size() == dmn.pvertex->vertexs.size())
		{
			return HandleDomain_N<DMeshNormalHandlerV<X> >(dmn, dmn.pvertex->normals.data());
		}
		else
		{
			return HandleDomain_N<DMeshHandlerT<X> >(dmn);
		}
	}

	bool HandleDomain_T(DMeshDomain& dmn)
	{
		dc.PushMatrix(dmn.m4tmp);
		bool flag;
		if (dmn.type == DMeshDomain::DOMAIN_STL)
		{
			flag = HandleDomain_T(static_cast<DMeshDomainStl&>(dmn));
		}
		else if (dmn.type == DMeshDomain::DOMAIN_GRP)
		{
			flag = HandleDomain_T(static_cast<DMeshDomainGrp&>(dmn));
		}
		else if (dmn.type == DMeshDomain::DOMAIN_STL_EX)
		{
			flag = HandleDomain_T(static_cast<DMeshDomainStlEx&>(dmn));
		}
		else
		{
			flag= true;
		}

		dc.PopMatrix();
		return flag;
	}
};


template<typename X>
class DomainContextWrapper
{
public:
	X& dc;
	DomainContextWrapper(X& d) :dc(d){}
	void Begin(){ dc.Begin(); }
	void Begin(int t) { dc.Begin(t); }
	void End(){ dc.End(); }
	void Color(DColor c){ dc.Color(c); }
	void Vertex(const vec3d& d){ dc.Vertex(d); }
	void Normal(const vec3d& d){ dc.Normal(d); }
	void Value(double v){ dc.Value(v); }
	void PushMatrix(const mat4d& m4){ dc.PushMatrix(m4); }
	void PopMatrix(){ dc.PopMatrix(); }
};


class StlFormat
{
public:

	static bool save(DMeshDomain* pstl, const String& file, int type);
	static bool save(DMeshDomain* pstl, Stream os,int type);
};


class TriangleEdgesContext
{
public:
	arr_1t<vec3d> g1;
	arr_1t<vec3d> g2;
	arr_1t<vec3d> g3;

	void adjust();
};


class MeshHandlerContext
{
public:

	static void split_edge_d(arr_1t<vec3d>& result, const vec3d& v1, const vec3d& v2, double hint);
	static void split_edge_n(arr_1t<vec3d>& result, const vec3d& v1, const vec3d& v2, int n);
	static bool merge_min_edges(indexer_set<vec3d>& v,arr_1t<vec3i>& t,double min_edge, const mat4d& m4=mat4d::id);
	static bool split_max_edges(indexer_set<vec3d>& v,arr_1t<vec3i>& t,double max_dege, const mat4d& m4=mat4d::id);

	indexer_set<vec3d> vertexs;
	arr_1t<vec3i> triangles;

	DataPtrT<DStlModel> get_stlmodel();

	DataPtrT<DMeshDomainStl> get_stldomain();

	void merge(DMeshDomain* p, const mat4d& m4);
	void merge(DMeshDomainStl* p, const mat4d& m4);
	void merge(DMeshDomainGrp* p, const mat4d& m4);

	bool merge_min_edges(double min_edge, const mat4d& m4);

	bool split_max_edges(double max_dege, const mat4d& m4);

	void add_triangle(const vec3d& v1, const vec3d& v2, const vec3d& v3);

private:

	template<typename C>
	bool do_merge_min_edges(double min_edge, C& ctx);

	template<typename C>
	bool do_split_max_edges(C& ctx,double max_dege);

};


class XMeshHandlerImpl : public Object
{
public:

	class MyDomain : public ObjectData
	{
	public:
		DataPtrT<MyDomain> real_domain;
		arr_1t<vec3i> face1;
		arr_1t<vec3i> face2;

		BitFlags flags;

		void flip();
		bool is_flip() const;

		virtual void update();
	};

	class MyEdge
	{
	public:
		bool get(DataPtrT<MyDomain>& val);
		void add(DataPtrT<MyDomain>& val);
		DataGrpT<MyDomain> domains;
	};

	class MyDomainEx;



	indexer_set<vec3d> vertexs;
	indexer_map<vec2i, MyEdge> edgemap;
	indexer_set<DataPtrT<MyDomain> > alldomains;
	DataGrpT<DMeshDomainStl> pstls;

	XMeshHandlerImpl();

	~XMeshHandlerImpl();

	void reset();

	void add(const vec3d& v1, const vec3d& v2, const vec3d& v3);
	void add(const vec3i& tri);

	void merge(bool fixnormal);

	static bool test_normal(DMeshDomainStl* pstl);

protected:

	void set_real_domain(DataPtrT<MyDomain>& val, MyDomain* preal = NULL);

	void get_domain_by_edge(DataPtrT<MyDomain>& hit0, const vec2i& v2, DataPtrT< MyDomain>& val);
	bool merge_domain(DataPtrT<MyDomain>& d1, DataPtrT<MyDomain>& d2);
	void merge_domain(DataGrpT<MyDomain>& domains);

	void fix_normal();
	void check_solid();

	MyDomainEx* get_domain_ex(DataPtrT<MyDomain>& p1);


};



class VertexHelperGlobal
{
public:
	VertexHelperGlobal(const mat4d&) {}
	static double length(const vec3d& v1, const vec3d& v2)
	{
		return (v1 - v2).length();
	}
};

class VertexHelperLocal
{
public:
	VertexHelperLocal(const mat4d& m4_) :m4(m4_) {}
	double length(const vec3d& v1, const vec3d& v2) const
	{
		return (m4 * v1 - m4 * v2).length();
	}

private:
	mat4d m4;
};

template<typename B>
class VertexHelperWithEdges : public B
{
public:
	VertexHelperWithEdges(const mat4d& m4) : B(m4) {}

	TriangleEdgesContext tri_edges;
	double edge_hint;
};

template<typename B, typename P>
class VertexHelperWithEdgesEx : public VertexHelperWithEdges<B>
{
public:

	typedef VertexHelperWithEdges<B> basetype;
	P& target;

	VertexHelperWithEdgesEx(const mat4d& m4, P& t) : basetype(m4), target(t)
	{

	}


	bool handle(const vec3d& v1, const vec3d& v2, const vec3d& v3)
	{
		auto& ctx(*this);

		double L1 = ctx.length(v2, v3);
		double L2 = ctx.length(v3, v1);
		double L3 = ctx.length(v1, v2);

		if (!(L1 > ctx.edge_hint || L2 > ctx.edge_hint || L3 > ctx.edge_hint))
		{
			return false;
		}

		MeshHandlerContext::split_edge_n(ctx.tri_edges.g1, v2, v3, ::ceil(L1 / ctx.edge_hint));
		MeshHandlerContext::split_edge_n(ctx.tri_edges.g2, v3, v1, ::ceil(L2 / ctx.edge_hint));
		MeshHandlerContext::split_edge_n(ctx.tri_edges.g3, v1, v2, ::ceil(L3 / ctx.edge_hint));

		do_mesh_triangle();
		return true;

	}

	void do_mesh_quad_stride(const arr_1t<vec3d>& k1, const arr_1t<vec3d>& k2)
	{
		if (k1.size() == 1)
		{
			for (size_t i = 1; i < k2.size(); i++)
			{
				target.add_triangle(k1[0], k2[i], k2[i - 1]);
			}
		}
		else if (k1.size() <= k2.size())
		{
			vec3d v1(k1[0]);
			vec3d v2(k2[0]);

			for (size_t i = 1; i < k1.size(); i++)
			{
				vec3d v3(k1[i]);
				vec3d v4(k2[i]);
				target.add_triangle(v1, v3, v2);
				target.add_triangle(v3, v4, v2);
				v1 = v3;
				v2 = v4;
			}
			for (size_t i = k1.size(); i < k2.size(); i++)
			{
				target.add_triangle(v1, k2[i], k2[i - 1]);
			}

		}
		else
		{
			EW_NOOP();
		}

	}

	void do_mesh_triangle()
	{
		auto& ctx(*this);

		while (1)
		{
			ctx.tri_edges.adjust();
			size_t d = std::min(ctx.tri_edges.g2.size(), ctx.tri_edges.g3.size());

			arr_1t<vec3d> k1, k2;
			k1.push_back(ctx.tri_edges.g3[0]);

			for (size_t i = 1; i < d; i++)
			{
				MeshHandlerContext::split_edge_d(k2, *(ctx.tri_edges.g2.rbegin() + i), *(ctx.tri_edges.g3.begin() + i), ctx.edge_hint);
				do_mesh_quad_stride(k1, k2);
				k1.swap(k2);
			}

			if (ctx.tri_edges.g2.size() > d)
			{
				ctx.tri_edges.g2.resize(ctx.tri_edges.g2.size() - d + 1);
				ctx.tri_edges.g3.swap(k1);
			}
			else if (ctx.tri_edges.g3.size() > d)
			{
				ctx.tri_edges.g3.erase(ctx.tri_edges.g3.begin(), ctx.tri_edges.g3.begin() + d - 1);
				ctx.tri_edges.g2.swap(k1);
			}
			else
			{
				return;
			}
		}

	}

};

EW_LEAVE

#endif
