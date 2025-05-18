#ifndef __H_EW_BASIC_APPBASE__
#define __H_EW_BASIC_APPBASE__

#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"


EW_ENTER

class VariantTable;

class DLLIMPEXP_EWA_BASE AppBase
{
public:

	AppBase();

	bool IsOk();

	static VariantTable& get_config();
	static Variant& get_config_var();

	static arr_1t<Functor<void()> >& get_fn_init();

	class InternalData : public ObjectData
	{
	public:

		typedef ObjectData basetype;

		enum
		{
				FLAG_INIT_OK=1<<0,
		};

		virtual bool Init();
		virtual void Fini();

		virtual bool DoInit();
		virtual bool DoInitEnter();
		virtual bool DoInitLeave();

		virtual void on_fini_counter();
		virtual void on_init_counter();

		BitFlags flags;
	};

protected:
	virtual InternalData* CreateInternalData();
	static InternalData* GetInternalData();

	DataPtrT<ObjectData> m_refData;

};



EW_LEAVE
#endif
