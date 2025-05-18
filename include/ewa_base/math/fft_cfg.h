#include "ewa_base/math/tiny_cpx.h"

EW_ENTER


DLLIMPEXP_EWA_BASE void fft(dcomplex *dst, const double *src, size_t n, bool inverse = false);
DLLIMPEXP_EWA_BASE void fft(dcomplex *dst,const dcomplex *src,size_t n,bool inverse=false);
DLLIMPEXP_EWA_BASE void fft2(dcomplex *dst, const double*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void fft2(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void fft3(dcomplex *dst, const double*src, size_t n0, size_t n1, size_t n2, bool inverse = false);
DLLIMPEXP_EWA_BASE void fft3(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, size_t n2, bool inverse = false);



DLLIMPEXP_EWA_BASE void fftw_fft(dcomplex *dst, const double *src, size_t n, bool inverse = false);
DLLIMPEXP_EWA_BASE void fftw_fft(dcomplex *dst, const dcomplex *src, size_t n, bool inverse = false);
DLLIMPEXP_EWA_BASE void fftw_fft2(dcomplex *dst, const double*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void fftw_fft2(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void fftw_fft3(dcomplex *dst, const double*src, size_t n0, size_t n1, size_t n2, bool inverse = false);
DLLIMPEXP_EWA_BASE void fftw_fft3(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, size_t n2, bool inverse = false);


DLLIMPEXP_EWA_BASE void wlib_fft(dcomplex *dst, const double *src, size_t n, bool inverse = false);
DLLIMPEXP_EWA_BASE void wlib_fft(dcomplex *dst, const dcomplex *src, size_t n, bool inverse = false);
DLLIMPEXP_EWA_BASE void wlib_fft2(dcomplex *dst, const double*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void wlib_fft2(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, bool inverse = false);
DLLIMPEXP_EWA_BASE void wlib_fft3(dcomplex *dst, const double*src, size_t n0, size_t n1, size_t n2, bool inverse = false);
DLLIMPEXP_EWA_BASE void wlib_fft3(dcomplex *dst, const dcomplex*src, size_t n0, size_t n1, size_t n2, bool inverse = false);

EW_LEAVE

