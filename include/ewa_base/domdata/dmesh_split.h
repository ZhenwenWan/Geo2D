
#ifndef __H_EW_DOMDATA_DMESH_SPLIT__
#define __H_EW_DOMDATA_DMESH_SPLIT__

#include "ewa_base/domdata/dmesh.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE DMeshDomainVisitorBBox : public DMeshDomainVisitor
{
public:

	DMeshDomainVisitorBBox()
	{
		bbox.load_min();
	}

	box3d bbox;

	virtual void visit_triangle(const vec3d& p1, const vec3d& p2, const vec3d& p3)
	{
		bbox.add(m4tmp*p1);
		bbox.add(m4tmp*p2);
		bbox.add(m4tmp*p3);
	}

	static box3d calc(DMeshDomain* result)
	{
		DMeshDomainVisitorBBox vbox;
		vbox.handle(result);
		return vbox.bbox;
	}

	static box3d calc(DataGrpT<DMeshDomain>& result)
	{
		DMeshDomainVisitorBBox vbox;
		for (auto it = result.begin(); it != result.end(); ++it)
		{
			vbox.handle((*it).get());
		}
		return vbox.bbox;
	}


};

class DLLIMPEXP_EWA_BASE DMeshDomainVisitorInfo : public DMeshDomainVisitor
{
public:
	DMeshDomainVisitorInfo() : numVertex(0), numTriangle(0) {}

	size_t numVertex;
	size_t numTriangle;

	virtual void visit(DMeshDomainStl* p)
	{
		numVertex += (p->pvertex.get()) ? p->pvertex->vertexs.size() : 0;
		numTriangle += p->triangles.size();
	}
};


class DLLIMPEXP_EWA_BASE DMeshDomainVisitorMesher : public DMeshDomainVisitor
{
public:

	typedef DMeshDomainVisitor basetype;

	indexer_set<vec3d> vertexs;
	arr_1t<vec3i> triangles;

	DataPtrT<DMeshDomainStl> get_mesh_stl();

	void apply(int d, double v = std::numeric_limits<double>::max(),double eps=1.0e-5);

	static void remesh(DStlModel* p, int d, double v = std::numeric_limits<double>::max(), double eps = 1.0e-5);


private:
	box3d cbox;

	double edge_hint_1;
	double edge_hint_2;

	virtual void visit_triangle(const vec3d& p1, const vec3d& p2, const vec3d& p3);


};


class StlMeshRefinePolicy : public Object
{
public:

	static void init(DStlModel*, mat4d){}
	static void fini(DataPtrT<DStlModel>&){}

	static DataPtrT<DMeshDomainStl> get_mesh_stl(DMeshDomainVisitorMesher& ctx)
	{
		return ctx.get_mesh_stl();
	}
};

template<typename P>
class StlMeshRefineContext
{
public:

	DMeshDomainVisitorMesher meshp;
	P policy;

	DataPtrT<DStlModel> remesh(DStlModel* p, mat4d m4)
	{
		policy.init(p, m4);

		DataPtrT<DStlModel> p2(new DStlModel);
		p2->children = p->children;
		remesh_children(p2->children, m4*p->m4tmp);

		policy.fini(p2);
		return p2;
	}


	void remesh_children(DataGrpT<DMeshDomain>& children, mat4d m4)
	{
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			DMeshDomain* pitem = (*it).get();

			if (DMeshDomainStl* p = dynamic_cast<DMeshDomainStl*>(pitem))
			{
				meshp.handle(p, m4);
				(*it) = policy.get_mesh_stl(meshp);
			}
			else if (DMeshDomainGrp* p = dynamic_cast<DMeshDomainGrp*>(pitem))
			{
				DataPtrT<DMeshDomainGrp> p2 = new DMeshDomainGrp;
				p2->children = p->children;
				remesh_children(p2->children, m4*p->m4tmp);
				(*it) = p2;
			}
			else
			{
				AtomicSpin::noop();
			}
		}
	}
};




class DLLIMPEXP_EWA_BASE MeshSplitterZ
{
public:

	MeshSplitterZ();

	mat4d m4c;
	DataGrpT<DMeshDomain> src;

	arr_1t<DataGrpT<DMeshDomain> > dst;

	mat4d m4d;
	arr_1t<double> pos;


	bool apply();

	void save(const String& file);

};



class CylinderSplitter
{
public:

	arr_xt<DataPtrT<DMeshDomain> > dst;
	DataPtrT<DMeshDomain> src;
	arr_1t<double> pos;
	arr_1t<double> phi;
	mat4d m4d;

	void split();


};

class DLLIMPEXP_EWA_BASE XYMeshSplitter
{
public:

	arr_xt<DataPtrT<DMeshDomain> > dst;
	DataPtrT<DMeshDomain> src;

	arr_1t<double> u_pos;
	arr_1t<double> v_pos;
	mat4d m4d;

	void split();


};




class DMeshDomainVisitorSplitter : public DMeshDomainVisitor
{
public:


	indexer_set<vec3d> vertexs;
	arr_1t<vec3i> triangles;

	arr_1t<vec3d> vpos;
	arr_1t<int> zidx;
	arr_1t<double> zpos;


	box3d bbox;

	arr_1t<arr_1t<vec3i> > domain_triangles;

	mat4d m4grp, m4inv;

	void setdir(const vec3d& v3dir);

	void apply(const arr_1t<double>& keyval);

	virtual void visit(DMeshDomainStl* p);

	DataPtrT<DStlModel> get();

protected:


	vec3d handle_vec(const vec3d& v1, const vec3d& v2, double z);

	template<int N>
	void handle_tri(const int* idx, vec3d* ptx);

	void add_tri(const vec3d& p1, const vec3d& p2, const vec3d& p3, int d);

	void _apply_initzidx();
	void _apply_generate();


};



EW_LEAVE

#endif
