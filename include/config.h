#ifndef __H_EW_EWA_BASE_CONFIG__
#define __H_EW_EWA_BASE_CONFIG__

//#define EWX_CONST_BUILD "20220920080102"

#include "tl.h"

#include <complex>
#include <utility>


namespace tl
{
	template<typename T>
	struct is_pod<std::complex<T> > : public is_pod<T> {};

	template<typename X,typename Y>
	struct is_pod<std::pair<X,Y> > : public value_type<is_pod<X>::value&&is_pod<Y>::value> {};

};

#ifdef _DEBUG
#define EW_DEBUG
#elif defined(NDEBUG)
#define EW_NDEBUG
#endif


#define EW_C11

// use MemPoolPaging as default memory allocator
// #define EW_MEMUSEPOOL

#define EW_OBJECT_USE_MP_ALLOC


#ifdef EW_C11
	#define EW_NOEXCEPT throw()
	#define EW_MOVE(x) std::move(x)
	#define EW_RVALUE(T) T&&
	#define EW_FORWARD(T,x) std::forward<T>(x)
	#define EW_ARG(T) T
#else
	#define EW_NOEXCEPT
	#define EW_MOVE(x) x
	#define EW_RVALUE(T) const T&
	#define EW_FORWARD(T,x) x
	#define EW_ARG(T) T
#endif




//#define EW_NO64BIT_ATOMIC 1

#if defined(_WIN64)
	#define EW_WIN64
	#define EW_X64
	#ifndef EW_WINDOWS
        #define EW_WINDOWS
    #endif
#elif defined(_WIN32)
	#define EW_WIN32
	#define EW_X86
	#ifndef EW_WINDOWS
        #define EW_WINDOWS
    #endif
#else
	#ifdef __LP64__
		#define EW_X64
	#else
		#define EW_X86
	#endif
	#define EW_LINUX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _MSC_VER
	#define EW_MSVC _MSC_VER

#if _MSC_VER>=1928
	#define EW_MSVC_16
#else
	#define EW_MSVC_14
#endif

#else

#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif


#define EW_UNUSED(X) ((void)&(X));


#ifdef EW_MSVC
#define EW_ATTRIBUTE(X) __declspec(X)
#else
#define EW_ATTRIBUTE(X) __attribute__((X))
#endif

#ifdef EW_MSVC
#define EW_THREAD_TLS __declspec(thread)
#else
#define EW_THREAD_TLS __thread
#endif

#define EW_NORETURN EW_ATTRIBUTE(noreturn)


#ifndef EW_UOS
#define EW_ALIGN(X) EW_ATTRIBUTE(align(X))
#else
#define EW_ALIGN(X)
#endif





#ifdef EWA_BASE_DLL
#ifdef EW_MSVC
#define EWA_BASE_EXPORT __declspec(dllexport)
#define EWA_BASE_IMPORT __declspec(dllimport)
#else
#define EWA_BASE_EXPORT __attribute__((dllexport))
#define EWA_BASE_IMPORT __attribute__((dllimport))
#endif
#else
#define EWA_BASE_EXPORT
#define EWA_BASE_IMPORT
#endif

#ifdef EWA_BASE_BUILDING
#define DLLIMPEXP_EWA_BASE EWA_BASE_EXPORT
#else
#define DLLIMPEXP_EWA_BASE EWA_BASE_IMPORT
#endif


#ifdef EW_MSVC
#pragma warning(disable:4251)
#endif

#define EW_ENSURE_CLASS_LINKED(X) X::sm_info.GetName();


#define EW_CALL_INIT_FUNCTION(X) ew::OnInitializingEastWave(#X); X();


#if !defined(EW_NDEBUG)
#define EW_FORCEINLINE inline
#elif defined(EW_MSVC)
#define EW_FORCEINLINE __forceinline
#elif defined(EW_LINUX)
#define EW_FORCEINLINE inline __attribute__((always_inline))
#else
#define EW_FORCEINLINE inline
#endif



//#define EW_CHECK_HEAP

#ifdef EW_DEBUG
	//detect heap corruption for mp_alloc/mp_free
	#define EW_CHECK_HEAP
	#define EW_CHECK_HEAP_BREAK
#endif


#ifdef EW_MSVC

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;


