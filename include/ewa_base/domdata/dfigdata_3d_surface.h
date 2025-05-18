#ifndef __H_EW_DOMDATA_DMESH_SURFACE__
#define __H_EW_DOMDATA_DMESH_SURFACE__

#include "ewa_base/domdata/dfigdata.h"
#include "ewa_base/domdata/dfigdata_3d.h"
#include "ewa_base/domdata/dmesh_field.h"
#include "ewa_base/domdata/dmesh.h"
#include "ewa_base/domdata/dmesh_split.h"
EW_ENTER


class DFigDataSurface : public DFigData3D
{
public:
	typedef DFigData3D basetype;

	DFigDataSurface(const String& name="surface_data");

	FieldIndexParam param;
	mat4d m4grp;

	DataPtrT<DStlModel> pmodel;

	DataPtrT<VolumeFunctionT<vec3d> > p_value;

	int __getindex(Executor& ks, const String& si);
	

	void Serialize(SerializerHelper sh);


	DECLARE_DOMDATA_INFO(DFigDataSurface, DObjectInfo);
};



class DFigDataSurfaceTime : public DFigDataTimeT<DFigDataSurface>
{
public:

	typedef DFigDataTimeT<DFigDataSurface> basetype;

	DFigDataSurfaceTime(const String& name = "surface_time");

	class TimeSerialDataArray
	{
	public:
		arr_1t<TimeSerialDataItem> value;
		String name;
		DataPtrT<VolumeFunctionT<vec3d> > pfunc;

		void Serialize(SerializerHelper sh);
	};

	arr_1t<TimeSerialDataArray> m_aFiles;

	void Serialize(SerializerHelper sh);


	DECLARE_DOMDATA_INFO(DFigDataSurfaceTime, DObjectInfo);
};


EW_LEAVE
#endif
