#ifndef __H_EW_DOMDATA_DCONTEXT__
#define __H_EW_DOMDATA_DCONTEXT__



#include "ewa_base/domdata/dunit.h"
#include "ewa_base/domdata/dmesh.h"
#include "ewa_base/domdata/xcolor.h"
#include "ewa_base/basic/functor.h"
#include "ewa_base/util/polygon.h"


EW_ENTER

enum enum_GL_CONTEXT_ATTRIBUTE
{
	GL_ATTRIBUTE_NONE = 0,
	GL_ATTRIBUTE_BAN_SHIFT = 1 << 1,
	GL_ATTRIBUTE_BAN_ROTATION = 1 << 2,
	GL_ATTRIBUTE_BAN_ZOOM = 1 << 3,
	GL_ATTRIBUTE_FIX_PIXEL = 1 << 4
};



class DContext;
class DTaskData;

template<typename T>
class DCtxState : public NonCopyable
{
public:

	DCtxState() :val(), ref(){}

	typedef T type;

	type val;
	type ref;
	arr_1t<type> stk;

	void push()
	{
		stk.push_back(val);
	}

	void push(const type& v)
	{
		stk.push_back(val);
		val = v;
	}

	void pop()
	{
		stk.pop_back(val);
	}
};

class DLLIMPEXP_EWA_BASE DCtxStateMatrix : public DCtxState<mat4d>
{
public:
	typedef DCtxState<mat4d> basetype;
	using basetype::push;

	void push(const mat4d& v);
};



class DLLIMPEXP_EWA_BASE DCtxStateObject
{
public:
	typedef DObject* type;

	class StkObject : public ObjectData
	{
	public:
		virtual void push(DObject*) = 0;
		virtual void pop() = 0;
		virtual void init(DObject*){}
	};

	void init(DObject* p);
	void push(DObject* p);
	void pop();
	void reset(StkObject* p);

protected:

	DataPtrT<StkObject> pobj;
};

class DLLIMPEXP_EWA_BASE DCtxStateProperty : public DCtxState<indexer_map<String, int> >
{
public:

	bool set(const String& id, int v);
	int get(const String& id);
};


class DLLIMPEXP_EWA_BASE DCtxStateMaterial : public DCtxState<int>
{
public:
	typedef DCtxState<int> basetype;
	typedef DObject* type;

	DCtxStateMaterial(DContext& m);

	DContext& dc;

	void init(DObject* p);
	void push(DObject* p);
	void pop();
	void push(int id);

	arr_1t<int> depth;

	indexer_set<DataPtrT<DObject> > arr;
};

class DLLIMPEXP_EWA_BASE DCtxStateBindExtra : public DCtxState<DBindExtraCtx>
{
public:

	void init(double t);
};



class DLLIMPEXP_EWA_BASE DCtxStateUnit : public DCtxState<KUnit>
{
public:

	typedef DUnit* type;
	typedef DCtxState<KUnit> basetype;
	using basetype::push;

	void init(DUnit* p);
	void push(DUnit* p);
	void push(const KUnit& ku);

};

template<typename T>
class DCtxLocker
{
public:
	DCtxLocker(T& t) :target(t){ target.push(); }
	DCtxLocker(T& t, const typename T::type& v) :target(t){ target.push(v); }
	~DCtxLocker(){ target.pop(); }
	T& target;
};


class DLLIMPEXP_EWA_BASE DCtxLocker2
{
public:
	DCtxLocker2(DContext& c, const mat4d& m, DUnit* p);
	~DCtxLocker2();
	DContext& dc;
};



//#define GL_POINTS                         0x0000
//#define GL_LINES                          0x0001
//#define GL_LINE_LOOP                      0x0002
//#define GL_LINE_STRIP                     0x0003
//#define GL_TRIANGLES                      0x0004
//#define GL_TRIANGLE_STRIP                 0x0005
//#define GL_TRIANGLE_FAN                   0x0006
//#define GL_QUADS                          0x0007
//#define GL_QUAD_STRIP                     0x0008
//#define GL_POLYGON                        0x0009



