#ifndef __H_EW_BASIC_FUNCTOR__
#define __H_EW_BASIC_FUNCTOR__

#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/functor_detail.h"

EW_ENTER

extern DLLIMPEXP_EWA_BASE tl::int2type<1> _1;
extern DLLIMPEXP_EWA_BASE tl::int2type<2> _2;
extern DLLIMPEXP_EWA_BASE tl::int2type<3> _3;
extern DLLIMPEXP_EWA_BASE tl::int2type<4> _4;
extern DLLIMPEXP_EWA_BASE tl::int2type<5> _5;
extern DLLIMPEXP_EWA_BASE tl::int2type<6> _6;


//hbind(function,parameter1,parameter2,...,parametern);
//sample usage:
//double fma(double,double,double);
//auto f0=hbind(fma,1.0,2.0,3.0); f0();
//auto f1=hbind(fma,1.0,_1,3.0); f1(2.0);
//auto f2=hbind(fma,1.0,_2,_1); f2(3.0,2.0);
//auto f3=hbind(fma,_2,_3,_1); f3(3.0,1.0,2.0);
//auto f4=hbind(f2,3.0,2.0); f4();

// Functor f0 to f4 all call fma(1.0,2.0,3.0);

#ifdef EW_C11

template<typename X>
struct normal_param_type
{
	typedef X type;
};

template<typename X>
struct normal_param_type<X&>
{
	typedef X type;
};



template<typename Func, typename... Args>
static inline BindImpl<void(Func, typename normal_param_type<Args>::type...)> hbind(Func func, Args&&... args)
{
	return BindImpl<void(Func, typename normal_param_type<Args>::type...)>(func, std::forward<Args>(args)...);
}

#else

template<typename P0>
static inline BindImpl<void(P0)> hbind(P0 p0)
{
	return BindImpl<void(P0)>(p0);
}

template<typename P0,typename P1>
static inline BindImpl<void(P0,P1)> hbind(P0 p0,P1 p1)
{
	return BindImpl<void(P0,P1)>(p0,p1);
}

template<typename P0,typename P1,typename P2>
static inline BindImpl<void(P0,P1,P2)> hbind(P0 p0,P1 p1,P2 p2)
{
	return BindImpl<void(P0,P1,P2)>(p0,p1,p2);
}

template<typename P0,typename P1,typename P2,typename P3>
static inline BindImpl<void(P0,P1,P2,P3)> hbind(P0 p0,P1 p1,P2 p2,P3 p3)
{
	return BindImpl<void(P0,P1,P2,P3)>(p0,p1,p2,p3);
}

template<typename P0,typename P1,typename P2,typename P3,typename P4>
static inline BindImpl<void(P0,P1,P2,P3,P4)> hbind(P0 p0,P1 p1,P2 p2,P3 p3,P4 p4)
{
	return BindImpl<void(P0,P1,P2,P3,P4)>(p0,p1,p2,p3,p4);
}

