/*****************************************************************************/
/**
 *  @file   GaussEliminationSolver.h
 *  @author Naohisa Sakamoto
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