enum NX_FLAGS
{
	NX_MIN			= NX_DEFAULT << 1,
	NX_MESH			= NX_MIN << 0,
	NX_BBOX			= NX_MIN << 1,
	NX_CBOX			= NX_MIN << 2,
	NX_WIRE_FRAME	= NX_MIN << 3,
	NX_EDGE_REFINE	= NX_MIN << 4,
	NX_KEY_POINT	= NX_MIN << 5,
	NX_SNAP_PLANE_X = NX_MIN << 6,
	NX_SNAP_PLANE_Y = NX_MIN << 7,
	NX_SNAP_PLANE_Z = NX_MIN << 8,
	NX_FACE_CENTER	= NX_MIN << 9,
	NX_EDGE_CENTER	= NX_MIN << 10,
	NX_VIEW_OBJECT	= NX_MIN << 11,	// 绘制一般物体（包括监视器、激励源的图示）
	NX_VIEW_CUSTOM	= NX_MIN << 12,	// 绘制额外标识符
	NX_ARROW		= NX_MIN<<13,
	NX_LABEL		= NX_MIN<<14,
	NX_SURFACE		= NX_MIN<<15,
	NX_CTRL_POINT	= NX_MIN<< 16,
	NX_2D_OBJECT	= NX_MIN<<17,
	NX_MESH_EX = NX_MESH | NX_VIEW_OBJECT,
	NX_SNAP_PLANES = NX_SNAP_PLANE_X | NX_SNAP_PLANE_Y | NX_SNAP_PLANE_Z,
};



// DCachedDataManager 用于管理缓存数据，暂时只缓存直径为1的单位圆周顶点数据和直径为1的球体数据
class DLLIMPEXP_EWA_BASE DCachedDataManager : public NonCopyableAndNonNewable
{
public:

	class CircleData : public ObjectData
	{
	public:
		arr_1t<vec3d> circle_data;
	};

	class SphereData :public ObjectData
	{
	public:
		arr_xt<vec3d> sphere_data;
	};

	static DCachedDataManager& current();
	DCachedDataManager();

	DataPtrT<CircleData> get_circle(size_t n);
	DataPtrT<CircleData> create_circle(size_t n);

	DataPtrT<SphereData> get_sphere(size_t stack, size_t slice);
	DataPtrT<SphereData> create_sphere(size_t stack, size_t slice);

protected:
	arr_1t<DataPtrT<CircleData> > cached_circles;
	arr_xt<DataPtrT<SphereData> > cached_spheres;

	AtomicSpin mutex;
};


class DLLIMPEXP_EWA_BASE DDocInfo
{
public:
	DDocInfo(){}
	DDocInfo(const String& f) :filename(f){}

	String filename;
};

class DLLIMPEXP_EWA_BASE DContext : public CallableObject
{
public:

	typedef CallableObject basetype;
	enum
	{
		FLAG_REVERSE = 1 << 0,
		FLAG_DC_POST = 1 << 1,


		FLAG_SHP	= 1 << 2,
		FLAG_RCD	= 1 << 3,
		FLAG_SRC	= 1 << 4,
		FLAG_SHOW	= 1 << 5,

		//FLAG_NO_MAT_DECORATE	=1<<8,
		//FLAG_NO_MAT_APPLY		=1<<9,
	};

	enum
	{
		EXTRA_FOR_PORT			=1<<0,
		EXTRA_NO_MAT_DECORATE	=1<<8,
		EXTRA_NO_MAT_APPLY		=1<<9,
		EXTRA_TEST_ONLY			=1<<10,
		EXTRA_IGNORE_ERROR		=1<<11,
	};


	DContext();
	~DContext();

	void Label(const String& s);
	void Color(const DColor& c);

	void Normal(const vec3d&);

