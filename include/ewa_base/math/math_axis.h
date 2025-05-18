#ifndef __EWA_MATH_AXIS_H__
#define __EWA_MATH_AXIS_H__

#include "ewa_base/math/math_def.h"
#include "ewa_base/math/tiny_dim.h"

EW_ENTER

// 球坐标系统
namespace SCS
{
	typedef enum
	{
		MATH = -1,	// 数学习惯球坐标，后处理时选择对Matlab兼容
		ISO  = 0,	// 物理习惯球坐标，绝大部分地方用到的
		HCS  = 1,	// 地平坐标系，工程用途
		GCS  = 2	// 地理坐标系，工程用途，和HCS类似，但翻译名称上有所区别
	} enum_SCS;

	typedef enum
	{
		RAD = 0,	// 弧度制
		DEG = 1		// 角度制
	} enum_degree_mode;


	DLLIMPEXP_EWA_BASE extern const vec3d Ox; // (1,0,0)
	DLLIMPEXP_EWA_BASE extern const vec3d Oy; // (0,1,0)
	DLLIMPEXP_EWA_BASE extern const vec3d Oz; // (0,0,1)

	// ISO 规范：theta表示极角、phi表示方位角
	template<int TYPE = ISO> // 默认采用ISO规范
	struct impl_base
	{
		// cartesian -> spherical (theta, phi, radial)
		static inline vec3d cart2sph(double x, double y, double z)
		{
			double radial = ::sqrt(x*x + y*y + z*z);
			double theta = ::acos(z / radial);
			double phi = y>=0?::atan2(y, x):(::atan2(y,x)+M_PI*2);
			return vec3d(theta, phi, radial);
		}

		// spherical (theta, phi, radial) -> cartesian
		static inline vec3d sph2cart(double theta, double phi)
		{
			double cos_theta = ::cos(theta);
			double sin_theta = ::sin(theta);
			double cos_phi = ::cos(phi);
			double sin_phi = ::sin(phi);

			return vec3d(sin_theta*cos_phi, sin_theta*sin_phi, cos_theta);
		}

		static inline vec3d sph2cart_deg(double theta, double phi)
		{
			double cos_theta = cosdeg(theta);
			double sin_theta = sindeg(theta);
			double cos_phi = cosdeg(phi);
			double sin_phi = sindeg(phi);

			return vec3d(sin_theta*cos_phi, sin_theta*sin_phi, cos_theta);
		}

		// 输入该坐标系下的(theta,phi)返回坐标变换
		// 当给定(theta,phi)组合的时候，并不是只给出了方向（方位），而且包含了一个转动变换，该转动变换可以决定真正的极化矢量。使用该方式可以避免某些修正问题。
		static inline mat4d make_deg(double theta, double phi)
		{
			mat4d m4;
			m4.LoadIdentity();
			m4.Rotate(phi, Oz);
			m4.Rotate(theta, Oy);
			return m4;
		}
	};


	// MATH 规范：theta表示方位角，phi表示极角，与ISO规定相反
	// 注意：Matlab采用的是这个规范，注意在脚本语言中兼容
	template<>
	struct impl_base< MATH >
	{
		// cartesian -> spherical (theta, phi, radial)
		static inline vec3d cart2sph(double x, double y, double z)
		{
			double radial = ::sqrt(x*x + y*y + z*z);
			double phi = ::acos(z / radial);
			double theta = y >= 0 ? ::atan2(y, x) : (::atan2(y, x) + M_PI * 2); // 注意atan2的值域是[-pi,pi)，修正为习惯[0, 2pi)
			return vec3d(theta, phi, radial);
		}

		// spherical (theta, phi, radial) -> cartesian
		static inline vec3d sph2cart(double theta, double phi)
		{
			return impl_base<ISO>::sph2cart(phi, theta);
		}

		static inline vec3d sph2cart_deg(double theta, double phi)
		{
			return impl_base<ISO>::sph2cart_deg(phi, theta);
		}