#ifndef _UINTPTR_T_DEFINED
#ifdef  _WIN64
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#else
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#endif
#define _UINTPTR_T_DEFINED
#endif

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#else
#include <sys/types.h>
#include <stdint.h>
#endif




template<int> struct integer_type;
template<> struct integer_type<1> { typedef uint8_t unsigned_type; typedef int8_t singed_type; };
template<> struct integer_type<2>{ typedef uint16_t unsigned_type; typedef int16_t signed_type;};
template<> struct integer_type<4>{ typedef uint32_t unsigned_type; typedef int32_t signed_type;};
template<> struct integer_type<8>{ typedef uint64_t unsigned_type; typedef int64_t signed_type;};

template<int N> struct unsigned_integer_type 
{
	typedef typename integer_type<N>::unsigned_type type;
};

template<int N> struct signed_integer_type 
{
	typedef typename integer_type<N>::signed_type type;
};



typedef float float32_t;
typedef double float64_t;


#ifndef _kT
#define _kT(x) (x)
#endif

#ifndef _hT
#define _hT(x) ew::Translate(x)
#endif

#ifndef _zT
#define _zT(x) ew::Translate(x,ew::empty_string)
#endif

#ifndef _dT
#define _dT(x) ("$" x)
#endif


#if defined(EW_MEMDEBUG) || defined(EW_MEMUSEPOOL)

void* operator new(size_t);
void* operator new(size_t,const char*,int);
void* operator new(size_t,int,const char*,int);
void* operator new[](size_t);
void* operator new[](size_t,const char*,int);
void* operator new[](size_t,int,const char*,int);

void operator delete(void*);
void operator delete(void*,const char*,int);
void operator delete(void*,int,const char*,int);
void operator delete[](void*);
void operator delete[](void*,const char*,int);
void operator delete[](void*,int,const char*,int);

#endif


#if defined(EW_MEMDEBUG)

#define new DEBUG_NEW
#define DEBUG_NEW  new (__FILE__,__LINE__)

#endif

#define DEFINE_OBJECT_NAME(TYPE,DESC)\
	template<> class ObjectNameT<TYPE>{public:\
		static String MakeName(const String& s){return (s+"#")+DESC;} \
		static String MakeName(){return DESC;} \
	};

#define DEFINE_OBJECT_NAME_T(TYPE,DESC)\
	template<typename T> class ObjectNameT<TYPE<T> >{public:static String MakeName(const String& s){return ObjectNameT<T>::MakeName((s+"#")+DESC);} };


#define EW_ENTER namespace ew{
#define EW_LEAVE };



#ifndef EW_NDEBUG
#define EW_NOOP() ew::OnNoop()
#define EW_ON_ERROR(x) ew::OnError(x)
#define EW_STATIC_ASSERT(x) {char buffer[(x)?1:0];(void)&buffer;}
#else
#define EW_NOOP()
#define EW_STATIC_ASSERT(x)
#define EW_ON_ERROR(x)
#endif


#define EW_CHECK_MSG(cond,msg) if(!(cond)) ew::OnAssertFailure((msg),__FILE__,__LINE__);
#define EW_CHECK(cond) EW_CHECK_MSG(cond,#cond);
#define EW_CHECK_LT(X,Y) EW_CHECK((X)<(Y))
#define EW_CHECK_LE(X,Y) EW_CHECK((X)<=(Y))



#ifndef EW_NDEBUG
#define EW_ASSERT_MSG(cond,msg) EW_CHECK_MSG(cond,msg)
#define EW_ASSERT(cond) EW_CHECK(cond)
#define EW_ASSERT_LT(X,Y) EW_CHECK_LT(X,Y)
#define EW_ASSERT_LE(X,Y) EW_CHECK_LE(X,Y)
#else
#define EW_ASSERT_MSG(cond,msg)
#define EW_ASSERT(cond)
#define EW_ASSERT_LT(X,Y)
#define EW_ASSERT_LE(X,Y)
#endif


#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL	((void *)0)
#endif
#endif


EW_ENTER

DLLIMPEXP_EWA_BASE void* mp_alloc(size_t n);
DLLIMPEXP_EWA_BASE void* mp_alloc_throw(size_t n);
DLLIMPEXP_EWA_BASE void mp_free(void* p);

DLLIMPEXP_EWA_BASE void OnInitializingEastWave(const char* msg);