	void Vertex(const vec2d& v);
	void Vertex(const vec3d& v);
	void VertexArray(const vec3d* p, size_t n);
	void VertexArray(const int* p, size_t n, const vec3d* v);
	void VertexArray(const int* p, size_t n, const vec3d* v, const mat4d& m4);

	void VertexArray(const vec2d* p, size_t n);
	void VertexArray(const int* p, size_t n, const vec2d* v);
	void VertexArray(const int* p, size_t n, const vec2d* v, const mat4d& m4);

	void PolygonTriangles(DataPtrT<XPolygonData> poly);
	void PolygonTriangles(DataPtrT<XPolygonData> poly, const mat4d& m4);
	void PolygonQuads(DataPtrT<XPolygonData> poly, int t,const mat4d& m1,mat4d& m2);

	virtual void VertexDomain(int type,const int* p, size_t n, const vec3d* v);

	virtual void ValueArray(double* p, size_t n);
	virtual void Value(double v);

	virtual bool Light(bool);

	void Begin();
	virtual void Begin(int t);
	virtual void End();
	virtual void Commit();

	void Translate(const vec3d& v);
	void Scale(const vec3d& v);
	void Scale(double v);
	void Rotate(double d, const vec3d& v);

	void LoadIdentity();
	void PushMatrix();
	void PushMatrix(const mat4d& m4);
	void PopMatrix();
	void LoadMatrix(const mat4d& m);
	void MultMatrix(const mat4d& m);
	//void MultMatrix(const mat4d& m, double u){ cx.val.MultMatrix(m, u); }

	void Reverse();

	const mat4d& GetMatrix();

	void StlDomain(DataPtrT<DMeshDomain> model, int t = 0, DColor c = DColor());


	virtual void B3Box_Points(const box3d& b3, int t = 0);
	virtual void B3Box(const box3d& b3, int t = 0, const DColor& color = DColor(127, 127, 127));

	virtual void EnterGroup();
	virtual void LeaveGroup();

	virtual void EnterDocument(const DDocInfo& di);
	virtual void LeaveDocument();

	virtual void RenderObject(DObject* p);

	virtual void RenderTask(DTaskData* p);

	virtual DObject* DecorateObject(DObject* pobj,const mat4d& m4);

	virtual DObject* BindExtra(DObject* pobj, DBindExtraCtx& ctx);

	template<typename T>
	void RenderObject(DataPtrT<T>& p){ RenderObject(p.get()); }

	int32_t vtype;
	int32_t phase;
	BitFlags flags;
	BitFlags vmask;
	BitFlags extra;

	DCtxStateUnit cu;
	DCtxStateMatrix cx;
	DCtxStateMaterial cm;
	DCtxStateObject co;
	DCtxStateProperty cp;
	DCtxState<String> fp;
	DCtxStateBindExtra be;

	arr_1t<vec3d> vertexs;
	arr_1t<String> labels;
	arr_1t<DColor> colors;

	template<typename P>
	void HandleTriangles()
	{
		P dc(*this); HandleTriangles(dc);
	}

	template<typename P>
	void HandleEdges()
	{
		P dc(*this); HandleEdges(dc);
	}

	template<typename P>
	void HandlePoints()
	{
		P dc(*this); HandlePoints(dc);
	}

	template<typename P>
	void HandleCtrlPoints()
	{
		P dc(*this); HandleCtrlPoints(dc);
	}

	template<typename P>
	void HandleTriangles(P& dc);

	template<typename P>
	void HandleTrianglesEx(P& dc);

	template<typename P>
	void HandleEdges(P& dc);

	template<typename P>
	void HandlePoints(P& dc);

	template<typename P>
	void HandleCtrlPoints(P& dc);


	virtual bool RenderDocument(DObject* pdoc);

	virtual int __getindex(Executor& ks, const String& si);

protected:
	int nx_default;

};

class DLLIMPEXP_EWA_BASE DContext1 : public DContext
{
public:

};

class DLLIMPEXP_EWA_BASE DContext2 : public DContext
{
public:

};

class DLLIMPEXP_EWA_BASE XContextPolicy : public Object
{
public:

