#ifndef __H_EW_BASIC_OBJECT_INFO__
#define __H_EW_BASIC_OBJECT_INFO__


#include "ewa_base/basic/atomic.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/basic/dlink.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/collection/indexer_set.h"
#include "ewa_base/collection/indexer_map.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/math/tiny_cpx.h"

#define INVOKETARGET_DEFINE(X)\
class InvokeTarget_##X : public ew::ObjectInfo\
{\
public:\
	InvokeTarget_##X() :ObjectInfo(#X){}\
	void DoInvoke(ew::InvokeParam& ipm)\
				{\
		if (\
			ipm.type != ew::InvokeParam::TYPE_CALL_ARGV ||\
			ipm.argvs.size() < 1 ||\
			ipm.argvs[0] != #X)	return;\
			RealInvoke();\
				}\
	void RealInvoke();\
}invoke_target_##X;\
void InvokeTarget_##X::RealInvoke()

EW_ENTER


class DLLIMPEXP_EWA_BASE Object;
class DLLIMPEXP_EWA_BASE Serializer;
class DLLIMPEXP_EWA_BASE ObjectInfo;
class DLLIMPEXP_EWA_BASE InvokeParam;


class DLLIMPEXP_EWA_BASE InvokeParam
{
public:
	enum
	{
		TYPE_NONE,
		TYPE_INIT,
		TYPE_FINI,
		TYPE_ON_CMDLINE_PARSED,
		TYPE_TRY_RELEASE_MEM,
		TYPE_CALL_ARGV,
	};


	InvokeParam(int t = TYPE_NONE);
	virtual ~InvokeParam();

	int type;
	BitFlags flags;
	Object* pobj;
	arr_1t<String> argvs;
	indexer_set<ObjectInfo*> infos;


	virtual void OnInvoke(ObjectInfo*);


};

class DLLIMPEXP_EWA_BASE ObjectInfo : public DLinkNode
{
public:


	static void Invoke(InvokeParam&);

	static void Invoke(int t);
	static void Invoke(const String& s);
	static void Register(ObjectInfo* p);

	ObjectInfo(const String& s = "");

	const String& GetName() const;

	virtual Object* CreateObject();

	virtual Object* CastFromObject(Object* p);

	virtual void DeleteObject(Object* p);

	virtual Object* GetCachedInstance();

	virtual Object* GetHelperObject(const String&);

	virtual void DoInvoke(InvokeParam&);

	template<typename T>
	T* GetHelperObjectT(const String& name)
	{
		return dynamic_cast<T*>(GetHelperObject(name));
	}

protected:
	virtual ~ObjectInfo();
	String m_sClassName;

};

template<typename T>
class ObjectInfoInvokable : public ObjectInfo
{
public:
	ObjectInfoInvokable(const String& s) :ObjectInfo(s){}
	virtual void DoInvoke(InvokeParam&);
};



class DLLIMPEXP_EWA_BASE ObjectCloneState
{
public:

	explicit inline ObjectCloneState(int t = 0) :type(t){}

	// if d is already cloned, return last cloned value else return d->Clone(*this)
	ObjectData* clone(ObjectData* d);

	// clear cloned state
	void clear();

	int type;
private:
	indexer_map<const ObjectData*, ObjectData*> aClonedState;
};

template<typename T, typename B, int N>
class ObjectInfoT;

template<typename T, typename B>
class ObjectInfoT<T,B,0> : public B
{
public:
	typedef B basetype;

	ObjectInfoT(const String &name) :basetype(name) {}

	Object *CreateObject()
	{
		return NULL;
	}

	T* CastFromObject(Object* p)
	{
		return dynamic_cast<T*>(p);
	}

	static inline T* Clone(const T&, ObjectCloneState&)
	{
		return NULL;
	}
};

template<typename T, typename B>
class ObjectInfoT<T,B,1> : public B
{
public:
	typedef B basetype;

	ObjectInfoT(const String &name) :basetype(name) {}

	Object *CreateObject()
	{
		return new T();
	}

	T* CastFromObject(Object* p)
	{
		return dynamic_cast<T*>(p);
	}

	static inline T* Clone(const T&, ObjectCloneState&)
	{
		return NULL;
	}
};


template<typename T, typename B>
class ObjectInfoT<T,B,2> : public B
{
public:
	typedef B basetype;

	ObjectInfoT(const String &name) :basetype(name) {}

	Object *CreateObject()
	{
		return new T();
	}

	T* CastFromObject(Object* p)
	{
		return dynamic_cast<T*>(p);
	}

	static inline T* Clone(T& obj, ObjectCloneState& cs)
	{
		return cs.type==0?(T*)&obj:new T(obj);
	}
};



EW_LEAVE

#endif
