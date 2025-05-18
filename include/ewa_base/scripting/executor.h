
#ifndef __H_EW_SCRIPTING_EXECUTOR__
#define __H_EW_SCRIPTING_EXECUTOR__

#include "ewa_base/scripting/variant_ggvar.h"
#include "ewa_base/scripting/callable_coroutine.h"
#include "ewa_base/scripting/codegen.h"
#include "ewa_base/threading/thread_mutex.h"

EW_ENTER

class StackState3
{
public:
	DataPtrT<CallableCoroutine> co_this;
	DataPtrT<CallableCoroutine> co_main;
};


class DLLIMPEXP_EWA_BASE CallState
{
public:

	StackState0 ci0;
	StackState1 ci1;
	StackState2 ci2;
	StackState3 ci3;
	CGenState cgs;
};

class StrMacro;
class CallableCoroutine;
class CallableExecutorState;


class SharedTableRef : public SharedRefT<VariantTable>
{
public:

	typedef SharedRefT<VariantTable> basetype;

	typedef VariantTable::key_type key_type;
	typedef VariantTable::mapped_type mapped_type;
	typedef VariantTable::index_type index_type;
	typedef VariantTable::value_type value_type;


	typedef VariantTable::iterator iterator;
	typedef VariantTable::const_iterator const_iterator;
	typedef VariantTable::reverse_iterator reverse_iterator;
	typedef VariantTable::const_reverse_iterator const_reverse_iterator;

	inline iterator begin() { return (*m_ptr).begin(); }
	inline iterator end() { return (*m_ptr).end(); }
	inline reverse_iterator rbegin() { return (*m_ptr).rbegin(); }
	inline reverse_iterator rend() { return (*m_ptr).rend(); }

	inline const_iterator begin() const { return (*m_ptr).begin(); }
	inline const_iterator end() const { return (*m_ptr).end(); }
	inline const_reverse_iterator rbegin() const { return (*m_ptr).rbegin(); }
	inline const_reverse_iterator rend() const { return (*m_ptr).rend(); }

	inline const_iterator cbegin() const { return (*m_ptr).begin(); }
	inline const_iterator cend() const { return (*m_ptr).end(); }
	inline const_reverse_iterator crbegin() const { return (*m_ptr).crbegin(); }
	inline const_reverse_iterator crend() const { return (*m_ptr).crend(); }


	SharedTableRef() {}


	SharedTableRef(VariantTable& t) : basetype(t)
	{

	}

	SharedTableRef(const SharedTableRef& o) : basetype(o), m_ptable(o.m_ptable)
	{

	}

	SharedTableRef& operator=(const VariantTable& o)
	{
		(*m_ptr) = o;
		return *this;
	}

	SharedTableRef& operator=(const SharedTableRef& o)
	{
		m_ptr = o.m_ptr;
		m_ptable = o.m_ptable;
		return *this;
	}

	void reset(VariantTable* t, bool need_delete = false)
	{
		if (need_delete)
		{
			m_ptr.reset(t);
		}
		else
		{
			m_ptr.reset(t, NULL);
		}

		m_ptable.reset(NULL);
	}

	void reset(DataPtrT<CallableWrapT<VariantTable> > p)
	{
		m_ptable = p;
		m_ptr.reset(&p->value, NULL);
	}

	void reset()
	{
		reset(new CallableWrapT<VariantTable>);
	}

	CallableData* get_ref_table()
	{
		if (m_ptable)
		{
			return m_ptable.get();
		}
		else
		{
			return new CallableTableProxy(*this);
		}
	}

	inline mapped_type& operator[](const key_type& k)
	{
		EW_ASSERT(m_ptr.get() != NULL);
		return (*m_ptr)[k];
	}

	inline const mapped_type& operator[](const key_type& k) const
	{
		EW_ASSERT(m_ptr.get() != NULL);
		return (*m_ptr)[k];
	}


