/*****************************************************************************/
/**
 *  @file   QRSolver.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Vector>
#include <kvs/Matrix>
#include "QRDecomposition.h"


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
    kvs::QRDecomposition<T> m_decomp; ///< QR decomposer

public:
    QRSolver();
    QRSolver( const kvs::QRDecomposition<T>& decomp );
    QRSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
    virtual ~QRSolver();

    QRSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Vector<T>& b );
    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs
