#ifndef __H_EW_BASIC_OBJECT__
#define __H_EW_BASIC_OBJECT__

#include "ewa_base/collection/indexer_set.h"
#include "ewa_base/collection/indexer_map.h"
#include "ewa_base/basic/object_info.h"
#include "ewa_base/basic/atomic.h"


/*
ObjectInfoT<TYPE,INFO,N>
N=0
N=1 create
N=2 create clone
N=3 create clone by cached instance
N=4 all cached
*/

#define DECLARE_OBJECT_INFO_REAL_NO_CLONE(TYPE,INFO,N)	\
	public:\
	typedef INFO infobase;\
	typedef ObjectInfoT<TYPE,INFO,N> infotype;\
	static infotype sm_info;\
	virtual infobase& GetObjectInfo() const{return sm_info;}\
	virtual const String& GetObjectName() const {return sm_info.GetName();}\


#define DECLARE_OBJECT_INFO_REAL(TYPE,INFO,N)	\
	DECLARE_OBJECT_INFO_REAL_NO_CLONE(TYPE,INFO,N)\
	virtual TYPE* DoClone(ObjectCloneState& cs){ return sm_info.Clone(*this,cs); }



#define DECLARE_OBJECT_INFO(TYPE,INFO)	DECLARE_OBJECT_INFO_REAL(TYPE,INFO,1)
#define DECLARE_OBJECT_INFO_NO_CREATE(TYPE,INFO) 	DECLARE_OBJECT_INFO_REAL(TYPE,INFO,0)

#define DECLARE_OBJECT_INFO_CACHED(TYPE,INFO) DECLARE_OBJECT_INFO_REAL(TYPE,INFO,4) \
	virtual void Serialize(SerializerHelper){}\
	virtual TYPE* DoCloneObject(){ return this; }

#define DECLARE_OBJECT_BY_CLONE_INFO(TYPE,INFO)	DECLARE_OBJECT_INFO_REAL(TYPE,INFO,3)


#define IMPLEMENT_OBJECT_INFO_NAME(TYPE,INFO,NAME)	\
	TYPE::infotype TYPE::sm_info(NAME);\

#define IMPLEMENT_OBJECT_INFO(TYPE,INFO)	IMPLEMENT_OBJECT_INFO_NAME(TYPE,INFO,#TYPE)

#ifdef EW_MSVC
	#define IMPLEMENT_OBJECT_INFO_TEMPLATE(TYPE,INFO)	IMPLEMENT_OBJECT_INFO_NAME(TYPE,INFO,#TYPE)
#else
	#define IMPLEMENT_OBJECT_INFO_TEMPLATE(TYPE,INFO)	template<> IMPLEMENT_OBJECT_INFO_NAME(TYPE,INFO,#TYPE)
#endif


#define IMPLEMENT_OBJECT_INFO_NAME_T1(TYPE,INFO,NAME)	\
	template<typename T1> typename TYPE<T1>::infotype  TYPE<T1>::sm_info(ew::ObjectNameT<T1>::MakeName(NAME));

#define IMPLEMENT_OBJECT_INFO_NAME_N1(TYPE,INFO,NAME)	\
	template<int N1> typename TYPE<N1>::infotype  TYPE<N1>::sm_info(String::Format("%s#%d",NAME,N1));

#define IMPLEMENT_OBJECT_INFO_NAME_T2(TYPE,INFO,NAME)	\
	template<typename T1,typename T2> typename TYPE<T1,T2>::infotype TYPE<T1,T2>::sm_info(ew::ObjectNameT<T2>::MakeName(ew::ObjectNameT<T1>::MakeName(NAME)));


#define IMPLEMENT_OBJECT_INFO_T1(TYPE,INFO)	IMPLEMENT_OBJECT_INFO_NAME_T1(TYPE,INFO,#TYPE)
#define IMPLEMENT_OBJECT_INFO_T2(TYPE,INFO)	IMPLEMENT_OBJECT_INFO_NAME_T2(TYPE,INFO,#TYPE)

#define IMPLEMENT_OBJECT_INFO_N1(TYPE,INFO)	IMPLEMENT_OBJECT_INFO_NAME_N1(TYPE,INFO,#TYPE)


EW_ENTER

class DLLIMPEXP_EWA_BASE Serializer;
class DLLIMPEXP_EWA_BASE ObjectCloneState;

class DLLIMPEXP_EWA_BASE SerializerHelper
{
public:
	int ver;

	SerializerHelper(const SerializerHelper&);
	SerializerHelper(Serializer& ar);
	Serializer& ref(int v);

	int global_version();
	int build_version();

private:
	Serializer& m_ar;

};

class DLLIMPEXP_EWA_BASE Object	: public mp_obj
{
public:

