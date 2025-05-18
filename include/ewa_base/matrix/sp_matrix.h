#ifndef __EWA_MATRIX_SP_MATRIX_H__
#define __EWA_MATRIX_SP_MATRIX_H__

#include "ewa_base/matrix/sp_matrix_api.h"
#include "ewa_base/scripting/callable_data.h"


EW_ENTER


template<typename T>
class compact_array_t : public arr_1t<T>
{
public:
    typedef arr_1t<T> basetype;
    typedef typename basetype::pointer pointer;
    typedef typename basetype::const_pointer const_pointer;

    compact_array_t() {}

    explicit compact_array_t(size_t n) :basetype(n) {}

    template<typename T1>
    inline compact_array_t(std::initializer_list<T1> o)
    {
        assign(o.begin(), o.end());
    }

    template<typename T1>
    inline compact_array_t& operator=(std::initializer_list<T1> o)
    {
        assign(o.begin(), o.end());
        return *this;
    }

    pointer data()
    {
        return impl.empty() ? NULL : impl.data();
    }

    const_pointer data() const
    {
        return impl.empty() ? NULL : impl.data();
    }

    operator pointer ()
    {
        return impl.empty() ? NULL : impl.data();
    }

    operator const_pointer () const
    {
        return impl.empty() ? NULL : impl.data();
    }

    void assign_vector(const_pointer p, size_t n)
    {
        if (!p)
        {
            clear();
        }
        else
        {
            assign(p, n);
        }

    }
};


template<typename T>
class compact_vector_t : public LiteArrT<T>
{
public:
    typedef LiteArrT<T> basetype;
    typedef const T* const_pointer;

    compact_vector_t() {}

    compact_vector_t(const arr_1t<T>& arr)
    {
        m_ptr = arr.empty() ? NULL : (T*)arr.data();
    }

    compact_vector_t(const arr_xt<T>& arr)
    {
        m_ptr = arr.empty() ? NULL : (T*)arr.data();
    }

    compact_vector_t(const T* p)
    {
        m_ptr = (T*)p;
    }

    compact_vector_t& operator=(const arr_1t<T>& arr)
    {
        m_ptr = arr.empty() ? NULL : (T*)arr.data();
        return *this;
    }

    compact_vector_t& operator=(const arr_xt<T>& arr)
    {
        m_ptr = arr.empty() ? NULL : (T*)arr.data();
        return *this;
    }

    bool empty() const
    {
        return !m_ptr;
    }

    T* data()
    {
        return m_ptr;
    }

    void clear()
    {
        m_ptr = NULL;
    }

    void assign_vector(const_pointer p, size_t n)
    {
        m_ptr = p!=NULL && n!=0 ? (T*)p : NULL;
    }


};


template<typename T,typename X>
class compact_matrix_t;

template<typename T, typename X>
class compact_storage_t;



template<typename T,typename X>
class compact_storage_t : public ObjectData
{
public:

    compact_storage_t()
    {
        NRow = NCol = 0;
    }

    compact_array_t<T> Value_real, Value_imag;
    compact_array_t<X> Rows, Cols, SRows;
    size_t NRow, NCol;

    virtual compact_storage_t* DoCloneObject()
    {
        return new compact_storage_t(*this);
    }
};


template<typename T,typename X>
class compact_matrix_t
{
public:

    typedef T value_type;
    typedef X index_type;
    typedef compact_storage_t<T,X> storage_type;

    compact_vector_t<value_type> Value_real,Value_imag;
    compact_vector_t<index_type> Rows, Cols, SRows;
    size_t NRow, NCol;
    size_t NZero;

    compact_matrix_t()
    {
        NRow = NCol = NZero = 0;
    }

    size_t get_rows() const { return NRow; }
    size_t get_cols() const { return NCol; }
    bool is_square() const { return NRow==NCol; }


    compact_matrix_t& operator=(const compact_matrix_t& A)
    {
        set_matrix(A);
        return *this;
    }

    bool ok() const;

    bool is_complex() const;

    bool is_diag() const;

    bool is_identity() const;

    void clear();

    bool ensure_srows();

    bool ensure_rows();

    void random(size_t row, size_t col = 0, double ratio = 0.1);

    template<typename T2>
    void set_matrix_by_array(const arr_xt<T2>& arr);

    void set_matrix_by_variant(const Variant& var);

    void diag(size_t n, double* p);

    void load_identity(size_t n);

    void load_matrix(size_t nrow, size_t ncol, size_t nval, int* rows, int* cols, double* value_real, double* value_imag = NULL);

    void load_matrix(size_t nrow, size_t ncol, size_t nval, int* rows, int* cols, dcomplex* value);

    void transpose();

    template<typename T2,typename X2>
    void set_matrix(const compact_matrix_t<T2,X2>& A);


    void reset(storage_type* p);

    void Serialize(SerializerHelper sh);

    DataPtrT<storage_type> p_data;

    storage_type& make_storage_unique();

 
};


typedef compact_matrix_t<double,umfpack_int> compact_matrix;

bool operator==(const compact_matrix& lhs, const compact_matrix& rhs);


template<typename T,typename X>
class sp_matrix_holder : public DataPtrT<compact_storage_t<T,X> >
{
public:
    typedef compact_storage_t<T, X> storage_type;
    typedef DataPtrT<compact_storage_t<T, X> > basetype;

    using basetype::reset;

