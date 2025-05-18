#ifndef __H_EW_SIMPLEX__
#define __H_EW_SIMPLEX__

#include "ewa_base/config.h"

EW_ENTER

template<typename T>
class OptiPolicy
{
public:

	class Data
	{
	public:
		T param;
		double value;
	};

	typedef T type;
	typedef Data value_type;
	typedef Data& reference;
	typedef Data* pointer;
	typedef const Data& const_reference;


	static void update(reference v);

	static void init_random(reference v);

	static bool init_value(reference) { return false; }

	static bool init_value(reference v, const type& value)
	{
		v.param = value;
		return false;
	}

	static bool init_value(reference v, const value_type& value)
	{
		v.param = value.param;
		v.value = value.value;
		return true;
	}

	static bool less(const_reference& v1, const_reference& v2)
	{
		return v1.value < v2.value;
	}

	static void set_or_swap(reference v1, reference v2)
	{
		v1 = v2;
	}

	static void swap(reference v1, reference v2)
	{
		std::swap(v1.value, v2.value);
		std::swap(v1.param, v2.param);
	}

	static bool init(){ return true; }

};


template<typename B>
class SaPolicy : public B
{
public:

	typedef B basetype;
	typedef typename basetype::type type;
	typedef typename basetype::value_type value_type;
	typedef typename basetype::reference reference;
	typedef typename basetype::const_reference const_reference;

	static size_t dimension();

	static double radius2(const_reference v1, const_reference v2);

	static void reflect(reference result, const_reference v2, const_reference v1, double rt)
	{
		result.param= v1.param + (v1.param - v2.param)*rt;
	}

	static void findcenter(reference result, const arr_1t<value_type>& arr, size_t bad)
	{
		size_t n = arr.size();
		EW_ASSERT(n > 1);

		result.param = 0;
		for (size_t i = 0; i < arr.size(); i++)
		{
			if(i!=bad) result.param += arr[i].param;
		}
		result.param /= double(n - 1);
	}

};


class OptiSaParam
{
public:

	OptiSaParam();

	double alpha, beta, gamma, sigma;
	int max_iter;
	int iter;
};

template<typename P>
class OptiSa
{
public:

	typedef typename P::type type;
	typedef typename P::value_type value_type;
	typedef typename P::reference reference;
	typedef typename P::const_reference const_reference;


	OptiSa(const P& p=P()) :policy(p)
	{
		key0 = key1 = key2 = -1;
	}

	P policy;
	OptiSaParam param;


	template<typename IT>
	bool init(IT item_beg,IT item_end)
	{
		if (!_do_init())
		{
			return false;
		}


		for (size_t i = 0; i <= N; i++)
		{
			if (item_beg >= item_end)
			{
				policy.init_random(nodes[i]);
			}
			else if (policy.init_value(nodes[i], *item_beg++))
			{
				continue;
			}

			policy.update(nodes[i]);

		}

		_do_find();
		return true;
	}


	template<typename X>
	bool init(const X& v)
	{
		if (!_do_init())
		{
			return false;
		}

		for (size_t i = 0; i <= N; i++)
		{

			if (i > 0)
			{
				policy.init_random(nodes[i]);
			}
			else if (policy.init_value(nodes[i], v))
			{
				continue;
			}	

			policy.update(nodes[i]);

		}

		_do_find();
		return true;
	}

	bool init()
	{
		if (Thread::this_thread().test_canceled())
		{
			return false;
		}

		if (!_do_init())
		{
			return false;
		}

		for (size_t i = 0; i <= N; i++)
		{
			if (i == key0) continue;
			policy.init_random(nodes[i]);
			policy.update(nodes[i]);
		}

		_do_find();
		return true;
	}

	bool test_end() const
	{
#ifndef NDEBUG
		// this_logger().LogMessage("iter = %d", param.iter);
#endif
		return param.iter > param.max_iter;
	}

	bool next()
	{
		if (Thread::this_thread().test_canceled())
		{
			return false;
		}

		++param.iter;

		if (test_end())
		{
			return false;
		}

		if (nodes.empty())
		{
			return init();
		}

		/* DEBUG INFO
		this_logger().LogMessage("alpha = %g", param.alpha);
		this_logger().LogMessage(" beta = %g", param.beta);
		this_logger().LogMessage("gamma = %g", param.gamma);
		this_logger().LogMessage("sigma = %g", param.sigma);
		*/

		_do_find();

		policy.reflect(node1,nodes[key2], center, param.alpha);
		policy.update(node1);

		if (policy.less(node1,nodes[key0]))
		{
			policy.reflect(node2,nodes[key2], center, param.gamma);
			policy.update(node2);
			if (node2.value < nodes[key0].value)
			{
				policy.set_or_swap(nodes[key2],node2);
			}
			else
			{
				policy.set_or_swap(nodes[key2],node1);
			}

			return true;
		}

		if (policy.less(node1,nodes[key1]))
		{
			policy.set_or_swap(nodes[key2],node1);
			return true;
		}

		if (policy.less(node1,nodes[key2]))
		{

			policy.reflect(node2, nodes[key2], center, param.beta);
			policy.update(node2);

			if (policy.less(node2,nodes[key2]))
			{
				policy.set_or_swap(nodes[key2],node2);
				return true;
			}
		}

		double rd = 0.0;
		for (size_t i = 0; i <= N; i++)
		{
			if (i == key0) continue;
			policy.reflect(nodes[i], nodes[i], nodes[key0], -0.5);
			rd += policy.radius2(nodes[i],nodes[key0]);
		}

		// 迭代停止条件1：小于收敛半径
		if (rd < param.sigma*param.sigma)
		{
			return false;
		}


		for (size_t i = 0; i <= N; i++)
		{
			if (i == key0) continue;
			policy.update(nodes[i]);
		}

		return true;
	}

	const_reference best()
	{
		return nodes.at(key0<nodes.size()?key0:0);
	}

	const arr_1t<value_type>& get_nodes() const { return nodes;}

private:

	arr_1t<value_type> nodes;
	value_type node1, node2;
	value_type center;

	size_t key0,key1,key2;
	size_t N;

	void _do_find()
	{
		key0 = key1 = key2 = 0;
		for (size_t i = 1; i <= N; i++)
		{
			if (policy.less(nodes[i], nodes[key0]))
			{
				key0 = i;
			}
			else if (policy.less(nodes[key2], nodes[i]))
			{
				key1 = key2; key2 = i;
			}
			else if (policy.less(nodes[key1], nodes[i]))
			{
				key1 = i;
			}
		}
		policy.findcenter(center, nodes, key2);
	}

	bool _do_init()
	{
		param.iter = 0;

		if (!policy.init())
		{
			return false;
		}

		if (nodes.empty())
		{
			N = policy.dimension();
			nodes.clear();
			nodes.resize(N + 1);
			for (auto it = nodes.begin(); it != nodes.end(); ++it)
			{
				policy.init_value(*it);
			}
			policy.init_value(node1);
			policy.init_value(node2);
			policy.init_value(center);
		}


		return true;
	}
};

EW_LEAVE
#endif
