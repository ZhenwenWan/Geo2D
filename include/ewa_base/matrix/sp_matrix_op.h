#ifndef __EWA_MATRIX_SP_MATRIX_OP_H__
#define __EWA_MATRIX_SP_MATRIX_OP_H__

#include "ewa_base/matrix/sp_matrix.h"
#include "ewa_base/scripting/callable_data.h"


EW_ENTER



class compact_matrix_op
{
public:

    template<typename T>
    static bool vector_is_zero(const T* p, size_t n)
    {
        return numeric_helper_t<T>::is_zero(p, n);
    }


    template<bool C, typename T, typename X>
    static void _To_real(compact_storage_t<T, X>& out, compact_matrix_t<T, X>& A)
    {

        A.ensure_srows();

        out.SRows.assign(1, 0);

        auto* p0 = &A.Cols[0];
        for (size_t row = 0; row < A.NRow; row++)
        {
            auto* p1 = p0 + A.SRows[row];
            auto* p2 = p0 + A.SRows[row + 1];
            for (auto* p = p1; p < p2; p++)
            {
                size_t col = *p;
                T v1 = A.Value_real[size_t(p - p0)];
                if (v1 != numeric_helper_t<T>::sm_value_0)
                {
                    out.Value_real.push_back(v1);
                    out.Cols.push_back(col * 2);
                }
                else
                {
                    EW_NOOP();
                }

                if (C)
                {
                    T v2 = A.Value_imag[size_t(p - p0)];
                    if (v2 != numeric_helper_t<T>::sm_value_0)
                    {
                        out.Value_real.push_back(-v2);
                        out.Cols.push_back(col * 2 + 1);
                    }
                }

            }
            out.SRows.push_back(out.Value_real.size());
            for (auto* p = p1; p < p2; p++)
            {
                size_t col = *p;
                if (C)
                {
                    T v2 = A.Value_imag[size_t(p - p0)];
                    if (v2 != numeric_helper_t<T>::sm_value_0)
                    {
                        out.Value_real.push_back(v2);
                        out.Cols.push_back(col * 2);
                    }
                }

                T v1 = A.Value_real[size_t(p - p0)];
                if (v1 != numeric_helper_t<T>::sm_value_0)
                {
                    out.Value_real.push_back(v1);
                    out.Cols.push_back(col * 2 + 1);
                }
                else
                {
                    EW_NOOP();
                }
            }
            out.SRows.push_back(out.Value_real.size());
        }
    }


    template<typename T,typename X>
    static void complex_to_real(compact_matrix_t<T,X>& A)
    {
        sp_matrix_holder<T, X> holder;
        auto& B(holder.init(A,A.NRow*2,A.NCol*2));

        if (A.is_complex())
        {
            _To_real<true>(B, A);
        }
        else
        {
            _To_real<false>(B, A);
        }
    }

    template<typename X>
    static void sort_matrix_data(arr_1t<int>& idx, X* rows, X* cols, size_t nval)
    {

        class RowColLesser
        {
        public:
            X* rows;
            X* cols;
            bool operator()(int i, int j)
            {
                if (rows[i] < rows[j]) return true;
                if (rows[i] > rows[j]) return false;
                return cols[i] < cols[j];
            }
        };

        RowColLesser lesser;
        lesser.rows = rows;
        lesser.cols = cols;

        idx.resize(nval);
        for (size_t i = 0; i < nval; i++)
        {
            idx[i] = i;
        }
        std::sort(idx.begin(), idx.end(), lesser);

    }

    template<typename T,typename X>
    static void reorder_data(T* pval, size_t nval, X* pidx, void* buffer)
    {
        if (!pval) return;

        T* ptmp = (T*)buffer;
        for (size_t i = 0; i < nval; i++)
        {
            ptmp[i] = pval[pidx[i]];
        }
        std::copy_n(ptmp, nval, pval);
    }

    template<typename P, bool C1, bool C2, typename M1,typename M3>
    static inline void _Add_elem_g1(M3& out, M1& A, size_t n)
    {
        if (C1 || C2)
        {
            dcomplex val = P::g1(dcomplex(A.Value_real[n], C1 ? A.Value_imag[n] : 0.0));
            out.Value_real.push_back(val.real());
            out.Value_imag.push_back(val.imag());
        }
        else
        {
            out.Value_real.push_back(P::g1(A.Value_real[n]));
        } 
    }

