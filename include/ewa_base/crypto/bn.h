#ifndef __H_EW_CRYPTO_BN__
#define __H_EW_CRYPTO_BN__


#include "ewa_base/basic/object.h"
#include "ewa_base/basic/stream.h"

EW_ENTER



class DLLIMPEXP_EWA_BASE BigNumber
{
public:
	class internal_data;

	BigNumber();
	BigNumber(const BigNumber&);
	~BigNumber();

	BigNumber& operator=(const BigNumber&);
	BigNumber& operator=(uint64_t);

	String get_hex() const;
	bool set_hex(const String&);


	operator internal_data*() const { return pimpl; }

protected:
	internal_data* pimpl;
};

DLLIMPEXP_EWA_BASE BigNumber operator+(const BigNumber& lhs, const BigNumber& rhs);
DLLIMPEXP_EWA_BASE BigNumber operator-(const BigNumber& lhs, const BigNumber& rhs);
DLLIMPEXP_EWA_BASE BigNumber operator*(const BigNumber& lhs, const BigNumber& rhs);
DLLIMPEXP_EWA_BASE BigNumber operator/(const BigNumber& lhs, const BigNumber& rhs);
DLLIMPEXP_EWA_BASE BigNumber operator%(const BigNumber& lhs, const BigNumber& rhs);
DLLIMPEXP_EWA_BASE BigNumber mod_exp(const BigNumber& a, const BigNumber& p, const BigNumber& m);

EW_LEAVE

#endif
