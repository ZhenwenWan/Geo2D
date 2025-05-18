#ifndef __H_EW_NET_HTTP__
#define __H_EW_NET_HTTP__


#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/stringbuffer.h"
#include "ewa_base/net/socket.h"
#include "ewa_base/util/strlib.h"
#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/util/json.h"


EW_ENTER


class Cookie
{
public:
	String name;
	String value;
	String expire;
	String path;
	String domain;
};


class HttpCookieData : public ObjectData
{
public:
	//String host;
	//int port;
	indexer_map<String,Cookie> data;
	AtomicSpin spin;
};

class DLLIMPEXP_EWA_BASE HttpBase : public ObjectData
{
public:

	int type;
	String head;

	indexer_map<String,String> prop;
	DataPtrT<HttpCookieData> pcookie;

	StringBuffer data;

	HttpBase();

	virtual void clear();

	virtual bool parse(const StringBuffer& sb);
};


class DLLIMPEXP_EWA_BASE HttpResponse : public HttpBase
{
public:
	HttpResponse();
	bool parse(const StringBuffer& sb);
};


class DLLIMPEXP_EWA_BASE HttpRequest : public HttpBase
{
public:
	enum
	{
		HTTP_GET,
		HTTP_POST,
	};

	int port;
	String uri;
	HttpRequest();
	HttpRequest(const String& url);

	bool SetUrl(const String& url);

	void clear();

	indexer_map<String, String> postdata;

	DataPtrT<HttpResponse> Execute();
};

class HttpClientContext : public CallableTableEx
{
public:

	DataPtrT<HttpCookieData> GetCookie(const String& hostport);

	DataPtrT<HttpCookieData> GetCookie(const String& host, int port);

	bst_map<String, DataPtrT<HttpCookieData> > cookies;
	HttpClientContext();

	AtomicSpin spin;
	int m_nTimeout;
};


class HttpClient : public Object
{
public:

	HttpClient(HttpClientContext* p = NULL);


	bool Get(StringBuffer& sb, const String& url)
	{
		return _DoHandle<VariantTable,0>(sb, url, NULL);
	}


	bool Post(StringBuffer& sb, const String& url, const VariantTable& data)
	{
		return _DoHandle<VariantTable, 1>(sb, url, &data);
	}

	bool Post(StringBuffer& sb, const String& url, const indexer_map<String,String>& data)
	{
		return _DoHandle<indexer_map<String, String>, 1>(sb, url, &data);
	}

	bool Post(StringBuffer& sb, const String& url, const bst_map<String,String>& data)
	{
		return _DoHandle<bst_map<String, String>, 1>(sb, url, &data);
	}

	bool Post(StringBuffer& sb, const String& url)
	{
		return _DoHandle<VariantTable, 1>(sb, url, NULL);
	}

	template<typename X>
	bool Post(StringBuffer& sb, const String& url, const X* pdata)
	{
		return _DoHandle<X,1>(sb, url, pdata);
	}

	template<typename X>
	bool Post(VariantTable& result, const String& url, const X& data)
	{
		StringBuffer sb;
		if (!Post(sb, url, data))
		{
			return false;
		}

		Variant var = parse_json(sb);
		if (var.is_nil())
		{
			return false;
		}

		result.swap(var.ref<VariantTable>());
		return true;
	}

protected:
	DataPtrT<HttpClientContext> m_refData;

	template<typename C,int M>
	bool _DoHandle(StringBuffer& sb, const String& url, const C* pdata)
	{

		DataPtrT<HttpRequest> req(new HttpRequest);
		if (!req->SetUrl(url))
		{
			return false;
		}

		req->pcookie = m_refData->GetCookie(String::Format("%s:%d", req->prop["Host"], req->port));;

		if (pdata)
		{
			req->type = HttpRequest::HTTP_POST;
			for (auto it = pdata->begin(); it != pdata->end(); ++it)
			{
				req->postdata[(*it).first] = pl_cast<String>::g((*it).second);
			}
		}
		else if (M == 1)
		{
			req->type = HttpRequest::HTTP_POST;
		}

		DataPtrT<HttpResponse> res = req->Execute();
		if (!res)
		{
			return false;
		}

		sb.swap(res->data);
		return true;
	}


};

EW_LEAVE
#endif
