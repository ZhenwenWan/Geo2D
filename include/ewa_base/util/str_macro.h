#ifndef __H_EW_UTIL_STR_MACRO__
#define __H_EW_UTIL_STR_MACRO__

#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/collection/bst_map.h"
#include "ewa_base/scripting/callable_table.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE StrMacro
{
public:

	enum
	{
		FLAG_FALLBACK_ENV		=1<<0,
		FLAG_KEEP_IF_NOT_FOUND	=1<<1,
	};

	String apply(const String& msg) const;

	void set_filename(const String& file);
	void set(const String& key, const String& val);
	void clear();

	StrMacro();
	~StrMacro();

	VariantTable items;
	BitFlags flags;

	void Serialize(SerializerHelper sh);
};





EW_LEAVE

#endif