	DContext& dc;
	const vec3d* pv;
	size_t nv;

	XContextPolicy(DContext &dc_)
		: dc(dc_)
		, pv(dc.vertexs.data())
		, nv(dc.vertexs.size())
	{
	}

	static void EnterTriangle(){}
	static void LeaveTriangle(){}
	static void EnterEdge(){}
	static void LeaveEdge(){}
	static void EnterPoint(){}
	static void LeavePoint(){}
	static void HandleQuad(size_t, size_t, size_t,size_t){}
	static void HandleTriangle(size_t, size_t, size_t){}
	static void HandleEdge(size_t, size_t){}
	static void HandlePoint(size_t){}

};


template<typename P>
void DContext::HandleTriangles(P& dc)
{

	size_t n = vertexs.size();

	dc.EnterTriangle();

	if (!flags.get(FLAG_REVERSE))
	{


		if (vtype == NX_TRIANGLES)
		{
			for (size_t i = 2; i<n; i += 3)
			{
				dc.HandleTriangle(i - 2, i - 1, i);
			}
		}
		else if (vtype == NX_TRIANGLE_STRIP)
		{
			for (size_t i = 2; i<n; i++)
			{
				if (i % 2 == 0)
				{
					dc.HandleTriangle(i - 2, i - 1, i);
				}
				else
				{
					dc.HandleTriangle(i - 2, i, i - 1);
				}
			}
		}
		else if ( vtype == NX_QUAD_STRIP)
		{
			for (size_t i = 3; i<n; i+=2)
			{
				dc.HandleQuad( i - 3, i - 2, i, i - 1);
			}
		}
		else if (vtype == NX_QUADS)
		{
			for (size_t i = 3; i<n; i += 4)
			{
				dc.HandleQuad(i - 3, i - 2, i - 1, i);
			}
		}
		else if (vtype == NX_TRIANGLE_FAN)
		{
			for (size_t i = 2; i < n; i++)
			{
				dc.HandleTriangle(0, i - 1, i);
			}
		}
		else if (vtype == NX_POINTS)
		{
			for (size_t i = 0; i < n; ++i)
			{
				dc.HandlePoint(i);
			}
		}
		//else if (vtype == NX_LINES)
		//{
		//	for (size_t i = 1; i < n; i+=2)
		//	{
		//		dc.HandleEdge(i - 1, i);
		//	}
		//}
	}
	else
	{
		if(vtype == NX_TRIANGLES)
		{
			for (size_t i = 2; i<n; i += 3)
			{
				dc.HandleTriangle(i - 2, i, i - 1);
			}
		}
		else if (vtype == NX_TRIANGLE_STRIP)
		{
			for (size_t i = 2; i<n; i++)
			{
				if (i % 2 == 0)
				{
					dc.HandleTriangle(i - 2, i, i - 1);
				}
				else
				{
					dc.HandleTriangle(i - 2, i - 1, i);
				}
			}
		}
		else if (vtype == NX_QUAD_STRIP)
		{
			for (size_t i = 3; i<n; i += 2)
			{
				dc.HandleQuad( i - 1 , i,i - 2, i - 3);
			}
		}
		else if (vtype == NX_QUADS)
		{
			for (size_t i = 3; i<n; i += 4)
			{
				dc.HandleQuad(i, i - 1 , i - 2, i - 3);
			}
		}
		else if (vtype == NX_TRIANGLE_FAN)
		{
			for (size_t i = 2; i < n; i++)
			{
				dc.HandleTriangle(0, i, i - 1);
			}
		}
		else if (vtype == NX_POINTS)
		{
			for (size_t i = 0; i < n; ++i)
			{
				dc.HandlePoint(i);
			}
		}
		//else if (vtype == NX_LINES)
		//{
		//	for (size_t i = 1; i < n; i += 2)
		//	{
		//		dc.HandleEdge(i, i-1);
		//	}
		//}
	}

	dc.LeaveTriangle();
}


