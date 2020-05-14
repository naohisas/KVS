/*****************************************************************************/
/**
 *  @file   LUDecomposition.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Matrix33>
#include <kvs/Matrix44>
#include <kvs/Matrix>
#include <kvs/Vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  LU decomposition class.
 */
/*===========================================================================*/
template <typename T>
class LUDecomposition
{
private:
    kvs::Matrix<T> m_l; ///< L matrix
    kvs::Matrix<T> m_u; ///< U matrix
    kvs::Matrix<T> m_lu; ///< LU matrix (marged L and U matrix by Crout's method)
    kvs::Vector<int> m_pivots; ///< pivot vector
    static size_t m_max_iterations; ///< maximum number of iterations

public:
    static void SetMaxIterations( const size_t max_iterations );

public:
    LUDecomposition();
    LUDecomposition( const kvs::Matrix33<T>& m );
    LUDecomposition( const kvs::Matrix44<T>& m );
    LUDecomposition( const kvs::Matrix<T>& m );

    LUDecomposition& operator = ( const LUDecomposition& lud );

    const kvs::Matrix<T>& L() const { return m_l; }
    const kvs::Matrix<T>& U() const { return m_u; }
    const kvs::Matrix<T>& LU() const { return m_lu; }
    const kvs::Vector<int>& pivots() const { return m_pivots; }

    void setMatrix( const kvs::Matrix33<T>& m );
    void setMatrix( const kvs::Matrix44<T>& m );
    void setMatrix( const kvs::Matrix<T>& m );
    void decompose();
};

} // end of namespace kvs
