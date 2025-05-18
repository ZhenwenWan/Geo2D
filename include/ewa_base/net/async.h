
#ifndef __H_EW_NET_ASYNC__
#define __H_EW_NET_ASYNC__

#include "ewa_base/basic/platform.h"
#include "ewa_base/net/session.h"
#include "ewa_base/threading/thread.h"
#include "ewa_base/threading/thread_event.h"
#include "ewa_base/threading/thread_spin.h"
#include "ewa_base/logging/logger.h"
#include "ewa_base/basic/lockguard.h"
#include "ewa_base/scripting/callable_data.h"
#include "ewa_base/collection.h"


EW_ENTER

typedef arr_1t<DataPtrT<Session> > SessionArray;

class DLLIMPEXP_EWA_BASE AsyncCommand : public ObjectData
{
public:
	virtual void Handle(SessionArray& akey)=0;
};

class  DLLIMPEXP_EWA_BASE AsyncObject : public CallableData
{
public:
	virtual void idle_check(){}

};

class DLLIMPEXP_EWA_BASE AsyncPool : public ThreadEx, private NonCopyable
{
public:
	typedef ThreadEx basetype;

	Logger logger;

	bool activate(int nWorker_ = 1);

	AsyncPool(const String& name_ = "iocp_server", int maxconn_ = 1024 * 4);
	~AsyncPool();

	void cancel();
	void wait_for_all_session_exit();
	bool wait_for_all_session_exit(const TimeSpan& ts);

	void DisconnectAll(bool c=false);

	KO_Policy_handle::type native_handle();

	int Register(Session* session);
	bool Execute(AsyncCommand* cmd);

	const IOCPAccounter& GetAccounter() const;

	void SetName(const String& s);

	const String& GetName() const;

	void AttachObject(DataPtrT<AsyncObject> pobj);
	void DetachObject(DataPtrT<AsyncObject> pobj);


	class DLLIMPEXP_EWA_BASE Holder
	{
	public:

		void Register(SessionTCP* p);
		void reset(AsyncPool* p);
		AsyncPool* get();

		operator AsyncPool&();
	private:
		void _ensure_instance();
		AutoPtrT<AsyncPool> m_ptr;
	};

	static Holder& current();

protected:

	void ccc_update_info();
	void ccc_handle_sock();
	void ccc_execute_cmd();


#ifdef EW_WINDOWS
	void ccc_handle_iocp(Session* pkey, MyOverLapped* pdat);
#endif

	AutoPtrT<IOCPAccounter> m_pAccounterLast, m_pAccounterTemp;

	IOCPAccounter accounter;

	SessionArray m_aSessions;
	bst_set<DataPtrT<AsyncObject> > m_aObjects;


	LockFreeQueue<int> m_lkfqSessionAvailable;
	LockFreeQueue<DataPtrT<AsyncCommand>, LockFreeQueuePolicyObj<DataPtrT<AsyncCommand> > > m_lkfqCommand;
	int m_nSessionMax;

	KO_Handle<KO_Policy_handle> hIOCPhandler;

	void svc_del(int n);

	void svc_worker();
	void svc_checker();

	Event m_nCanClose;
	AtomicInt32 m_bAcceptConnection;

	String m_sName;

#ifndef EW_WINDOWS
	void HandleSend(Session& ikey);
	void HandleRecv(Session& ikey);
#endif

};



EW_LEAVE
#endif
