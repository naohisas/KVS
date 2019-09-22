/*****************************************************************************/
/**
 *  @file   LUDecomposer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LUDecomposer.cpp 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "LUDecomposer.h"
#include <kvs/Macro>
#include <kvs/Math>


namespace kvs
{

template <typename T> size_t LUDecomposer<T>::m_max_iterations = 30;

/*===========================================================================*/
/**
 *  @brief  Sets a maximum number of iterations.
 *  @param  max_iterations [in] maximum number of iterations
 */
/*===========================================================================*/
template <typename T>
void LUDecomposer<T>::SetMaxIterations( const size_t max_iterations )
{
    m_max_iterations = max_iterations;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposer class.
 */
/*===========================================================================*/
template <typename T>
LUDecomposer<T>::LUDecomposer()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposer class.
 *  @param  m [in] input matrix (3x3)
 */
/*===========================================================================*/
template <typename T>
LUDecomposer<T>::LUDecomposer( const Matrix33<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposer class.
 *  @param  m [in] input matrix (4x4)
 */
/*===========================================================================*/
template <typename T>
LUDecomposer<T>::LUDecomposer( const Matrix44<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposer class.
 *  @param  m [in] input matrix (MxM)
 */
/*===========================================================================*/
template <typename T>
LUDecomposer<T>::LUDecomposer( const Matrix<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the LUDecomposer class.
 *  @param  l [in] LUDecomposer
 */
/*===========================================================================*/
template <typename T>
LUDecomposer<T>& LUDecomposer<T>::operator = ( const LUDecomposer<T>& l )
{
    m_l = l.m_l;
    m_u = l.m_u;
    m_lu = l.m_lu;
    m_pivots = l.m_pivots;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Returns the L matrix.
 *  @return L matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& LUDecomposer<T>::L() const
{
    return m_l;
}

/*===========================================================================*/
/**
 *  @brief  Returns the U matrix.
 *  @return U matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& LUDecomposer<T>::U() const
{
    return m_u;
}

/*===========================================================================*/
/**
 *  @brief  Returns the LU matrix.
 *  @return LU matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& LUDecomposer<T>::LU() const
{
    return m_lu;
}

/*===========================================================================*/
/**
 *  @brief  Returns the pivot vector.
 *  @return pivot vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<int>& LUDecomposer<T>::pivots() const
{
    return m_pivots;
}

/*===========================================================================*/
/**
 *  @brief  Sets a 3x3 matrix.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
void LUDecomposer<T>::setMatrix( const kvs::Matrix33<T>& m )
{
    m_l.resize( 3, 3 );
    m_u.resize( 3, 3 );
    m_pivots.resize( 3 );
    m_lu.resize( 3, 3 );
    for ( size_t i = 0; i < 3; i++ )
    {
        for ( size_t j = 0; j < 3; j++ )
        {
            m_lu[i][j] = m[i][j];
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
void LUDecomposer<T>::setMatrix( const kvs::Matrix44<T>& m )
{
    m_l.resize( 4, 4 );
    m_u.resize( 4, 4 );
    m_pivots.resize( 4 );
    m_lu.resize( 4, 4 );
    for ( size_t i = 0; i < 4; i++ )
    {
        for ( size_t j = 0; j < 4; j++ )
        {
            m_lu[i][j] = m[i][j];
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
void LUDecomposer<T>::setMatrix( const Matrix<T>& m )
{
    m_l.resize( m.rowSize(), m.columnSize() );
    m_u.resize( m.rowSize(), m.columnSize() );
    m_pivots.resize( m.rowSize() );
    m_lu = m;
}

/*===========================================================================*/
/**
 *  @brief  Decompose.
 */
/*===========================================================================*/
template <typename T>
void LUDecomposer<T>::decompose()
{
    KVS_ASSERT( m_lu.rowSize() == m_lu.columnSize() );

    const int row = static_cast<int>(m_lu.rowSize());

    // Loop over rows for implicit scaling info.
    kvs::Vector<T> scaling( row );
    for ( int i = 0; i < row; i++ )
    {
        T max = T(0);
        for ( int j = 0; j < row; j++ )
        {
            T temp = kvs::Math::Abs( m_lu[i][j] );
            if ( temp > max ) max = temp;
        }

        // Singular.
        KVS_ASSERT( !kvs::Math::IsZero( max ) );

        // Save the scaling.
        scaling[i] = T(1) / max;
    }

    // Loop over columns of Crout's method.
    for ( int j = 0; j < row; j++ )
    {
        // Eq. (2.3.12) in the Numetical Recipes in C.
        for ( int i = 0; i < j; i++ )
        {
            T sum = m_lu[i][j];
            for ( int k = 0; k < i; k++ )
            {
                sum -= m_lu[i][k] * m_lu[k][j];
            }
            m_lu[i][j] = sum;
        }

        // Initialize for search for largest pivot (implicit pivotting)
        int pivot = j;
        T   max   = T(0);
        for ( int i = j; i < row; i++ )
        {
            T sum = m_lu[i][j];
            for ( int k = 0; k < j; k++ )
            {
                sum -= m_lu[i][k] * m_lu[k][j];
            }
            m_lu[i][j] = sum;

            T temp = scaling[i] * kvs::Math::Abs( sum );
            if ( temp >= max )
            {
                max = temp;
                pivot = i;
            }
        }

        // Interchange rows.
        if ( j != pivot )
        {
            for ( int k = 0; k < row; k++ )
            {
                T temp         = m_lu[pivot][k];
                m_lu[pivot][k] = m_lu[j][k];
                m_lu[j][k]     = temp;
            }
            scaling[pivot] = scaling[j];
        }

        m_pivots[j] = pivot;

        // Singular.
        KVS_ASSERT( !kvs::Math::IsZero( m_lu[j][j] ) );

        // Now, finally, divide by the pivot element.
        if ( j != row - 1 )
        {
            T temp = T(1) / m_lu[j][j];
            for ( int i = j + 1; i < row; i++ ) m_lu[i][j] *= temp;
        }
    }

    // Make L matrix and U matrix.
    for ( int i = 0; i < row; i++ )
    {
        for ( int j = 0; j < i; j++ )
        {
            if ( i != j ) m_l[i][j] = m_lu[i][j];
            m_u[j][i] = m_lu[j][i];
        }
        m_u[i][i] = m_lu[i][i];
    }
}

// template instantiation
template class LUDecomposer<float>;
template class LUDecomposer<double>;

} // end of namespace kvs
