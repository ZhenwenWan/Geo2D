#ifndef __H_EW_BASIC_LANGUAGE__
#define __H_EW_BASIC_LANGUAGE__

#include "ewa_base/config.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/scripting/callable_data.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE String;
class DLLIMPEXP_EWA_BASE GTextCatalog;
class DLLIMPEXP_EWA_BASE ConfigBase;


class const_char_pointer_map : public indexer_trait<const char*, const char*, int>
{
public:
	static int hashcode_key(const char* v)
	{
		hash_t<char*> h;
		return h(v);
	}
	static bool equal(const char* k, const char* v)
	{
		return strcmp(k, v) == 0;
	}
};


class LangData : public ObjectData
{
public:
	String m_sName;
	StringBuffer m_aCont;

	bool ReadString(String& s, const char*& p1);


	bool LoadPo(const String& file);

	bool LoadGt(const GTextCatalog& gt);

	void SetCatalog(const indexer_map<String, String>& smap);

	bool LoadMo(const String& file);

	void AddPair(const std::pair<String, String>& kv);

};

class LangFunc : public ObjectData
{
public:
	virtual String& Translate(const String& msg) = 0;
};


class DLLIMPEXP_EWA_BASE Language : public CallableData
{
public:

	Language();
	Language& operator=(const Language& o);


	bool AddLanguage(const String& s);

	bool AddPath(const String& path);

	// Init with a file, po file or files that saved by Language::Save
	bool AddCatalog(const String& file);

	bool AddCatalog(const indexer_map<String,String>& mp);

	bool AddCatalog(const GTextCatalog& gt);

	bool AddCatalog(DataPtrT<LangData>& pData);

	bool GetCatalog(indexer_map<String,String>& mp);

	bool Save(const String& file);

	const String& Translate(const String& msg) const;

	String Translate2(const String& msg) const;

	const String& Translate(const String& msg,const String& def) const;

	void Clear();

	bool SetLanguage(const String& s);

	String GetLanguage();

	String GetLanguageShort();

	bool InitByConfig(ConfigBase& cfg,bool force);

	static Language& current();

	static arr_1t<String> GetLanguages();

	bool operator==(const Language& o) const { return this == &o; }

	void Serialize(SerializerHelper sh);

	virtual CallableMetatable* GetMetaTable();

	int __getindex(Executor&, const String&); // top.s

	void SetFallback(DataPtrT<LangFunc> p);

	DECLARE_OBJECT_INFO(Language, ObjectInfo);

private:

	const String* DoTranslate(const char* msg) const;

	String sLanguage;
	DataGrpT<LangData> aDatas;
	indexer_map<const char*, const char*, def_allocator, const_char_pointer_map> mapStrings;
	DataPtrT< LangFunc> p_fallback;
};


DEFINE_OBJECT_NAME(Language, "language")

EW_LEAVE
#endif
