/*****************************************************************************/
/**
 *  @file   LUSolver.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LUSolver.cpp 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
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
 *  @param  decomposer [in] LU decomposer
 */
/*===========================================================================*/
template <typename T>
LUSolver<T>::LUSolver( const kvs::LUDecomposer<T>& decomposer )
{
    m_decomposer = decomposer;
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

    return( *this );
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
    int row = m_decomposer.LU().rowSize();
    int ii  = -1;
    for( int i = 0; i < row; i++ )
    {
        int pivot = m_decomposer.pivots()[i];
        T   sum   = x[pivot];
        x[pivot]  = x[i];
        if( ii >= 0 )
        {
            for( int j = ii; j <= i - 1; j++ )
            {
                sum -= m_decomposer.LU()[i][j] * x[j];
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
            sum -= m_decomposer.LU()[i][j] * x[j];
        }
        x[i] = sum / m_decomposer.LU()[i][i];
    }

    return( *this = x );
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
    m_decomposer.setMatrix( A );
    m_decomposer.decompose();

    return( this->solve( b ) );
}

// template instantiation
template class LUSolver<float>;
template class LUSolver<double>;

} // end of namespace kvs
