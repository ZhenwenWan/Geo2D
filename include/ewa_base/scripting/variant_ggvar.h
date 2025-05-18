#ifndef __H_EW_SCRIPTING_VARIANT_GGVAR__
#define __H_EW_SCRIPTING_VARIANT_GGVAR__

#include "ewa_base/scripting/callable_table.h"


EW_ENTER

class DLLIMPEXP_EWA_BASE CG_GGVar : public VariantTable
{
public:

	void add(const Variant& p,const String& s,int f=0);

	template<typename T>
	void add(const T& p, const String& s, int f = 0)
	{
		Variant v;
		v.reset(p);
		add(v, s, f);
	}

	void run_script_file(const String& fp);

	void add_function(CallableCode* p, const String& s, int f = 0);
	void add_function(const String& p, const String& s, int f = 0);

	void add_function(const String& p,int type=0);
	void add_constant(int64_t value,const String& s);
	void add_constant(const String& value,const String& s);

	void add(CallableFunction* p);
	void add(CallableMetatable* p);

	CG_Variable* find_var(const String& s);

	void import(const String& lib,int flags=0);
	int find_symbol(const String& name,bool setvar);

	void expand(const String& lib,bool autoimport=false);

	void import(CallableMetatable* q);
	void unload(const String& lib);

	void init_std_handles(const String& p);

	template<typename T>
	void add_inner();

	template<typename T>
	void add_inner(const String& s,int f=0);

	static CG_GGVar& current();

	static void regfunc(void(*func)(),int v=0);

	
	static bool b_autoimport;
	static bool b_initialized;

	void define_sys(const String& var);

private:

	CG_GGVar();
	CG_GGVar(const CG_GGVar&);
	~CG_GGVar();

	indexer_map<String, CG_Variable*> sm_vmap;
	bst_set<String> _a_loading;

	void _def_sys(const String& var);
	void _init();

	AtomicMutex mutex;
};


template<typename T>
inline void CG_GGVar::add_inner()
{
	add(T::sm_info.GetCachedInstance());
}

template<typename T>
inline void CG_GGVar::add_inner(const String& s,int f)
{
	add(T::sm_info.GetCachedInstance(), s, f);
}

EW_LEAVE
#endif