// Prevent form copy
class DLLIMPEXP_EWA_BASE NonCopyable
{
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&);
protected:
	EW_FORCEINLINE NonCopyable() {}
	EW_FORCEINLINE ~NonCopyable() {}
};

class DLLIMPEXP_EWA_BASE NonCopyableAndNonNewable
{
	NonCopyableAndNonNewable(const NonCopyableAndNonNewable&);
	NonCopyableAndNonNewable& operator=(const NonCopyableAndNonNewable&);
protected:
	EW_FORCEINLINE NonCopyableAndNonNewable() {}
	EW_FORCEINLINE ~NonCopyableAndNonNewable() {}

	static void* operator new(size_t);
	static void* operator new(size_t, const char*, int);
	static void* operator new(size_t, void* p);
};


template<typename T> class ObjectNameT;

#pragma push_macro("new")
#undef new

// class mp_obj use MemPool to allocate and deallocate memory
class DLLIMPEXP_EWA_BASE mp_obj
{
public:

	static void* operator new(size_t);
	static void operator delete(void*);
	static void* operator new(size_t,const char*,int);
	static void operator delete(void*,const char*,int);
	static void* operator new(size_t,void* p){return p;}
	static void operator delete(void*,void*){}

	static void* operator new[](size_t);
	static void operator delete[](void*);
	static void* operator new[](size_t,const char*,int);
	static void operator delete[](void*,const char*,int);
	static void* operator new[](size_t,void* p){return p;}
	static void operator delete[](void*,void*){}

};

class DLLIMPEXP_EWA_BASE mp_obj_v : public mp_obj
{
public:
	virtual ~mp_obj_v(){}
};

#pragma pop_macro("new")


class DLLIMPEXP_EWA_BASE String;
class DLLIMPEXP_EWA_BASE TimeSpan;
class DLLIMPEXP_EWA_BASE TimePoint;

DLLIMPEXP_EWA_BASE extern const char const_empty_buffer[64];

void DLLIMPEXP_EWA_BASE OnAssertFailure(const char* what,const char* file,long line);
void DLLIMPEXP_EWA_BASE OnNoop();
void DLLIMPEXP_EWA_BASE OnError(const char*);
void DLLIMPEXP_EWA_BASE OnIntegerOverflow();


enum
{
	LOGLEVEL_DEBUG,
	LOGLEVEL_INFO,
	LOGLEVEL_TRACE,
	LOGLEVEL_PRINT,
	LOGLEVEL_PRINTLN,
	LOGLEVEL_PRINTTMP,
	LOGLEVEL_DETAIL,
	LOGLEVEL_MESSAGE,
	LOGLEVEL_COMMAND,
	LOGLEVEL_USER,
	LOGLEVEL_WARNING=20,
	LOGLEVEL_LITE_ERROR,
	LOGLEVEL_ERROR,
	LOGLEVEL_FATAL,
	LOGLEVEL_MAX,
};

template<typename T>
class hash_t;


template<typename T, typename T2>
T variant_cast(const T2& v);

class DLLIMPEXP_EWA_BASE Serializer;

template<typename T>
Serializer& operator &(Serializer& ar,T& val);


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#endif


enum
{
	DIR_XMIN,
	DIR_YMIN,
	DIR_ZMIN,
	DIR_XMAX,
	DIR_YMAX,
	DIR_ZMAX,
	DIR_X,
	DIR_Y,
	DIR_Z,
	AXIS_X=DIR_X,
	AXIS_Y=DIR_Y,
	AXIS_Z=DIR_Z,
	SLICE_XY=DIR_Z,
	SLICE_YZ=DIR_X,
	SLICE_ZX=DIR_Y,
};


template<typename T>
class pl_cast_base;

template<typename T>
class pl_cast;

void debug_trace(const char* msg);

class Version : public NonCopyableAndNonNewable
{
public:


	const char* value;
	const char* sname;
	int major;
	int minor;
	int build;

	static const Version& current();

private:
	Version();
	Version(const Version&);
	~Version();
};


class GMutexLocker
{
public:
	GMutexLocker();
	~GMutexLocker();
};


template<typename K, typename V, typename I>
class indexer_trait;

class DLLIMPEXP_EWA_BASE def_allocator;

template<typename K, typename V, typename A = def_allocator, typename P = indexer_trait<K, V, int> >
class indexer_map;

EW_LEAVE


#ifndef EW_WINDOWS
#include <unistd.h>
#endif

#endif
