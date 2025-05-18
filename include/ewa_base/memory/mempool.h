
#ifndef __H_EW_MEMORY_MEMPOOL__
#define __H_EW_MEMORY_MEMPOOL__

#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/atomic.h"
#include "ewa_base/basic/misc.h"

EW_ENTER

DLLIMPEXP_EWA_BASE void* mp_alloc(size_t n);
DLLIMPEXP_EWA_BASE void* mp_alloc(size_t n,const char* f,int line);
DLLIMPEXP_EWA_BASE void* mp_realloc(void* p,size_t n);
DLLIMPEXP_EWA_BASE void mp_free(void* p);
DLLIMPEXP_EWA_BASE size_t page_limit(size_t n);

DLLIMPEXP_EWA_BASE void mp_force_gc(int level);

DLLIMPEXP_EWA_BASE void mp_break_alloc(int n);
DLLIMPEXP_EWA_BASE void mp_check_leak(int t);

DLLIMPEXP_EWA_BASE void mp_fill_memory(const char* s);

DLLIMPEXP_EWA_BASE void* mp_alloc_with_page_protect(size_t n);
DLLIMPEXP_EWA_BASE void mp_free_with_page_protect(void* p);

DLLIMPEXP_EWA_BASE void* page_alloc(size_t n);
DLLIMPEXP_EWA_BASE void page_free(void* p,size_t n);
DLLIMPEXP_EWA_BASE int page_access(void* p,size_t n,int f);

DLLIMPEXP_EWA_BASE size_t mp_hint(size_t n);

DLLIMPEXP_EWA_BASE intptr_t mp_size(void* p);


class DLLIMPEXP_EWA_BASE MemPoolPaging
{
public:

	// Allocate memory with given size, if the given size exceed the max fixed-size, mempoll will use malloc to allocate.
	static void* allocate(size_t size);

	// Deallocate memory. if p is NOT allocated by this mempool, mempool will use free to deallocate.
	static void deallocate(void* p);


	static void* allocate(size_t size,const char* file,int line);

	static void deallocate(void* p,size_t)
	{
		deallocate(p);
	}

	static inline MemPoolPaging& current()
	{
		return detail::StaticInstanceEmptyObject<MemPoolPaging>::current();
	}

};


// MemPoolMalloc use malloc/free to allocate/deallocate memory.
class DLLIMPEXP_EWA_BASE MemPoolMalloc
{
public:

	static void* allocate(size_t size);

	static void* allocate(size_t size,const char*,int)
	{
		return allocate(size);
	}

	static void deallocate(void* p);

	static void deallocate(void* p,size_t)
	{
		deallocate(p);
	}

	static inline MemPoolMalloc& current()
	{
		return detail::StaticInstanceEmptyObject<MemPoolMalloc>::current();
	}
};


typedef MemPoolPaging MemPool;


class DLLIMPEXP_EWA_BASE MpAllocNode
{
public:
	MpAllocNode* nd_next;
	static void link(void* p1,size_t sz,size_t tp,void* p3=NULL);
};


// TS = ThreadSafe
// DG = DynamicGrow
template<size_t N,bool TS,bool DG>
class DLLIMPEXP_EWA_BASE MpFixedSizePoolBase : public ThreadSafe<TS>
{
public:
	static const size_t nd_size=(N+sizeof(void*)-1)&~(sizeof(void*)-1);
	static const size_t sp_size=1024*1024;

	typedef ThreadSafe<TS> basetype;

	MpAllocNode* nd_free;
	MpAllocNode* pg_list;

	void swap(MpFixedSizePoolBase& p);

	MpFixedSizePoolBase():nd_free(NULL),pg_list(NULL){}

	MpFixedSizePoolBase(const MpFixedSizePoolBase&):nd_free(NULL),pg_list(NULL){}
	MpFixedSizePoolBase& operator=(const MpFixedSizePoolBase&){return *this;}

	~MpFixedSizePoolBase();

	void* alloc();

	void dealloc(void* p);

	void dealloc_list(void* head,void* tail);

	void dealloc_all();

	bool alloc_batch();

protected:

	bool alloc_batch_nolock();


};

template<size_t N, bool TS = false, bool DG = true>
class DLLIMPEXP_EWA_BASE MpFixedSizePool : public MpFixedSizePoolBase<(N+sizeof(void*)-1)&~(sizeof(void*)-1),TS,DG>
{
public:

};


EW_LEAVE

#endif