	inline index_type find1(const key_type& v) const
	{
		return m_ptr->find1(v);
	}

	inline index_type find2(const key_type& v)
	{
		return m_ptr->find2(v);
	}

	void clear()
	{
		m_ptr->clear();
	}

	size_t size() const
	{
		return m_ptr->size();
	}

	inline value_type& get(index_type n)
	{
		return m_ptr->get(n);
	}

	inline const value_type& get(index_type n) const
	{
		return m_ptr->get(n);
	}

	bool empty() const
	{
		return m_ptr->empty();
	}

protected:
	DataPtrT<CallableWrapT<VariantTable> > m_ptable;
};



class DLLIMPEXP_EWA_BASE Executor : public CallState
{
public:

	friend class CallableCode;

	explicit Executor(VariantTable& t,size_t stack_size = 0);
	explicit Executor(VariantTable& t, CallableCoroutine* p);
	explicit Executor(SharedTableRef& t,size_t stack_size = 0);

	explicit Executor(size_t stack_size = 0);
	explicit Executor(CallableCoroutine* p);

	Executor(const Executor& o);
	Executor(const Executor& o,int flag);

	~Executor();

	Executor& operator=(const Executor& o);

	void assign_state(const Executor* p);


	void push(Variant* v, size_t n);

	EW_FORCEINLINE void push(CallableData* v){(*++ci1.nsp).kptr(v);}
	EW_FORCEINLINE void push(){(*++ci1.nsp).clear();}

	bool callxs(const Variant& func, const arr_1t<Variant>& input, arr_1t<Variant>& output, int retnum);
	bool callxs(const String& func, const arr_1t<Variant>& input, arr_1t<Variant>& output, int retnum);

	template<typename T>
	EW_FORCEINLINE void push(const T& v=T()){ (*++ci1.nsp).reset(v); }

#ifdef EW_C11
	template<typename T>
	EW_FORCEINLINE void push(T&& v=T()){ (*++ci1.nsp).reset(std::forward<T>(v)); }
#endif

	void popq_ref(Variant& d);
	void popq(Variant& d);

	EW_FORCEINLINE void popq(){ --ci1.nsp; }

	template<typename T>
	void popq(T& v)
	{
		v=variant_cast<T>(*ci1.nsp);
		--ci1.nsp;
	}

	template<typename T>
	void popq(DataPtrT<T>& v)
	{
		v.cast_and_set(ci1.nsp->kptr());
		--ci1.nsp;
	}

	EW_FORCEINLINE Variant& top(){ return *ci1.nsp; }

	template<typename P>
	void call1();

	template<typename P>
	void call2();

	bool callx(int n,int k=1);
	int callx_raw(int n,int k);

	EW_NORETURN void kerror(const String& s);
	EW_NORETURN void kerror_invalid_param(int n);
	EW_NORETURN void kerror_invalid_this();

	void kthrow(int n);
	void kabort(int n);

	inline void check_pmc(CallableData*,int pm1,int pm2)
	{
		if(pm1==pm2) return;
		kerror(String::Format("invalid param count, %d param(s) expected",pm2));
	}

	inline void check_pmc(CallableData*,int pm1,int pm_lo,int pm_hi)
	{
		if( (pm_lo<0||pm1>=pm_lo) && (pm_hi<0||pm1<=pm_hi)) return;
		kerror(String::Format("invalid param count, %d-%d params expected",pm_lo,pm_hi));
	}

	inline void check_pmc(CallableData*, int pm, bool(*cond)(int), const String& nfo = empty_string)
	{
		if (cond(pm)) return;
		kerror(String::Format("invalid param count, %s", nfo));
	}