		// 输入该坐标系下的(theta,phi)返回坐标变换，角度制
		// 当给定(theta,phi)组合的时候，并不是只给出了方向（方位），而且包含了一个转动变换，该转动变换可以决定真正的极化矢量。使用该方式可以避免某些修正问题。
		static inline mat4d make_deg(double theta, double phi)
		{
			return impl_base<ISO>::make_deg(phi, theta);
		}
	};

	// 地平坐标系
	// theta 表示高度角 elevation，水平面为0度，北极点为+90度，南极点位-90度
	// phi 表示方位角 azimuth
	template<>
	struct impl_base < HCS >
	{
		// cartesian -> spherical (elevation, azimuth, radial)
		static inline vec3d cart2sph(double x, double y, double z)
		{
			vec3d sph = impl_base<ISO>::cart2sph(x, y, z);
			sph[0] = M_PI_2 - sph[0]; // 90 - theta_ISO
			return sph;
		}

		// spherical -> cartesian
		static inline vec3d sph2cart(double elevation /* latitude */, double azimuth /* longitude */)
		{
			return impl_base<ISO>::sph2cart(M_PI_2 - elevation, azimuth);
		}

		static inline vec3d sph2cart_deg(double elevation /* latitude */, double azimuth /* longitude */)
		{
			return impl_base<ISO>::sph2cart_deg(90 - elevation, azimuth);
		}

		// 输入该坐标系下的(theta,phi)返回坐标变换，角度制
		// 当给定(theta,phi)组合的时候，并不是只给出了方向（方位），而且包含了一个转动变换，该转动变换可以决定真正的极化矢量。使用该方式可以避免某些修正问题。
		static inline mat4d make_deg(double elevation /* latitude */, double azimuth /* longitude */)
		{
			return impl_base<ISO>::make_deg(90 - elevation, azimuth);
		}
	};

	template<>
	struct impl_base< GCS > : public impl_base < HCS >
	{};

	template<int SCS_TYPE>
	struct impl : public impl_base < SCS_TYPE >
	{
		typedef impl_base<SCS_TYPE> basetype;
		using basetype::cart2sph;
		using basetype::sph2cart;
		using basetype::make_deg;

		static inline vec3d cart2sph(const vec3d& v)
		{
			return basetype::cart2sph(v[0], v[1], v[2]);
		}

		static inline vec3d cart2sph_deg(double x, double y, double z)
		{
			vec3d sph = basetype::cart2sph(x, y, z);
			sph[0] = sph[0] * M_RAD2DEG;
			sph[1] = sph[1] * M_RAD2DEG;
			return sph;
		}

		static inline vec3d cart2sph_deg(const vec3d& v)
		{
			return cart2sph_deg(v[0], v[1], v[2]);
		}

		static inline vec3d sph2cart(double theta /*HCS/GCS: elevation, latitude*/, double phi /*HCS/GCS: azimuth, longitude*/, double radial)
		{
			return basetype::sph2cart(theta, phi) * radial;
		}

		static inline vec3d sph2cart_deg(double theta /*HCS/GCS: elevation, latitude*/, double phi /*HCS/GCS: azimuth, longitude*/)
		{
			return basetype::sph2cart_deg(theta, phi);
		}

		static inline vec3d sph2cart_deg(double theta /*HCS/GCS: elevation, latitude*/, double phi /*HCS/GCS: azimuth, longitude*/, double radial)
		{
			return basetype::sph2cart_deg(theta, phi) * radial;
		}

		static inline mat4d make(double theta /*HCS/GCS: elevation, latitude*/, double phi /*HCS/GCS: azimuth, longitude*/)
		{
			return basetype::make_deg(theta * M_RAD2DEG, phi * M_RAD2DEG);
		}

		static inline mat4d make_deg(double theta /*HCS/GCS: elevation, latitude*/, double phi /*HCS/GCS: azimuth, longitude*/)
		{
			return basetype::make_deg(theta, phi);
		}
	};
};

// cartesian -> polar's theta angle, theta \in [0, 2\pi)
inline double cart2pol_theta(double x, double y)
{
	return (y >= 0) ? ::atan2(y, x) : (::atan2(y, x) + M_PI * 2);
}

