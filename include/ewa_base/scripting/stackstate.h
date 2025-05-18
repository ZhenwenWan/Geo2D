#ifndef __EW_SCRIPTING_STACKSTATE_H__
#define __EW_SCRIPTING_STACKSTATE_H__


#include "ewa_base/scripting/callable_code.h"

EW_ENTER

class StackState0
{
public:

	XopInst* nip;		//pc
	Variant* dat;		//llvar base

	Variant* sp1;		//stackframe base
	Variant* sp2;		//stackframe top
	Variant* nbx;		//function param base

	intptr_t kep;		//keep return_value_count, -1 for keep all

	DataPtrT<CallableCode> frm;

	BitFlags cgf;

	StackState0();

};

class StackState1
{
public:

	Variant* nbp;	//stack base pointer
	Variant* nsp;	//stack pointer
	Variant* end;	//stack end


	enum
	{
		SBASE_TEMP,
		SBASE_THIS,
		SBASE_META,
		SBASE_CWD,
		SBASE_STATE,
		SBASE_HOLDER,
		SBASE_RAND,
		SBASE_STRMACRO,
		SBASE_COLAST,
		SBASE_KEEP=16,
	};


};

class StackState2
{
public:

	DataPtrT<CallableData> pfn;	//self
	intptr_t pmc;				//function param count
	const String* idx;			//string index

	XopInst xop;

};

class DLLIMPEXP_EWA_BASE CGenState
{
public:
	CGenState();

	BitFlags flags;
	BitFlags extra;

	enum
	{
		FLAG_SAVE_TEMP			= 1 << 0,
		FLAG_RETURN_TEMP		= 1 << 1,
		FLAG_SHOW_RESULTS		= 1 << 2,
		FLAG_IMPLICIT_GLOBAL	= 1 << 3,
		FLAG_IMPLICIT_LOCAL		= 1 << 4,
		FLAG_ALLOW_EXPORT		= 1 << 5,
		FLAG_NO_CAPTURE_HELP	= 1 << 6,
		FLAG_NO_CAPTURE_SRC		= 1 << 7,
		FLAG_IMPLICIT_EXPORT = 1 << 8,

		FLAG_MASK				= (1<<16)-1,
		FLAG_ALLOW_MIN			= 1 << 16,
		FLAG_ALLOW_SET_GLOBAL	= FLAG_ALLOW_MIN << 0,
		FLAG_ALLOW_FILE_READ	= FLAG_ALLOW_MIN << 1,
		FLAG_ALLOW_FILE_WRITE	= FLAG_ALLOW_MIN << 2,
		FLAG_ALLOW_FILE_EXECUTE	= FLAG_ALLOW_MIN << 3,
		FLAG_ALLOW_FILE_LIST	= FLAG_ALLOW_MIN << 4,
		FLAG_ALLOW_NET			= FLAG_ALLOW_MIN << 5,
		FLAG_ALLOW_FILE_ACCESS = FLAG_ALLOW_FILE_READ | FLAG_ALLOW_FILE_WRITE | FLAG_ALLOW_FILE_EXECUTE|FLAG_ALLOW_FILE_LIST|FLAG_ALLOW_NET,
	};


};

EW_LEAVE
#endif
