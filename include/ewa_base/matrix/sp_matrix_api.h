#ifndef __EWA_MATRIX_SP_MATRIX_API_H__
#define __EWA_MATRIX_SP_MATRIX_API_H__

#include "ewa_base/math/math_op.h"
#include "ewa_base/math/tiny_cpx.h"

extern "C"
{
    typedef int umfpack_int;
    typedef ew::dcomplex umfpack_cpx;
}

EW_ENTER

class compact_matrix_dll;

class compact_matrix_api
{
public:
	compact_matrix_api();

    static bool ok();

    void dnaupd(
        int& IDO,
        const char* BMAT,
        int N,
        const char* WHICH,
        int NEV,
        double TOL,
        double* RESID,
        int NCV,
        double* V,
        int LDV,
        int* IPARAM,
        int* IPNTR,
        double* WORKD,
        double* WORKL,
        int LWORKL,
        int& INFO
        );


    void dneupd(
        int RVEC,
        const char* HOWMNY,  // A or P or S
        int* SELECT, // ogical array of dimension NCV
        double* DR, // Double precision array of dimension NEV+1.  (OUTPUT)
        double* DI,  // Double precision array of dimension NEV+1.  (OUTPUT)
        double* Z, // output 
        int LDZ, // Integer.  (INPUT)
        double SIGMAR, //  Double precision  (INPUT)
        double SIGMAI, // Double precision  (INPUT)
        double* WORKEV, // Double precision work array of dimension 3*NCV.  (WORKSPACE)
        const char* BMAT,
        int N,
        const char* WHICH,
        int NEV,
        double TOL,
        double* RESID,
        int NCV,
        double* V, // Double precision N by NCV array.  (INPUT/OUTPUT)
        int LDV,
        int* IPARAM,
        int* IPNTR,
        double* WORKD,
        double* WORKL, // Double precision work array of length LWORKL.  (OUTPUT/WORKSPACE)
        int LWORKL,
        int& INFO
    );


    void znaupd(
        int& IDO,
        const char* BMAT,
        int N,
        const char* WHICH,
        int NEV,
        double TOL,
        umfpack_cpx* RESID,
        int NCV,
        umfpack_cpx* V,
        int LDV,
        int* IPARAM,
        int* IPNTR,
        umfpack_cpx* WORKD,
        umfpack_cpx* WORKL,
        int LWORKL,
        umfpack_cpx* RWORK,
        int& INFO
    );

    void zneupd(
        int RVEC,
        const char* HOWMNY,
        int* SELECT,
        umfpack_cpx* D,
        umfpack_cpx* Z,
        int LDZ,
        umfpack_cpx SIGMA,
        umfpack_cpx* WORKEV,
        const char* BMAT,
        int N,
        const char* WHICH,
        int NEV,
        double TOL,
        umfpack_cpx* RESID,
        int NCV,
        umfpack_cpx* V,
        int LDV,
        int* IPARAM,
        int* IPNTR,
        umfpack_cpx* WORKD,
        umfpack_cpx* WORKL,
        int LWORKL,
        umfpack_cpx* RWORK,
        int& INFO
    );


protected:
	compact_matrix_dll& _impl;
};

EW_LEAVE

#endif
