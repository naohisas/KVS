/*****************************************************************************/
/**
 *  @file   SVSolver.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SVSolver.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__SV_SOLVER_H_INCLUDE
#define KVS__SV_SOLVER_H_INCLUDE

#include "SVDecomposer.h"
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

    kvs::SVDecomposer<T> m_decomposer; ///< SV decomposer

public:

    SVSolver();
    SVSolver( const kvs::SVDecomposer<T>& decomposer );
    SVSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~SVSolver();

    SVSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs

#endif // KVS__SV_SOLVER_H_INCLUDE
