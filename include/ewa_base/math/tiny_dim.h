#ifndef __H_EW_MATH_DIM__
#define __H_EW_MATH_DIM__

#include "ewa_base/config.h"
#include "ewa_base/math/tiny_vec.h"
//#include "ewa_base/math/math_def.h"

EW_ENTER

template <typename T, int N, template<typename, int>class U /* = tiny_storage, tiny_vec */>
struct tiny_dim
{
	typedef U<T, N> type;

	template<int K>
	static type rshift(const type& v)
	{
		if (K % N == 0) return v;

		type _v;
		for (int i = 0; i < N; ++i)
		{
			_v[((i + K) % N + N) % N] = v[i];
		}
		return _v;
	}

	template <int K>
	static type lshift(const type& v)
	{
		if (K % N == 0) return v;

		type _v;
		for (int i = 0; i < N; ++i)
		{
			_v[i] = v[((i + K) % N + N) % N];
		}
		return _v;
	}

	template <int I, int Axis = N - 1>
	static T& get(type& v)
	{
		return v[((Axis + I + 1) % N + N) % N];
	}

	template <int I, int Axis = N - 1>
	static const T& get(const type& v)
	{
		return v[((Axis + I + 1) % N + N) % N];
	}

	template <int Axis = N - 1>
	static U<T, N - 1> reduce(const type& v)
	{
		return _reduce(lshift<Axis + 1>(v));
	}

	template <int Axis = N>
	static U<T, N + 1> raise(const type& v, const T& t = T())
	{
		return tiny_dim<T, N + 1, U>::template rshift<Axis + 1>(_raise(v, t));
	}
private:

	static U<T, N-1> _reduce(const type& v)
	{
		U<T, N - 1> _v;
		for (int i = 0; i < N - 1; ++i)
		{
			_v[i] = v[i];
		}
		return _v;
	}
	static U<T, N+1> _raise(const type& v, const T& t = T())
	{
		U<T, N + 1> _v;
		for (int i = 0; i < N; ++i)
		{
			_v[i] = v[i];
		}
		_v[N] = t;
		return _v;
	}
};

template <int slice = 2>
struct dim3
{
	static const int _u = 0;
	static const int _v = 1;
	static const int _n = 2;

	static const int _yz = 0;
	static const int _zx = 1;
	static const int _xy = 2;

	template <typename T, template<typename, int> class U>
	static T& u(U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::get<_u, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static T& v(U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::get<_v, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static T& n(U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::get<_n, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static const T& u(const U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::template get<_u, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static const T& v(const U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::template get<_v, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static const T& n(const U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::template get<_n, slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static U<T, 2> reduce(const U<T, 3>& vec)
	{
		typedef tiny_dim<T, 3, U> D;
		return D::template reduce<slice>(vec);
	}

	template <typename T, template<typename, int> class U>
	static U<T, 3> raise(const U<T, 2>& vec, const T& t = T())
	{
		typedef tiny_dim<T, 2, U> D;
		return D::template raise<slice>(vec, t);
	}
};

template <int N>
struct tiny_levi
{
	typedef tiny_vec<int32_t, N> type;
	typedef tiny_dim<int32_t, N, tiny_vec> dim;
	static int levi(const type& v)
	{
		int num = 1;
		for (int i = 0; i < N - 1; i++)
		{
			if (v[i] == v[N - 1])
			{
				return 0;
			}
			else if (v[i] > v[N - 1])
			{
				num *= -1;
			}
		}

		return num * tiny_levi<N - 1>::levi(dim::reduce(v));
	}
};

template <>
struct tiny_levi < 1 >
{
	typedef tiny_vec<int32_t, 1> type;
	static int levi(const type&)
	{
		return 1;
	}
};

template <>
struct tiny_levi < 2 >
{
	typedef tiny_vec<int32_t, 2> type;
	static int levi(const type& v)
	{
		return (v[0] == v[1]) ? 0 : ((v[0] < v[1]) ? 1 : -1);
	}
};

template <>
struct tiny_levi < 3 >
{
	typedef tiny_vec<int32_t, 3> type;
	static int levi(const type& v)
	{
		if (v[0] == v[1] || v[1] == v[2] || v[0] == v[2]) return 0;
		if (v[0] < v[1] && v[1] < v[2]) return 1;
		if (v[1] < v[2] && v[2] < v[0]) return 1;
		if (v[2] < v[0] && v[0] < v[1]) return 1;
		return -1;
	}
};

EW_LEAVE

#endif
