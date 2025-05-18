#ifndef __EWA_MATH_DEF_H__
#define __EWA_MATH_DEF_H__

#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/math/tiny_vec.h"
#include "ewa_base/math/tiny_mat.h"
#include "ewa_base/math/tiny_box.h"

#define M_LIGHTSPEED		(2.99792458e8)

#define M_EPS0				(8.854187817e-12)
#define M_XMU0				(M_PI*4.0e-07)

#define M_HBAR				(1.0545887e-34)
#define M_EV				(1.60217653e-19)

#define M_DEG2RAD			(M_PI / 180.0)
#define M_RAD2DEG			(180.0 / M_PI)


EW_ENTER

typedef tiny_vec<size_t, 2> vec2ui;
typedef tiny_vec<size_t, 3> vec3ui;
typedef tiny_vec<size_t, 4> vec4ui;

typedef tiny_vec<int32_t,2> vec2i;
typedef tiny_vec<int32_t,3> vec3i;
typedef tiny_vec<int32_t,4> vec4i;

typedef tiny_vec<double,2> vec2d;
typedef tiny_vec<double,3> vec3d;
typedef tiny_vec<double,4> vec4d;

typedef tiny_vec<float,2> vec2f;
typedef tiny_vec<float,3> vec3f;
typedef tiny_vec<float,4> vec4f;

typedef tiny_vec<fcomplex, 2> vec2fc;
typedef tiny_vec<fcomplex, 3> vec3fc;
typedef tiny_vec<fcomplex, 4> vec4fc;

typedef tiny_vec<dcomplex, 2> vec2dc;
typedef tiny_vec<dcomplex, 3> vec3dc;
typedef tiny_vec<dcomplex, 4> vec4dc;

typedef tiny_box<int32_t,1> box1i;
typedef tiny_box<double,1> box1d;
typedef tiny_box<float,1> box1f;

typedef tiny_box<int32_t,2> box2i;
typedef tiny_box<double,2> box2d;
typedef tiny_box<float,2> box2f;

typedef tiny_box<int32_t,3> box3i;
typedef tiny_box<double,3> box3d;
typedef tiny_box<float,3> box3f;

typedef tiny_box<int32_t, 2> box2i;
typedef tiny_box<double, 2> box2d;
typedef tiny_box<float, 2> box2f;

typedef type_mat<double, 3, 3> mat3d;
typedef type_mat<double, 4, 4> mat4d;


typedef tiny_storage<String,3> vec3s;
typedef tiny_storage<String,2> vec2s;

typedef tiny_box<String, 3> box3s;
typedef tiny_box<String, 2> box2s;

DLLIMPEXP_EWA_BASE bool is_nan(double d);
DLLIMPEXP_EWA_BASE bool is_nan(float d);

DLLIMPEXP_EWA_BASE double round_digit(double v, unsigned n);
DLLIMPEXP_EWA_BASE double round(double v);


template<typename T>
void linspace_internal(T a, T b, T* p, size_t n)
{

	if (n == 0)
	{
		return;
	}

	if (a == b)
	{
		for (size_t i = 0; i < n; ++i)
		{
			p[i] = a;
		}
		return;
	}

	if (n == 1)
	{
		Exception::XError("invalid_number_of_points");
	}

	size_t m = n - 1;

	p[0] = a;
	p[m] = b;

	if (b > a)
	{
		for (size_t i = 1; i < m; ++i)
		{
			p[i] = (a * double(m - i) + b * double(i)) / double(m);
		}
	}
	else if (a > b)
	{
		for (size_t i = 1; i < m; ++i)
		{
			p[i] = (b * double(i) + a * double(m - i)) / double(m);
		}
	}
	else
	{
		Exception::XError("invalid_number");
	}
}

EW_LEAVE
#endif
