#ifndef __EW_SCRIPTING_VARIANT_COROUTINE_H__
#define __EW_SCRIPTING_VARIANT_COROUTINE_H__

#include "ewa_base/scripting/stackstate.h"
#include "ewa_base/scripting/callable_data.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE CallableCoroutine : public CallableObject
{
public:

	explicit CallableCoroutine(size_t stack_size = 0);
	~CallableCoroutine();

	void reset(size_t stack_size);

	class InternalData : public CallableObject
	{
	public:

		InternalData(CallableCoroutine* p) :pdata(p), state(0)
		{

		}

		LitePtrT<CallableCoroutine> pdata;

		int state;


		void UnLink()
		{
			pdata.reset(NULL);
		}

	};



	DataPtrT<InternalData> p_shared;


	static CallableCoroutine* get_colast(Executor& ks);

	enum
	{
		STATE_DEAD,
		STATE_SUSPEND,
		STATE_RUNNING,
	};

	Variant aLLVar[4];

	class tagCatch
	{
	public:
		StackState0 ci0;
		int frm;
	};

	arr_1t<XopInst> aInsts;
	arr_1t<Variant> aStack;
	arr_1t<StackState0> aFrame;
	arr_1t<tagCatch> aCatch;

	//bool bResumeOrYield;


	int nState;

	StackState0 ci0;
	StackState1 ci1;

	virtual void Serialize(SerializerHelper sh);

	void Init(Variant& fn);
	void UnInit();

	int __fun_call(Executor&,int);

	virtual CallableCoroutine* ToCoroutine(){return this;}
	virtual CallableMetatable* GetMetaTable();

	DECLARE_OBJECT_INFO(CallableCoroutine, ObjectInfo);
};


EW_LEAVE

#endif
