#ifndef __H_EW_MATH_RANDOM__
#define __H_EW_MATH_RANDOM__

#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE rand_gen
{
public:

	rand_gen();

	uint32_t random_int();
	uint32_t random_int(uint32_t v);

	uint64_t random_int64();
	uint64_t random_int64(uint64_t v);

	int32_t random_int_range(int32_t v1,int32_t v2);
	int64_t random_int64_range(int64_t v1,int64_t v2);


	void fill_memory(uint8_t* ptr, size_t num);
	void fill_memory(uint32_t* ptr, size_t num);
	void fill_memory(uint64_t* ptr, size_t num);

	void fill_memory(int32_t* ptr, size_t num){ fill_memory((uint32_t*)ptr,num); }
	void fill_memory(int64_t* ptr, size_t num){ fill_memory((uint64_t*)ptr,num); }
	void fill_memory(char* ptr, size_t num){ fill_memory((uint8_t*)ptr,num); }


	float random_float();
	double random_double();
	double random_double_range(double lo, double hi);

	float random_float_0_to_1();
	double random_double_0_to_1();

	double gauss(double x0=0.0, double dx=1.0);

	uint32_t operator()();

	void seed();
	void seed(double v);
	void seed(uint32_t v);
	void seed(uint32_t* p,int n);

	void save_state();
	void load_state();
	void load_state(uint32_t i,uint32_t j=0,uint32_t k=0);

	static rand_gen& current();

protected:
	DataPtrT<ObjectData> m_pData1,m_pData2;
};

EW_LEAVE
#endif
