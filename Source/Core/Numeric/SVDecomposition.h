/*****************************************************************************/
/**
 *  @file   SVDecomposition.h
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
 *  @brief  Singular value decomposition class.
 */
/*===========================================================================*/
template <typename T>
class SVDecomposition
{
private:
    kvs::Matrix<T> m_u; ///< U matrix (left singular matrix)
    kvs::Vector<T> m_w; ///< diagonal elements of W matrix (singular values)
    kvs::Matrix<T> m_v; ///< V matrix (right singular matrix)
    static size_t m_max_iterations; ///< maximum number of iterations

public:
    static void SetMaxIterations( const size_t max_iterations );

public:
    SVDecomposition();
    SVDecomposition( const kvs::Matrix33<T>& m );
    SVDecomposition( const kvs::Matrix44<T>& m );
    SVDecomposition( const kvs::Matrix<T>& m );

    SVDecomposition& operator = ( const SVDecomposition& s );

    const kvs::Matrix<T>& U() const;
    const kvs::Vector<T>& W() const;
    const kvs::Matrix<T>& V() const;
    const kvs::Matrix<T>& leftSingularMatrix() const;
    const kvs::Vector<T>& singularValues() const;
    const kvs::Matrix<T>& rightSingularMatrix() const;

    void setMatrix( const kvs::Matrix33<T>& m );
    void setMatrix( const kvs::Matrix44<T>& m );
    void setMatrix( const kvs::Matrix<T>& m );
    void decompose();
    void correctSingularValues();

private:
    void sort( kvs::Matrix<T>* umat, kvs::Matrix<T>* vmat, kvs::Vector<T>* wvec );
};

} // end of namespace kvs
