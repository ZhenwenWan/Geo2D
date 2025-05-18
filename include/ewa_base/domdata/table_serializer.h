#ifndef __H_EW_DOMDATA_TABLE_SERIALIZER__
#define __H_EW_DOMDATA_TABLE_SERIALIZER__


#include "ewa_base/domdata/dobject.h"
#include "ewa_base/math/math_def.h"
#include "ewa_base/scripting/variant.h"


EW_ENTER


template<typename T>
struct table_serializer_linker
{
	static void g(Variant& var, T& value, int dir)
	{
		value.SerializeVariant(var, dir);
	}
};

template<typename T>
struct table_serializer_linker_cast
{
	static void g(Variant& var, T& value, int dir)
	{
		if (dir == +1)
		{
			var.reset(variant_cast<String>(value));
		}
		else
		{
			value = variant_cast<T>(var);
		}
	}
};

#define __SERIALIZER_LINKER_USE_CAST__(T) \
	template<> struct table_serializer_linker<T > : public table_serializer_linker_cast<T > \
{ \
	using table_serializer_linker_cast<T >::g; \
};

__SERIALIZER_LINKER_USE_CAST__(String);
__SERIALIZER_LINKER_USE_CAST__(int);
__SERIALIZER_LINKER_USE_CAST__(int64_t);
__SERIALIZER_LINKER_USE_CAST__(float);
__SERIALIZER_LINKER_USE_CAST__(double);
__SERIALIZER_LINKER_USE_CAST__(tiny_cpx<float>);
__SERIALIZER_LINKER_USE_CAST__(tiny_cpx<double>);

template<typename T>
struct table_serializer_linker < arr_1t<T> >
{
	static void g(Variant& var, arr_1t<T>& data, int dir)
	{
		if (dir == +1)
		{
			arr_xt<Variant>& value(var.ref<arr_xt<Variant> >());
			value.resize(data.size());
			for (size_t i = 0; i < value.size(); ++i)
			{
				table_serializer_linker<T>::g(value[i], data[i], dir);
			}
		}
		else if (dir == -1)
		{
			arr_xt<Variant>& value(var.get<arr_xt<Variant> >());

			data.resize(value.size());
			for (size_t i = 0; i < data.size(); ++i)
			{
				table_serializer_linker<T>::g(value[i], data[i], dir);
			}
		}
	}
};


template<typename T>
struct table_serializer_linker < DataGrpT<T> >
{
	static void g(Variant& var, DataGrpT<T>& data, int dir)
	{
		if (dir == +1)
		{
			arr_xt<Variant>& value(var.ref<arr_xt<Variant> >());
			value.resize(data.size());
			for (size_t i = 0; i < value.size(); ++i)
			{
				table_serializer_linker<T>::g(value[i], *data[i], dir);
			}
		}
		else if (dir == -1)
		{
			arr_xt<Variant>& value(var.get<arr_xt<Variant> >());

			data.resize(value.size());

			for (size_t i = 0; i < data.size(); ++i)
			{
				data[i].reset(new T);
				table_serializer_linker<T>::g(value[i], *data[i], dir);
			}
		}
	}
};




template<typename T, int N>
struct table_serializer_linker < tiny_storage<T, N> >
{
	static void g(Variant& var, tiny_storage<T, N>& value, int dir)
	{
		arr_xt<Variant>& a(var.ref<arr_xt<Variant> >());

		if (dir == 1)
		{
			a.resize(N);
			for (int i = 0; i < N; ++i)
			{
				a[i].reset(value[i]);
			}
		}
		else
		{
			int d = std::min(N, (int)a.size());
			for (int i = 0; i < d; ++i)
			{
				value[i] = variant_cast<T>(a[i]);
			}
		}

	}
};



class DLLIMPEXP_EWA_BASE CallableTableSerializer;

// 兼容不同向量分量名称
// x/y/z/w
// u/v/n
// a/b/c/d/e/f/g/h/i
// arr_1t<String> GetTextArr(const String& text, char c);

class XSerializerNameGenerator
{
public:

	XSerializerNameGenerator(const String& n);

	String make(size_t i) const;
	String make(const String& s) const;

protected:
	String name;
	const char* p1;
	const char* pn;
	size_t n1;
	size_t n2;
	size_t n3;
};

