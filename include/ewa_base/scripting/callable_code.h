#ifndef __H_EW_SCRIPTING_VARIANT_BYTECODE__
#define __H_EW_SCRIPTING_VARIANT_BYTECODE__

#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/collection/bst_set.h"

#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/bytecode_inst.h"
#include "ewa_base/scripting/callable_function.h"
#include "ewa_base/scripting/scanner.h"


EW_ENTER


class DLLIMPEXP_EWA_BASE CallableSource : public CallableData
{
public:
	CallableSource();
	CallableSource(const String& s);
	CallableSource(const char* s1,const char* s2);

	void Serialize(SerializerHelper sh);


	arr_1t<String> aLines;

	DECLARE_OBJECT_INFO(CallableSource, ObjectInfo);
};

class DLLIMPEXP_EWA_BASE CallableCode : public CallableFunction
{
public:

	friend class CodeGen;

	enum
	{
		STATE_SYSTEM_VARIABLES_LOADED	= 1<<0,
	};

	CallableCode();
	~CallableCode();

	arr_1t<XopInst> aInsts;
	arr_1t<Variant> aLLVar;

	arr_1t<tokInfo> aInfos;
	bst_set<String> aDepends;

	tokInfo tok_beg,tok_end;

	DataPtrT<CallableSource> pSource;

	uint32_t nLocal;	// stackframe size
	uint32_t nParam;	// function param count
	uint32_t nShift;	// captured target shift

	virtual CallableCode* ToCode(){return this;}

	int __fun_call(Executor&,int);

	void SetName(const String& s) { m_sName=s;}

	void Serialize(SerializerHelper sh);

	void __set_helpdata(const String& s);

	int __getindex(Executor& ks,const String& index);

	bool ToValue(String& v,int) const;

	DECLARE_OBJECT_INFO(CallableCode,ObjectInfo);

protected:

	BitFlags state;
	BitFlags cgs_flags;


};


class DLLIMPEXP_EWA_BASE CallableClosure : public CallableCode
{
public:

	typedef CallableCode basetype;

	CallableClosure();
	CallableClosure(CallableCode& o);
#ifdef EW_C11
	CallableClosure(CallableCode&& o);
#endif
	arr_1t<Variant> aCapture;

	int __fun_call(Executor& ks,int pm);

	virtual CallableClosure* ToClosure(){return this;}

	void Serialize(SerializerHelper sh);

	DECLARE_OBJECT_INFO(CallableClosure,ObjectInfo);
};


EW_LEAVE



#endif
