#ifndef __H_EW_BASIC_LOOKUPTABLE__
#define __H_EW_BASIC_LOOKUPTABLE__

#include "ewa_base/config.h"

EW_ENTER

template<template<unsigned> class P, typename T = const unsigned char>
class lookup_table
{
public:
	static inline T test(unsigned char text)
	{
		return cmap[text];
	}
	inline T operator[](unsigned char text) const
	{
		return cmap[text];
	}
	inline T operator()(unsigned char text) const
	{
		return cmap[text];
	}
	static T cmap[256];
};

template<template<unsigned> class P,unsigned N, typename T = const unsigned char>
class lookup_table_u
{
public:
	static inline T test(unsigned text)
	{
		if (text > 255) return N;
		return lookup_table<P,T>::cmap[text];
	}
	inline T operator[](unsigned text) const
	{
		if (text > 255) return N;
		return lookup_table<P, T>::cmap[text];
	}
	inline T operator()(unsigned text) const
	{
		if (text > 255) return N;
		return lookup_table<P, T>::cmap[text];
	}
};

template<unsigned N>
class lkt2uppercase
{
public:
	static const unsigned value = (N >= 'a' && N <= 'z') ? (N + 'A' - 'a') : N;
};

template<unsigned N>
class lkt2lowercase
{
public:
	static const unsigned value = (N >= 'A'&&N <= 'Z') ? (N - 'A' + 'a') : N;
};


template<unsigned N>
class lkt_slash
{
public:
	static const unsigned value =
		N == 't' ? '\t' :
		N == 'n' ? '\n' :
		N == 'r' ? '\r' :
		N == 'v' ? '\v' :
		N == 'f' ? '\f' :
		N == '0' ? '\0' :
		N;
};


template<unsigned N>
class lkt_whitespace
{
public:
	static const unsigned value = N == ' ' || N == '\t' || N == '\r' || N == '\n';
};

template<unsigned N>
class lkt_whitespace_and_null : public lkt_whitespace<N>
{
public:
	static const unsigned value = N == 0 || lkt_whitespace<N>::value;
};


template<unsigned N>
class lkt_tabspace
{
public:
	static const unsigned value = N == ' ' || N == '\t';
};



template<unsigned N>
class lkt_number10b
{
public:
	static const unsigned char value = (N >= '0'&&N <= '9') ? (N - '0') : 0xFF;
};

template<unsigned N>
class lkt_number16b
{
public:
	static const unsigned upperN = lkt2uppercase<N>::value;
	static const unsigned char value = (upperN >= 'A'&&upperN <= 'F') ? (upperN + 10 - 'A') : lkt_number10b<N>::value;
};

template<unsigned N>
class lkt_number08b
{
public:
	static const unsigned char value = (N >= '0'&&N <= '7') ? (N - '0') : 0xFF;
};

template<unsigned N>
class lkt_number02b
{
public:
	static const unsigned char value = (N >= '0'&&N <= '1') ? (N - '0') : 0xFF;
};

template<unsigned N>
class lkt_is_character
{
public:
	static const int value = (N >= 'a'&&N <= 'z') || (N >= 'A'&&N <= 'Z');
};

template<unsigned N>
class lkt_is_number10
{
public:
	static const int value = (N >= '0'&&N <= '9');
};

template<unsigned N>
class lkt_is_number08
{
public:
	static const int value = (N >= '0'&&N <= '7');
};

template<unsigned N>
class lkt_is_number02
{
public:
	static const int value = (N >= '0'&&N <= '1');
};

template<unsigned N>
class lkt_is_number16
{
public:
	static const int value = lkt_is_number10<N>::value || (N >= 'a'&&N <= 'f') || (N >= 'A'&&N <= 'F');
};

template<unsigned N>
class lkt_not_newline
{
public:
	static const int value = N != 0 || N != '\n';
};

