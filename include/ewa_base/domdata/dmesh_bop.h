
#ifndef __H_EW_DOMDATA_DMESH_BOP__
#define __H_EW_DOMDATA_DMESH_BOP__


#include "ewa_base/collection/tmp_xt.h"
#include "ewa_base/domdata/holder.h"
#include "ewa_base/domdata/dmesh.h"
#include "ewa_base/util/polygon.h"

EW_ENTER

inline vec2i make_edge(int pt1, int pt2)
{
	return pt1 < pt2 ? vec2i(pt1, pt2) : vec2i(pt2, pt1);
}

inline vec2i make_edge(const vec2i& edge)
{
	if (edge[0] <= edge[1]) return edge;
	return vec2i(edge[1], edge[0]);
}


class DLLIMPEXP_EWA_BASE DoubleTriangle
{
public:

	vec3d result;
	bool is_vec;

	bool edge_cross(const vec3d& A1, const vec3d& A2, const vec3d& B1, const vec3d& B2);
};

class DLLIMPEXP_EWA_BASE BOpSharedVertex : public ObjectData
{
public:
	indexer_set<vec3d> vertexs;
};

class DLLIMPEXP_EWA_BASE BOpSharedData : public BOpSharedVertex
{
public:
	BOpSharedData();

	indexer_set<vec2i> edges;

	DataPtrT<DMeshVertex> pvertex;
	DataGrpT<DMeshDomainStlEx> error_loops;

	void add_error_loops(arr_1t<int>& loops, int type);

	void add_error_cross(const vec3i& tri1, const vec3i& tri2, const vec2i& edge);

	DataPtrT<DMeshDomain> get_edges();
	const vec3d* get_vertex();


};

class DLLIMPEXP_EWA_BASE XTriCtx
{
public:

	XTriCtx();

	DataPtrT< BOpSharedData> pdata;
	vec3d v1, v2, v3;
	vec3d p1, p2;
	vec3d c1, c2;
	double r1, r2;

	int cnt;
	box3d bbox;

	bool flip;

	void set_cross(const vec3d& v);


	void set_tri(const vec3i& tri);
	void set_tri_no_update(const vec3i& tri);

	int test_edge(const vec3d& p1, const vec3d& p2);
	int test_edge(int k1,int k2);

	int handle_edge_tri(const vec2i& edge, const vec3i& tri);

	void update();

	double dis_p(const vec3d& p);

	bool is_point_in_line(const vec3d& pt, const vec3d& p1, const vec3d& p2, double& ratio);

	bool is_point_in_triangle(const vec3d& p);

	int test();
};


class EdgeInfo
{
public:
	linear_set<int> points;
};


class LinkFacesCtx;

class DLLIMPEXP_EWA_BASE DomainFaces
{
public:
	DataPtrT<BOpSharedData> pdata;
	arr_1t<vec3i> faces;
	arr_1t<linear_set<vec2i> > face_edges;
	indexer_map<vec2i, EdgeInfo> edges;

	box3d bbox;

	DataGrpT<DMeshDomainStl> stls;
	DataPtrT<DMeshDomainGrp> grps[2];

	void load_faces_stl(DMeshDomainStl* p);
	void load_faces_stl(DataGrpT<DMeshDomainStl>& arr);

	void handle_faces_stl(DMeshDomainStl* p, const mat4d& m4_);
	void handle_faces(DMeshDomain* p, const mat4d& m4_);

	void update_bbox();

	void test_solid();

	void load_domain(DMeshDomain* p);

	int is_point_in_solid(const vec3d& v);

	void update_edges_seq();

	int* handle_tri_loop(LinkFacesCtx& ctx, arr_1t<int>& tmp_loop,int* p1, int* p2);

	void link_faces();

	void generate_results(int domain);

	void add_edge_point(const vec2i& edge, int pt);
	int add_edge_point(const vec2i& edge, const vec3d& pt);

	void add_face_edge(int face, const vec2i& edge);

};

class BOpCtx
{
public:

	BOpCtx();

	void handle(DataPtrT<DStlModel> A, DataPtrT<DStlModel> B,int t1, int t2);

	DataPtrT<DStlModel> pstl;

};


class DLLIMPEXP_EWA_BASE MeshDomainCtx
{
public:

	static bool check_solid(const arr_1t<vec3i>& faces);

	bool is_solid();

	void set(const arr_1t<vec3i>& faces);

	void link(DataGrpT<DMeshDomainStl>& stls, DataPtrT<DMeshVertex> pvertex);

	void erase(const vec2i& edge);

	void erase(const arr_1t<vec2i>& edges);

protected:

	DataPtrT<ObjectData> p_impl;
};



EW_LEAVE
#endif