    template<typename P,bool C1, bool C2, typename M1,typename M3>
    static inline void _Add_elem_g2(M3& out, M1& A, size_t n)
    {
        if (C1 || C2)
        {
            dcomplex val = P::g2(dcomplex(A.Value_real[n], C1 ? A.Value_imag[n] : 0.0));
            out.Value_real.push_back(val.real());
            out.Value_real.push_back(val.imag());
        }
        else
        {
            out.Value_real.push_back(P::g2(A.Value_real[n]));
        }
    }


    template<typename P,bool C1, bool C2, typename M1, typename M2,typename M3>
    static inline bool _Add_elem(M3& out, M1& A, size_t na, M2& B, size_t nb)
    {
        if (C1 || C2)
        {
            dcomplex v1 = dcomplex(A.Value_real[na],C1 ? A.Value_imag[na] : 0.0);
            dcomplex v2 = dcomplex(B.Value_real[nb],C2 ? B.Value_imag[nb] : 0.0);
            dcomplex v3 = P::g(v1, v2);
            if (v3 == numeric_helper_t<dcomplex>::sm_value_0)
            {
                return false;
            }
            out.Value_real.push_back(v3.real());
            out.Value_imag.push_back(v3.imag());
        }
        else
        {
            double v3 = P::g(A.Value_real[na], B.Value_real[nb]);
            if (v3 == numeric_helper_t<double>::sm_value_0)
            {
                return false;
            }
            out.Value_real.push_back(v3);
        }

        return true;
    }


    template<typename P, bool C1, bool C2, typename T,typename X>
    static void _Add_matrix(compact_storage_t<T,X>& out, compact_matrix_t<T, X>& A, compact_matrix_t<T, X>& B)
    {

        out.SRows.push_back(0);

        size_t n_val_size = std::max(A.NZero, B.NZero);
        out.Value_real.reserve(n_val_size);
        if (C1 || C2)
        {
            out.Value_imag.reserve(n_val_size);
        }


        auto* a0 = &A.Cols[0];
        auto* b0 = &B.Cols[0];

        for (size_t i = 0; i < out.NRow; i++)
        {
            auto* a1 = a0 + A.SRows[i];
            auto* a2 = a0 + A.SRows[i + 1];
            auto* b1 = b0 + B.SRows[i];
            auto* b2 = b0 + B.SRows[i + 1];

            while (1)
            {
                if (a1 == a2)
                {
                    while (b1 < b2)
                    {
                        _Add_elem_g2<P, C2, C1>(out, B, b1 - b0);
                        out.Cols.push_back(*b1++);
                    }
                    break;
                }

                if (b1 == b2)
                {
                    while (a1 < a2)
                    {
                        _Add_elem_g1<P, C1, C2>(out, A, a1 - a0);
                        out.Cols.push_back(*a1++);
                    }
                    break;
                }


                if (*a1 < *b1)
                {
                    _Add_elem_g1<P, C1, C2>(out, A, a1 - a0);
                    out.Cols.push_back(*a1++);
                }
                else if (*b1 < *a1)
                {
                    _Add_elem_g2<P, C2, C1>(out, B, b1 - b0);
                    out.Cols.push_back(*b1++);
                }
                else
                {
                    if (_Add_elem<P, C1, C2>(out, A, a1 - a0, B, b1 - b0))
                    {
                        out.Cols.push_back(*a1);
                    }
                    a1++;
                    b1++;
                }
            }

            out.SRows.push_back(out.Value_real.size());
        }

    }

    template<typename P,typename T,typename X>
    static void calc_matrix(compact_matrix_t<T,X>& out, compact_matrix_t<T, X>& A, compact_matrix_t<T, X>& B)
    {
        if (A.NRow != B.NRow || A.NCol != B.NCol)
        {
            Exception::XArraySizeNotMatch();
        }

        if (!A.ensure_srows() || !B.ensure_srows())
        {
            Exception::XError("Invalid SRows");
        }

        sp_matrix_holder<T,X> holder;
        auto out_mat(holder.init(out,A.NRow,A.NCol));

        if (A.is_complex())
        {
            if (B.is_complex())
            {
                _Add_matrix<P, true, true>(out_mat, A, B);
            }
            else
            {
                _Add_matrix<P, true, false>(out_mat, A, B);
            }
        }
        else
        {
            if (B.is_complex())
            {
                _Add_matrix<P, false, true>(out_mat, A, B);
            }
            else
            {
                _Add_matrix<P, false, false>(out_mat, A, B);
            }
        }

    }

