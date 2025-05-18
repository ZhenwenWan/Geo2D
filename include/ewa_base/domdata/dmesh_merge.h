
#ifndef __H_EW_DOMDATA_DMESH_MERGE__
#define __H_EW_DOMDATA_DMESH_MERGE__


#include "ewa_base/domdata/dmesh.h"

EW_ENTER



class DLLIMPEXP_EWA_BASE DMeshDomainVisitorMerger : public DMeshDomainVisitor
{
public:

	indexer_set<vec3d> vertexs;
	arr_1t<vec3i> triangles;

	DMeshDomainVisitorMerger();

	virtual void visit_triangle(const vec3d& p1, const vec3d& p2, const vec3d& p3);

	DataPtrT<DStlModel> get();

};




EW_LEAVE

#endif

