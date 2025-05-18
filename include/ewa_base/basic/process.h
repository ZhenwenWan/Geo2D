#ifndef __H_EW_PROCESS__
#define __H_EW_PROCESS__

#include "ewa_base/config.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/stream.h"
#include "ewa_base/collection/bst_map.h"

EW_ENTER

class ProcessImpl;

class DLLIMPEXP_EWA_BASE Process : public Object
{
public:

	Process();

	bool Redirect(Stream& s);

	bool Redirect(KO_Handle<KO_Policy_handle> h);

	bool Execute(const String& cmd);

	void SetCwd(const String& cwd);

	void SetTitle(const String& title);

	void SetEnvironment(const bst_map<String, String>& envs);

	void SetWindow(bool flag);

	bool Kill(int r=0);

	void Wait();

	bool WaitFor(int ms);

	void Close();

	bool GetExitCode(int* code=NULL);

	int GetPid();

protected:

	DataPtrT<ObjectData> m_impl;
};


EW_LEAVE

#endif
