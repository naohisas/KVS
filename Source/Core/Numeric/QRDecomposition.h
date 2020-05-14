/*****************************************************************************/
/**
 *  @file   QRDecomposition.h
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
 *  @brief  QR decomposition class.
 */
/*===========================================================================*/
template <typename T>
class QRDecomposition
{
private:
    kvs::Matrix<T> m_m;  ///< original matrix
    kvs::Matrix<T> m_qt; ///< Transpose matrix of Q matrix
    kvs::Matrix<T> m_r;  ///< R matrix (upper triangle matrix)

public:
    QRDecomposition();
    QRDecomposition( const kvs::Matrix33<T>& m );
    QRDecomposition( const kvs::Matrix44<T>& m );
    QRDecomposition( const kvs::Matrix<T>& m );

    QRDecomposition& operator = ( const QRDecomposition& q );

    const kvs::Matrix<T> Q() const;
    const kvs::Matrix<T>& R() const;
    const kvs::Matrix<T>& Qt() const;

    void setMatrix( const kvs::Matrix33<T>& m );
    void setMatrix( const kvs::Matrix44<T>& m );
    void setMatrix( const kvs::Matrix<T>& m );
    void decompose();
};

} // end of namespace kvs