template<typename P>
void DContext::HandleTrianglesEx(P& dc)
{

	size_t n = vertexs.size();

	dc.EnterTriangle();

	if (!flags.get(FLAG_REVERSE))
	{
		if (vtype == NX_TRIANGLES)
		{
			for (size_t i = 2; i<n; i += 3)
			{
				dc.HandleTriangle(i - 2, i - 1, i);
			}
		}
		else if (vtype == NX_TRIANGLE_STRIP)
		{
			for (size_t i = 2; i<n; i++)
			{
				if (i % 2 == 0)
				{
					dc.HandleTriangle(i - 2, i - 1, i);
				}
				else
				{
					dc.HandleTriangle(i - 2, i, i - 1);
				}
			}
		}
		else if ( vtype == NX_QUAD_STRIP)
		{
			for (size_t i = 3; i<n; i+=2)
			{
				dc.HandleQuad( i - 3, i - 2, i, i - 1);
			}
		}
		else if (vtype == NX_QUADS)
		{
			for (size_t i = 3; i<n; i += 4)
			{
				dc.HandleQuad(i - 3, i - 2, i - 1, i);
			}
		}
		else if (vtype == NX_TRIANGLE_FAN)
		{
			for (size_t i = 2; i < n; i++)
			{
				dc.HandleTriangle(0, i - 1, i);
			}
		}
		else if (vtype == NX_POINTS)
		{
			for (size_t i = 0; i < n; ++i)
			{
				dc.HandlePoint(i);
			}
		}
		else if (vtype == NX_LINES)
		{
			for (size_t i = 1; i < n; i+=2)
			{
				dc.HandleEdge(i - 1, i);
			}
		}
	}
	else
	{
		if(vtype == NX_TRIANGLES)
		{
			for (size_t i = 2; i<n; i += 3)
			{
				dc.HandleTriangle(i - 2, i, i - 1);
			}
		}
		else if (vtype == NX_TRIANGLE_STRIP)
		{
			for (size_t i = 2; i<n; i++)
			{
				if (i % 2 == 0)
				{
					dc.HandleTriangle(i - 2, i, i - 1);
				}
				else
				{
					dc.HandleTriangle(i - 2, i - 1, i);
				}
			}
		}
		else if (vtype == NX_QUAD_STRIP)
		{
			for (size_t i = 3; i<n; i += 2)
			{
				dc.HandleQuad( i - 1 , i,i - 2, i - 3);
			}
		}
		else if (vtype == NX_QUADS)
		{
			for (size_t i = 3; i<n; i += 4)
			{
				dc.HandleQuad(i, i - 1 , i - 2, i - 3);
			}
		}
		else if (vtype == NX_TRIANGLE_FAN)
		{
			for (size_t i = 2; i < n; i++)
			{
				dc.HandleTriangle(0, i, i - 1);
			}
		}
		else if (vtype == NX_POINTS)
		{
			for (size_t i = 0; i < n; ++i)
			{
				dc.HandlePoint(i);
			}
		}
		else if (vtype == NX_LINES)
		{
			for (size_t i = 1; i < n; i += 2)
			{
				dc.HandleEdge(i, i-1);
			}
		}
	}

	dc.LeaveTriangle();
}
template<typename P>
void DContext::HandleEdges(P& dc)
{
	size_t n = vertexs.size();

	dc.EnterEdge();
	if (vtype == NX_TRIANGLES)
	{
		for (size_t i = 2; i<n; i += 3)
		{
			dc.HandleEdge(i - 2, i - 1);
			dc.HandleEdge(i - 1, i - 0);
			dc.HandleEdge(i - 0, i - 2);
		}
	}
	else if (vtype == NX_TRIANGLE_STRIP)
	{
		if (n > 2)
		{
			dc.HandleEdge(0, 1);
			for (size_t i = 2; i<n; i++)
			{
				dc.HandleEdge(i, i - 1);
				dc.HandleEdge(i, i - 2);
			}
		}

	}
	else if (vtype == NX_QUAD_STRIP)
	{
		if (n > 3)
		{
			dc.HandleEdge(0, 1);
			for (size_t i = 3; i<n; i += 2)
			{
				dc.HandleEdge(i - 1, i - 3);
				dc.HandleEdge(i - 0, i - 2);
				dc.HandleEdge(i - 1, i - 0);
			}
		}

	}
	else if (vtype == NX_QUADS)
	{
		for (size_t i = 3; i<n; i += 4)
		{
			dc.HandleEdge(i - 0, i - 1);
			dc.HandleEdge(i - 1, i - 2);
			dc.HandleEdge(i - 2, i - 3);
			dc.HandleEdge(i - 3, i - 0);
		}
	}
	else if (vtype == NX_LINES)
	{
		for (size_t i = 1; i<n; i += 2)
		{
			dc.HandleEdge(i - 1, i);
		}
	}
	else if (vtype == NX_LINE_LOOP)
	{
		if (n > 1)
		{
			for (size_t i = 1; i<n; i++)
			{
				dc.HandleEdge(i - 1, i);
			}
			dc.HandleEdge(n - 1, 0);
		}
	}
	else if (vtype == NX_LINE_STRIP)
	{
		for (size_t i = 1; i<n; i++)
		{
			dc.HandleEdge(i - 1, i);
		}
	}
	else if (vtype == NX_TRIANGLE_FAN)
	{
		if (n > 1)
		{
			dc.HandleEdge(0, 1);
			for (size_t i = 2; i < n; i++)
			{
				dc.HandleEdge(0, i);
				dc.HandleEdge(i - 1, i);
			}
		}
	}

	dc.LeaveEdge();
}


