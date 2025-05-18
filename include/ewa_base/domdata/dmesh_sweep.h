
#ifndef __H_EW_DOMDATA_DMESH_SWEEP__
#define __H_EW_DOMDATA_DMESH_SWEEP__

#include "ewa_base/domdata/dmesh.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE BSweepCtx
{
public:

	arr_1t<vec3d> vertexs;
	arr_1t<vec3i> triangles;

	arr_1t<vec2i> edges;
	arr_1t<vec3d> ndirs;

	int method;
	BitFlags flags;

	vec3d ref_position;
	vec3d ref_direction;


	void gen_edges();
	void gen_ndirs();

	void sweep(arr_1t<vec3d>& result, double thickness);
	void sweep(arr_1t<vec3d>& result, Variant thickness, Executor* plexer);

	DataPtrT<DStlModel> construct(const arr_1t<vec3d>& face1, const arr_1t<vec3d>& face2,bool reverse);

	void gen_ndirs_and_edges(DataPtrT<DStlModel> p);
	void gen_ndirs_and_edges(DataPtrT<DMeshDomainStl> p);

};




class DLLIMPEXP_EWA_BASE XSweepCtx
{
public:

	arr_1t<vec3d> vertexs;
	arr_1t<vec3i> triangles;
	arr_1t<vec2i> edges;
	arr_1t<vec3d> ndirs;
	arr_1t<double> vlength;
	arr_1t<double> thickness;

	void handle(DMeshDomain* p);

};



EW_LEAVE

#endif
