#ifndef __H_EW_STREAM__
#define __H_EW_STREAM__

#include "ewa_base/config.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/misc.h"
#include "ewa_base/basic/file.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE File;
class DLLIMPEXP_EWA_BASE Socket;
class DLLIMPEXP_EWA_BASE Stream;

typedef int64_t stream_size_type;

class DLLIMPEXP_EWA_BASE IStreamData : public ObjectData
{
public:


	virtual stream_size_type send(const char* p,size_t n);
	virtual stream_size_type recv(char* p,size_t n);
	virtual stream_size_type peek(char* p, size_t n);

	virtual int64_t seekg(int64_t p,int t);
	virtual int64_t tellg();
	virtual int64_t sizeg();

	virtual int64_t seekp(int64_t p,int t);
	virtual int64_t tellp();
	virtual int64_t sizep();

	virtual void close();

	virtual void flush();

	virtual bool send_all(const char* p,size_t n);
	virtual bool recv_all(char* p,size_t n);

	virtual bool on_sync_send(bool b);

	bool getline(String& line,int flag=0);

	BitFlags flags;

protected:

	StringBuffer buffer;
	virtual bool do_getline(String& line);
};



class DLLIMPEXP_EWA_BASE IStreamData2 : public IStreamData
{
public:

	virtual int64_t seek(int64_t p,int t);
	virtual int64_t tell();
	virtual int64_t size();

	virtual int64_t seekg(int64_t p,int t){return seek(p,t);}
	virtual int64_t tellg(){return tell();}
	virtual int64_t sizeg(){return size();}

	virtual int64_t seekp(int64_t p,int t){return seek(p,t);}
	virtual int64_t tellp(){return tell();}
	virtual int64_t sizep(){return size();}
};


class DLLIMPEXP_EWA_BASE IStreamFile : public IStreamData2
{
public:

	IStreamFile();
	IStreamFile(File fp);
	IStreamFile(const String& fp, int fg = FLAG_FILE_RD);
	~IStreamFile();

	File file;

	int64_t seek(int64_t p, int t);
	int64_t tell();
	int64_t size();

	void flush();

	stream_size_type send(const char* data, size_t size);
	stream_size_type recv(char* data, size_t size);

	void close();

};

class DLLIMPEXP_EWA_BASE IStreamMemory : public IStreamData
{
public:

	IStreamMemory(){}

	MemoryBufferT<char> mb;

	stream_size_type send(const char* data, size_t size)
	{
		return mb.send(data, size);
	}

	stream_size_type recv(char* data, size_t size)
	{
		return mb.recv(data, size);
	}

	stream_size_type peek(char* data, size_t size)
	{
		return mb.peek(data, size);
	}

	int64_t sizeg()
	{
		return mb.sizeg();
	}

	void close()
	{
		mb.clear();
		mb.shrink();
	}

};

class DLLIMPEXP_EWA_BASE IStreamDecorator : public ObjectT<IStreamData,IStreamData>
{
public:
	virtual void close();
	virtual void flush();
	virtual int64_t sizeg();
	virtual int64_t sizep();
};

class DLLIMPEXP_EWA_BASE IStreamRanged : public IStreamDecorator
{
public:
	IStreamRanged(IStreamData& d,int64_t p1,int64_t p2);

	virtual stream_size_type send(const char* p, size_t n);
	virtual stream_size_type recv(char* p, size_t n);
	virtual stream_size_type peek(char* p, size_t n);

	virtual int64_t seekg(int64_t p, int t);
	virtual int64_t tellg();
	virtual int64_t sizeg();

	virtual int64_t seekp(int64_t p, int t);
	virtual int64_t tellp();
	virtual int64_t sizep();


	virtual bool send_all(const char* p, size_t n);
	virtual bool recv_all(char* p, size_t n);

protected:
	int64_t pos_cur;
	int64_t pos_end, pos_beg;
};



class DLLIMPEXP_EWA_BASE IStreamDataBuffer : public IStreamDecorator
{
public:
	typedef IStreamDecorator basetype;

	IStreamDataBuffer(File fp);
	IStreamDataBuffer(IStreamData& d);
	~IStreamDataBuffer();

	virtual void on_fini_counter();

