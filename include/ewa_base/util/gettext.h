#ifndef __EW_UTIL_GETTEXT_H__
#define __EW_UTIL_GETTEXT_H__


#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/scripting/variant.h"
#include "ewa_base/basic/scanner_helper.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE GTextItem
{
public:
	enum
	{
		FLAG_USED		=1<<0,
		FLAG_TRANSLATED	=1<<1,
		FLAG_ORIGIN		=1<<2,
		FLAG_LAST_USED	=1<<3,
	};

	GTextItem();

	String text; // 词条
	String translate; // 翻译
	BitFlags flags; // 状态标识
	int index; // 前缀
	TimePoint tp_update; //词条更新时间
	TimePoint tp_translate; // 词条翻译时间

	arr_1t<std::pair<int,int> > fileline;

	void Serialize(SerializerHelper sh);


};

class DLLIMPEXP_EWA_BASE GTextCatalog
{
public:


	String sname;
	String sbase;
	arr_1t<String> paths;
	indexer_set<String> files;
	indexer_map<String,GTextItem> items;
	void Serialize(SerializerHelper sh);


	void Update();

	void UpdateIndex();

	bool Load(const String& fp);
	bool Save(const String& fp);

	bool Export(const String& mo);

	String GetPath();

	String filename;

	void swap(GTextCatalog& o);

};




EW_LEAVE
#endif
