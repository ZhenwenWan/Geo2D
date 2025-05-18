
#ifndef __H_EW_DOMDATA_DSTATE__
#define __H_EW_DOMDATA_DSTATE__


#include "ewa_base/basic/object.h"
#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/ddocdata.h"

EW_ENTER


template<typename T>
class LockStack
{
public:

	arr_1t<T>& stk;

	LockStack(arr_1t<T>& a, const T& v) :stk(a)
	{
		stk.push_back(v);
	}

	~LockStack()
	{
		stk.pop_back();
	}

	bool test()
	{
		auto& val(stk.back());
		for (auto it = stk.begin(); it != stk.end() - 1; it++)
		{
			if (val == *it)
			{
				return false;
			}
		}
		return true;
	}
};

class DLLIMPEXP_EWA_BASE SymbolManager;
class DLLIMPEXP_EWA_BASE DDocumentBase;

class DLLIMPEXP_EWA_BASE DState : public CallableObject
{
public:

	DState();
	DState(Executor& k, SymbolManager* s = NULL);
	DState(DataPtrT<XDocumentData> pdoc,Executor* plexer=NULL);
	DState(DataPtrT<DDocumentBase> pdoc,Executor* plexer=NULL);

	void reset(DataPtrT<XDocumentData> pdoc,Executor* plexer=NULL);

	~DState();

	LitePtrT<SymbolManager> psmap;
	arr_1t<SymbolManager*> asmap;
	DataPtrT<DDocumentBase> pdocument;
	DataPtrT<DTaskModifier> pmodifier;
	DataPtrT <XDocumentData> p_doc;

	DDocumentBase* get_document();

	void set_modifier(DTaskModifier* p);

	bool check_recursive(DObject* p);

	LitePtrT<Executor> plexer;
	BitFlags flags;
	int32_t var_version;
	bool var_changed;

	bool test_version(int32_t& v);


	enum
	{
		FLAG_POST	= 1 << 0,
		FLAG_FORCE	= 1 << 1,
	};

	int phase;
	int phase_min;
	int phase_max;

	indexer_map<DataPtrT<ObjectData>, int> tested;

	arr_1t<DObject*> objstk;


	template<typename T>
	void ptr_add(T* p)
	{
		return p_doc->ptrmgr.add(p);
	}

	template<typename T>
	T* ptr_get()
	{
		return p_doc->ptrmgr.get<T>();
	}

	template<typename T>
	bool link(const T& s, T& v)
	{
		v = s;
		return true;
	}

	void link_var(const String& name, double& val);
	void link_path(const String& p, String& d);

	bool link(const String& s, Variant& v);
	bool link(const String& s, int &v);
	bool link(const String& s, double &v);
	bool link(const String& s, double& v1, double& v2);

	bool link(const String& s, dcomplex &v);

	bool link_ex(const String& s, vec3d& v);

	bool link(const vec3s& s, vec3d& v);
	bool link(const vec3s& s, vec3i& v);
	bool link(const vec2s& s, vec2d& v);
	bool link(const vec2s& s, vec2i& v);
	bool link(const box3s& s, box3d& v);
	bool link(const box3s& s, box3i& v);
	bool link(const box2s& s, box2d& v);
	bool link(const box2s& s, box2i& v);

	bool link(const String& s, arr_1t<int64_t>& v);
	bool link(const String& s, arr_1t<double>& v);
	bool link(const String& s, arr_xt<double>& v);
	bool link(const String& s, arr_xt<dcomplex>& v);

	bool link(const String& s, DataPtrT<DObject>& v);

	bool link(const arr_1t<DTransformItem>& a1transform, mat4d& m4tmp);