	inline void check_pmc(CallableData*, int pm, const int* pm_lst, size_t sz)
	{
		EW_ASSERT(pm_lst != NULL && sz > 0);

		for (size_t i = 0; i < sz; ++i)
		{
			if (pm == pm_lst[i])
			{
				return;
			}
		}
		StringBuffer nfo("invalid param count, only {");
		nfo << pm_lst[0];
		for (size_t i = 1; i < sz; ++i)
		{
			nfo << ", " << pm_lst[i];
		}
		nfo << "} are allowed";

		kerror(nfo);
	}

	inline void check_stk(size_t n)
	{
		if (ci0.nbx + n < ci1.end) return;
		kerror("stack overflow");
	}


	void get_array(int n);
	void set_array(int n);
	void get_global(const String& s){push(tb1[s]);}
	void set_global(const String& s){popq(tb1[s]);}
	void get_system(const String& s){push(tb0[s]);}

	void get_index(const String& s);
	void set_index(const String& s);

	void reset();

	int move_return_values(int n);

	DataPtrT<CallableData> parse(const String& s);

	enum
	{
		EWSL_INDIRECT_AUTO=-1,
		EWSL_INDIRECT_FILE=0,
		EWSL_DIRECT_STRING=1,

	};

	// compile s(string or filename) and push to stack
	bool prepare(const String& s, int t = EWSL_DIRECT_STRING);
	bool prepare(const char* p1, const char* p2);

	bool load_ewd(const String& file);

	Variant compile(const String& s, int t = EWSL_DIRECT_STRING);

	// complie s(string) and execute
	bool execute(const String& s,int pmc=0,int kep=0);

	// complie s(filename) and execute
	bool execute_file(const String& s,int pmc=0,int kep=0);

	// compile or load function object and execute
	bool execute_file_auto(const String& s,int pmc=0,int kep=0);

	template<typename T>
	T eval(const String &s);

	template<typename T>
	bool eval(const String& s,T& v);


	const CG_GGVar& tb0;


	SharedTableRef tb1;

	bool set_cwd(const String& s);
	bool set_filename(const String& s);

	String get_cwd();


	StrMacro& get_macro();

	String make_path(const String& s);

	Mutex mutex;
	BitFlags flags;

	void set_time_limit(const TimeSpan& tp=TimeSpan(),bool flag=true);
	void set_page_limit(size_t n,bool flag=true);

	TimeSpan get_time_limit();
	size_t get_page_limit();

	DataPtrT<VisExtraHandler> set_vis_handler(DataPtrT<VisExtraHandler> p);

	void check_file_access(int f);

	bool realfile(String& file);

	template<typename T>
	T& get_this_ref();

protected:

	CallableExecutorState* _get_vm_state();

	void _vm_run1();

	template<typename P>
	void _vm_run2(P& t);

	void _vm_check_ret(int ret);

	void _vm_check_call_ret(int ret);

	void _vm_run_check_ret(int ret);

	void _vm_handle_exception(std::exception &e);

	void _init();

	TimeSpan ts_limit;
	size_t pg_limit;

	TimeSpan ts_limit_max;
	size_t pg_limit_max;




};

EW_FORCEINLINE void Executor::popq_ref(Variant& d)
{
	d = *ci1.nsp--;
}

EW_FORCEINLINE void Executor::popq(Variant& d)
{
	Variant& s(*ci1.nsp--);
	if(s.flag==-1&&s.data.pval->GetRef()!=1)
	{
		variant_handler<CallableData*>::clone(d,s.data.pval);
	}
	else
	{
		d=s;
	}
}

template<typename T>
T Executor::eval(const String &s)
{
	LockState<BitFlags> lock(cgs.flags, CGenState::FLAG_SAVE_TEMP | CGenState::FLAG_RETURN_TEMP |  CGenState::FLAG_ALLOW_FILE_READ);
	ci1.nbp[StackState1::SBASE_TEMP].clear();
	if(!execute(s,0,1))
	{
		Exception::XError(_hT("fail_to_evaluate"));
	}
	return pl_cast<T>::g(*ci1.nsp--);
}

