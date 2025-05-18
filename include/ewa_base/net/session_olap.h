#ifndef __H_EW_IOCP_SESSION_OLAP__
#define __H_EW_IOCP_SESSION_OLAP__

#include "ewa_base/net/session.h"
#include "ewa_base/scripting/variant.h"
#include "ewa_base/serialization/serializer.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE IOlapLink : public NonCopyable
{
public:

	~IOlapLink();

	void putq(MyOverLappedEx* q);
	TempOlapPtr getq();

	bool empty(){ return !phead; }

	LitePtrT<MyOverLappedEx> phead, ptail;
};


class DLLIMPEXP_EWA_BASE IStreamDataOlapWriter : public IStreamData
{
public:

	IStreamDataOlapWriter(){}

	virtual stream_size_type send(const char* p, size_t n);

	void sync();

	TempOlapPtr getq(){ return qlink.getq(); }

	IOlapLink qlink;
	TempOlapPtr q;
	int sz;

};




class DLLIMPEXP_EWA_BASE IStreamDataOlapReader : public IStreamData
{
public:

	IStreamDataOlapReader(){}

	stream_size_type recv(char* p, size_t n);

	virtual bool sync();

	bool putq(TempOlapPtr q);

	class Position
	{
	public:

		Position(){ n_buf = 0; n_seq = -1; }

		LitePtrT<MyOverLappedEx> g_ptr;
		int n_buf;
		int n_seq;
		int n_pos;

		bool update_seq();
		bool update();
	};

	IOlapLink qlink;
	class Position pos_rd, pos_wr;
};


class DLLIMPEXP_EWA_BASE SessionTCP_OlapHandler : public SessionTCP
{
public:
	DataPtrT<IStreamDataOlapReader> olap_rd;
	DataPtrT<IStreamDataOlapWriter> olap_wr;
	SerializerReader reader;
	SerializerWriter writer;

	uint32_t seq;

	class DLLIMPEXP_EWA_BASE RpcParamData
	{
	public:

		enum
		{
			TYPE_NONE		=0,
			TYPE_REQUEST	=1,
			TYPE_RESPONSE	=2,
		};

		RpcParamData(uint32_t t=TYPE_NONE) :type(t){}

		uint32_t type;
		uint32_t seq;
		String cmd;
		Variant var;

		void Serialize(Serializer& ar)
		{
			ar & type & seq & cmd & var;
		}
	};

	SessionTCP_OlapHandler();

	virtual bool OnReaderSync();
	virtual bool OnWriterSend(uint32_t seq,const Variant& var);

	void OnConnected();
	void OnDisconnected();

	virtual void OnRecvCompleted(TempOlapPtr& q);

};

EW_LEAVE

#endif
