#ifndef __H_EW_INDEXER_MAP__
#define __H_EW_INDEXER_MAP__

#include "ewa_base/collection/detail/indexer_container.h"


EW_ENTER


template<typename K,typename V,typename A,typename P>
class indexer_map : public indexer_container<P,A>
{
protected:
	typedef indexer_container<P,A> basetype;
	typedef typename basetype::impl_type impl_type;
	using basetype::impl;

public:

	typedef typename impl_type::mapped_type mapped_type;
	typedef typename impl_type::key_type key_type;
	typedef typename impl_type::index_type index_type;
	typedef typename impl_type::value_type value_type;
	typedef typename impl_type::size_type size_type;

	inline indexer_map() {}
	inline indexer_map(const indexer_map& o):basetype(o) {}

	indexer_map& operator=(const indexer_map& o)
	{
		this->impl = o.impl;
		return *this;
	}

#ifdef EW_C11
	indexer_map(indexer_map&& o){this->swap(o);}
	indexer_map& operator=(indexer_map&& o)
	{
		this->swap(o);
		return *this;
	}
#endif

	inline mapped_type& operator[](const key_type& k)
	{
		index_type id=impl.find2(k);
		return this->get(id).second;
	}

	inline const mapped_type& operator[](const key_type& k) const
	{
		index_type id=impl.find1(k);
		if(id==impl_type::invalid_pos) Exception::XNotFound();
		return this->get(id).second;
	}

};

EW_LEAVE

namespace std
{
	template<typename K,typename V,typename A,typename P>
	void swap(ew::indexer_map<K, V, A, P>& v1, ew::indexer_map<K, V, A, P>& v2)
	{
		v1.swap(v2);
	}
}

#endif