    template<typename M3, typename M1, typename M2>
    static void add_matrix(M3& out, M1& A, M2& B)
    {
        calc_matrix<pl_add>(out, A, B);
    }

    template<typename M3, typename M1, typename M2>
    static void sub_matrix(M3& out, M1& A, M2& B)
    {
        calc_matrix<pl_sub>(out, A, B);
    }


    template<bool C1, bool C2, typename T,typename X>
    static void _Mult_scalar(compact_storage_t<T,X>& out, compact_matrix_t<T,X>& A, dcomplex cc)
    {
        out.Value_real.resize(A.NZero);
        out.Rows.assign_vector(A.Rows,A.NZero);
        out.Cols.assign_vector(A.Cols,A.NZero);
        out.SRows.assign_vector(A.SRows,A.NRow+1);

        if (C1 || C2)
        {
            out.Value_imag.resize(A.NZero);
            if (!C1)
            {
                for (size_t i = 0; i < A.NZero; i++)
                {
                    out.Value_real[i] = cc.real() * A.Value_real[i];
                    out.Value_imag[i] = cc.imag() * A.Value_real[i];
                }
            }
            else
            {
                for (size_t i = 0; i < A.NZero; i++)
                {
                    dcomplex val = cc * dcomplex(A.Value_real[i], A.Value_imag[i]);
                    out.Value_real[i] = val.real();
                    out.Value_imag[i] = val.imag();
                }
            }
        }
        else
        {
            for (size_t i = 0; i < A.NZero; i++)
            {
                out.Value_real[i] = cc.real() * A.Value_real[i];
            }
        }

    }

    template<typename T,typename X>
    static void mult_scalar(compact_matrix_t<T,X>& out, compact_matrix_t<T,X>& A, dcomplex cc)
    {
        sp_matrix_holder<T,X> holder;
        auto& B(holder.init(out,A.NRow,A.NCol));

        if (cc == dcomplex(0.0))
        {
            B.SRows.assign(A.NRow + 1, 0);
        }
        else if (A.is_complex())
        {
            if (cc.imag() != 0.0)
            {
                _Mult_scalar<true, true>(B, A, cc);
            }
            else
            {
                _Mult_scalar<true, false>(B, A, cc);
            }
        }
        else
        {
            if (cc.imag() != 0.0)
            {
                _Mult_scalar<false, true>(B, A, cc);
            }
            else
            {
                _Mult_scalar<false, false>(B, A, cc);
            }
        }
    }

    template<bool C1, bool C2, typename T,typename X>
    static void _Add_scalar(compact_storage_t<T,X>& out, compact_matrix_t<T,X>& A, dcomplex cc)
    {

        auto* a0 = &A.Cols[0];

        for (size_t i = 0; i < out.NRow; i++)
        {
            auto* a1 = a0 + A.SRows[i];
            auto* a2 = a0 + A.SRows[i + 1];

            auto* aa = a1;

            for (;aa < a2 ; ++aa)
            {
                size_t idx = aa - a0;
                if (*aa != i)
                {
                    out.Value_real.push_back(A.Value_real[idx]);
                    if (C1)
                    {
                        out.Value_imag.push_back(A.Value_imag[idx]);
                    }
                    else if (C2)
                    {
                        out.Value_imag.push_back(0.0);
                    }
                    out.Cols.push_back(*aa);
                }
                else
                {
                    dcomplex val = dcomplex(A.Value_real[idx] + cc.real(), cc.imag());
                    if (C1)
                    {
                        val.imag()+=A.Value_imag[idx];
                    }
                    if (C1 || C2)
                    {
                        if (val != dcomplex(0.0))
                        {
                            out.Value_real.push_back(val.real());
                            out.Value_imag.push_back(val.imag());
                            out.Cols.push_back(*aa);
                        }
                    }
                    else
                    {
                        if (val.real() != 0.0)
                        {
                            out.Value_real.push_back(val.real());
                            out.Cols.push_back(*aa);
                        }
                    }                            

                }
 
            }

            out.SRows[i+1]=out.Value_real.size();
        }

    }

