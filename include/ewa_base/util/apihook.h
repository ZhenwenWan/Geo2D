#ifndef __H_EW_APIHOOK__
#define __H_EW_APIHOOK__

#include "ewa_base/config.h"

EW_ENTER




class DLLIMPEXP_EWA_BASE ApiManager
{
public:

	template<typename T>
	static T hook(T f1, T f2)
	{
		return (T)api_hook(f1, f2);
	}

	template<typename T>
	static T hook(T f1)
	{
		return (T)api_hook(f1, NULL);
	}

private:
	static void* api_hook(void* f_old, void* f_new);
};




EW_LEAVE
#endif
