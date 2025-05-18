#ifndef __H_EW_BASIC_UTIL_STRLIB__
#define __H_EW_BASIC_UTIL_STRLIB__

#include "ewa_base/basic/string.h"
#include "ewa_base/basic/stringbuffer.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/collection/arr_xt.h"


EW_ENTER

class Variant;
String num2exp(double val);
String var2exp(const Variant& v);

template<typename IT>
String arr2exp(IT t1, IT t2)
{
	if (t1 == t2)
	{
		return empty_string;
	}

	StringBuffer val;
	for (IT it = t1; it != t2;++it)
	{
		val << num2exp(*it);
		val << ",";
	}

	val.back() = '\0';
	return val;
}


template<typename T>
String string_join(T t1,T t2,const String& br)
{
	StringBuilder sb;
	while(t1!=t2)
	{
		sb<<*t1;
		if(++t1==t2) break;
		sb<<br;
	}
	return sb.get_str();
}


String string_join_with_esc(const String* p,size_t n);
arr_1t<String> string_split_with_esc(const String& s);



DLLIMPEXP_EWA_BASE arr_1t<String> string_split(const String& s, const String& b);
DLLIMPEXP_EWA_BASE void string_lines(arr_1t<String>& results, const char* s1,const char* s2);
DLLIMPEXP_EWA_BASE arr_1t<String> string_lines(const String& s);
DLLIMPEXP_EWA_BASE arr_1t<String> string_lines(const char* s);
DLLIMPEXP_EWA_BASE arr_1t<String> string_words(const String& s);

DLLIMPEXP_EWA_BASE String string_repeat(const String& s, size_t n);


DLLIMPEXP_EWA_BASE bool string_to_array(arr_xt<String>& val, const String& s, const String& br);


DLLIMPEXP_EWA_BASE int string_replace(String& str_src, const String& str_old, const String& str_new);

DLLIMPEXP_EWA_BASE String string_unescape(const char* p1,const char* p2);
DLLIMPEXP_EWA_BASE String string_unescape(const String& str_src);
DLLIMPEXP_EWA_BASE String string_escape(const String& str_src);

DLLIMPEXP_EWA_BASE String string_urlencode(const String& str_src);
DLLIMPEXP_EWA_BASE String string_urldecode(const String& str_src);

DLLIMPEXP_EWA_BASE String string_strencode(const String& str_src);

DLLIMPEXP_EWA_BASE String string_htmlencode(const String& str_src);

DLLIMPEXP_EWA_BASE int string_compare_no_case(const String& lhs, const String& rhs);

DLLIMPEXP_EWA_BASE int string_compare(const String& lhs, const String& rhs,String::caseCompare case_compare);

DLLIMPEXP_EWA_BASE int string_start_with(const String& s1, const String& s2,String::caseCompare case_compare);

DLLIMPEXP_EWA_BASE int string_end_with(const String& s1, const String& s2,String::caseCompare case_compare);


enum
{
	TRIM_L=1<<0,
	TRIM_R=1<<1,
	TRIM_M=1<<2,
	TRIM_B=TRIM_L|TRIM_R,
	TRIM_A=TRIM_B|TRIM_M,
};

DLLIMPEXP_EWA_BASE String string_trim(const String& s, int flag = TRIM_B, char ch = ' ');
DLLIMPEXP_EWA_BASE String string_trim_whitespace(const String& s, int flag = TRIM_B);

DLLIMPEXP_EWA_BASE String string_to_lower(const String& s);
DLLIMPEXP_EWA_BASE String string_to_upper(const String& s);


DLLIMPEXP_EWA_BASE const char* str_find_first(const char* p, char c,size_t pos=0);
DLLIMPEXP_EWA_BASE const char* str_find_last(const char* p, char c);

DLLIMPEXP_EWA_BASE const char* str_find_first(const char* p, const char* c,size_t pos=0);
DLLIMPEXP_EWA_BASE const char* str_find_last(const char* p, const char* c);


DLLIMPEXP_EWA_BASE size_t utf8_word_length(const char* u8);
DLLIMPEXP_EWA_BASE size_t utf8_word_bytes(char u8);
DLLIMPEXP_EWA_BASE String utf8_substr(const char* u8,intptr_t p1,intptr_t p2);


DLLIMPEXP_EWA_BASE String base64_encode(const void* p, size_t n);
DLLIMPEXP_EWA_BASE void base64_encode(const void* p, size_t n, StringBuffer& buf);
DLLIMPEXP_EWA_BASE arr_1t<char> base64_decode(const char* p,size_t n);
DLLIMPEXP_EWA_BASE void base64_decode(const char* p, size_t n, StringBuffer& buf);
DLLIMPEXP_EWA_BASE arr_1t<char> base64_decode(const String& s);
DLLIMPEXP_EWA_BASE void base64_decode(const String& s, StringBuffer& buf);


DLLIMPEXP_EWA_BASE bool string_to_number(const char* p, double& val);
DLLIMPEXP_EWA_BASE bool string_to_number_ex(const char* p, double& val);
DLLIMPEXP_EWA_BASE bool string_to_number(const char* p, int64_t& val);
DLLIMPEXP_EWA_BASE bool string_to_number_10(const char* p, int64_t& val);

DLLIMPEXP_EWA_BASE bool string_is_blank(const char* p);


DLLIMPEXP_EWA_BASE size_t string_table_format_innter(String* p, size_t n);

DLLIMPEXP_EWA_BASE String string_table_format(const String* p, size_t n,bool header=false);

DLLIMPEXP_EWA_BASE size_t string_width(const String& s);

EW_LEAVE
#endif
