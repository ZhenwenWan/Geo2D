#ifndef __H_EW_STRING__
#define __H_EW_STRING__

#include "ewa_base/config.h"
#include "ewa_base/basic/hashing.h"
#include "ewa_base/basic/string_detail.h"
#include "ewa_base/basic/stringbuffer.h"
#include "ewa_base/basic/formater.h"

EW_ENTER

template<typename T>
class DLLIMPEXP_EWA_BASE StringBufferT;

class DLLIMPEXP_EWA_BASE String : public FormatHelper<String>
{
public:

	// 字符串比较，带一个大小写敏感参数，按字符比较，没有字符
	// 按字符顺序从左到右扫描，左边的字符比右边的小，返回-1，如果大返回+1，如果相等就向后扫描，如果都相等返回0，否则按照前面的规则继续比较，没有字符（字符串结束后的）是最小的
	typedef enum
	{
		case_sensitive		=0,
		case_insensitive	=1
	} caseCompare;

	typedef enum 
	{
		leading		= 0x1,
		trailing	= 0x2,
		both		= 0x3
	}stripType;

	friend class StringDetail;

	String();
	String(const String& o);

	String(detail::RawStringPtr o);

	String(const StringBuffer& o);
	String(const StringBufferT<unsigned char>& o);
	String(const StringBufferT<wchar_t>& o);

	String(const char* p);
	String(const char* p,size_t n);
	String(const char* p1,const char* p2);

	String(const unsigned char* p);
	String(const unsigned char* p,size_t n);
	String(const unsigned char* p1,unsigned const char* p2);

	String(const wchar_t* p);
	String(const wchar_t* p,size_t n);
	String(const wchar_t* p1,const wchar_t* p2);

	void assign(const char* p,size_t n);
	void assign(const char* p1,const char* p2);
	void assign(const unsigned char* p,size_t n);
	void assign(const unsigned char* p1,const unsigned char* p2);
	void assign(const wchar_t* p,size_t n);
	void assign(const wchar_t* p1, const wchar_t* p2);

	void assign(const uint16_t* p1, const uint16_t* p2);
	void assign(const uint32_t* p1, const uint32_t* p2);

	void append(const char* p,size_t n);
	void append(const char* p1,const char* p2);
	void append(const unsigned char* p,size_t n);
	void append(const unsigned char* p1,const unsigned char* p2);
	void append(const wchar_t* p,size_t n);
	void append(const wchar_t* p1,const wchar_t* p2);

	template<typename T>
	String(const std::basic_string<T>& o);

#if defined(EW_C11)
	String(String&& p) EW_NOEXCEPT
	{
		m_ptr=p.m_ptr;
		p.m_ptr=const_empty_buffer;
	}
	String& operator=(String&& p) EW_NOEXCEPT 
	{
		std::swap(m_ptr, p.m_ptr);
		return *this;
	}
#endif

	~String();

	void swap(String& s1);

	const char* c_str() const;
	size_t size() const;
	size_t length() const;

	bool empty() const;

	String& operator=(const String& p);
	String& operator=(const char* p);
	String& operator=(const unsigned char* p);
	String& operator=(const wchar_t* p);

	template<typename T>
	String& operator=(const std::basic_string<T>& o)
	{
		return (*this)=o.c_str();
	}


	String& operator+=(const String& p);
	String& operator+=(const char* p);
	String& operator+=(const unsigned char* p);
	String& operator+=(const wchar_t* p);

	template<typename T>
	String& operator+=(const std::basic_string<T>& o)
	{
		return (*this)+=o.c_str();
	}


	friend DLLIMPEXP_EWA_BASE std::ostream& operator<<(std::ostream&o,const String& s);
	friend DLLIMPEXP_EWA_BASE std::istream& operator>>(std::istream&o,String& s);


	bool ToNumber(float32_t* val) const;
	bool ToNumber(float64_t* val) const;

// diference between ToNumber and ToNumber10
// "010" ToNumber -> 8 (base 8)
// "010" ToNumber10 -> 10 (base 10)

	bool ToNumber10(int64_t* val) const;
	bool ToNumber10(int32_t* val) const;

	bool ToNumber(int64_t* val) const;
	bool ToNumber(int32_t* val) const;

	int ToNumber2(int64_t* v1, double* v2) const;

	static const size_t npos=(size_t)(-1);

