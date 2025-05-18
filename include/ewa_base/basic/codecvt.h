#ifndef __H_EW_BASIC_CODECVT__
#define __H_EW_BASIC_CODECVT__

#include "ewa_base/config.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/basic/stringbuffer.h"

#ifdef EW_WINDOWS
#include "windows.h"
#else
#include <iconv.h>
#include <errno.h>
#endif

EW_ENTER

template<int N> class char_implN;
template<> class char_implN<1>
{
public:
	typedef unsigned char type;
};
template<> class char_implN<2>
{
public:
	typedef uint16_t type;
};
template<> class char_implN<4>
{
public:
	typedef uint32_t type;
};
template<typename T> class char_impl : public char_implN<sizeof(T)> {};


class DLLIMPEXP_EWA_BASE IConv
{
public:

	static StringBufferT<wchar_t> to_wide(const String& s)
	{
		StringBufferT<wchar_t> sb;
		utf8_to_unicode(sb,s.c_str(),s.size());
		return sb;
	}

	static StringBuffer to_utf8(const wchar_t* s)
	{
		StringBuffer sb;
		unicode_to_utf8(sb,s,std::char_traits<wchar_t>::length(s));
		return sb;
	}

	static StringBuffer to_gbk(const String& s)
	{
		StringBuffer sb;
		utf8_to_gbk(sb,s.c_str(),s.size());
		return sb;
	}

#ifdef EW_WINDOWS
	static StringBuffer to_ansi(const String& s)
	{
		StringBuffer sb;
		utf8_to_ansi(sb,s.c_str(),s.size());
		return sb;
	}
	static String from_ansi(const char* s)
	{
		if (!s)
		{
			return String();
		}

		StringBuffer sb;
		ansi_to_utf8(sb,s,::strlen(s));
		return sb.c_str();
	}

#else
	static const String& to_ansi(const String& s){return s;}
	static const String from_ansi(const char* s){return s;}

#endif

	static String from_gbk(const char* s)
	{
		StringBuffer sb;
		gbk_to_utf8(sb,s,::strlen(s));
		return sb.c_str();
	}

	static String from_unknown(const char* s);

	// return  0 : ascii
	// return  1 : utf8
	// return -1 : gbk
	static int str_type(const char* s,size_t n=0);

	static void str_normal(String& str);

	static bool ensure_utf8(StringBuffer& sb);

	static bool unicode_to_gbk(StringBufferT<uint8_t>& aa_,const uint16_t* pw_,size_t ln_);
	static bool unicode_to_gbk(StringBufferT<uint8_t>& aa_,const uint32_t* pw_,size_t ln_);

	static bool gbk_to_unicode(StringBufferT<uint16_t>& aw_,const uint8_t* pa_,size_t ln_);
	static bool gbk_to_unicode(StringBufferT<uint32_t>& aw_,const uint8_t* pa_,size_t ln_);

	static bool unicode_to_utf8(StringBufferT<uint8_t>& aa_,const uint16_t* pw_,size_t ln_);
	static bool unicode_to_utf8(StringBufferT<uint8_t>& aa_,const uint32_t* pw_,size_t ln_);

	static bool utf8_to_unicode(StringBufferT<uint16_t>& aw_,const uint8_t* pa_,size_t ln_);
	static bool utf8_to_unicode(StringBufferT<uint32_t>& aw_,const uint8_t* pa_,size_t ln_);

	template<typename WC>
	static bool unicode_to_gbk(StringBuffer& aa_,const WC* pw_,size_t ln_)
	{
		typedef typename char_impl<WC>::type char_wcs;
		return unicode_to_gbk((StringBufferT<uint8_t>&)aa_,(const char_wcs*)pw_,ln_);
	}

	template<typename WC>
	static bool gbk_to_unicode(StringBufferT<WC>& aw_,const char* pa_,size_t ln_)
	{
		typedef typename char_impl<WC>::type char_wcs;
		return gbk_to_unicode((StringBufferT<char_wcs>&)aw_,(const uint8_t*)pa_,ln_);
	}

	template<typename WC>
	static bool unicode_to_utf8(StringBuffer& aa_,const WC* pw_,size_t ln_)
	{
		typedef typename char_impl<WC>::type char_wcs;
		return unicode_to_utf8((StringBufferT<uint8_t>&)aa_,(const char_wcs*)pw_,ln_);
	}

	template<typename WC>
	static bool utf8_to_unicode(StringBufferT<WC>& aw_,const char* pa_,size_t ln_)
	{
		typedef typename char_impl<WC>::type char_wcs;
		return utf8_to_unicode((StringBufferT<char_wcs>&)aw_,(const uint8_t*)pa_,ln_);
	}


	template<typename WC>
	static bool unicode_to_ansi(StringBuffer& aa_,const WC* pw_,size_t ln_)
	{
#ifdef EW_WINDOWS
		return unicode_to_gbk(aa_,pw_,ln_);
#else
		return unicode_to_utf8(aa_,pw_,ln_);
#endif
	}

	template<typename WC>
	static bool ansi_to_unicode(StringBufferT<WC>& aw_,const char* pa_,size_t ln_)
	{
#ifdef EW_WINDOWS
		return gbk_to_unicode(aw_,pa_,ln_);
#else
		return utf8_to_unicode(aw_,pa_,ln_);
#endif
	}

	static bool utf8_to_ansi(StringBuffer& aa_,const char* pa_,size_t ln_);
	static bool ansi_to_utf8(StringBuffer& aa_,const char* pa_,size_t ln_);
	static bool utf8_to_gbk(StringBuffer& aa_,const char* pa_,size_t ln_);
	static bool gbk_to_utf8(StringBuffer& aa_,const char* pa_,size_t ln_);
};


template<typename B, typename P>
B& FormatHelper<B, P>::operator << (wchar_t v_)
{ 
	StringBuffer v;
	IConv::unicode_to_utf8(v, &v_, 1);
	return P::append(_fmt_container(), v.data(), v.size());
}


template<typename B, typename P>
B& FormatHelper<B, P>::operator << (const wchar_t* v_)
{ 
	StringBuffer v;
	IConv::unicode_to_utf8(v, v_, std::char_traits<wchar_t>::length(v_));
	return P::append(_fmt_container(), v.data(), v.size()); 
}


EW_LEAVE
#endif
