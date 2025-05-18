#ifndef __H_EW_CRYPTO_MD5__
#define __H_EW_CRYPTO_MD5__


#include "ewa_base/collection/arr_1t.h"

EW_ENTER

class MD5_ctx
{
private:

	uint32_t Atemp, Btemp, Ctemp, Dtemp;

	static arr_1t<uint32_t> ms_K;
	static arr_1t<uint32_t> ms_s;

	static inline uint32_t shift(uint32_t a, uint32_t s)
	{
		return (a << s) | (a >> (32 - s));
	}

	static inline uint32_t swap_ending(uint32_t v)
	{
		char* p = (char*)&v;
		std::swap(p[0], p[3]);
		std::swap(p[1], p[2]);
		return v;
	}

	void _process();

	uint8_t tmp_data[64];
	size_t tmp_size, all_size;

public:

	MD5_ctx();

	void init();
	String fini();

	void add_data(const char* p, size_t n);

};

String md5(const char* p, size_t n);
String md5(const String& val);
String md5_file(const String& fp);

EW_LEAVE
#endif
