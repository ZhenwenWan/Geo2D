#ifndef __H_EW_BASIC_OBJECT_EX__
#define __H_EW_BASIC_OBJECT_EX__

#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/collection/arr_1t.h"

EW_ENTER


template<typename T>
class DataGrpT : public arr_1t < DataPtrT<T> >
{
public:



	typedef arr_1t<DataPtrT<T> > basetype;
	using basetype::size;
	using basetype::operator[];

	DataGrpT(){}
	DataGrpT(const DataPtrT<T>& o)
	{
		this->push_back(o);		
	}

	void Serialize(Serializer& ar)
	{
		ar & static_cast<basetype&>(*this);
	}
};



class DLLIMPEXP_EWA_BASE Creator : public ObjectData
{
public:

	template<typename T> T* CreateT(const String& name)
	{
		Object* pobj = Create(name);

		if (T* p = dynamic_cast<T*>(pobj))
		{
			return p;
		}

		if (pobj)
		{
			pobj->GetObjectInfo().DeleteObject(pobj);
		}

		return NULL;
	}

	virtual Object* Create(const String&)=0;

};

class DLLIMPEXP_EWA_BASE CreatorByInfo : public Creator
{
public:

	virtual void Remove(ObjectInfo* info) = 0;
	virtual void Append(ObjectInfo *info) = 0;
	virtual ObjectInfo *GetInfo(const String &name) = 0;
	virtual void Alias(const String& alias_name, const String& name) =0 ;

	static CreatorByInfo& current();
};


class DLLIMPEXP_EWA_BASE ObjectCreator : public ObjectT<Creator>
{
public:
	ObjectCreator();
	static ObjectCreator& current();

	Object* Create(const String& name);

	template<typename T>
	T* CreateT()
	{
		return this->template CreateT<T>(T::sm_info.GetName());
	}

	template<typename T>
	T* CreateT(const String& name)
	{
		if (!m_refData)
		{
			return NULL;
		}

		return m_refData->CreateT<T>(name);
	}

};




EW_LEAVE
#endif