template<typename P>
void DContext::HandlePoints(P& dc)
{
	size_t n = vertexs.size();

	dc.EnterPoint();
	for (size_t i = 0; i < n; ++i)
	{
		dc.HandlePoint(i);
	}
	dc.LeavePoint();
}

template<typename P>
void DContext::HandleCtrlPoints(P& dc)
{

	if (flags.get(NX_CTRL_POINT) && vmask.get(NX_CTRL_POINT))
	{
		HandlePoints<P>(dc);
	}

	if (flags.get(NX_EDGE_CENTER) && vmask.get(NX_EDGE_CENTER))
	{
		HandleEdges<P>(dc);
	}

	if (flags.get(NX_FACE_CENTER) && vmask.get(NX_FACE_CENTER))
	{
		HandleTriangles<P>(dc);
	}

}


class DContextDataRange : public DContext
{
public:
	typedef DContext basetype;

	DContextDataRange();

	virtual void End();

	virtual void Value(double p);
	virtual void ValueArray(double* p, size_t n);

	bool RenderDocument(DObject* pdoc);

	void RenderTask(DTaskData* p);

	box3d cbox;

	double vmin, vmax;

};




class DTriangle
{
public:

	DTriangle();

	vec3d p1, p2, p3;
	vec3d nl;

	double zmin() { return std::min<double>(std::min<double>(p1[2], p2[2]), p3[2]); }
	double zmax() { return std::max<double>(std::max<double>(p1[2], p2[2]), p3[2]); }

	void update();

	void cshift(int d);

	int test(double x);
	int sety(double y);


	vec2d j1, j2;

	double zp;

	//int id;
	int wg;
	int fg;

	bool fp;

private:

	void _sety_real(vec3d& p1, vec3d& p2, double yp);

	template<int N>
	void _sety_real(vec3d& p1, vec3d& p2, double yp, vec2d& jd);
};


class DCtxReverse
{
public:
	DCtxReverse(DContext& _dc, bool _flag)
		: dc(_dc), flag(_flag)
	{
		if (flag)
		{
			dc.Reverse();
		}
	}

	~DCtxReverse()
	{
		if (flag)
		{
			dc.Reverse();
		}
	}

	bool flag;
	DContext& dc;
};

EW_LEAVE

#endif

