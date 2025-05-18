#ifndef __MISC_H__
#define __MISC_H__

/* 难以归类的杂项工具 */

#include "ewa_base/config.h"
#include "ewa_base/scripting/variant.h"


EW_ENTER
// Variant -> Numerics

// 脚本变量表转换为其他常见C++基本类型，弱转换，转换失败返回false
bool variant2m4d(const Variant&, mat4d&);
bool variant2v3d(const Variant&, vec3d&);
bool variant2v3dc(const Variant&, vec3dc&);
bool variant2b3d(const Variant&, box3d&);

// 转换失败抛出异常XBadCast
mat4d variant2m4d_(const Variant&);
vec3d variant2v3d_(const Variant&);
vec3dc variant2v3dc_(const Variant&);
box3d variant2b3d_(const Variant&);

bool variant2v3ds(const Variant&, arr_1t<vec3d>& v, bool transp = false);
bool variant2v3dcs(const Variant&, arr_1t<vec3d>& v, bool transp = false);

void variant2v3ds_(const Variant&, arr_1t<vec3d>&, bool transp = false);
void variant2v3dcs_(const Variant&, arr_1t<vec3dc>&, bool transp = false);

template<typename T>
Variant arr2variant(const T* ptr, int s0, int s1 = 1, int s2 = 1, int s3 = 1, int s4 = 1, int s5 = 1)
{
	Variant ret;
	arr_xt<T>& arr = ret.ref<arr_xt<T> >();

	arr.resize(s0, s1, s2, s3, s4, s5);

	for (size_t i = 0; i < arr.size(); ++i)
	{
		arr[i] = ptr[i];
	}

	return ret;
}

// Numerics -> Variant
Variant m4d2variant(const mat4d&);
Variant v3d2variant(const vec3d&);
Variant v3dc2variant(const vec3dc&);
Variant b3d2variant(const box3d&);

Variant v3ds2variant(const arr_1t<vec3d>&);
Variant v3dcs2variant(const arr_1t<vec3dc>&);




class ArrayWriter
{
public:

	template<typename X>
	void handle_array(X& sb, const Variant& v)
	{
		if (const arr_xt<int64_t>* p = v.ptr < arr_xt<int64_t> >())
		{
			handle_array(sb, *p);
		}
		else if (const arr_xt<double>* p = v.ptr < arr_xt<double> >())
		{
			handle_array(sb, *p);
		}
		else if (const arr_xt<dcomplex>* p = v.ptr < arr_xt<dcomplex> >())
		{
			handle_array(sb, *p);
		}
		else
		{
			Exception::XError("unknown_array_type");
		}

	}

	template<typename X, typename T>
	void handle_value(X& sb, const T& v)
	{
		sb << v;
	}

	template<typename X,typename T>
	void handle_array(X& sb, const arr_xt<T>& v)
	{
		if (v.size(2) == 1)
		{
			for (size_t i = 0; i < v.size(0); ++i)
			{
				for (size_t j = 0; j < v.size(1); ++j)
				{
					if (j != 0) sb << " ";
					handle_value(sb, v(i, j));
				}
				sb << "\r\n";
			}
		}
		else
		{
			for (size_t k = 0; k < v.size(2); k++)
			{
				sb << "k=" << k << "\r\n";
				for (size_t i = 0; i < v.size(0); ++i)
				{
					for (size_t j = 0; j < v.size(1); ++j)
					{
						if (j != 0) sb << " ";
						handle_value(sb, v(i, j, k));
					}
					sb << "\r\n";
				}
				sb << "\r\n";
			}
		}

	}
};



class MxApp
{
public:

	MxApp();

	int run(const String& name, const arr_xt<Variant>& args);

	int run(int argc, char** argv);

};

EW_LEAVE
#endif