class TableSerializerHolder;

// 变量表序列化基类
class DLLIMPEXP_EWA_BASE TableSerializer : public CallableObject
{
protected:
	TableSerializer(int t, VariantTable& v);

public:
	friend class TableSerializerHolder;
	typedef CallableObject basetype;

	enum
	{
		READER,
		WRITER,
	};

	enum
	{
		FLAG_PROPERTY=1<<0,
	};


	bool is_reader(){return type==READER;}
	bool is_writer(){return type==WRITER;}

	const int type;
	BitFlags flags;
	VariantTable& value;
	DataPtrT<CallableTableEx> ptable;

	String subject;

	void link(const String& s,int32_t& v);
	void link(const String& s,uint32_t& v);
	void link(const String& s,int64_t& v);
	void link(const String& s,uint64_t& v);


	void link(const String& s,bool& v);
	void link(const String& s,BitFlags& v);
	void link(const String& s,DColor& v);
	void link(const String& s,double& v);
	void link(const String& s,float& v);

	void link(const String& s,BitFlags& v,int m);

	void link(const String& s,String& v);
	void link(const String& s,String& v,const String& d);

	// arr_xt数据特殊处理
	template<typename T>
	void link_xt(const String& s, arr_xt<T>& d)
	{
		if (is_reader())
		{
			d = variant_cast<arr_xt<T> >(value[s]);
		}

		if (is_writer())
		{
			arr_xt<Variant>& a(value[s].ref<arr_xt<Variant> >());
			a = variant_cast<arr_xt<Variant> >(d);
		}
	}

	// 链接到变量v，并且内部转换为T类型的值
	template<typename T>
	void link_v(const String& s, Variant& v)
	{
		if (is_writer())
		{
			value[s].reset(variant_cast<T>(v));
		}
		else if (value.find1(s) >= 0)
		{
			v.reset(variant_cast<T>(value[s]));
		}
	}

	// 链接到T类型变量v
	template<typename T>
	void link_t(const String& s, T& v)
	{
		if (is_writer())
		{
			value[s].reset(v);
		}
		else
		{
			int id = value.find1(s);
			if (id >= 0)
			{
				v = variant_cast<T>(value.get(id).second);
			}
		}	
	}

	// 链接到数组arr_1t<T>类型变量v
	template<typename T>
	void link_a(const String& s,arr_1t<T>& v)
	{
		// read:  var a   --> value v
		// write: value v --> var a
		arr_xt<Variant> &a(value[s].ref<arr_xt<Variant> >());
		if(is_reader())
		{
			v.resize(a.size());
		}
		else
		{
			a.resize(v.size());
		}

		int dir = is_reader() ? -1 : +1;

		for (size_t i = 0; i < v.size(); i++)
		{
			table_serializer_linker<T>::g(a[i], v[i], dir);
		}
	}


	template<typename T>
	void link_a(const String& s,DataGrpT<T>& v)
	{
		// read:  var a   --> value v
		// write: value v --> var a
		arr_xt<Variant> &a(value[s].ref<arr_xt<Variant> >());
		if(is_reader())
		{
			v.resize(a.size());
			for (size_t i = 0; i < v.size(); i++)
			{
				if (!v[i])
				{
					v[i].reset(new T);
				}
			}
		}
		else
		{
			a.resize(v.size());
		}

		int dir = is_reader() ? -1 : +1;

		for (size_t i = 0; i < v.size(); i++)
		{
			table_serializer_linker<T>::g(a[i], *v[i], dir);
		}
	}



	String make_name(const String& s, const String& d);


	bool handle(DObject* p);

	// 链接DWhen参数
	void link(const String& s,DWhen& v)
	{
		link(make_name(s,".t_begin"),v.t_begin);
		link(make_name(s,".t_end"),v.t_end);
		link(make_name(s,".t_step"),v.t_step);
	}

