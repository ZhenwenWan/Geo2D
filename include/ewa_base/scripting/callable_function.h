#ifndef __H_EW_SCRIPTING_VARIANT_FUNCTION__
#define __H_EW_SCRIPTING_VARIANT_FUNCTION__

#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/scripting/helpdata.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/pointer.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE CallableFunction : public CallableObject
{
public:

	enum
	{
		FLAG_SHORT_NAME		= 1<<0,
		FLAG_IS_MFUN		= 1<<1,
		FLAG_IS_META		= 1<<2,
	};

	CallableFunction();
	CallableFunction(const String& s, int f = 0);

	virtual void Serialize(SerializerHelper sh);

	const String& GetName() const{return m_sName;}
	virtual CallableFunction* ToFunction(){return this;}

	BitFlags flags;

	virtual CallableData_helpdata* __get_helpdata();
	void __set_helpdata(const String& s);

	virtual int __fun_call(Executor& ks, int pm);	// top(...)
	virtual int __getindex(Executor& ks,const String& si); // top.s

	bool ToValue(String& v,int) const;

	DECLARE_OBJECT_INFO(CallableFunction,ObjectInfo);

protected:
	String m_sName;
	DataPtrT<CallableData_helpdata> m_pHelp;

};


EW_LEAVE
#endif
