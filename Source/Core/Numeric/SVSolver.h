/*****************************************************************************/
/**
 *  @file   SVSolver.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "SVDecomposition.h"
#include <kvs/Vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Singular value decomposition solver.
 */
/*===========================================================================*/
template <typename T>
class SVSolver : public kvs::Vector<T>
{
private:
    kvs::SVDecomposition<T> m_decomp; ///< Singular value decomposition

public:
    SVSolver();
    SVSolver( const kvs::SVDecomposition<T>& decomp );
    SVSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~SVSolver();

    SVSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs
