#ifndef __H_EW_CRYPTO_RSA__
#define __H_EW_CRYPTO_RSA__


#include "ewa_base/basic/object.h"
#include "ewa_base/basic/stream.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE RsaPublic;
class DLLIMPEXP_EWA_BASE RsaPrivate;
class DLLIMPEXP_EWA_BASE BigNumber;


class DLLIMPEXP_EWA_BASE Rsa
{
public:

	class internal_data;

	Rsa();
	Rsa(const Rsa& o);
	Rsa& operator=(const Rsa&);
	~Rsa();

	void generate(int n, unsigned long e = 0x10001);


	int private_encrypt(const char* p, int n, char* out);
	int private_decrypt(const char* p, int n, char* out);

	int public_encrypt(const char* p, int n, char* out);
	int public_decrypt(const char* p, int n, char* out);

	void set_key(const String& n, const String& e, const String& d = "");
	void get_key(String& n, String& e, String& d);

	int get_desired_decrypt_len(){ return n_len; }
	int get_desired_encrypt_len(){ return n_len_enc; }

private:
	void _reset_impl(internal_data* p);

	int n_len;
	int n_len_enc;

	internal_data* pimpl;
};


class DLLIMPEXP_EWA_BASE RsaPublic
{
public:

	void set_key(const String& n, const String& e)
	{
		return impl.set_key(n, e);
	}

	int encrypt(const char* p, int n, char* out)
	{
		return impl.public_encrypt(p, n, out);
	}

	int decrypt(const char* p, int n, char* out)
	{
		return impl.public_decrypt(p, n, out);
	}

	int get_desired_decrypt_len(){ return impl.get_desired_decrypt_len(); }
	int get_desired_encrypt_len(){ return impl.get_desired_encrypt_len(); }

protected:
	Rsa impl;
};


class DLLIMPEXP_EWA_BASE RsaPrivate
{
public:


	void set_key(const String& n, const String& e, const String& d)
	{
		return impl.set_key(n, e, d);
	}

	int encrypt(const char* p, int n, char* out)
	{
		return impl.private_encrypt(p, n, out);
	}

	int decrypt(const char* p, int n, char* out)
	{
		return impl.private_decrypt(p, n, out);
	}

	int get_desired_decrypt_len(){ return impl.get_desired_decrypt_len(); }
	int get_desired_encrypt_len(){ return impl.get_desired_encrypt_len(); }

protected:
	Rsa impl;
};


template<typename T>
class IStreamEncoderT : public IStreamDataBuffer
{
public:
	T impl;
	arr_1t<char> sb2;

	IStreamEncoderT(IStreamData& d) :IStreamDataBuffer(d)
	{
		sb2.resize(sz * 2);
	}

	bool on_sync_send(bool b)
	{
		while (mb_send.sizeg() >= (int)sz)
		{
			mb_send.recv(sb.data(), sz);
			int len=impl.encrypt(sb.data(), sz, sb2.data());
			if (len < 0)
			{
				return false;
			}

			if (!m_refData->send_all(sb2.data(), len))
			{
				return false;
			}
		}
		if (!b)
		{
			return true;
		}

		int v = mb_send.recv(sb.data(), sz);
		if (v == 0) return true;

		int len=impl.encrypt(sb.data(), v, sb2.data());
		if (len < 0)
		{
			return false;
		}

		if (!m_refData->send_all(sb2.data(), len))
		{
			return false;
		}

		return m_refData->on_sync_send(true);
	
	}
};
template<typename T>
class IStreamDecoderT : public IStreamDataBuffer
{
public:
	T impl;
	arr_1t<char> sb2;

	IStreamDecoderT(IStreamData& d) :IStreamDataBuffer(d)
	{
		sb2.resize(sz * 2);
	}


	bool on_sync_recv(size_t n)
	{

		int v = m_refData->recv(sb.data(), sz);

		if (v<0||v%impl.get_desired_decrypt_len() != 0)
		{
			close();
			return false;
		}

		if (v == 0)
		{
			return true;
		}

		int len=impl.decrypt(sb.data(), v, sb2.data());
		if (len < 0)
		{
			close();
			return false;
		}

		mb_recv.send(sb2.data(), len);
		
		return true;
	}


};



EW_LEAVE

#endif