	String substr(size_t pos,size_t len=npos) const;

	int replace(char c1,char c2);
	int replace(const String& s1,const String& c2);

	int find(char c, int pos = 0) const;
	int find_last(char c) const;

	int find(const String& s, int pos = 0) const;
	bool start_with(const String& s, caseCompare case_compare = case_sensitive) const;
	bool start_with(const char c, caseCompare case_compare = case_sensitive) const;
	bool end_with(const String& s, caseCompare case_compare = case_sensitive) const;


	int find_if(bool(*func)(char), int pos = 0) const;
	int find_if_not(bool(*func)(char), int pos = 0) const;

	static String takeWhile(const String& str, bool(*func)(char));
	static String dropWhile(const String& str, bool(*func)(char));

	static bool is_alphabet(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	static bool is_alphabet(wchar_t c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	static bool is_digit(char c)
	{
		return (c >= '0' && c <= '9');
	}

	static bool is_digit(wchar_t c)
	{
		return (c >= '0' && c <= '9');
	}

	// case conversion for char/wchar_t
	static char to_lower(char c);
	static wchar_t to_lower(wchar_t c);
	static char to_upper(char c);
	static wchar_t to_upper(wchar_t c);

	static String to_lower(const String& str);
	static String to_upper(const String& arr);
	// member function
	String& lower();
	String& upper();

	// Note: wide character's visibility if complicated
	static bool is_visible(char c) /* reference to the ASCII table */
	{
		return c >= 33 && c <= 126;
	}

	static bool is_invisible(char c)
	{
		return !is_visible(c);
	}

	// 只扣掉ASCII的不可见字符
	static bool is_visible(wchar_t c) /* reference to the ASCII table */
	{
		return !is_invisible(c);
	}

	// 只考虑ASCII的不可见字符
	static bool is_invisible(wchar_t c)
	{
		return c < 33 || (c > 126 && c < 128);
	}

	static bool is_ascii(wchar_t c)
	{
		return (c >= 0 && c <= 127);
	}

	static bool is_ascii(char c)
	{
		return (c >= 0 && c <= 127);
	}


	STRING_FORMATER_FORMAT_FUNCS_SB(static String Format,return fb;)
	STRING_FORMATER_FORMAT_FUNCS_SB(String Printf,*this=fb;return *this;)

private:


	const char* m_ptr;

	void _do_append(const char* p1,size_t n);
	void _do_append(const wchar_t* p1,size_t n);
	void _do_assign(const char* p1,size_t n);
	void _do_assign(const wchar_t* p1,size_t n);

public :


	static String Strip(String s, String::stripType w = String::trailing);
	static bool StartsWith(const String& s, const String& prefix, caseCompare case_compare = case_sensitive);
	static void Split(const String& str, const char sep, arr_1t<String>& arrayString);
	static void SplitWhen(const String& str, arr_1t<String>& arrayString, bool(*pred)(char));
	static void SplitWhen(const String& str, arr_1t<String>& arrayString, bool(*pred)(wchar_t));
	static void Split(const String& str, const char sep, arr_1t<String>& arrayString,
		const char escape /* 转义字符，如果是'\0'则忽略转义字符 */ , bool reserve_nil = false /* 是否保留空白字符串（例如csv格式应设置为true） */);
	static void Words(const String& str, arr_1t<String>& arrayString);
	static void Lines(const String& str, arr_1t<String>& arrayString);


	static int compare(const String& s1, const String& s2, caseCompare case_compare = case_sensitive);

};

DLLIMPEXP_EWA_BASE extern const String& empty_string;


inline const char* String::c_str() const
{
	return m_ptr;
}

inline bool String::empty() const
{
	return m_ptr[0]=='\0';
}

inline String::~String()
{
	StringDetail::str_free(m_ptr);
}

inline String::String()
{
	m_ptr=StringDetail::str_empty();
}

inline String::String(const String& o)
{
	m_ptr=StringDetail::str_dup(o.m_ptr);
}

inline String::String(const char* p)
{
	m_ptr=StringDetail::str_dup(p);
}

inline String::String(const char* p1,size_t n)
{
	m_ptr=StringDetail::str_dup(p1,n);
}

inline String::String(const char* p1,const char* p2)
{
	m_ptr=StringDetail::str_dup(p1,p2-p1);
}

inline void String::swap(String& s1)
{
	std::swap(m_ptr,s1.m_ptr);
}

DLLIMPEXP_EWA_BASE std::ostream& operator<<(std::ostream&o,const String& s);
DLLIMPEXP_EWA_BASE std::istream& operator>>(std::istream&o,String& s);

template<typename T>
String::String(const std::basic_string<T>& o)
{
	m_ptr=StringDetail::str_dup(o.c_str());
}

String DLLIMPEXP_EWA_BASE operator+(const String& lhs,const String& rhs);
String DLLIMPEXP_EWA_BASE operator+(const char* lhs,const String& rhs);
String DLLIMPEXP_EWA_BASE operator+(const String& lhs,const char* rhs);

template<typename T>
String operator+(const std::basic_string<T>& lhs,const String& rhs)
{
	String ret;
	ret.m_ptr=StringDetail::str_cat(lhs.c_str(), rhs.c_str());
	return EW_MOVE(ret);
}

template<typename T>
String operator+(const String& lhs,const std::basic_string<T>& rhs)
{
	String ret;
	ret.m_ptr=StringDetail::str_cat(lhs.c_str(), rhs.c_str());
	return EW_MOVE(ret);
}


class pl_strcmp
{
public:

	static inline int g(const char* s1, const char* s2)
	{
		return ::strcmp(s1, s2);
	}

	static inline int g(const String& s1, const char* s2)
	{
		return ::strcmp(s1.c_str(), s2);
	}

	static inline int g(const char* s1, const String& s2)
	{
		return ::strcmp(s1, s2.c_str());
	}

	static inline int g(const String& s1, const String& s2)
	{
		return ::strcmp(s1.c_str(), s2.c_str());
	}

};

#define STRING_REL_OP2(X)	\
	inline bool operator X (const String& lhs,const String& rhs){return pl_strcmp::g(lhs,rhs) X 0;}\
	inline bool operator X (const char* lhs,const String& rhs){return pl_strcmp::g(lhs,rhs) X 0;}\
	inline bool operator X (const String& lhs,const char* rhs){return pl_strcmp::g(lhs,rhs) X 0;}\


STRING_REL_OP2(==)
STRING_REL_OP2(!=)
STRING_REL_OP2(>=)
STRING_REL_OP2(<=)
STRING_REL_OP2(<)
STRING_REL_OP2(>)

template<> class hash_t<char*>
{
public:
	inline uint32_t operator()(const char* v)
	{
		return hash_raw<1>::hash_string(v);
	}
};


template<> class hash_t<String>
{
public:
	inline uint32_t operator()(const String& v)
	{
		return hash_raw<1>::hash_string(v.c_str());
	}
};


inline const char* FormatPolicy::cast(const String& v)
{
	return v.c_str();
}

inline void StringDetail::str_reset(String& str, const char* p)
{
	str_free(str.m_ptr);
	str.m_ptr = p;
}

inline char String::to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + ('a' - 'A');
	}
	else
	{
		return c;
	}
}