inline double cart2pol_theta(const vec2d& v)
{
	return cart2pol_theta(v[0], v[1]);
}

inline double cart2pol_theta_deg(double x, double y)
{
	return cart2pol_theta(x, y) * M_RAD2DEG;
}

inline double cart2pol_theta_deg(const vec2d& v)
{
	return cart2pol_theta_deg(v[0], v[1]);
}

// cartesian -> polar coordinate: (x,y) -> (theta, radial), theta \in [0, 2\pi)
inline vec2d cart2pol(double x, double y)
{
	double theta = (y>=0)?::atan2(y, x):(::atan2(y, x) + M_PI*2);
	double radial = ::sqrt(x*x + y*y);
	return vec2d(theta, radial);
}

// cartesian -> polar coordinate: (x,y) -> (theta, radial), theta \in [0, 360)
inline vec2d cart2pol_deg(double x, double y)
{
	double theta = (y >= 0) ? ::atan2(y, x) : (::atan2(y, x) + M_PI * 2);
	double radial = ::sqrt(x*x + y*y);
	return vec2d(theta * M_RAD2DEG, radial);
}

inline vec2d pol2cart(double theta)
{
	return vec2d(::cos(theta), ::sin(theta));
}

inline vec2d pol2cart(double theta, double radial)
{
	return vec2d(::cos(theta) * radial, ::sin(theta) * radial);
}

inline vec2d pol2cart_deg(double theta)
{
	return vec2d(cosdeg(theta), sindeg(theta));

	//return pol2cart(theta * M_DEG2RAD);
}

inline vec2d pol2cart_deg(double theta, double radial)
{
	return vec2d(cosdeg(theta) * radial, sindeg(theta) * radial);

	/*return pol2cart(theta * M_DEG2RAD, radial);*/
}

// 默认的球坐标系为ISO球坐标系，即物理常用的球坐标系
typedef SCS::impl<SCS::ISO> SCS_ISO;   // ISO球坐标系，按标准的(theta, phi, radial)顺序定义
typedef SCS::impl<SCS::MATH> SCS_MATH; // 数学球坐标系和ISO球坐标系类似，但theta/phi的含义交换一下，注意：参数名称不变，因此应特别注意，这与MATLAB一致，可自然拓展至高维
typedef SCS::impl<SCS::HCS> SCS_HCS;   // 地平坐标系，用高度角（或俯仰角）、方位角定义
typedef SCS::impl<SCS::GCS> SCS_GCS;   // 地理坐标系，与HCS一致，可用于文档文字区分时表示差异

inline vec3d sph2cart(double theta, double phi)
{
	return SCS_ISO::sph2cart(theta, phi);
}

inline vec3d sph2cart(double theta, double phi, double radial)
{
	return SCS_ISO::sph2cart(theta, phi, radial);
}

inline vec3d sph2cart_deg(double theta, double phi)
{
	return SCS_ISO::sph2cart_deg(theta, phi);
}

inline vec3d sph2cart_deg(double theta, double phi, double radial)
{
	return SCS_ISO::sph2cart_deg(theta, phi, radial);
}

inline vec3d cart2sph(double x, double y, double z)
{
	return SCS_ISO::cart2sph(x, y, z);
}

inline vec3d cart2sph(const vec3d& v)
{
	return SCS_ISO::cart2sph(v);
}

inline vec3d cart2sph_deg(double x, double y, double z)
{
	return SCS_ISO::cart2sph_deg(x, y, z);
}

inline vec3d cart2sph_deg(const vec3d& v)
{
	return SCS_ISO::cart2sph_deg(v);
}

typedef mat4d(*matrix_func_t)(double, double);
typedef mat4d(*matrix_deg_func_t)(double, double);

// 获得(theta,phi)->mat4d的计算函数
inline matrix_func_t get_matrix_func(int mode)
{
	switch (mode)
	{
	case SCS::ISO:  return SCS_ISO::make;
	case SCS::MATH: return SCS_MATH::make;
	case SCS::HCS:  return SCS_HCS::make;
	case SCS::GCS:  return SCS_GCS::make;
	default:
		EW_ASSERT(false);
		return SCS_ISO::make;
	}
}

