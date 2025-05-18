#ifndef __EW_SCRIPTING_VARIANT_BUFFER_H__
#define __EW_SCRIPTING_VARIANT_BUFFER_H__

//#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/scripting/callable_wrap.h"
#include "ewa_base/basic/stringbuffer.h"


EW_ENTER



template<typename T>
class DLLIMPEXP_EWA_BASE CallableWrapT<StringBufferT<T> > : public CallableDataBaseT<StringBufferT<T> >
{
public:
	typedef CallableDataBaseT<StringBufferT<T> > basetype;
	using basetype::value;

	typedef StringBufferT<T> arr_type;

	CallableWrapT():basetype(){}
	CallableWrapT(const StringBufferT<T>& v):basetype(v){}

	virtual CallableMetatable* GetMetaTable();

	virtual bool ToValue(String& s,int n=0) const;

	virtual bool ToJson(JsonWriter&);

	virtual CallableData* DoClone(ObjectCloneState&){return this;}
	virtual CallableData* DoCloneObject(){return this;}


	DECLARE_OBJECT_INFO_REAL_NO_CLONE(CallableWrapT, ObjectInfo,1);

};


EW_LEAVE
#endif
