#ifndef __H_EW_OPTI_GA__
#define __H_EW_OPTI_GA__
#include "ewa_base/util/opti_sa.h"

EW_ENTER

class OptiGaParam
{
public:

	OptiGaParam();

	size_t num;
	double rt_jc;
	double rt_by;

	int max_iter;
	int iter;

};

template<typename B>
class GaPolicy : public B
{
public:
	typedef B basetype;
	typedef typename basetype::type type;
	typedef typename basetype::value_type value_type;
	typedef typename basetype::reference reference;
	typedef typename basetype::const_reference const_reference;

	static void bianyi(reference val);

	static void jiaocha(reference val, const_reference v2, const_reference v1);

	static double krand()
	{
		static const uint64_t mmm = 1000000;
		static uint64_t val = 123421313113;
		val = val * 1341261 + 134152 + (val >> 8);
		return double(val % (mmm + 1)) / double(mmm);
	}
};



template<typename P>
class OptiGa
{
public:

	typedef typename P::type type;
	typedef typename P::value_type value_type;

	OptiGa(const P& p = P()) :policy(p)
	{

	}

	P policy;
	OptiGaParam param;

	template<typename IT>
	bool init(IT item_beg, IT item_end)
	{
		if (!policy.init())
		{
			return false;
		}

		if (nodes.empty())
		{
			size_t n = (1.0 + param.rt_jc)*param.num;

			nodes.resize(n);

			it0 = nodes.begin();
			it1 = it0 + param.num;
			it2 = nodes.end();

			for(auto it=it0;it!=it2;it++)
			{
				if (item_beg >= item_end)
				{
					policy.init_random(*it);
				}
				else if(policy.init_value(*it,*item_beg++))
				{
					continue;
				}
				policy.update(*it);
			}
		}
		else
		{
			for (auto it = it1; it != it2; it++)
			{
				if (item_beg >= item_end)
				{
					policy.init_random(*it);
				}
				else if (policy.init_value(*it, *item_beg++))
				{
					continue;
				}
				policy.update(*it);
			}

		}
		select_parents();

		return true;
	}

	bool init()
	{
		param.iter = 1;

		if (!policy.init())
		{
			return false;
		}

		if (!nodes.empty())
		{
			return true;
		}

		size_t n = (1.0 + param.rt_jc)*param.num;

		nodes.resize(n);

		it0 = nodes.begin();
		it1 = it0 + param.num;
		it2 = nodes.end();

		for(auto it=it0;it!=it2;it++)
		{
			policy.init_random(*it);
		}

		for(auto it=it0;it!=it2;it++)
		{
			policy.update(*it);
		}

		select_parents();
	

		return true;
	}

	template<typename X>
	bool init(const X& v)
	{
		if (!init())
		{
			return false;
		}

		if (!policy.init_value(nodes[1],v))
		{
			policy.update(nodes[1]);
		}

		if (nodes[1].value < nodes[0].value)
		{
			policy.swap(nodes[0], nodes[1]);		
		}

		return true;
	}

	typename arr_1t<value_type>::iterator it0,it1, it2;



	const value_type& best()
	{
		return nodes.at(0);
	}

	bool next()
	{
		if (nodes.empty())
		{
			if (!init())
			{
				return false;
			}
			return ++param.iter < param.max_iter;
		}

		for(auto it=it1;it!=it2;it++)
		{

			double rd1 = policy.krand()*0.5;
			double rd2 = policy.krand();
			size_t kp1 = rd1*param.num;
			size_t kp2 = rd2*param.num;
			policy.jiaocha(*it, nodes[kp1], nodes[kp2]);
		}

		for (size_t i = 1; i < nodes.size(); i++)
		{
			if ( policy.krand()<param.rt_by)
			{
				policy.bianyi(nodes[i]);
				policy.update(nodes[i]);
			}
			else if (i >= param.num)
			{
				policy.update(nodes[i]);
			}
		}

		select_parents();
		return ++param.iter<param.max_iter;
	}

	void select_parents()
	{
		std::sort(it0,it2,policy.less);
		std::random_shuffle(it0+1,it1);
		std::random_shuffle(it0+param.num/2, it2);
		std::sort(it0,it1, policy.less);
	}

	const arr_1t<value_type>& get_nodes() const { return nodes; }

private:
	arr_1t<value_type> nodes;
};

EW_LEAVE
#endif