// 获得(theta,phi)->mat4d的计算函数
inline matrix_deg_func_t get_matrix_deg_func(int mode)
{
	switch (mode)
	{
	case SCS::ISO:  return SCS_ISO::make_deg;
	case SCS::MATH: return SCS_MATH::make_deg;
	case SCS::HCS:  return SCS_HCS::make_deg;
	case SCS::GCS:  return SCS_GCS::make_deg;
	default:
		EW_ASSERT(false);
		return SCS_ISO::make_deg;
	}
}


typedef vec3d(*sph2cart_func_t)(double, double);
typedef vec3d(*sph2cart_deg_func_t)(double, double);

// sph2cart function callback
inline sph2cart_func_t get_sph2cart_func(int mode)
{
	switch (mode)
	{
	case SCS::ISO: return SCS_ISO::sph2cart;
	case SCS::MATH: return SCS_MATH::sph2cart;
	case SCS::HCS: return SCS_HCS::sph2cart;
	case SCS::GCS: return SCS_GCS::sph2cart;
	default: return SCS_ISO::sph2cart;
	}
}

// sph2cart_deg function callback
inline sph2cart_deg_func_t get_sph2cart_deg_func(int mode)
{
	switch (mode)
	{
	case SCS::ISO: return SCS_ISO::sph2cart_deg;
	case SCS::MATH: return SCS_MATH::sph2cart_deg;
	case SCS::HCS: return SCS_HCS::sph2cart_deg;
	case SCS::GCS: return SCS_GCS::sph2cart_deg;
	default: return SCS_ISO::sph2cart_deg;
	}
}

template<typename T>
inline bool IsAligned(const tiny_vec<T, 3>& _vec, T eps = 1e-7)
{
	tiny_vec<T, 3> vec = _vec / _vec.length();
	int flag = (fabs(vec[0]) <= eps) | ((fabs(vec[1]) <= eps) << 1) | ((fabs(vec[2]) <= eps) << 2);

	enum
	{
		FLAG_AxisX = (1 << 1) | (1 << 2),
		FLAG_AxisY = (1 << 2) | 1,
		FLAG_AxisZ = 1 | (1 << 1)
	};

	switch (flag)
	{
	case FLAG_AxisX:
	case FLAG_AxisY:
	case FLAG_AxisZ:
		return true;
	default:
		return false;
	}
}

template<typename T>
inline bool IsStrictAligned(const tiny_vec<T, 3>& vec)
{
	int flag = (vec[0] == 0) | ((vec[1] == 0) << 1) | ((vec[2] == 0) << 2);

	enum
	{
		FLAG_AxisX = (1<<1) | (1<<2),
		FLAG_AxisY = (1<<2) | 1,
		FLAG_AxisZ = 1 | (1 << 1)
	};

	switch (flag)
	{
	case FLAG_AxisX:
	case FLAG_AxisY:
	case FLAG_AxisZ:
		return true;
	default:
		return false;
	}
}

template<typename T>
inline bool IsStrictMatAligned(const type_mat<T, 3, 3>& m3)
{
	vec3d Ou = m3 * SCS::Ox;
	vec3d Ov = m3 * SCS::Oy;
	vec3d On = m3 * SCS::Oz;
	return IsStrictAligned(Ou) && IsStrictAligned(Ov) && IsStrictAligned(On);
}

namespace detail
{
	// 一些辅助工具函数

	// 获得某个平面指标的UV坐标轴
	inline vec3d axis(int n, bool positive = true)
	{
		n = n % 3;
		if (n < 0)
		{
			n += 3;
		}

		vec3d v(0, 0, 0);
		v[n] = positive ? 1 : (-1);

		return v;
	}

	// 平面坐标轴映射规则
	inline void plane2uvn(int n, vec3d& eu, vec3d& ev, vec3d& en, bool positive = true)
	{
		n = n % 3;
		if (n < 0) n += 3;

		int u = (n + 1) % 3;
		int v = (n + 2) % 3;

		eu = axis(u);
		ev = axis(v, positive);
		en = axis(n, positive);
	}
}

