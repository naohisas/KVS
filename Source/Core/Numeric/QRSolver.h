/*****************************************************************************/
/**
 *  @file   QRSolver.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QRSolver.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__QR_SOLVER_H_INCLUDE
#define KVS__QR_SOLVER_H_INCLUDE

#include <kvs/Vector>
#include <kvs/Matrix>
#include "QRDecomposer.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  QR decomposition solver.
 */
/*===========================================================================*/
template <typename T>
class QRSolver : public kvs::Vector<T>
{
private:

    QRDecomposer<T> m_decomposer; ///< QR decomposer

public:

    QRSolver();
    QRSolver( const kvs::QRDecomposer<T>& decomposer );
    QRSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~QRSolver();

    QRSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs

#endif // KVS__QR_SOLVER_H_INCLUDE