	virtual ~Object();
	virtual void Serialize(SerializerHelper sh);


	typedef ObjectInfo infobase;
	typedef ObjectInfoT<Object, ObjectInfo, 1> infotype;
	static infotype sm_info;
	virtual infobase& GetObjectInfo() const;
	virtual const String& GetObjectName() const;

};



class DLLIMPEXP_EWA_BASE ObjectData : public Object
{
public:

	EW_FORCEINLINE ObjectData():m_counter(0){}
	EW_FORCEINLINE ObjectData(const ObjectData&):m_counter(0){}
	EW_FORCEINLINE ObjectData& operator=(const ObjectData&){return *this;}

	~ObjectData();

	// Increase reference counter,
	EW_FORCEINLINE void IncRef()
	{
		if (m_counter == 0)
		{
			on_init_counter();
		}		
		else if(m_counter&0x1)
		{
			AtomicOps::fetch_add(&m_counter,(uintptr_t)2);
		}
		else
		{
			RefCounter* p = (RefCounter*)m_counter;
			p->IncUseCount();
		}

#ifdef _DEBUG
		on_counter_changed(+1);
#endif

	}

	// Decrease reference counter,
	EW_FORCEINLINE void DecRef()
	{
		EW_ASSERT(m_counter!=0);

#ifdef _DEBUG
		on_counter_changed(-1);
#endif

		if ((m_counter & 0x1)==0)
		{
			RefCounter* p = (RefCounter*)m_counter;
			if (p->DecUseCount())
			{
				on_fini_counter();
			}
		}
		else if (AtomicOps::fetch_sub(&m_counter, (uintptr_t)2)==3)
		{
			on_fini_counter();
		}
	

	}

	// Get reference count.
	EW_FORCEINLINE int GetRef() const
	{
		if (!m_counter)
		{
			return 0;
		}

		if (m_counter & 0x1)
		{
			return m_counter >> 1;
		}

		RefCounter* p = (RefCounter*)m_counter;
		return p->GetUseCount();
	}

	RefCounter* GetRefCounter();

	virtual ObjectData* DoCloneObject();

	ObjectData* Clone(int t=0);

	template<typename T>
	T* CloneT(int t=0)
	{
		ObjectData* pd=Clone(t);
		if(!pd) return NULL;
		T* pt=dynamic_cast<T*>(pd);
		if(pt) return pt;
		pd->IncRef();
		pd->DecRef();
		return NULL;
	}


	// Increase reference count of p2, decrease reference count of p1, and set p1=p2;
	template<typename T>
	static typename tl::enable_if_c<tl::is_convertible<T,ObjectData> >::type locked_reset(T*& p1,T* p2)
	{
		if(p1==p2) return;
		if(p2) p2->IncRef();
		if(p1) p1->DecRef();
		p1=p2;
	}

	DECLARE_OBJECT_INFO(ObjectData,ObjectInfo)

protected:



	// 初始化计数器，注意，多线程环境下，可能存在多个线程同时进入这个函数的情况
	// 需要特别处理，可使用compare_exchange来处理
	// 如果需要使用外置计算数，需要重载这个函数，处理上要非常注意
	virtual void on_init_counter();

	// 计数降为0时的处理函数，默认行为是 delete this
	// 如果使用了外置计数器，需要进行额外的处理
	virtual void on_fini_counter();

	virtual void on_counter_changed(int);

	volatile uintptr_t m_counter;
};


template<typename T>
class ObjectValueT : public ObjectData
{
public:
	T value;
};



class DLLIMPEXP_EWA_BASE InternalRefData : public ObjectData
{
public:
	InternalRefData(Object* p = NULL) :pobj(p){}
	Object* pobj;

};

template<typename T>
class FunctionCreator
{
public:

	static Object* Create()
	{
		return new T();
	}
};


// ObjectName is used to generate template objects' names.

DEFINE_OBJECT_NAME(String,"str");
DEFINE_OBJECT_NAME(bool,"i01");
DEFINE_OBJECT_NAME(char,"i08");
DEFINE_OBJECT_NAME(wchar_t,"wch");
DEFINE_OBJECT_NAME(int32_t,"i32");
DEFINE_OBJECT_NAME(int64_t,"i64");
DEFINE_OBJECT_NAME(uint32_t,"u32");
DEFINE_OBJECT_NAME(uint64_t,"u64");
DEFINE_OBJECT_NAME(float32_t,"f32");
DEFINE_OBJECT_NAME(float64_t,"f64");

DEFINE_OBJECT_NAME(dcomplex, "cf64");
DEFINE_OBJECT_NAME(fcomplex, "cf32");

EW_LEAVE

#endif