/* 以下类型存在设计问题，暂时少用 */
class Axis2D
{
public:
	tiny_storage<vec2d, 2> uv;

	Axis2D()
	{
		uv[0] = vec2d(1, 0, 0);
		uv[1] = vec2d(0, 1, 0);
	}
	Axis2D(vec2d x, vec2d y)	{ u() = x; v() = y;}

	static const int _u = 0;
	static const int _v = 1;

	static vec2d base(int axis) { return Axis2D().uv[axis]; }

	vec2d& u() { return uv[0]; }
	vec2d& v() { return uv[1]; }

	const vec2d& u() const { return uv[0]; }
	const vec2d& v() const { return uv[1]; }

	bool IsOrtho() const {
		return fabs(dot(uv[0], uv[1])) < 1e-7;
	}

	bool IsNormal() const {
		return IsOrtho() && fabs(uv[0].length() - 1) < 1e-7 && fabs(uv[1].length() - 1) < 1e-7 ;
	}

	vec2d proj(vec2d pos) const { return vec2d(dot(uv[0], pos), dot(uv[1], pos)); }

public:
	void FromCartesian(double x, double y)
	{
		u()[0] = x; v()[1] = y;
	}

	void FromSpherical(double phi)
	{
		u() = vec2d(-sin(phi), cos(phi));	// phi()
		v() = vec2d(cos(phi), sin(phi));	// r()
	}

	void FromSphericalDeg(double phi)
	{
		FromSpherical(phi * M_DEG2RAD);
	}

	void GetSpherical(double& phi) const
	{
		phi = atan2(-u()[0], u()[1]);
	}

	void GetSphericalDeg(double& theta, double& phi) const
	{
		GetSpherical(phi);
		phi = phi * M_RAD2DEG;
	}

public:
	void ReduceFromMatrix(mat3d m3)
	{
		u() = m3 * vec2d(1, 0) - m3 * vec2d(0, 0);
		v() = m3 * vec2d(0, 1) - m3 * vec2d(0, 0);
	}

	void GetMatrix(mat3d& m3) const
	{
		m3 = GetMatrix();
	}

	mat3d GetMatrix() const
	{
		mat3d m3;
		m3(0, 0) = u()[0];	m3(0, 1) = v()[0];
		m3(1, 0) = u()[1];	m3(1, 1) = v()[1];
		return m3;
	}
};

/* 以下类型存在设计问题，暂时少用 */
class Cartesian2D : public Axis2D
{
public:
	static const int _x = 0;
	static const int _y = 1;

	vec2d& x() { return u(); }
	vec2d& y() { return v(); }

	const vec2d& x() const { return u(); }
	const vec2d& y() const { return v(); }

};

/* 以下类型存在设计问题，暂时少用 */
class Spherical2D : public Axis2D
{
public:
	static const int _phi = 0;
	static const int _r = 1;

	vec2d& phi() { return u(); }
	vec2d& r() { return v(); }

	const vec2d& phi() const { return u(); }
	const vec2d& r() const { return v(); }
};

/* 以下类型存在设计问题，暂时少用 */
class Axis3D
{
public:
	tiny_storage<vec3d, 3> uvn;

	Axis3D()
	{
		uvn[0] = vec3d(1, 0, 0);
		uvn[1] = vec3d(0, 1, 0);
		uvn[2] = vec3d(0, 0, 1);
	}
	Axis3D(vec3d x, vec3d y, vec3d z)	{	u() = x; v() = y; n() = z;	}

	static const int _u = 0;
	static const int _v = 1;
	static const int _n = 2;

	static vec3d base(int axis) { return Axis3D().uvn[axis]; }

	vec3d& u() { return uvn[0]; }
	vec3d& v() { return uvn[1]; }
	vec3d& n() { return uvn[2]; }

	const vec3d& u() const { return uvn[0]; }
	const vec3d& v() const { return uvn[1]; }
	const vec3d& n() const { return uvn[2]; }

