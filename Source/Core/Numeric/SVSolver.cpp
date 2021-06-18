/*****************************************************************************/
/**
 *  @file   SVSolver.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "SVSolver.h"
#include <kvs/Macro>
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVSolver class.
 */
/*===========================================================================*/
template <typename T>
SVSolver<T>::SVSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVSolver class.
 *  @param  decomp [in] singular value decomposition
 */
/*===========================================================================*/
template <typename T>
SVSolver<T>::SVSolver( const kvs::SVDecomposition<T>& decomp )
{
    m_decomp = decomp;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVSolver class.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 */
/*===========================================================================*/
template <typename T>
SVSolver<T>::SVSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    this->solve( A, b );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the SVSolver class.
 */
/*===========================================================================*/
template <typename T>
SVSolver<T>::~SVSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the SVSolver class.
 *  @param  v [in] vector
 */
/*===========================================================================*/
template <typename T>
SVSolver<T>& SVSolver<T>::operator = ( const kvs::Vector<T>& v )
{
    this->resize( v.size() );
    for ( size_t i = 0; i < this->size(); i++ ){ (*this)[i] = v[i]; }
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
const kvs::Vector<T>& SVSolver<T>::solve( const kvs::Vector<T>& b )
{
    int row = m_decomp.U().rowSize();
    int column = m_decomp.U().columnSize();
    kvs::Vector<T> x( b.size() );

    // Editing of the singular values.
    m_decomp.correctSingularValues();

    // Calculate U^t * b.
    kvs::Vector<T> temp( column );
    for( int j = 0; j < column; j++ )
    {
        T s = T(0);
        if( !kvs::Math::IsZero( m_decomp.W()[j] ) )
        {
            for( int i = 0; i < row; i++ ) s += m_decomp.U()[i][j] * b[i];
            s /= m_decomp.W()[j];
        }
        temp[j] = s;
    }

    // Multiply the V matrix.
    x = m_decomp.V() * temp;

    return ( *this = x );
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
const kvs::Vector<T>& SVSolver<T>::solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    KVS_ASSERT( A.columnSize() == b.size() );

    // Singular value decomposition.
    m_decomp.setMatrix( A );
    m_decomp.decompose();

    return this->solve( b );
}

// template instantiation
template class SVSolver<float>;
template class SVSolver<double>;

} // end of namespace kvs
