/*****************************************************************************/
/**
 *  @file   QRDecomposer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QRDecomposer.cpp 1385 2012-12-04 03:25:29Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "QRDecomposer.h"
#include <cmath>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRDecomposer class.
 */
/*===========================================================================*/
template <typename T>
QRDecomposer<T>::QRDecomposer()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRDecomposer class.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
QRDecomposer<T>::QRDecomposer( const kvs::Matrix33<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRDecomposer class.
 *  @param  m [in] 4x4 matrix
 */
/*===========================================================================*/
template <typename T>
QRDecomposer<T>::QRDecomposer( const kvs::Matrix44<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new QRDecomposer class.
 *  @param  m [in] MxM matrix
 */
/*===========================================================================*/
template <typename T>
QRDecomposer<T>::QRDecomposer( const kvs::Matrix<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the QRDecomposer class.
 *  @param  q [in] QRDecomposer
 */
/*===========================================================================*/
template <typename T>
QRDecomposer<T>& QRDecomposer<T>::operator = ( const QRDecomposer<T>& q )
{
    m_m = q.m_m;
    m_qt = q.m_qt;
    m_r = q.m_r;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Returns the Q matrix.
 *  @return Q matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T> QRDecomposer<T>::Q() const
{
    return m_qt.transposed();
}

/*===========================================================================*/
/**
 *  @brief  Returns the R matrix.
 *  @return R matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& QRDecomposer<T>::R() const
{
    return m_r;
}

/*===========================================================================*/
/**
 *  @brief  Retruns the transposed Q matrix.
 *  @return transposed Q matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& QRDecomposer<T>::Qt() const
{
    return m_qt;
}

/*===========================================================================*/
/**
 *  @brief  Sets a 3x3 matrix.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
void QRDecomposer<T>::setMatrix( const Matrix33<T>& m )
{
    m_qt.resize( 3, 3 ); m_qt.setIdentity();
    m_r.resize( 3, 3 );
    m_m.resize( 3, 3 );
    for ( size_t i = 0; i < 3; i++ )
    {
        for ( size_t j = 0; j < 3; j++ )
        {
            m_r[i][j] = m[i][j];
            m_m[i][j] = m[i][j];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a 4x4 matrix.
 *  @param  m [in] 4x4 matrix
 */
/*===========================================================================*/
template <typename T>
void QRDecomposer<T>::setMatrix( const kvs::Matrix44<T>& m )
{
    m_qt.resize( 4, 4 ); m_qt.setIdentity();
    m_r.resize( 4, 4 );
    m_m.resize( 4, 4 );
    for ( size_t i = 0; i < 4; i++ )
    {
        for ( size_t j = 0; j < 4; j++ )
        {
            m_r[i][j] = m[i][j];
            m_m[i][j] = m[i][j];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a MxM matrix.
 *  @param  m [in] MxM matrix
 */
/*===========================================================================*/
template <typename T>
void QRDecomposer<T>::setMatrix( const kvs::Matrix<T>& m )
{
    m_qt.resize( m.rowSize(), m.columnSize() ); m_qt.setIdentity();
    m_r = m;
    m_m = m;
}

/*===========================================================================*/
/**
 *  @brief  Decompose.
 */
/*===========================================================================*/
template <typename T>
void QRDecomposer<T>::decompose()
{
    int row = m_m.rowSize();
    int column = m_m.columnSize();
    int size = row != column ? column : column - 1;

    kvs::Vector<T> u( row );
    kvs::Matrix<T> w( row, row );
    kvs::Matrix<T> q( row, row );
    for( int i = 0; i < size; i++ )
    {
        T sig2 = T(0);
        for( int j = i; j < row; j++ )
        {
            sig2 += m_r[j][i] * m_r[j][i];
        }

        T temp = static_cast<T>(std::sqrt((double)sig2));
        T sig  = m_r[i][i] < T(0) ? -temp : temp;
        T a    = sig2 + m_r[i][i] * sig;

        u[i] = m_r[i][i] + sig;
        for( int j = i + 1; j < row; j++ )
        {
            u[j] = m_r[j][i];
        }

        // Calculate Householder matrix for solving Q matrix.
        for( int k = 0; k < row; k++ )
        {
            for( int j = 0; j < row; j++ )
            {
                q[k][j] = ((k==j)? T(1) : T(0)) - u[k] * u[j] / a;
            }
        }

        // Calculate Q matrix.
        for( int k = 0; k < row; k++ )
        {
            for( int j = 0; j < row; j++ )
            {
                w[k][j] = T(0);
                for( int n = 0; n < row; n++ )
                {
                    w[k][j] += q[k][n] * m_qt[n][j];
                }
            }
        }
        m_qt = w;

        // Calculate R matrix.
        for( int k = 0; k < row; k++ )
        {
            for( int j = 0; j < column; j++ )
            {
                w[k][j] = T(0);
                for( int n = 0; n < row; n++ )
                {
                    w[k][j] += q[k][n] * m_r[n][j];
                }
            }
        }
        m_r = w;

        u[i] = T(0);
    }
}

// template instantiation
template class QRDecomposer<float>;
template class QRDecomposer<double>;

} // end of namespace kvs