    inline storage_type& init(compact_matrix_t<T,X>& A)
    {
        p_matrix = &A;
        reset(new storage_type);
        return *m_ptr;
    }

    inline storage_type& init(compact_matrix_t<T,X>& A,size_t row,size_t col)
    {
        init(A);
        m_ptr->NRow = row;
        m_ptr->NCol = col;
        return *m_ptr;
    }

    inline storage_type& init_with_srows(compact_matrix_t<T,X>& A,size_t row,size_t col)
    {
        init(A, row, col);
        m_ptr->SRows.resize(row + 1);
        return *m_ptr;
    }

    ~sp_matrix_holder()
    {
        if (!p_matrix)
        {
            return;
        }
        p_matrix->reset(m_ptr);
    }

    inline storage_type& get_unique(compact_matrix_t<T, X>& A)
    {
        p_matrix = &A;
        reset(&p_matrix->make_storage_unique());
        return *m_ptr;
    }

    LitePtrT<compact_matrix> p_matrix;

protected:

    using basetype::m_ptr;

};

class umf_matrix : public NonCopyable
{
public:

    umf_matrix();

    ~umf_matrix();

    void clear();
    void reset(compact_matrix* pA,bool transpose);

    bool ok() const;

    int solve(double* x, double* b, int type);

private:

    template<typename T>
    void _reset(T* p,bool transpose=false);

    void* Numeric;
};




class ARPack_IPARAM_type
{
public:

    ARPack_IPARAM_type()
    {
        memset(this, 0, sizeof(*this));
        maxiter = 100;
    }

    operator int* ()
    {
        return (int*)this;
    }

    int ishift;
    int dummy1;
    int maxiter;
    int nb;
    int nconv;
    int dummy_iupd;
    int mode;
    int np;
    int numop;
    int numopb;
    int numreo;
    int padding;
};




class EigenSolver_arpack
{
public:

    EigenSolver_arpack();

    enum
    {
        FLAG_KEEP_RESID = 1 << 2,
    };

    void init(compact_matrix& A_, compact_matrix& B_, int NEV_);
    void init(compact_matrix& A_, int NEV_);

    void apply();

    compact_matrix A, B;

    dcomplex SIGMA;
    const char* WHICH;

    BitFlags flags;
    int N;
    int NEV;
    int NCV;
    int LWORKL;
    int LDV;
    double TOL;

    int ITER;

    ARPack_IPARAM_type IPARAM;

    arr_1t<double> RESID;

    arr_1t<dcomplex> EigValues;
    arr_xt<dcomplex> EigVectors;

};


template<typename T>
class compact_matrix_complex_helper
{
public:

    template<typename X>
    static void set_array(X&A, const arr_xt<T>&arr)
    {
        for (size_t row = 0; row < A.NRow; row++)
        {
            for (size_t col = 0; col < A.NCol; col++)
            {
                auto& val(arr(row, col));
                if (val == 0.0) continue;
                A.Cols.push_back(col);
                A.Value_real.push_back(val);
            }
            A.SRows[row + 1] = A.Value_real.size();
        }
    }
};

template<typename T>
class compact_matrix_complex_helper<tiny_cpx<T> >
{
public:

    template<typename X>
    static void set_array(X& A,const arr_xt<tiny_cpx<T> >& arr)
    {
        for (size_t row = 0; row < A.NRow; row++)
        {
            for (size_t col = 0; col < A.NCol; col++)
            {
                auto& val(arr(row, col));
                if (val == dcomplex(0.0)) continue;
                A.Cols.push_back(col);
                A.Value_real.push_back(val.real());
                A.Value_imag.push_back(val.imag());
            }
            A.SRows[row + 1] = A.Value_real.size();
        }
    }

};



template<typename T,typename X>
template<typename T2>
void compact_matrix_t<T,X>::set_matrix_by_array(const arr_xt<T2>& arr)
{
    sp_matrix_holder<T,X> holder;
    auto& A(holder.init(*this, arr.size(0), arr.size(1)));
    compact_matrix_complex_helper<T2>::set_array(A, arr);
}


template<typename T,typename X>
template<typename T2, typename X2>
void compact_matrix_t<T,X>::set_matrix(const compact_matrix_t<T2,X2>& A)
{
    if (&A == (void*)this)
    {
        return;
    }

    DataPtrT<storage_type> p(new storage_type);
    auto& B(*p);

    B.Value_real.assign_vector(A.Value_real, A.NZero);
    B.Value_imag.assign_vector(A.Value_imag, A.NZero);
    B.Rows.assign_vector(A.Rows, A.NZero);
    B.Cols.assign_vector(A.Cols, A.NZero);
    B.SRows.assign_vector(A.SRows, A.NRow + 1);

    B.NRow = A.NRow;
    B.NCol = A.NCol;

    reset(p.get());
}


template<typename A, typename T>
class serial_helper_func<A, compact_array_t<T> > : public serial_arr<A, T, compact_array_t<T> >
{
public:

};


template<> class hash_t<compact_matrix>
{
public:
    inline uint32_t operator()(const compact_matrix& o)
    {
        return -1;
    }
};

template<>
class CallableWrapT<compact_matrix> : public CallableDataBaseT<compact_matrix>
{
public:
    int __getindex(Executor& ks, const String& si);
    CallableMetatable* GetMetaTable();
};

EW_LEAVE

#endif
