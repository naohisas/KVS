/*****************************************************************************/
/**
 *  @file   LUSolver.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LUSolver.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__LU_SOLVER_H_INCLUDE
#define KVS__LU_SOLVER_H_INCLUDE

#include "LUDecomposer.h"
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

    kvs::LUDecomposer<T> m_decomposer; ///< LU decomposer

public:

    LUSolver();
    LUSolver( const kvs::LUDecomposer<T>& decomposer );
    LUSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~LUSolver();

    LUSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs

#endif // KVS__LU_SOLVER_H_INCLUDE