	// 链接存储向量tiny_storage<T, N>类型变量v
	template <typename T, int N>
	void link(const String& s, tiny_storage<T, N>& v)
	{
		if (N > 0)
		{
			link(make_name(s,".x"), v[0]);
		}
		if (N > 1)
		{
			link(make_name(s, ".y"), v[1]);
		}
		if (N > 2)
		{
			link(make_name(s, ".z"), v[2]);
		}
		if (N > 3)
		{
			link(make_name(s, ".3"), v[3]);
		}
		if (N > 4)
		{
			link(make_name(s, ".4"), v[4]);
		}
	}

	// 链接矩阵数据表到存储向量tiny_storage<T,N>类型变量v
	template <typename T, int N>
	void link_cb(const String& s, tiny_storage<T, N>& v)
	{

		//arr_1t<String> as = GetTextArr(s, s.c_str()[0]);
		//for (int i = 0; i < N; ++i)
		//{
		//	link(as[i], v[i]);
		//}
		XSerializerNameGenerator ngen(s);
		for (int i = 0; i < N; ++i)
		{
			link(ngen.make(i), v[i]);
		}
	}

	// 链接到小向量tiny_vec<T,N>类型变量v
	template <typename T, int N>
	void link(const String& s, tiny_vec<T, N>& v)
	{
		link(s, v.get_storage());
	}

	// 链接到小立方体tiny_box<T,N>类型变量v
	template <typename T, int N>
	void link(const String& s, tiny_box<T, N>& v)
	{
		link(make_name(s, ".lo"), v.lo);
		link(make_name(s, ".hi"), v.hi);
	}

	// 链接到模板类参数为字符串型的小立方体类型变量v
	// tiny_box<String,N>接口比tiny_box<T,N>少，后者要求T具有算术运算能力（可以兼容tiny_vec<T,N>）
	template <int N>
	void link(const String& s, tiny_box<String, N>& v)
	{
		link(make_name(s,".lo"), v.lo);
		link(make_name(s,".hi"), v.hi);
	}

	int __getindex(Executor&, const String&); // top.s

private:
	template<typename T>
	void _link_integer(const String& s, T& v);

	template<typename T,typename X>
	void _link_cast(const String& s, T& v);
};


class TableSerializerHolder
{
public:

	TableSerializerHolder(int t, VariantTable& v)
	{
		m_ptr.reset(new TableSerializer(t,v));
	}

	bool handle(DObject* p)
	{
		return m_ptr->handle(p);
	}

	operator TableSerializer& ()
	{
		return *m_ptr;
	}

	VariantTable& get_value()
	{
		return m_ptr->value;
	}

	bool set_table(ObjectData* p)
	{
		return m_ptr->ptable.cast_and_set(p);
	}

	void add_flags(int v)
	{
		m_ptr->flags.add(v);
	}

	void set_flags(int v)
	{
		m_ptr->flags.clr(v);
	}

protected:

	DataPtrT<TableSerializer> m_ptr;
};


class DLLIMPEXP_EWA_BASE TableSerializerReader : public TableSerializerHolder
{
public:

	TableSerializerReader(VariantTable& v):TableSerializerHolder(TableSerializer::READER,v){}
};

class DLLIMPEXP_EWA_BASE TableSerializerWriter : public TableSerializerHolder
{
public:
	TableSerializerWriter(VariantTable& v):TableSerializerHolder(TableSerializer::WRITER,v){}
};


class TableSerializerEx
{
public:

	TableSerializerEx();
	TableSerializerEx(VariantTable& t);

	static void sync(DObject& dst, DObject& src);

	VariantTable& value;
	TableSerializerReader reader;
	TableSerializerWriter writer;

	template<typename T>
	static T* CreateT(const String& name)
	{
		return ObjectCreator::current().template CreateT<T>(name);
	}

	template<typename T>
	static T* CreateT(const String& name, VariantTable& table)
	{
		T* p = ObjectCreator::current().template CreateT<T>(name);
		if (!p) return NULL;
		TableSerializerReader ar(table);
		p->DoTransferData(ar);
		return p;
	}

private:
	VariantTable _storage;
};

class DLLIMPEXP_EWA_BASE CallableTableSerializer : public CallableTableEx
{
public:
	TableSerializer& ar;
	Variant tb;

	CallableTableSerializer(TableSerializer& s,Variant t);

	static CallableTableSerializer& get_by_this(Executor& ks);
};


EW_LEAVE
#endif
