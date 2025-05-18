#ifndef __H_EW_IOCP_SESSION_HTTP__
#define __H_EW_IOCP_SESSION_HTTP__

#include "ewa_base/net/session.h"
#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/basic/file.h"
#include "ewa_base/net/async.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE SessionManager;
class DLLIMPEXP_EWA_BASE SessionHttp;

class DLLIMPEXP_EWA_BASE SessionData : public CallableTableEx
{
public:

	String id;
	TimePoint tplast;
	AtomicSpin spin;
	BitFlags flags;

	SessionData();

	enum
	{
		FLAG_ABONDON=1<<0,
	};


	virtual CallableData* DoClone(ObjectCloneState& cs)
	{
		if(cs.type==0) return this;
		return new SessionData(*this);
	}

	bool is_timeout(const SessionManager& sm);
	void abondon(){flags.add(FLAG_ABONDON);}
	void touch(){tplast=Clock::now();}

};

class DLLIMPEXP_EWA_BASE HttpHandler : public ObjectData
{
public:

	static void HandleFile(SessionHttp& h);
	static void HandleGzip(SessionHttp& h);
	static void HandleStream(SessionHttp& h);
	static void Handle_404(SessionHttp& h);
	static void Handle_500(SessionHttp& h);

	virtual void Handle(SessionHttp& h)=0;
};


class DLLIMPEXP_EWA_BASE SessionManager : public ObjectData
{
public:
	typedef DataPtrT<SessionData> session_ptr;
	typedef bst_map<String,session_ptr> session_maptype;

	typedef DataPtrT<HttpHandler> handler_type;
	typedef indexer_map<String,handler_type> handler_maptype;


	SessionManager();

	virtual String NewSessionId();

	session_ptr GetSession(String& session_id);

	void HandleRequest(SessionHttp&);

	SessionData* CreateSessionData();

	virtual void EraseTimeoutSessions();


	TimePoint tp_now;
	

	String server_path;

	DataPtrT<CallableTableEx> server_objects;

	handler_maptype handler_map;
	session_maptype session_map;



	AtomicSpin spin;

	uint32_t maxage;

	uint32_t maxmem;

	uint32_t maxtime;
	

	BitFlags flags;

	enum
	{
		FLAG_SEND_ERROR	=1<<0,
		FLAG_ENABLE_GZIP =1<<1,
	};

	Variant fn_handler;

	void SetHandler(const String& exts,const String& content_type,int flag=0);

protected:

	void _InitHandlers();

	//handler_maptype handlers;

};

class DLLIMPEXP_EWA_BASE MultiPartFormData : public ObjectData
{
public:

	MultiPartFormData(SessionHttp& t,const String& s);

	void HandleData(TempOlapPtr& q);

	SessionHttp& Target;

	String name;
	String type;
	String filename;
	String boundary;

	int length_max;
	int length_cur;
	int length_tag;

	int phase;

	File file;

	const char* _find_boundary(const char* p1,const char* p2);

	void _handle_phase1_line(const char* p1);

	void _handle_phase1();
	void _handle_phase2();
	void _handle_phase3();


	typedef char* mychar_ptr;
	mychar_ptr pbeg;
	mychar_ptr pend;
	mychar_ptr pcur;
};


class DLLIMPEXP_EWA_BASE SessionHttp : public SessionClient
{
public:

	enum
	{
		FLAG_REQUEST_CHUNKED	=1<<0,
		FLAG_RESPONSE_CHUNKED	=1<<1,
		FLAG_ACCEPT_ENCODING_GZIP		=1<<2,
	};

	StringBuffer sb;

	String uri;
	String method;
	String anchor;
	String filepath;
	String extraheader;
	String headline;

	int64_t length;

	uint32_t phase;
	BitFlags flags;

	DataPtrT<MultiPartFormData> multipart_formdata;

	typedef indexer_map<String,String> map_type;

	map_type props;
	map_type cookie;

	VariantTable query;

	uint32_t httpstatus;
	uint32_t maxage;

	SessionManager& Target;

	SessionHttp(SessionManager& t);

	Stream chunked_stream;

	void HandleQuery(const char* p);
	void HandleRequest(int t);

	virtual void OnSendCompleted(TempOlapPtr& q);
	virtual void OnRecvCompleted(TempOlapPtr& q);

	void OnConnected();

	void Redirect(const String& url);

protected:

	void _HandleData(TempOlapPtr& q,size_t s1);
	void _ParseRequestHeaders();

	arr_1t<int> lines;
};


class  DLLIMPEXP_EWA_BASE SessionHttpServer : public SessionServer
{
public:

	DataPtrT<SessionManager> Target;

	SessionHttpServer(SessionManager* t=NULL);


	void NewSession(PerIO_socket& sk);
	void Register(const String& name,const Variant& object);

	void SetPath(const String& path);
	void SetMaxAge(int second);


};


class  DLLIMPEXP_EWA_BASE SessionHttpSSLServer : public SessionHttpServer
{
public:

	SessionHttpSSLServer(SessionManager* t = NULL);

	bool InitSSL(const String& cert,const String& key);

	void NewSession(PerIO_socket& sk);

	void* GetSSLContext();

protected:
	AutoPtrT<Object> m_pSSLContext;
};


EW_LEAVE
#endif