	template<typename T>
	bool link_a(const arr_1t<String>& s, DataGrpT<T>& v)
	{
		v.resize(s.size());
		for (size_t i = 0; i < s.size(); i++)
		{
			if (!link_t(s[i], v[i]))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool link_t(const String& s, DataPtrT<T>& v,bool allow_null=false)
	{
		DataPtrT<DObject> h;
		if (!link(s, h))
		{
			this_logger().LogError(_hT("CANNOT find symbol %s"), s);
			return false;
		}

		if (!h && allow_null)
		{
			v.reset(NULL);
			return true;
		}

		return v.cast_and_set(h);
	}

	template<typename T>
	bool link_t(NamedReferenceT<T>& p,bool allow_null=false)
	{
		return link_t<T>(p.name, p, allow_null);
	}

	template<typename T1, typename T2>
	bool link_t(const arr_1t<T1>& s, arr_1t<T2>& v)
	{
		v.resize(s.size());
		for (size_t i = 0; i < s.size(); i++)
		{
			if (!link(s[i], v[i]))
			{
				return false;
			}
		}
		return true;
	}

	bool update(DObject* p);

	template<typename T>
	bool update(DataPtrT<T>& p)
	{
		return update(p.get());
	}

	template<typename T>
	bool update(DataGrpT<T>& p)
	{
		int errcnt = 0;
		for (size_t i = 0; i<p.size(); i++)
		{
			if (!update(p[i]))
			{
				errcnt++;
			}
		}
		return errcnt == 0;
	}

	bool update2(DObject* p)
	{
		if(!p)
		{
			return true;
		}
		return update(p);
	}

	// 允许引用名字为空
	template<typename T>
	bool update2(NamedReferenceT<T>& p)
	{
		if(phase<=DPHASE_VAR)
		{
			if (p.name.empty())
			{
				p.reset(NULL);
			}
			else if(!link_t<T>(p.name,p,true))
			{
				this_logger().LogError(_hT("unknown symbol %s"),p.name);
				return false;
			}
		}
		return update2(p.get());
	}

	// 不允许引用名字为空
	template<typename T>
	bool update(NamedReferenceT<T>& p)
	{
		if(phase<=DPHASE_VAR)
		{
			if(!link_t<T>(p.name,p,false))
			{
				if (p.name.empty())
				{
					this_logger().LogError(_hT("symbol name is empty"));
				}
				else
				{
					this_logger().LogError(_hT("unknown symbol %s"),p.name);
				}

				return false;
			}
		}
		return update(p.get());
	}

	template<typename T>
	bool update(DataPtrT<T>& p,const String& name)
	{
		if(phase<=DPHASE_VAR)
		{
			if(!link_t<T>(name,p,false))
			{
				if (name.empty())
				{
					this_logger().LogError(_hT("symbol name is empty"));
				}
				else
				{
					this_logger().LogError(_hT("unknown symbol %s"),name);
				}

				return false;
			}
		}
		return update(p.get());
	}


	int testp(ObjectData* p);


	class LockerSM
	{
	public:
		DState& ds;
		LockerSM(DState& d_,SymbolManager& s_):ds(d_)
		{
			ds.asmap.push_back(&s_);
			ds.psmap=ds.asmap.back();
		}
		LockerSM(DState& d_,SymbolManager* s_):ds(d_)
		{
			if (!s_) s_ = ds.psmap;
			ds.asmap.push_back(s_);
			ds.psmap=ds.asmap.back();
		}
		~LockerSM()
		{
			ds.asmap.pop_back();
			ds.psmap=ds.asmap.empty()?NULL:ds.asmap.back();
		}
	};

	virtual int __getindex(Executor&, const String&);

private:
	void _init();
	AutoPtrT<Executor> _plexer;
};




class DLLIMPEXP_EWA_BASE ObjectStateRecursiveChecker : public ObjectVisitorState
{
public:
	typedef ObjectVisitorState basetype;

	ObjectStateRecursiveChecker();

	virtual void handle(String& name, DObject*);

	virtual void handle(arr_1t<String>& name, DataGrpT<DObject>&);

	virtual void handle(String& name);

	virtual void handle(DObject* p);


	bst_set<DObject*> p_visited;
	bst_set<String> a_unkown_names;
	LitePtrT<SymbolManager> p_objmgr;

};


EW_LEAVE
#endif
