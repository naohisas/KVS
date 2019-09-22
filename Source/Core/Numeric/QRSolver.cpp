/*****************************************************************************/
/**
 *  @file   QRSolver.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QRSolver.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "QRSolver.h"
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRSolver class.
 */
/*===========================================================================*/
template <typename T>
QRSolver<T>::QRSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRSolver class.
 *  @param  decomposer [in] QR decomposer
 */
/*===========================================================================*/
template <typename T>
QRSolver<T>::QRSolver( const kvs::QRDecomposer<T>& decomposer )
{
    m_decomposer = decomposer;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRSolver class.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 */
/*===========================================================================*/
template <typename T>
QRSolver<T>::QRSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    this->solve( A, b );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the QRSolver class.
 */
/*===========================================================================*/
template <typename T>
QRSolver<T>::~QRSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the QRSolver class.
 *  @param  v [in] vector
 */
/*===========================================================================*/
template <typename T>
QRSolver<T>& QRSolver<T>::operator = ( const kvs::Vector<T>& v )
{
    this->resize( v.size() );
    for( size_t i = 0; i < this->size(); i++ ){ (*this)[i] = v[i]; }

    return *this;
};

/*===========================================================================*/
/**
 *  @brief  Solve the simultaneous equations.
 *  @param  b [i] right-hand side vector
 *  @return solution vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& QRSolver<T>::solve( const kvs::Vector<T>& b )
{
    int column = m_decomposer.R().columnSize();
    kvs::Vector<T> x( b.size() );

    // From Q^t * b.
    kvs::Vector<T> v = m_decomposer.Qt() * b;

    // Solve R * x = Q^t * b
    for( int i = column - 1; i >= 0; i-- )
    {
        for( int j = i + 1; j < column; j++ )
        {
            v[i] -= x[j] * m_decomposer.R()[i][j];
        }
        x[i] = v[i] / m_decomposer.R()[i][i];
    }

    return *this = x;
}

/*===========================================================================*/
/**
 *  @brief  Solve the simultaneous equations.
 *  @param  A [i] coefficient matrix
 *  @param  b [i] right-hand side vector
 *  @return solution vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& QRSolver<T>::solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    KVS_ASSERT( A.columnSize() == b.size() );

    // QR decomposition.
    m_decomposer.setMatrix( A );
    m_decomposer.decompose();

    return this->solve( b );
}

// template instantiation
template class QRSolver<float>;
template class QRSolver<double>;

} // end of namespace kvs
