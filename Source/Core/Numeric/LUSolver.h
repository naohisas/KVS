/*****************************************************************************/
/**
 *  @file   LUSolver.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "LUDecomposition.h"
#include <kvs/Vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  LU decomposition solver.
 */
/*===========================================================================*/
template <typename T>
class LUSolver : public kvs::Vector<T>
{
private:
    kvs::LUDecomposition<T> m_decomp; ///< LU decomposition

public:
    LUSolver();
    LUSolver( const kvs::LUDecomposition<T>& decomp );
    LUSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~LUSolver();

    LUSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs
