/*****************************************************************************/
/**
 *  @file   LUSolver.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "LUSolver.h"
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUSolver class.
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>::LUSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUSolver class.
 *  @param  decomp [in] LU decomposition
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>::LUSolver( const kvs::LUDecomposition<T>& decomp )
{
    m_decomp = decomp;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUSolver class.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>::LUSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    this->solve( A, b );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the LUSolver class.
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>::~LUSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the LUSolver class.
 *  @param  v [in] vector
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>& LUSolver<T>::operator = ( const kvs::Vector<T>& v )
{
    this->resize( v.size() );
    for( size_t i = 0; i < this->size(); i++ ){ (*this)[i] = v[i]; }
    return ( *this );
}

/*===========================================================================*/
/**
 *  @brief  Solve the simultaneous equations.
 *  @param  b [in] right-hand side vector
 *  @return solution vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& LUSolver<T>::solve( const kvs::Vector<T>& b )
{
    // Solution vector.
    kvs::Vector<T> x = b;

    // Forward substitution.
    int row = m_decomp.LU().rowSize();
    int ii  = -1;
    for( int i = 0; i < row; i++ )
    {
        int pivot = m_decomp.pivots()[i];
        T   sum   = x[pivot];
        x[pivot]  = x[i];
        if( ii >= 0 )
        {
            for( int j = ii; j <= i - 1; j++ )
            {
                sum -= m_decomp.LU()[i][j] * x[j];
            }
        }
        else if( sum )
        {
            ii = i;
        }
        x[i] = sum;
    }

    // back substitution.
    for( int i = row - 1; i >= 0; i-- )
    {
        T sum = x[i];
        for( int j = i + 1; j < row; j++ )
        {
            sum -= m_decomp.LU()[i][j] * x[j];
        }
        x[i] = sum / m_decomp.LU()[i][i];
    }

    return ( *this = x );
}

/*===========================================================================*/
/**
 *  @brief  Solve the simultaneous equations.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] right-hand side vector
 *  @return solution vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& LUSolver<T>::solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    KVS_ASSERT( A.rowSize() == A.columnSize() );
    KVS_ASSERT( A.columnSize() == b.size() );

    // LU decomposition.
    m_decomp.setMatrix( A );
    m_decomp.decompose();

    return this->solve( b );
}

// template instantiation
template class LUSolver<float>;
template class LUSolver<double>;

} // end of namespace kvs
