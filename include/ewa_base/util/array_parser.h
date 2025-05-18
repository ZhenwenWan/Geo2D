#ifndef __EW_UTIL_ARRAYP_ARSER_H__
#define __EW_UTIL_ARRAYP_ARSER_H__


#include "ewa_base/collection/arr_xt.h"
#include "ewa_base/math/tiny_cpx.h"
#include "ewa_base/scripting/variant.h"
#include "ewa_base/basic/scanner_helper.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE IParserParam
{
public:
	IParserParam();

	enum
	{
		FLAG_CONTINUE		=1<<0,
		FLAG_ALLOW_COMMENT_2=1<<1,
		FLAG_ALLOW_COMMENT_1=1<<2,
	};

	String breaker;    // 分隔符
	String comment;    // 注释（仅行注释）
	BitFlags flags;
	int32_t skipline;  // 跳过的行数

};

class DLLIMPEXP_EWA_BASE ArrayParser
{
public:

	static Variant parse(const char* p, const IParserParam& param=IParserParam());
	static bool parse(const char* p, arr_xt<double>& val, const IParserParam& param=IParserParam());
	static bool parse(const char* p, arr_xt<dcomplex>& val,const IParserParam& param=IParserParam());

};

class DLLIMPEXP_EWA_BASE IOArray
{
public:
	static void write(StringBuffer& sb, const arr_xt<float>& a, bool write_file = true);
	static void write(StringBuffer& sb, const arr_xt<double>& a, bool write_file = true);

	static bool save(const String& fn, const arr_xt<float>& a, int flags = FLAG_FILE_WC);
	static bool save(const String& fn, const arr_xt<double>& a, int flags = FLAG_FILE_WC);
	static bool save(const String& fn, const arr_xt<int>& a, int flags = FLAG_FILE_WC);
	static bool save(const String& fn, const arr_xt<int64_t>& a, int flags = FLAG_FILE_WC);
	static bool save(const String& fn, const arr_xt<dcomplex>& a, int flags = FLAG_FILE_WC);
	static bool save(const String& fn, const arr_xt<Variant>& a, int flags = FLAG_FILE_WC);

	static bool save(const String& fn, const Variant& a, int flags = FLAG_FILE_WC);

	static bool append(const String& fn, const arr_xt<float>& a);
	static bool append(const String& fn, const arr_xt<double>& a);
	static bool append(const String& fn, Variant var);

	static bool read(StringBuffer& sb, arr_xt<float>& a);
	static bool read(StringBuffer& sb, arr_xt<double>& a);

	static bool load(const String& fn, arr_xt<float>& a);
	static bool load(const String& fn, arr_xt<double>& a);

	// 暂时仅限于ASCII字符的多行不定列数据，用于一些较为古老的数据集
	static bool load_lines(const StringBuffer& sb, arr_1t<String>& lines, const String& comment = String("#%*/"));
	static bool load_lines(const StringBuffer& sb, arr_1t<arr_1t<String> >& lines, const String& comment = String("#%*/"));
	static bool load_lines(const String& fn, arr_1t<arr_1t<Variant> >& lines, const String& comment = String("#%*/"), bool evaluation = true);
};

EW_LEAVE
#endif
