
#ifndef __H_EW_CACHEDOBJECTS__
#define __H_EW_CACHEDOBJECTS__


#include "ewa_base/basic/object_ex.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE Serializer;
class DLLIMPEXP_EWA_BASE SerializerReader;
class DLLIMPEXP_EWA_BASE SerializerWriter;

class XPtrExtraLoad : public ObjectData
{
public:
	virtual void ExtraLoad(SerializerReader&) = 0;
	DataPtrT<XPtrExtraLoad> next;
};

class DLLIMPEXP_EWA_BASE CachedObjectManager
{
public:

	CachedObjectManager();

	class PtrOffset
	{
	public:
		int64_t lo;
		int64_t hi;
	};

	class PtrExtra
	{
	public:
		DataPtrT< XPtrExtraLoad> pload;

		void append(XPtrExtraLoad* p)
		{
			p->next = pload;
			pload.reset(p);
		}

		void Serialize(SerializerReader& ar)
		{
			for (XPtrExtraLoad* p = pload.get(); p; p = p->next.get())
			{
				p->ExtraLoad(ar);
			}
			clear();
		}

		void clear()
		{
			while (pload)
			{
				DataPtrT<XPtrExtraLoad> p = pload->next;
				pload->next.reset(NULL);
				pload.swap(p);
			}
		}

		~PtrExtra()
		{
			clear();
		}
	}extra;

	class PtrLoader
	{
	public:

		enum
		{
			IS_LOADED		=1<<0,
		};

		DataPtrT<ObjectData> m_ptr;
		BitFlags flags;

		void Serialize(SerializerReader& ar);
	};


	void clear();
	size_t shrink();

	void save_ptr(SerializerWriter& ar,ObjectData* ptr,bool write_index=false);
	ObjectData* load_ptr(SerializerReader& ar,int pos);

	void handle_pending(SerializerWriter& ar);
	void handle_pending(SerializerReader& ar,bool use_seek=false);

	ObjectData* read_object(SerializerReader& ar,size_t val);

	Object* create(const String& name);

	arr_1t<int32_t> pendings;
	arr_1t<PtrOffset> aOffset;
	arr_1t<PtrLoader> aLoader;
	indexer_map<ObjectData*,int32_t> aObject;

};


EW_LEAVE
#endif
