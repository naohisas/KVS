/*****************************************************************************/
/**
 *  @file   QRDecomposer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QRDecomposer.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__QR_DECOMPOSER_H_INCLUDE
#define KVS__QR_DECOMPOSER_H_INCLUDE

#include <kvs/Matrix33>
#include <kvs/Matrix44>
#include <kvs/Matrix>
#include <kvs/Vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  QR decomposition class.
 */
/*===========================================================================*/
template <typename T>
class QRDecomposer
{
private:

    kvs::Matrix<T> m_m;  ///< original matrix
    kvs::Matrix<T> m_qt; ///< Transpose matrix of Q matrix
    kvs::Matrix<T> m_r;  ///< R matrix (upper triangle matrix)

public:

    QRDecomposer();
    QRDecomposer( const kvs::Matrix33<T>& m );
    QRDecomposer( const kvs::Matrix44<T>& m );
    QRDecomposer( const kvs::Matrix<T>& m );

    QRDecomposer& operator = ( const QRDecomposer& q );

    const kvs::Matrix<T> Q() const;
    const kvs::Matrix<T>& R() const;
    const kvs::Matrix<T>& Qt() const;

    void setMatrix( const kvs::Matrix33<T>& m );
    void setMatrix( const kvs::Matrix44<T>& m );
    void setMatrix( const kvs::Matrix<T>& m );
    void decompose();
};

} // end of namespace kvs

#endif // KVS__QR_DECOMPOSER_H_INCLUDE