inline wchar_t String::to_lower(wchar_t c)
{
	if (c >= L'A' && c <= L'Z')
	{
		return c + (L'a' - L'A');
	}
	else
	{
		return c;
	}
}

inline char String::to_upper(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return c + ('A' - 'a');
	}
	else
	{
		return c;
	}
}

inline wchar_t String::to_upper(wchar_t c)
{
	if (c >= L'a' && c <= L'z')
	{
		return c + (L'A' - L'a');
	}
	else
	{
		return c;
	}
}

DEFINE_OBJECT_NAME_T(StringBufferT, "buffer");


template<typename B, typename P>
B& FormatHelper<B, P>::operator << (const String& v){ return P::append(_fmt_container(), v.c_str()); }



DLLIMPEXP_EWA_BASE const String& Translate(const String& msg);
DLLIMPEXP_EWA_BASE const String& Translate(const String& msg, const String& def);
DLLIMPEXP_EWA_BASE String Translate2(const String& msg);


inline int FormatHandlerString::printf_s(char* p, const char* s, const String& v)
{
	return ::sprintf(p, s, v.c_str());
}

inline int FormatHandlerString::printf_s(char* p, const char* s, const wchar_t* v)
{
	return ::sprintf(p, s, String(v).c_str());
}


EW_LEAVE

namespace std
{
	inline void swap(ew::String& v1, ew::String& v2)
	{
		v1.swap(v2);
	}
}

#endif
