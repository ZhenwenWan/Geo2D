
#ifndef __H_EW_SCRIPTING_EXECUTOR_PROXY__
#define __H_EW_SCRIPTING_EXECUTOR_PROXY__


#include "ewa_base/scripting/executor.h"
#include "ewa_base/logging/logtarget.h"
#include "ewa_base/basic/stream_buffer.h"
#include "ewa_base/net/session_olap.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE RpcHandler : public CallableObject
{
public:

	virtual int __getindex(Executor& ks,const String& cmd);

	Variant Handle(const String& cmd,const Variant& var=Variant());

	Variant Handle(const String& cmd,const String& sreq)
	{
		return Handle(cmd, Variant(sreq));
	}

	virtual Variant berror(const String& msg);
	virtual Variant bmessage(const String& msg);
	virtual Variant bresult(const Variant& result);
	virtual Variant bunknown(const String& cmd,const Variant&);
	virtual Variant bexception(const String& cmd,std::exception& e);
	virtual Variant bpending(const String msg,const Variant& val);

protected:
	virtual Variant DoHandle(const String& cmd, const Variant& var) = 0;
};

class DLLIMPEXP_EWA_BASE RpcHandlerFunction : public ObjectData
{
public:
	virtual Variant invoke(const Variant&) = 0;
};

class DLLIMPEXP_EWA_BASE RpcHandlerEx : public RpcHandler
{
public:

	virtual Variant DoHandle(const String& cmd,const Variant& req);
	indexer_map<String, DataPtrT<RpcHandlerFunction> > aHandlers;
};

class DLLIMPEXP_EWA_BASE RpcHandlerEwsl : public RpcHandler
{
public:
	RpcHandlerEwsl();
	virtual Variant DoHandle(const String& cmd,const Variant& req);
	Executor lexer;
};



class DLLIMPEXP_EWA_BASE SessionRpcHandler : public SessionTCP_OlapHandler
{
public:
	DataPtrT<RpcHandler> rpc;

	bool OnReaderSync();
	SessionRpcHandler(DataPtrT<RpcHandler> p);

};


EW_LEAVE
#endif