template<template<unsigned> class P, typename T>
T lookup_table<P, T>::cmap[256] =
{
	(T)P<0x00>::value, (T)P<0x01>::value, (T)P<0x02>::value, (T)P<0x03>::value,
	(T)P<0x04>::value, (T)P<0x05>::value, (T)P<0x06>::value, (T)P<0x07>::value,
	(T)P<0x08>::value, (T)P<0x09>::value, (T)P<0x0A>::value, (T)P<0x0B>::value,
	(T)P<0x0C>::value, (T)P<0x0D>::value, (T)P<0x0E>::value, (T)P<0x0F>::value,
	(T)P<0x10>::value, (T)P<0x11>::value, (T)P<0x12>::value, (T)P<0x13>::value,
	(T)P<0x14>::value, (T)P<0x15>::value, (T)P<0x16>::value, (T)P<0x17>::value,
	(T)P<0x18>::value, (T)P<0x19>::value, (T)P<0x1A>::value, (T)P<0x1B>::value,
	(T)P<0x1C>::value, (T)P<0x1D>::value, (T)P<0x1E>::value, (T)P<0x1F>::value,
	(T)P<0x20>::value, (T)P<0x21>::value, (T)P<0x22>::value, (T)P<0x23>::value,
	(T)P<0x24>::value, (T)P<0x25>::value, (T)P<0x26>::value, (T)P<0x27>::value,
	(T)P<0x28>::value, (T)P<0x29>::value, (T)P<0x2A>::value, (T)P<0x2B>::value,
	(T)P<0x2C>::value, (T)P<0x2D>::value, (T)P<0x2E>::value, (T)P<0x2F>::value,
	(T)P<0x30>::value, (T)P<0x31>::value, (T)P<0x32>::value, (T)P<0x33>::value,
	(T)P<0x34>::value, (T)P<0x35>::value, (T)P<0x36>::value, (T)P<0x37>::value,
	(T)P<0x38>::value, (T)P<0x39>::value, (T)P<0x3A>::value, (T)P<0x3B>::value,
	(T)P<0x3C>::value, (T)P<0x3D>::value, (T)P<0x3E>::value, (T)P<0x3F>::value,
	(T)P<0x40>::value, (T)P<0x41>::value, (T)P<0x42>::value, (T)P<0x43>::value,
	(T)P<0x44>::value, (T)P<0x45>::value, (T)P<0x46>::value, (T)P<0x47>::value,
	(T)P<0x48>::value, (T)P<0x49>::value, (T)P<0x4A>::value, (T)P<0x4B>::value,
	(T)P<0x4C>::value, (T)P<0x4D>::value, (T)P<0x4E>::value, (T)P<0x4F>::value,
	(T)P<0x50>::value, (T)P<0x51>::value, (T)P<0x52>::value, (T)P<0x53>::value,
	(T)P<0x54>::value, (T)P<0x55>::value, (T)P<0x56>::value, (T)P<0x57>::value,
	(T)P<0x58>::value, (T)P<0x59>::value, (T)P<0x5A>::value, (T)P<0x5B>::value,
	(T)P<0x5C>::value, (T)P<0x5D>::value, (T)P<0x5E>::value, (T)P<0x5F>::value,
	(T)P<0x60>::value, (T)P<0x61>::value, (T)P<0x62>::value, (T)P<0x63>::value,
	(T)P<0x64>::value, (T)P<0x65>::value, (T)P<0x66>::value, (T)P<0x67>::value,
	(T)P<0x68>::value, (T)P<0x69>::value, (T)P<0x6A>::value, (T)P<0x6B>::value,
	(T)P<0x6C>::value, (T)P<0x6D>::value, (T)P<0x6E>::value, (T)P<0x6F>::value,
	(T)P<0x70>::value, (T)P<0x71>::value, (T)P<0x72>::value, (T)P<0x73>::value,
	(T)P<0x74>::value, (T)P<0x75>::value, (T)P<0x76>::value, (T)P<0x77>::value,
	(T)P<0x78>::value, (T)P<0x79>::value, (T)P<0x7A>::value, (T)P<0x7B>::value,
	(T)P<0x7C>::value, (T)P<0x7D>::value, (T)P<0x7E>::value, (T)P<0x7F>::value,
	(T)P<0x80>::value, (T)P<0x81>::value, (T)P<0x82>::value, (T)P<0x83>::value,
	(T)P<0x84>::value, (T)P<0x85>::value, (T)P<0x86>::value, (T)P<0x87>::value,
	(T)P<0x88>::value, (T)P<0x89>::value, (T)P<0x8A>::value, (T)P<0x8B>::value,
	(T)P<0x8C>::value, (T)P<0x8D>::value, (T)P<0x8E>::value, (T)P<0x8F>::value,
	(T)P<0x90>::value, (T)P<0x91>::value, (T)P<0x92>::value, (T)P<0x93>::value,
	(T)P<0x94>::value, (T)P<0x95>::value, (T)P<0x96>::value, (T)P<0x97>::value,
	(T)P<0x98>::value, (T)P<0x99>::value, (T)P<0x9A>::value, (T)P<0x9B>::value,
	(T)P<0x9C>::value, (T)P<0x9D>::value, (T)P<0x9E>::value, (T)P<0x9F>::value,
	(T)P<0xA0>::value, (T)P<0xA1>::value, (T)P<0xA2>::value, (T)P<0xA3>::value,
	(T)P<0xA4>::value, (T)P<0xA5>::value, (T)P<0xA6>::value, (T)P<0xA7>::value,
	(T)P<0xA8>::value, (T)P<0xA9>::value, (T)P<0xAA>::value, (T)P<0xAB>::value,
	(T)P<0xAC>::value, (T)P<0xAD>::value, (T)P<0xAE>::value, (T)P<0xAF>::value,
	(T)P<0xB0>::value, (T)P<0xB1>::value, (T)P<0xB2>::value, (T)P<0xB3>::value,
	(T)P<0xB4>::value, (T)P<0xB5>::value, (T)P<0xB6>::value, (T)P<0xB7>::value,
	(T)P<0xB8>::value, (T)P<0xB9>::value, (T)P<0xBA>::value, (T)P<0xBB>::value,
	(T)P<0xBC>::value, (T)P<0xBD>::value, (T)P<0xBE>::value, (T)P<0xBF>::value,
	(T)P<0xC0>::value, (T)P<0xC1>::value, (T)P<0xC2>::value, (T)P<0xC3>::value,
	(T)P<0xC4>::value, (T)P<0xC5>::value, (T)P<0xC6>::value, (T)P<0xC7>::value,
	(T)P<0xC8>::value, (T)P<0xC9>::value, (T)P<0xCA>::value, (T)P<0xCB>::value,
	(T)P<0xCC>::value, (T)P<0xCD>::value, (T)P<0xCE>::value, (T)P<0xCF>::value,
	(T)P<0xD0>::value, (T)P<0xD1>::value, (T)P<0xD2>::value, (T)P<0xD3>::value,
	(T)P<0xD4>::value, (T)P<0xD5>::value, (T)P<0xD6>::value, (T)P<0xD7>::value,
	(T)P<0xD8>::value, (T)P<0xD9>::value, (T)P<0xDA>::value, (T)P<0xDB>::value,
	(T)P<0xDC>::value, (T)P<0xDD>::value, (T)P<0xDE>::value, (T)P<0xDF>::value,
	(T)P<0xE0>::value, (T)P<0xE1>::value, (T)P<0xE2>::value, (T)P<0xE3>::value,
	(T)P<0xE4>::value, (T)P<0xE5>::value, (T)P<0xE6>::value, (T)P<0xE7>::value,
	(T)P<0xE8>::value, (T)P<0xE9>::value, (T)P<0xEA>::value, (T)P<0xEB>::value,
	(T)P<0xEC>::value, (T)P<0xED>::value, (T)P<0xEE>::value, (T)P<0xEF>::value,
	(T)P<0xF0>::value, (T)P<0xF1>::value, (T)P<0xF2>::value, (T)P<0xF3>::value,
	(T)P<0xF4>::value, (T)P<0xF5>::value, (T)P<0xF6>::value, (T)P<0xF7>::value,
	(T)P<0xF8>::value, (T)P<0xF9>::value, (T)P<0xFA>::value, (T)P<0xFB>::value,
	(T)P<0xFC>::value, (T)P<0xFD>::value, (T)P<0xFE>::value, (T)P<0xFF>::value
};





EW_LEAVE
#endif