	virtual stream_size_type send(const char* p, size_t n);
	virtual stream_size_type recv(char* p, size_t n);
	virtual bool send_all(const char* p, size_t n);
	virtual bool recv_all(char* p, size_t n);


	virtual int64_t seekg(int64_t p, int t);
	virtual int64_t tellg();
	virtual int64_t seekp(int64_t p, int t);
	virtual int64_t tellp();


	void flush();
	void close();

protected:

	MemoryBufferT<char> mb_send,mb_recv;
	arr_1t<char> sb;
	size_t sz;

	virtual bool on_sync_send(bool b=false);
	virtual bool on_sync_recv(size_t n);
};

class DLLIMPEXP_EWA_BASE IStreamDataText : public IStreamDataBuffer
{
public:
	typedef IStreamDataBuffer basetype;

	IStreamDataText(IStreamData& d);

	virtual bool on_sync_recv(size_t n);

	enum
	{
		ENCODE_NONE,
		ENCODE_UTF8,
		ENCODE_UTF16_BE,
		ENCODE_UTF16_LE,
		ENCODE_UTF32_BE,
		ENCODE_UTF32_LE,
		ENCODE_GBK,

	};

protected:

	int encode;
	size_t remain;
	arr_1t<char> cached;


};



class DLLIMPEXP_EWA_BASE Process;

class DLLIMPEXP_EWA_BASE Stream : public FormatHelper<Stream>
{
public:

	Stream();
	Stream(File& file);
	Stream(Socket& socket);
	Stream(DataPtrT<IStreamData> p);
	Stream(DataPtrT<IStreamData> p_reader,DataPtrT<IStreamData> p_writer);

	bool openfile(const String& fp,int fg=FLAG_FILE_RD);
	bool openuri(const String& fp,int fg=FLAG_FILE_RD);
	bool connect(const String& ip,int port);

	bool execute(const String& cmd,Process* p=NULL);

	void assign(File& file);
	void assign(Socket& socket);

	void assign(DataPtrT<IStreamData> p);
	void assign_reader(DataPtrT<IStreamData> p1);
	void assign_writer(DataPtrT<IStreamData> p2);

	int64_t seekg(int64_t p,int t){return hReader->seekg(p,t);}
	int64_t tellg(){return hReader->tellg();}
	int64_t sizeg(){return hReader->sizeg();}

	stream_size_type peek(char* buf, size_t len){return hReader->peek(buf, len);}

	int64_t seekp(int64_t p,int t){return hWriter->seekp(p,t);}
	int64_t tellp(){return hWriter->tellp();}
	int64_t sizep(){return hWriter->sizep();}

	stream_size_type send(const char* buf,size_t len){return hWriter->send(buf,len);}
	stream_size_type recv(char* buf,size_t len){return hReader->recv(buf,len);}

	bool send_all(const char* buf,size_t len){return hWriter->send_all(buf,len);}
	bool recv_all(char* buf,size_t len){return hReader->recv_all(buf,len);}

	DataPtrT<IStreamData> get_reader();
	DataPtrT<IStreamData> get_writer();

	void close();

	bool write_to_file(const String& fp,int flag=0);
	bool write_to_buffer(StringBuffer& sb,int type=FILE_TYPE_BINARY);
	bool write_to_stream(Stream& stream);
	bool write_to_writer(DataPtrT<IStreamData> wr);

	bool read_from_file(const String& fp);
	bool read_from_buffer(StringBuffer& sb);
	bool read_from_stream(Stream& stream);
	bool read_from_reader(DataPtrT<IStreamData> rd);

	bool reader_ok();
	bool writer_ok();

	void Serialize(SerializerHelper sh);

	void append(const char* buf, size_t len);

	bool getline(String& line, int flag = 0);

protected:

	DataPtrT<IStreamData> hReader;
	DataPtrT<IStreamData> hWriter;
};

template<> class hash_t<Stream>
{
public:
	inline uint32_t operator()(const Stream&){return -1;}
};

inline bool operator==(const Stream& lhs,const Stream& rhs){return &lhs==&rhs;}

DEFINE_OBJECT_NAME(Stream,"stream");

EW_LEAVE

#endif
