#ifndef __EW_BASE_UTIL_RADIX_SORT_H__
#define __EW_BASE_UTIL_RADIX_SORT_H__


#include "ewa_base/config.h"
#include "ewa_base/collection/arr_1t.h"

EW_ENTER


namespace detail
{
	template<typename X, typename Y, typename F>
	void radix_sort_real(X it1, X it2, Y dst, F&& f, arr_1t<uint32_t>& cnt)
	{

		for (auto it = it1; it < it2; it++)
		{
			cnt[f(*it)]++;
		}

		uint32_t sum = 0;
		for (auto it = cnt.begin(); it != cnt.end(); ++it)
		{
			uint32_t nn = *it;
			*it = sum;
			sum += nn;
		}

		for (auto it = it1; it < it2; it++)
		{
			*(dst + cnt[f(*it)]++) = *it;
		}

	}
	template<typename X, typename Y, typename F>
	void radix_sort_n(X it1, X it2, Y dst, F&& f, size_t n)
	{
		arr_1t<uint32_t> cnt;
		cnt.resize(n);
		radix_sort_real(it1, it2, dst, f, cnt);
	}

	template<typename X, typename Y, typename F>
	void radix_sort_t(X it1, X it2, arr_1t<Y>& dst, arr_1t<Y> tmp, F&& f)
	{
		size_t count = std::distance(it1, it2);
		if (dst.size() < count) dst.resize(count);

		typedef typename std::iterator_traits<X>::value_type type;

		auto kmin = f(*it1);
		auto kmax = kmin;

		for (auto it = it1 + 1; it != it2; ++it)
		{
			auto k = f(*it);
			if (k < kmin) kmin = k;
			if (k > kmax) kmax = k;
		}

		size_t n = kmax - kmin;

		size_t N = 16;
		size_t S = size_t(1) << N;

		arr_1t<uint32_t> cnt;
		cnt.resize(S);

		if (kmin >= 0 && kmax < S)
		{
			radix_sort_real(it1, it2, dst.begin(), f, cnt);
			return;
		}


		class part_radix
		{
		public:

			int64_t kmin, mask, shift;
			F& f0;

			part_radix(F& f) :f0(f) {}

			inline size_t operator()(const type& v)
			{
				return ((f0(v) - kmin) >> shift) & mask;
			}


		};

		part_radix f2(f);
		f2.shift = 0;
		f2.mask = S - 1;
		f2.kmin = kmin;

		radix_sort_real(it1, it2, dst.begin(), f2, cnt);

		if (kmax - kmin < S)
		{
			return;
		}

		if (tmp.size() < count)
		{
			tmp.resize(count);
		}

		tmp.swap(dst);

		while (1)
		{
			f2.shift += N;
			std::fill(cnt.begin(), cnt.end(), 0);

			radix_sort_real(tmp.begin(), tmp.begin() + count, dst.begin(), f2, cnt);

			if ((kmax - kmin) >> f2.shift <= f2.mask)
			{
				break;
			}
		}


	}

}


class radix_direct
{
public:
	inline uint32_t operator()(uint32_t t) { return t; }
	inline uint64_t operator()(uint64_t t) { return t; }
	inline int32_t operator()(int32_t t) { return t; }
	inline int64_t operator()(int64_t t) { return t; }
};


template<typename T, typename F = radix_direct>
void radix_sort(arr_1t<T>& arr, size_t n, F&& f = F())
{

#ifndef EW_NDEBUG
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		auto val = f(*it);
		EW_ASSERT(val >= 0 && val < n);
	}
#endif

	arr_1t<T> tmp;
	tmp.resize(arr.size());
	detail::radix_sort_n(arr.begin(), arr.end(), tmp.begin(), n, f);
	tmp.swap(arr);
}

template<typename T, typename F = radix_direct>
void radix_sort(arr_1t<T>& arr, F&& f = F())
{
	arr_1t<T> tmp;
	tmp.resize(arr.size());
	tmp.swap(arr);

	detail::radix_sort_t(tmp.begin(), tmp.end(), arr, tmp, f);
}

template<typename IT, typename F = radix_direct>
void radix_sort(IT it1, IT it2, F&& f = F())
{
	typedef typename std::iterator_traits<IT>::value_type type;
	arr_1t<type> dst, tmp;
	dst.resize(std::distance(it1, it2));

	detail::radix_sort_t(it1, it2, dst, tmp, f);
	std::copy(dst.begin(), dst.end(), it1);
}


EW_LEAVE

#endif
