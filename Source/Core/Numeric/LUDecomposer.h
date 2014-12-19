/*****************************************************************************/
/**
 *  @file   LUDecomposer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LUDecomposer.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__LU_DECOMPOSER_H_INCLUDE
#define KVS__LU_DECOMPOSER_H_INCLUDE

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
class LUDecomposer
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

    LUDecomposer();
    LUDecomposer( const kvs::Matrix33<T>& m );
    LUDecomposer( const kvs::Matrix44<T>& m );
    LUDecomposer( const kvs::Matrix<T>& m );

    LUDecomposer& operator = ( const LUDecomposer& l );

    const kvs::Matrix<T>& L() const;
    const kvs::Matrix<T>& U() const;
    const kvs::Matrix<T>& LU() const;
    const kvs::Vector<int>& pivots() const;

    void setMatrix( const kvs::Matrix33<T>& m );
    void setMatrix( const kvs::Matrix44<T>& m );
    void setMatrix( const kvs::Matrix<T>& m );
    void decompose();
};

} // end of namespace kvs

#endif // KVS__LU_DECOMPOSER_H_INCLUDE
