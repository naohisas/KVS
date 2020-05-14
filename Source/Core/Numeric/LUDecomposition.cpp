/*****************************************************************************/
/**
 *  @file   LUDecomposition.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "LUDecomposition.h"
#include <kvs/Macro>
#include <kvs/Math>


namespace kvs
{

template <typename T> size_t LUDecomposition<T>::m_max_iterations = 30;

/*===========================================================================*/
/**
 *  @brief  Sets a maximum number of iterations.
 *  @param  max_iterations [in] maximum number of iterations
 */
/*===========================================================================*/
template <typename T>
void LUDecomposition<T>::SetMaxIterations( const size_t max_iterations )
{
    m_max_iterations = max_iterations;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposition class.
 */
/*===========================================================================*/
template <typename T>
LUDecomposition<T>::LUDecomposition()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposition class.
 *  @param  m [in] input matrix (3x3)
 */
/*===========================================================================*/
template <typename T>
LUDecomposition<T>::LUDecomposition( const Matrix33<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposition class.
 *  @param  m [in] input matrix (4x4)
 */
/*===========================================================================*/
template <typename T>
LUDecomposition<T>::LUDecomposition( const Matrix44<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LUDecomposition class.
 *  @param  m [in] input matrix (MxM)
 */
/*===========================================================================*/
template <typename T>
LUDecomposition<T>::LUDecomposition( const Matrix<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the LUDecomposition class.
 *  @param  lud [in] LUDecomposition
 */
/*===========================================================================*/
template <typename T>
LUDecomposition<T>& LUDecomposition<T>::operator = ( const LUDecomposition<T>& lud )
{
    m_l = lud.m_l;
    m_u = lud.m_u;
    m_lu = lud.m_lu;
    m_pivots = lud.m_pivots;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Sets a 3x3 matrix.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
void LUDecomposition<T>::setMatrix( const kvs::Matrix33<T>& m )
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
void LUDecomposition<T>::setMatrix( const kvs::Matrix44<T>& m )
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
void LUDecomposition<T>::setMatrix( const Matrix<T>& m )
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
void LUDecomposition<T>::decompose()
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
template class LUDecomposition<float>;
template class LUDecomposition<double>;

} // end of namespace kvs
