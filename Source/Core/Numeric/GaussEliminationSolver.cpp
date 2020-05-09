/*****************************************************************************/
/**
 *  @file   GaussEliminationSolver.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GaussEliminationSolver.cpp 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GaussEliminationSolver.h"
#include <kvs/Matrix>
#include <kvs/Vector>
#include <kvs/Assert>
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  '=' operator for the GaussEliminationSolver class.
 *  @param  v [in] vector
 */
/*===========================================================================*/
template <typename T>
GaussEliminationSolver<T>& GaussEliminationSolver<T>::operator = ( const kvs::Vector<T>& v )
{
    this->resize( v.size() );
    for ( size_t i = 0; i < this->size(); ++i ) { (*this)[i] = v[i]; }
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Solve the simultaneous equations.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 *  @return solution vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& GaussEliminationSolver<T>::solve(
    const kvs::Matrix<T>& A,
    const kvs::Vector<T>& b )
{
    KVS_ASSERT( A.isSquare() );
    KVS_ASSERT( A.columnSize() == b.size() );

    kvs::Matrix<T> temp_mat( A );
    kvs::Vector<T> temp_vec( b );
    const size_t nrows = A.rowSize();
    for ( size_t k = 0; k < nrows; ++k )
    {
        // Search a pivot element.
        const size_t index = temp_mat.pivot( k );

        // This matrix is a singular matrix. So it is impossible to
        // calculate the invert matrix, this method return a identity matrix.
        if ( kvs::Math::Abs( temp_mat[index][k] ) < 1.0e-10 )
        {
            kvsMessageError("Coefficient matrix is a singular marix.");
            return *this;
        }

        // Swap the k-row vector and the index-row vector.
        if ( index != k )
        {
            std::swap( temp_mat[k], temp_mat[index] );
            std::swap( temp_vec[k], temp_vec[index] );
        }

        // Forward elimination
        const T a1 = temp_mat[k][k];
        temp_mat[k] /= a1;
        temp_vec[k] /= a1;
        for ( size_t i = 0; i < nrows; ++i )
        {
            if( i != k ) // Skip the pivot-row.
            {
                const T a2 = temp_mat[i][k];
                temp_mat[i] -= a2 * temp_mat[k];
                temp_vec[i] -= a2 * temp_vec[k];
            }
        }
    }

    return *this = std::move( temp_vec );
}

// template instantiation
template class GaussEliminationSolver<float>;
template class GaussEliminationSolver<double>;

} // end of namespace kvs
