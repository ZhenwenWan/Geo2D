#ifndef __H_EW_SCRIPTING_VARIANT_DATA__
#define __H_EW_SCRIPTING_VARIANT_DATA__

#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/scripting/typeflag.h"
#include "ewa_base/scripting/variant_op.h"

#include <complex>

EW_ENTER


class DLLIMPEXP_EWA_BASE idx_1t;
class DLLIMPEXP_EWA_BASE JsonWriter;
class DLLIMPEXP_EWA_BASE XCtxChildrenState;

class XCtxGetImage
{
public:
	arr_1t<String> images;
    typedef typename arr_1t<String>::const_iterator const_iterator;
	const_iterator begin() const { return images.begin(); }
	const_iterator end() const { return images.end(); }
	void append(const String& icon);
	void insert(const String& icon);
};


class DLLIMPEXP_EWA_BASE CallableData : public ObjectData
{
public:

	friend class Variant;

	typedef ObjectData basetype;

	enum
	{
		INVALID_CALL	=-2,
		STACK_BALANCED	=-1,
	};

	inline CallableData():m_nType(15)
	{

	}

	inline CallableData(int t):m_nType(t)
	{

	}

	inline int type() const {return m_nType;}

	virtual int __getindex(Executor&,const String&); // top.s
	virtual int __setindex(Executor&,const String&); // top.s=val
	virtual int __getarray(Executor&,int);	// top[s]
	virtual int __getarray_dot(Executor& ks, int pm); // top.[s]
	virtual int __setarray(Executor&,int);	// top[s]=val
	virtual int __fun_call(Executor&,int);	// top(...)
	virtual int __new_item(Executor&,int);	// new top(...)

	virtual int __getarray_index_range(Executor&,int);


	// return value, -1 : error, 0 : ok, 1 : ok but need to enlarge
	virtual int __update_idx(idx_1t&, intptr_t, intptr_t);

	virtual bool __test_dims(arr_xt_dims&, int);

	// top=get_iterator(top)
	virtual void __get_iterator(Executor&,int);

	CallableData* DoCloneObject();

	virtual uint32_t hashcode() const;

	virtual bool IsEqualTo(const CallableData* d) const;

	virtual CallableData_helpdata* __get_helpdata();
	virtual void __set_helpdata(const String&);

	virtual CallableClass* ToClass();
	virtual CallableWrapT<VariantTable>* ToTable();
	virtual CallableCoroutine* ToCoroutine();
	virtual CallableMetatable* ToMetatable();
	virtual CallableClosure* ToClosure();
	virtual CallableFunction* ToFunction();
	virtual CallableCode* ToCode();

	virtual CallableModule* ToModule();

	virtual CallableMetatable* GetMetaTable();

	virtual bool DoGetChildren(XCtxChildrenState&);

	virtual VariantTable* get_table_ptr();

	virtual void GetIcon(XCtxGetImage& ctx) const;

	enum
	{
		LABEL_VALUE,
		LABEL_NODE,
		LABEL_MENU,
		LABEL_TOOL,
		LABEL_MENUBAR,
	};

	virtual String MakeLabel(int hint) const;

	virtual bool ToValue(String& s,int n=0) const;
	virtual bool ToValue(int64_t&) const;
	virtual bool ToValue(bool&) const;
	virtual bool ToValue(double&) const;

	virtual bool ToJson(JsonWriter&);

	bool ToValue(int32_t& v) const;
	bool ToValue(uint32_t& v) const;
	bool ToValue(uint64_t& v) const;

	bool ToValue(float& v) const;


	inline CallableData* Clone(int t=0)
	{
		basetype*p=basetype::Clone(t);
		return static_cast<CallableData*>(p);
	}



	static CallableMetatable* sm_meta[16];

	DECLARE_OBJECT_INFO(CallableData,ObjectInfo);

protected:
	const int m_nType;

};

class StartupInfo : public CallableData
{
public:

	StartupInfo();

	enum
	{
		FLAG_SYNC = 1 << 0,
		FLAG_WITH_WINDOW = 1 << 1,
		FLAG_REDIRECT = 1 << 2,
		FLAG_RUNAS = 1 << 3,
	};

	String title;
	String s_cwd;
	Stream stream;
	int n_ret;
	BitFlags flags;

	DECLARE_OBJECT_INFO(StartupInfo, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE CallableObject : public CallableData
{
public:
	typedef CallableData basetype;
	CallableData* DoClone(ObjectCloneState&);
	CallableData* DoCloneObject();
};


template<typename T>
T kany_cast_number<T>::g(CallableData* v)
{
	type n(0);
	if (!v)
	{
		Exception::XError(_hT("cannot_cast_nil_to_number"));
	}

	if (!v->ToValue(n))
	{
		Exception::XBadCast();
	}
	return n;
}


inline String pl_cast_base<String>::g(CallableData* v)
{
	type s; if (!v) return "nil";
	v->ToValue(s);
	return s;
}



EW_LEAVE
#endif
