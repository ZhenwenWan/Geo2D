#ifndef __H_EW_DOMDATA_DCOMMAND__
#define __H_EW_DOMDATA_DCOMMAND__


#include "ewa_base/basic/object.h"
#include "ewa_base/domdata/dobject.h"
#include "ewa_base/scripting/callable_data.h"

EW_ENTER

class XCommandCtx;

class DCommandBase : public CallableObject
{
public:

	enum
	{
		FLAG_INIT	= 1<<0,
		FLAG_DONE	= 1<<1,
	};

	virtual bool Redo(XCommandCtx& ctx);
	virtual bool Undo(XCommandCtx& ctx);

	virtual bool InternalRedo(XCommandCtx& ctx) = 0;
	virtual bool InternalUndo(XCommandCtx& ctx) = 0;

	String name;

protected:
	BitFlags flags;

};

class DCommandScript : public DCommandBase
{
public:
	arr_1t<String> a_redo_script;
	arr_1t<String> a_undo_script;

	virtual bool InternalRedo(XCommandCtx& ctx);
	virtual bool InternalUndo(XCommandCtx& ctx);
};



EW_LEAVE

#endif