    template<typename T,typename X>
    static void add_scalar(compact_matrix_t<T,X>& out, compact_matrix_t<T,X>& A, dcomplex cc)
    {

        if (cc == dcomplex(0.0))
        {
            out = A;
            return;
        }

        EW_ASSERT((void*)&out != (void*)&A);

        if (!A.ensure_srows())
        {
            Exception::XError("Invalid matrix");
        }

        sp_matrix_holder<T,X> holder;
        auto& B(holder.init_with_srows(out,A.NRow,A.NCol));

        if (A.is_complex())
        {
            if (cc.imag() != 0.0)
            {
                _Add_scalar<true, true>(B, A, cc);
            }
            else
            {
                _Add_scalar<true, false>(B, A, cc);
            }
        }
        else
        {
            if (cc.imag() != 0.0)
            {
                _Add_scalar<false, true>(B, A, cc);
            }
            else
            {
                _Add_scalar<false, false>(B, A, cc);
            }
        }
    }



    template<typename T,typename X>
    static void mult_vector_real(double* out, compact_matrix_t<T, X>& A, const double* x)
    {
        std::fill_n(out, A.NRow, 0.0);

        if (!A.Rows.empty())
        {
            for (size_t n = 0; n < A.NZero; n++)
            {
                size_t i = A.Rows[n];
                size_t j = A.Cols[n];
                out[i] += A.Value_real[n] * x[j];
            }
        }
        else if (!A.SRows.empty())
        {
            auto* j0 = &A.Cols[0];
            for (size_t i = 0; i < A.NRow; i++)
            {
                auto* j1 = j0 + A.SRows[i];
                auto* j2 = j0 + A.SRows[i + 1];
                for (auto jj = j1; jj < j2; jj++)
                {
                    out[i] += A.Value_real[size_t(jj - j0)] * x[*jj];
                }
            }
        }
        else
        {
            Exception::XError("Invalid compact_matrix");
        }
    }
    template<typename T,typename X>
    static void mult_vector_real(dcomplex* out, compact_matrix_t<T,X>& A, const dcomplex* x)
    {
        mult_vector_complex(out, A, x);
    }

    template<typename T,typename X,typename T2>
    static void mult_vector_complex(dcomplex* out, compact_matrix_t<T,X>& A, const T2* x)
    {
        std::fill_n(out, A.NRow, 0.0);

        if (!A.Rows.empty())
        {
            if (A.is_complex())
            {
                for (size_t n = 0; n < A.NZero; n++)
                {
                    size_t i = A.Rows[n];
                    size_t j = A.Cols[n];
                    out[i] += dcomplex(A.Value_real[n], A.Value_imag[n]) * x[j];
                }
            }
            else
            {
                for (size_t n = 0; n < A.NZero; n++)
                {
                    size_t i = A.Rows[n];
                    size_t j = A.Cols[n];
                    out[i] += A.Value_real[n] * x[j];
                }
            }
        }
        else if (!A.SRows.empty())
        {
            if (A.is_complex())
            {
                auto* j0 = &A.Cols[0];
                for (size_t i = 0; i < A.NCol; i++)
                {
                    auto* j1 = j0 + A.SRows[i];
                    auto* j2 = j0 + A.SRows[i + 1];
                    for (auto jj = j1; jj < j2; jj++)
                    {
                        out[i] += dcomplex(A.Value_real[size_t(jj - j0)], A.Value_imag[size_t(jj - j0)]) * x[*jj];
                    }
                }
            }
            else
            {
                auto* j0 = &A.Cols[0];
                for (size_t i = 0; i < A.NRow; i++)
                {
                    auto* j1 = j0 + A.SRows[i];
                    auto* j2 = j0 + A.SRows[i + 1];
                    for (auto jj = j1; jj < j2; jj++)
                    {
                        out[i] += A.Value_real[size_t(jj - j0)] * x[*jj];
                    }
                }
            }

        }
        else
        {
            Exception::XError("Invalid compact_matrix");
        }
    }

    //template<typename M>
    //static void mult_vector(arr_1t<double>& out, M& A, compact_vector_t<double> x)
    //{
    //    arr_1t<double> tmp(A.NRow);
    //    mult_vector_real(tmp, A, x);
    //    tmp.swap(out);
    //}

 
};



EW_LEAVE

#endif
