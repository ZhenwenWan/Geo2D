#ifndef __UTIL_POLYGON_H__
#define __UTIL_POLYGON_H__


#include "ewa_base/config.h"
#include "ewa_base/math/math_def.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"

EW_ENTER


class XPolygonData : public ObjectData
{
public:
	arr_1t<vec2d> vertexs;
	arr_1t<arr_1t<int> > loops;
	arr_1t<vec3i> triangles;

	void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(XPolygonData, ObjectInfo);
};


template<typename T>
void polygon_normalize(tiny_vec<T, 2>* poly, size_t size)
{
	if (size < 3)
	{
		return;
	}

	if (poly[0]!=poly[size-1])
	{
		EW_NOOP();
	}

	T sum = 0.0;
	for (size_t i = 1; i < size; i++)
	{
		T val = cross(poly[i - 1], poly[i]);
		sum += val;
	}
	if (sum < T(0))
	{
		std::reverse(poly, poly+size);
	}
}

template<typename T,typename X>
void polygon_normalize(X* pidx, size_t size,tiny_vec<T, 2>* vpos)
{
	if (size < 3)
	{
		return;
	}

	if (vpos[pidx[0]]!= vpos[pidx[size-1]])
	{
		EW_NOOP();
	}

	T sum = 0.0;
	for (size_t i = 1; i < size; i++)
	{
		T val = cross(vpos[pidx[i - 1]], vpos[pidx[i]]);
		sum += val;
	}

	if (sum < T(0))
	{
		std::reverse(pidx, pidx+size);
	}
}

class PolygonGenerator
{
public:

	PolygonGenerator();

	void reset();
	void add(const arr_1t<vec2d>& polygon,int dir=+1);
	DataPtrT<XPolygonData> merge();

protected:

	DataPtrT<ObjectData> m_pImpl;

};

EW_LEAVE

#endif
