#ifndef __H_EW_BASIC_DLL_MODULE__
#define __H_EW_BASIC_DLL_MODULE__

#include "ewa_base/config.h"
#include "ewa_base/basic/string_detail.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/object.h"


EW_ENTER

class String;

class KO_Policy_module
{
public:
	typedef void* type;
	typedef type const_reference;
	static type invalid_value(){ return NULL; }
	static void destroy(type& o);
	static type open(const String& file);

	static void* getsym(type h, const char* p);

};



class DLLIMPEXP_EWA_BASE DllGroup;

class DLLIMPEXP_EWA_BASE DllBase
{
public:
	typedef KO_Handle<KO_Policy_module> impl_type;


	enum
	{
		FLAG_INITAL_OPEN				= 1 << 0,
		FLAG_ON_SYMBOL_NOT_FOUND_CLOSE	= 1 << 1,
		FLAG_SEARCH_PATHS				= 1 << 2,
		FLAG_ON_FAIL_NO_PROMPT			= 1 << 3,
	};

	virtual void Close() = 0;
	virtual void* GetSymbol(const String& s) = 0;
	virtual ~DllBase(){}

	template<typename T>
	T* GetSymbolEx(const String& name, T*& func){ func = (T*)GetSymbol(name); return func; }

	AtomicMutex mutex;

protected:
	virtual void* OnUnkownSymbol(const String& s);

	DataPtrT<ObjectData> names;
	BitFlags flags;

};

class DLLIMPEXP_EWA_BASE DllModule : public DllBase
{
public:


	friend class DllGroup;

	DllModule();

	bool Open(const String& dll, int f = -1);
	void Close();
	void* GetSymbol(const String& s);

	bool IsOk(){ return impl.ok(); }

protected:

	impl_type impl;
};

class DLLIMPEXP_EWA_BASE DllGroup : public DllBase
{
public:

	bool Open(const String& dll, int f = -1);
	void Close();
	void* GetSymbol(const String& s);

	bool IsOk() { return !impls.empty(); }

protected:
	arr_1t<impl_type> impls;
	void* DoGetSymbol(const String& s);
};

EW_LEAVE

#endif
