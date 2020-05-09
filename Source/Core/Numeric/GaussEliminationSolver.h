/*****************************************************************************/
/**
 *  @file   GaussEliminationSolver.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GaussEliminationSolver.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GAUSS_ELIMINATION_SOLVER_H_INCLUDE
#define KVS__GAUSS_ELIMINATION_SOLVER_H_INCLUDE


namespace kvs
{

template <typename T> class Vector;
template <typename T> class Matrix;

/*===========================================================================*/
/**
 *  @brief  Gauss elimination method.
 */
/*===========================================================================*/
template <typename T>
class GaussEliminationSolver : public kvs::Vector<T>
{
public:
    GaussEliminationSolver() {}
    GaussEliminationSolver( const kvs::Matrix<T>& A, const kvs::Vector<T>& b ) { this->solve( A, b ); }
    virtual ~GaussEliminationSolver() {}

    GaussEliminationSolver<T>& operator = ( const kvs::Vector<T>& v );

    const kvs::Vector<T>& solve( const kvs::Matrix<T>& A, const kvs::Vector<T>& b );
};

} // end of namespace kvs

#endif // KVS__GAUSS_ELIMINATION_SOLVER_H_INCLUDE
