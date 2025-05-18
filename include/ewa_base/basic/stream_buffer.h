
#ifndef __H_EW_STREAM_BUFFER__
#define __H_EW_STREAM_BUFFER__

#include "ewa_base/basic/stream.h"
#include "ewa_base/collection/linear_buffer.h"


EW_ENTER


class DLLIMPEXP_EWA_BASE IStreamBuffer : public IStreamData
{
public:

	IStreamBuffer();

	// skip data, return true if gptr()==gend();
	bool skip();

	// assign external buffer
	void assign(char* pbuf,size_t size);

	// allocate buffer
	void alloc(size_t bufsize);

	void setbuf(char* pbuf,size_t size)
	{
		lbuf.assign(pbuf,size);
		lbuf.wr_flip(size);
	}


	char* gbeg()
	{
		return lbuf.gbeg();   // buffer begin
	}

	char* gptr()
	{
		return lbuf.gptr();   // get position begin
	}

	char* gend()
	{
		return lbuf.gend();   // get position end or put position begin
	}

	char* last()
	{
		return lbuf.last();
	}

	stream_size_type send(const char* data,size_t size);
	stream_size_type recv(char* data,size_t size);

protected:
	
	LinearBufferEx<char> lbuf;

};

class IMemoryBuffer : public  IStreamData
{
public:
	MemoryBufferT < char, BufferPolicyChained<char, 2048> > lbuf;

	stream_size_type send(const char* data, size_t size){ return lbuf.send(data, size); }
	stream_size_type recv(char* data, size_t size){ return lbuf.recv(data, size); }

};


EW_LEAVE
#endif

