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
 *  @brief  Constructs a new GaussEliminationSolver class.
 */
/*===========================================================================*/
template <typename T>
GaussEliminationSolver<T>::GaussEliminationSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GaussEliminationSolver class.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 */
/*===========================================================================*/
template <typename T>
GaussEliminationSolver<T>::GaussEliminationSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    this->solve( A, b );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the GaussEliminationSolver class.
 */
/*===========================================================================*/
template <typename T>
GaussEliminationSolver<T>::~GaussEliminationSolver()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the GaussEliminationSolver class.
 *  @param  v [in] vector
 */
/*===========================================================================*/
template <typename T>
GaussEliminationSolver<T>& GaussEliminationSolver<T>::operator = ( const kvs::Vector<T>& v )
{
    this->setSize( v.size() );
    for ( size_t i = 0; i < this->size(); ++i )
    {
        (*this)[i] = v[i];
    }
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
const kvs::Vector<T>& GaussEliminationSolver<T>::solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b )
{
    KVS_ASSERT( A.rowSize() == A.columnSize() );
    KVS_ASSERT( A.columnSize() == b.size() );

    kvs::Matrix<T> temp_mat( A );
    kvs::Vector<T> temp_vec( b );

    const int row = A.rowSize();
    const int column = A.columnSize();
    for ( int k = 0; k < row; ++k )
    {
        // Search a pivot element.
        const int index = temp_mat.pivot( static_cast<int>(k) );

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
            const kvs::Vector<T> temp_row = temp_mat[ k ];
            temp_mat[ k ]     = temp_mat[ index ];
            temp_mat[ index ] = temp_row;

            const T temp_elem = temp_vec[ k ];
            temp_vec[ k ]     = temp_vec[ index ];
            temp_vec[ index ] = temp_elem;
        }

        // Forward elimination

        const T a1 = temp_mat[ k ][ k ];
        for ( int j = 0; j < column; ++j )
        {
            temp_mat[ k ][ j ] /= a1;
        }
        temp_vec[ k ] /= a1;

        for ( int i = 0; i < row; ++i )
        {
            if( i != k ) // Skip the pivot-row.
            {
                const T a2 = temp_mat[ i ][ k ];
                for ( int j = 0; j < column; ++j )
                {
                    temp_mat[ i ][ j ] -= a2 * temp_mat[ k ][ j ];
                }
                temp_vec[ i ] -= a2 * temp_vec[ k ];
            }
        }
    }

    return *this = temp_vec;
}

// template instantiation
template class GaussEliminationSolver<float>;
template class GaussEliminationSolver<double>;

} // end of namespace kvs
