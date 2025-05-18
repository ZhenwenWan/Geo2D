#ifndef __EW_SCRIPTING_VARIANT_THREAD_H__
#define __EW_SCRIPTING_VARIANT_THREAD_H__

#include "ewa_base/scripting/variant.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/threading/thread.h"
#include "ewa_base/logging/logtarget.h"
#include "ewa_base/basic/process.h"
#include "ewa_base/ipc/shm.h"
#include "ewa_base/scripting/executor.h"

EW_ENTER


class ThreadStatus : public LogCache
{
public:

	typedef LogCache basetype;

	ThreadStatus();

	void SetProgressStep(double val);
	void SetProgressTotal(double val,double p2);

	virtual void Handle(const LogRecord& o);

	void HandleLine(const String& msg);

	virtual int __getindex(Executor& ks, const String& si);

	virtual void DoNotify(int) {}

	virtual void HandleProgress(const LogRecord& o);

	void ResetProgress();

	double progress_step;
	double progress_finished;
	double progress_progressing;

protected:
	int __getindex_no_lock(Executor& ks, const String& si);

	double _get_progress_all();
};


class RunnerStatus : public ThreadStatus
{
public:

	typedef ThreadStatus basetype;

	DataPtrT<CallableData> p_notifier;
	enum
	{
		STATUS_READY	= 0,
		STATUS_STARTING = 1,
		STATUS_STARTED	= 2,
		STATUS_RUNNING	= 3,
		STATUS_FINISHED = 4,
	};

	int status;
	int retval;

	RunnerStatus();

	void SetStatus(int val);
	void SetNotifier(DataPtrT<CallableData> p);

	virtual int __getindex(Executor&, const String&);

	DECLARE_OBJECT_INFO(RunnerStatus, ObjectInfo);

protected:
	Executor _lexer;

	void DoNotify(int type);

};



class ProcessRunner : public Thread
{
public:

	typedef Thread basetype;

	DataPtrT<RunnerStatus> pstatus;

	String solver;
	String cmdline;
	Stream stream;
	Logger logger;

	ProcessRunner(RunnerStatus* p=NULL);

	void detach();

	bool activate();

	bool stop();

	void finish();

	int get_pid();

	void cancel();

	bool is_stoping();


protected:


	Process proc;
	//ShmIPC ipc;

	void _handle_line(char* p1);

	char* last_cr;

	void _handle_msg(char* &p0, char* &p1);

	void svc();


	int n_exitcode;
	bool _is_stoping;
};


class CallableThread : public CallableData
{
public:

	typedef CallableData basetype;

	CallableThread();
	~CallableThread();

	virtual int __getindex(Executor&, const String&);

	virtual CallableMetatable* GetMetaTable();

	virtual CallableData* DoClone(ObjectCloneState&);
	virtual CallableData* DoCloneObject();

	AutoPtrT<Thread> pthread;
	BitFlags flags;

};




EW_LEAVE
#endif