template<typename T>
bool Executor::eval(const String& s,T& v)
{
	LockState<BitFlags> lock(cgs.flags, CGenState::FLAG_SAVE_TEMP | CGenState::FLAG_RETURN_TEMP |  CGenState::FLAG_ALLOW_FILE_READ);

	ci1.nbp[StackState1::SBASE_TEMP].clear();
	if(!execute(s,0,1))
	{
		return false;
	}

	try
	{
		v=pl_cast<T>::g(*ci1.nsp--);
		return true;
	}
	catch(std::exception& e)
	{
		try
		{
			this_logger().LogError(e.what());
		}
		catch (...)
		{

		}
		return false;
	}
}


template<typename T>
class get_this_ref_heler
{
public:
	static T* g(Executor& ks)
	{
		return dynamic_cast<T*>(ks.ci1.nbp[StackState1::SBASE_THIS].kptr());
	}
};

template<typename T>
T& Executor::get_this_ref()
{
	T* ptr = get_this_ref_heler<T>::g(*this);
	if (!ptr)
	{
		this->kerror_invalid_this();
	}
	return *ptr;
}


template<typename T>
VariantTable& get_callable_state_holder_table();

template<typename T>
class CallableStateHolder;

template<typename T>
class CallableStateHolder<T&> : public CallableTableProxy
{
public:
	T& value;
	CallableStateHolder(T& v) :CallableTableProxy(get_callable_state_holder_table<T>()), value(v)
	{
		flags.clr(FLAG_READONLY | FLAG_SET_THIS);
	}

};

template<typename T>
class CallableStateHolder : public CallableStateHolder<T&>
{
public:
	CallableStateHolder(T& value) : CallableStateHolder<T&>(value){}
};

template<typename T>
class CallableStateHolder2 : public CallableStateHolder<T&>
{
public:
	CallableStateHolder2(T& value) : CallableStateHolder<T&>(value){}
	CallableStateHolder2() : CallableStateHolder<T&>(_internal_value){}
private:
	T _internal_value;
};


template<typename T>
T& ewsl_helper_get_this_by_holder(Executor& ks)
{
	CallableStateHolder<T&>* p = dynamic_cast<CallableStateHolder<T&>*>(ks.ci1.nbp[StackState1::SBASE_THIS].kptr());
	if (!p) ks.kerror_invalid_this();
	return p->value;
}

template<typename T>
T& ewsl_helper_get_this(Executor& ks);


// get thread private executor;
DLLIMPEXP_EWA_BASE Executor& this_executor();


class EwslCgsLocker : public LockState < BitFlags >
{
public:
	typedef LockState < BitFlags > basetype;

	EwslCgsLocker(Executor& ks) : basetype(ks.cgs.flags)
	{
		init(ks);
	}

	EwslCgsLocker(Executor& ks, int flags) : basetype(ks.cgs.flags)
	{
		init(ks);
		ks.cgs.flags.clr((ks.cgs.flags.val()&~CodeGen::FLAG_MASK) | flags);
	}

	void init(Executor& ks)
	{

		Variant& var(ks.ci1.nbp[StackState1::SBASE_THIS]);
		if (var.is_nil()) return;

		int64_t f = 0;
		if (var.get(f))
		{
			f = (f*CodeGen::FLAG_ALLOW_MIN)&~CodeGen::FLAG_MASK;
			ks.cgs.flags.clr(f | (ks.cgs.flags.val()&CodeGen::FLAG_MASK));
		}
	}

};

class EwslCwdLocker
{
public:

	Executor& lexer;
	String value;

	EwslCwdLocker(Executor& ks) : lexer(ks)
	{
		value = lexer.get_cwd();
	}

	EwslCwdLocker(Executor& ks,const String& s) : lexer(ks)
	{
		value = lexer.get_cwd();
		lexer.set_cwd(s);
	}

	~EwslCwdLocker()
	{
		lexer.set_cwd(value);
	}
};

EW_LEAVE

#endif
