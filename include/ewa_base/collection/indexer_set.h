#ifndef __H_EW_INDEXER_SET__
#define __H_EW_INDEXER_SET__

#include "ewa_base/collection/detail/indexer_container.h"

EW_ENTER


template<typename K,typename A=def_allocator,typename P=indexer_trait<K,void,int> >
class indexer_set : public indexer_container<P,A>
{
protected:
	typedef indexer_container<P,A> basetype;
	typedef typename basetype::impl_type impl_type;
	using basetype::impl;

public:

	typedef typename impl_type::key_type key_type;
	typedef typename impl_type::index_type index_type;
	typedef typename impl_type::value_type value_type;
	typedef typename impl_type::size_type size_type;

	inline indexer_set(){}
	inline indexer_set(const indexer_set& o):basetype(o) {}

	indexer_set& operator=(const indexer_set& o)
	{
		this->impl = o.impl;
		return *this;
	}

#ifdef EW_C11
	indexer_set(indexer_set&& o){this->swap(o);}
	indexer_set& operator=(indexer_set&& o)
	{
		this->swap(o);
		return *this;
	}
#endif

};

EW_LEAVE

namespace std
{
	template<typename T, typename A, typename P>
	void swap(ew::indexer_set<T, A, P>& v1, ew::indexer_set<T, A, P>& v2)
	{
		v1.swap(v2);
	}
}
#endif
