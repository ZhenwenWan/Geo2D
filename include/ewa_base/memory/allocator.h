
#ifndef __H_EW_MEMORY_ALLOCATOR__
#define __H_EW_MEMORY_ALLOCATOR__

#include "ewa_base/basic/exception.h"
#include "ewa_base/memory/mempool.h"
#include "ewa_base/collection/detail/collection_base.h"

EW_ENTER


#pragma push_macro("new")
#undef new


class AllocatorBase{};

template <typename T>
class AllocatorBaseT : public AllocatorBase
{
public:

	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef T *pointer;
	typedef const T *const_pointer;

	typedef T &reference;
	typedef const T &const_reference;

	inline pointer address(reference r){return &r;}
	inline const_pointer address(const_reference r) const{return &r;}
	inline void construct(pointer p, const value_type &o=value_type()){new(p) value_type(o);}

#ifdef EW_C11
	inline void construct(pointer p, value_type &&o){new(p) value_type(std::forward<value_type>(o));}
#endif

	inline void construct(pointer p){new(p) value_type();}
	inline void destroy(pointer p){EW_UNUSED(p);p->~value_type();}
	inline size_type max_size() const throw(){return (size_type(-1))/sizeof(value_type);}

	bool operator!=(const AllocatorBaseT &) const{return false;}

	// Returns true if and only if storage allocated from *this
	// can be deallocated from other, and vice versa.
	// Always returns true for stateless allocators.
	bool operator==(const AllocatorBaseT &other) const{return true;}

	static const size_t extraElem = 0;

};

class DLLIMPEXP_EWA_BASE MemPoolPaging;
class DLLIMPEXP_EWA_BASE MemPoolMalloc;

template <typename T,typename P=MemPoolPaging>
class AllocatorP : public AllocatorBaseT<T>
{
public:
	typedef AllocatorBaseT<T> basetype;
	typedef std::size_t size_type;
	typedef T *pointer;

	AllocatorP() throw() {}
	AllocatorP(const AllocatorP&) {}

	template <typename T2>
	AllocatorP(const AllocatorP<T2,P> &) throw() {}

	~AllocatorP(){}

	inline pointer allocate(size_type n)
	{
		if (n > basetype::max_size())
		{
			Exception::XBadAlloc_count(n);
		}
		return (pointer)P::current().allocate((n)*sizeof(T));
	}

	inline void deallocate(pointer p, size_type)
	{
		P::current().deallocate(p);
	}

	template <typename T2>
	struct rebind
	{
		typedef AllocatorP<T2,P> other;
	};
};




// allocate Extra storage of E before returned address
template<typename A,typename E>
class AllocatorE : public A
{
public:

	typedef A basetype;
	typedef typename A::pointer pointer;
	typedef typename A::size_type size_type;
	typedef typename A::value_type value_type;

	AllocatorE() {}

	template<typename A2>
	AllocatorE(const A2& o):A(o) {}

	static const size_t shiftElem=(sizeof(E)+sizeof(value_type)-1)/(sizeof(value_type));
	static const size_t extraElem = 0;

	inline pointer allocate(size_type n)
	{
		if (n > max_size())
		{
			Exception::XBadAlloc_count(n);
		}
		pointer p=basetype::allocate(n+shiftElem);
		new(p) E();
		return p+shiftElem;
	}

	static E& extra(pointer p)
	{
		if(!p)
		{
			static E g;
			return g;
		}
		return *(E*)(p-shiftElem);
	}

	inline void deallocate(pointer p, size_type n)
	{
		if(!p) return;
		basetype::deallocate(p-shiftElem,n);
	}

	inline size_type max_size() const throw()
	{
		return basetype::max_size()-shiftElem;
	}

	template <typename T2>
	struct rebind
	{
		typedef AllocatorE<typename A::template rebind<T2>::other,E> other;
	};

};


// allocate extra N elements at the end of storage
template<typename A,size_t N>
class AllocatorN : public A
{
public:

	typedef A basetype;
	typedef typename A::pointer pointer;
	typedef typename A::size_type size_type;
	typedef typename A::value_type value_type;

	AllocatorN() {}

	template<typename A2>
	AllocatorN(const A2& o):A(o) {}

	static const size_t extraElem = N;

	inline pointer allocate(size_type n)
	{
		size_t sz=n+N;
		if (sz < n)
		{
			Exception::XBadAlloc_count(n);
		}
		return basetype::allocate(sz);
	}

	inline void deallocate(pointer p, size_type n)
	{
		if(!p) return;
		basetype::deallocate(p,n+N);
	}

	inline size_type max_size() const throw()
	{
		return basetype::max_size()-N;
	}

	template <typename T2>
	struct rebind
	{
		typedef AllocatorN<typename A::template rebind<T2>::other,N> other;
	};

};

class DLLIMPEXP_EWA_BASE def_allocator : public AllocatorP<int,MemPoolPaging>
{
public:
	def_allocator() {}
	def_allocator(const AllocatorP<int,MemPoolPaging>&) {}
};

namespace detail
{
	template<typename A,bool F>
	class SizeHelperImpl;

	template<typename A>
	class SizeHelperImpl < A, true >
	{
	public:

		static size_t adj(size_t sz)
		{
			return mp_hint((A::extraElem + sz)*sizeof(typename A::value_type))/sizeof(typename A::value_type) - A::extraElem;
		}

		static size_t gen2(size_t n0,size_t n1,size_t max_size)
		{
			size_t max_elem = max_size - n0;
			size_t n2 = std::max(n0 >> 1, n1);
			if (n2 <= max_elem)
			{
				return adj(n0 + n2);
			}
			else if (n1 <= max_elem)
			{
				return adj(n0 + n1);
			}
			else
			{
				Exception::XBadAlloc_count(max_size);
			}
		}

	};

	template<typename A>
	class SizeHelperImpl < A, false >
	{
	public:

		static size_t adj(size_t sz)
		{
			return sz_helper::adj(sz,sizeof(size_t));
		}

		static size_t gen2(size_t n0, size_t n1, size_t max_size)
		{
			size_t max_elem = max_size - n0;
			if (n1 >= max_elem)
			{
				Exception::XBadAlloc_count(max_size);
			}
			size_t nd= sz_helper::gen(n0 + n1);
			if (nd - n0 >= max_elem)
			{
				return n0 + n1;
			}
			return nd;
		}

		//static size_t gen(size_t n0)
		//{
		//	return sz_helper::gen(n0);
		//}
	};


	template<typename A>
	class SizeHelper : public SizeHelperImpl < A, tl::is_convertible<A,AllocatorBase>::value >
	{
	public:

	};
}



#pragma pop_macro("new")

EW_LEAVE

#endif