	bool IsOrtho() const {
		return fabs(dot(uvn[0], uvn[1])) < 1e-7 && fabs(dot(uvn[1], uvn[2])) < 1e-7 && fabs(dot(uvn[2], uvn[0])) < 1e-7;
	}

	bool IsNormal() const {
		return IsOrtho() && fabs(uvn[0].length() - 1) < 1e-7 && fabs(uvn[1].length() - 1) < 1e-7 && fabs(uvn[2].length() - 1) < 1e-7;
	}

	vec3d proj(vec3d pos) const { return vec3d(dot(uvn[0], pos), dot(uvn[1], pos), dot(uvn[2], pos)); }

public:

	void FromCartesianDir(int dir)
	{
		switch (dir)
		{
		case (dim3<>::_u) :
			u() = vec3d(0, 1, 0);
			v() = vec3d(0, 0, 1);
			n() = vec3d(1, 0, 0);
			break;
		case (dim3<>::_v) :
			u() = vec3d(0, 0, 1);
			v() = vec3d(1, 0, 0);
			n() = vec3d(0, 1, 0);
			break;
		case (dim3<>::_n) :
		default :
			u() = vec3d(1, 0, 0);
			v() = vec3d(0, 1, 0);
			n() = vec3d(0, 0, 1);
			break;
		}
	}

	void FromCartesian(double x, double y, double z)
	{
		u()[0] = x; v()[1] = y; n()[2] = z;
	}

	void FromSpherical(double theta, double phi)
	{
		u() = vec3d(cos(theta)*cos(phi), cos(theta)*sin(phi), -sin(theta));
		v() = vec3d(-sin(phi), cos(phi), 0);
		n() = vec3d(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
	}

	void FromSphericalDeg(double theta, double phi)
	{
		FromSpherical(theta * M_DEG2RAD, phi * M_DEG2RAD);
	}

	void GetSpherical(double& theta, double& phi) const
	{
		phi = atan2(-v()[0], v()[1]);
		theta = atan2(-u()[2], n()[2]);
	}

	void GetSphericalDeg(double& theta, double& phi) const
	{
		GetSpherical(theta, phi);
		theta = theta * M_RAD2DEG;
		phi = phi * M_RAD2DEG;
	}

public:
	void ReduceFromMatrix(mat4d m4)
	{
		u() = m4 * vec3d(1, 0, 0) - m4 * vec3d(0, 0, 0);
		v() = m4 * vec3d(0, 1, 0) - m4 * vec3d(0, 0, 0);
		n() = m4 * vec3d(0, 0, 1) - m4 * vec3d(0, 0, 0);
	}

	void GetMatrix(mat4d& m4) const
	{
		m4 = GetMatrix();
	}

	mat4d GetMatrix() const
	{
		mat4d m4;
		m4(0, 0) = u()[0];	m4(0, 1) = v()[0];	m4(0, 2) = n()[0];
		m4(1, 0) = u()[1];	m4(1, 1) = v()[1];	m4(1, 2) = n()[1];
		m4(2, 0) = u()[2];	m4(2, 1) = v()[2];	m4(2, 2) = n()[2];
		return m4;
	}
};

/* 以下类型存在设计问题，暂时少用 */
class Cartesian3D : public Axis3D
{
public:
	static const int _x = 0;
	static const int _y = 1;
	static const int _z = 2;

	vec3d& x() { return u(); }
	vec3d& y() { return v(); }
	vec3d& z() { return n(); }

	const vec3d& x() const { return u(); }
	const vec3d& y() const { return v(); }
	const vec3d& z() const { return n(); }
};

class Spherical3D : public Axis3D
{
public:
	static const int _theta = 0;
	static const int _phi = 1;
	static const int _r = 2;

	vec3d& theta() { return u(); }
	vec3d& phi() { return v(); }
	vec3d& r() { return n(); }

	const vec3d& theta() const { return u(); }
	const vec3d& phi() const { return v(); }
	const vec3d& r() const { return n(); }
};

EW_LEAVE

#endif