template<typename P0,typename P1,typename P2,typename P3,typename P4,typename P5>
static inline BindImpl<void(P0,P1,P2,P3,P4,P5)> hbind(P0 p0,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
{
	return BindImpl<void(P0,P1,P2,P3,P4,P5)>(p0,p1,p2,p3,p4,p5);
}

template<typename P0,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
static inline BindImpl<void(P0,P1,P2,P3,P4,P5,P6)> hbind(P0 p0,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
{
	return BindImpl<void(P0,P1,P2,P3,P4,P5,P6)>(p0,p1,p2,p3,p4,p5,p6);
}

template<typename P0,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
static inline BindImpl<void(P0,P1,P2,P3,P4,P5,P6,P7)> hbind(P0 p0,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
{
	return BindImpl<void(P0,P1,P2,P3,P4,P5,P6,P7)>(p0,p1,p2,p3,p4,p5,p6,p7);
}

#endif


template<typename T>
class Functor;

#ifdef EW_C11

	#define DEFINE_FUNCTOR_BASIC_FUNCTIONS()	\
		Functor() {}\
		Functor(const Functor& o):invoker(o.invoker) {}\
		Functor& operator=(const Functor& o){invoker = o.invoker;return *this;}\
		void reset(Invoker* p){invoker.reset(p);}\
		void clear(){invoker.reset(NULL);}\
		inline operator bool() const{return invoker.get() != NULL;}
#else

	#define DEFINE_FUNCTOR_BASIC_FUNCTIONS()	\
		Functor() {}\
		Functor(const Functor& o):invoker(o.invoker) {} \
		Functor& operator=(const Functor& o){invoker = o.invoker;return *this;}\
		void reset(Invoker* p) { invoker.reset(p); }\
		void clear() { invoker.reset(NULL); }\
		inline operator bool() const { return invoker.get() != NULL; }

#endif





template<typename T>
class Functor<T()>
{
public:

	class Invoker: public ObjectData
	{
		const Invoker& operator=(const Invoker&);
	public:
		virtual T Invoke()=0;
	};


	DEFINE_FUNCTOR_BASIC_FUNCTIONS();

	template<typename G>
	Functor(EW_RVALUE(G) o)
	{
		*this=EW_FORWARD(G,o);
	}


	template<typename G>
	typename tl::disable_if_c<tl::is_same_type<typename tl::type_trait<G>::base_type,Functor>,Functor&>::type operator=(EW_RVALUE(G) o)
	{
		class KInvoker : public Invoker
		{
		public:
			G impl;
			KInvoker(EW_RVALUE(G) o):impl(EW_FORWARD(G,o)) {}
			virtual T Invoke()
			{
				return impl();
			}
		};
		invoker.reset(new KInvoker(EW_FORWARD(G,o)));
		return *this;
	}	
	
	inline T operator()()
	{
		if(!invoker) Exception::XBadFunctor();
		return invoker->Invoke();
	}

private:
	DataPtrT<Invoker> invoker;
};

template<typename T,typename X>
class Functor<T(X)>
{
public:

	class Invoker: public ObjectData
	{
		const Invoker& operator=(const Invoker&);
	public:
		virtual T Invoke(EW_ARG(X))=0;
	};

	DEFINE_FUNCTOR_BASIC_FUNCTIONS();

	template<typename G>
	Functor(EW_RVALUE(G) o)
	{
		*this=EW_FORWARD(G, o);
	}

	template<typename G>
	typename tl::disable_if_c<tl::is_same_type<typename tl::type_trait<G>::base_type, Functor>, Functor&>::type operator=(EW_RVALUE(G) o)
	{
		class KInvoker : public Invoker
		{
		public:
			G impl;
			KInvoker(EW_RVALUE(G) o):impl(EW_FORWARD(G, o)) {}
			virtual T Invoke(EW_ARG(X) x)
			{
				return impl(EW_FORWARD(X,x));
			}
		};
		invoker.reset(new KInvoker(EW_FORWARD(G, o)));
		return *this;
	}

	inline T operator()(EW_ARG(X) x)
	{
		if(!invoker) Exception::XBadFunctor();
		return invoker->Invoke(EW_FORWARD(X,x));
	}


private:
	DataPtrT<Invoker> invoker;
};


template<typename T,typename X,typename Y>
class Functor<T(X,Y)>
{
public:

	class Invoker: public ObjectData
	{
		const Invoker& operator=(const Invoker&);
	public:
		virtual T Invoke(EW_ARG(X),EW_ARG(Y))=0;
	};

	DEFINE_FUNCTOR_BASIC_FUNCTIONS();

	template<typename G>
	Functor(EW_RVALUE(G) o)
	{
		*this = EW_FORWARD(G, o);
	}

	template<typename G>
	typename tl::disable_if_c<tl::is_same_type<typename tl::type_trait<G>::base_type, Functor>, Functor&>::type operator=(EW_RVALUE(G) o)
	{
		class KInvoker : public Invoker
		{
		public:
			G impl;
			KInvoker(EW_RVALUE(G) o):impl(EW_FORWARD(G, o)) {}
			virtual T Invoke(EW_ARG(X) x,EW_ARG(Y) y)
			{
				return impl(EW_FORWARD(X,x),EW_FORWARD(Y,y));
			}
		};
		invoker.reset(new KInvoker(EW_FORWARD(G, o)));
		return *this;
	}

	inline T operator()(EW_ARG(X) x, EW_ARG(Y) y)
	{
		if(!invoker) Exception::XBadFunctor();
		return invoker->Invoke(EW_FORWARD(X,x), EW_FORWARD(Y,y));
	}

private:
	DataPtrT<Invoker> invoker;
};


template<typename T,typename X,typename Y,typename Z>
class Functor<T(X,Y,Z)>
{
public:

	class Invoker: public ObjectData
	{
		const Invoker& operator=(const Invoker&);
	public:
		virtual T Invoke(EW_ARG(X),EW_ARG(Y),EW_ARG(Z))=0;
	};

	DEFINE_FUNCTOR_BASIC_FUNCTIONS();

	template<typename G>
	Functor(EW_RVALUE(G) o)
	{
		*this = EW_FORWARD(G, o);
	}

	template<typename G>
	typename tl::disable_if_c<tl::is_same_type<typename tl::type_trait<G>::base_type, Functor>, Functor&>::type operator=(EW_RVALUE(G) o)
	{
		class KInvoker : public Invoker
		{
		public:
			G impl;
			KInvoker(EW_RVALUE(G) o):impl(EW_FORWARD(G, o)) {}
			virtual T Invoke(EW_ARG(X) x, EW_ARG(Y) y, EW_ARG(Z) z)
			{
				return impl(EW_FORWARD(X,x),EW_FORWARD(Y,y),EW_FORWARD(Z,z));
			}
		};
		invoker.reset(new KInvoker(EW_FORWARD(G, o)));
		return *this;
	}

	inline T operator()(EW_ARG(X) x, EW_ARG(Y) y, EW_ARG(Z) z)
	{
		if(!invoker) Exception::XBadFunctor();
		return invoker->Invoke(EW_FORWARD(X, x), EW_FORWARD(Y, y), EW_FORWARD(Z, z));
	}


private:
	DataPtrT<Invoker> invoker;
};


EW_LEAVE

#endif
