/*****************************************************************************/
/**
 *  @file   EigenDecomposer.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EigenDecomposer.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
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
 *  @brief  Eigen-values decomposition class.
 */
/*===========================================================================*/
template <typename T>
class EigenDecomposer
{
public:
    enum MatrixType
    {
        Unknown = 0, ///< unknown (auto-estimation)
        Unsymmetric = 1, ///< unsymmetric matrix (using the power method)
        Symmetric = 2 ///< symmetric matrix (using the QR method)
    };

private:
    MatrixType m_matrix_type; ///< matrix type (symmetric or unsymmetric)
    kvs::Matrix<T> m_eigen_vectors; ///< eigen vectors as row vectors
    kvs::Vector<T> m_eigen_values; ///< eigen values as vector
    static double m_max_tolerance; ///< tolerance
    static size_t m_max_iterations; ///< maximum number of iterations

public:
    static void SetMaxTolerance( const double tolerance ) { m_max_tolerance = tolerance; }
    static void SetMaxIterations( const size_t iterations ) { m_max_iterations = iterations; }
    static double MaxTolerance() { return m_max_tolerance; }
    static size_t MaxIterations() { return m_max_iterations; }

public:
    EigenDecomposer();
    EigenDecomposer( const kvs::Matrix33<T>& m, MatrixType type = EigenDecomposer::Unknown );
    EigenDecomposer( const kvs::Matrix44<T>& m, MatrixType type = EigenDecomposer::Unknown );
    EigenDecomposer( const kvs::Matrix<T>& m, MatrixType type = EigenDecomposer::Unknown );

    const kvs::Matrix<T>& eigenVectors() const;
    const kvs::Vector<T>& eigenVector( const size_t index ) const;
    const kvs::Vector<T>& eigenValues() const;
    T eigenValue( const size_t index ) const;

    void setMatrix( const kvs::Matrix33<T>& m, MatrixType type = EigenDecomposer::Unknown );
    void setMatrix( const kvs::Matrix44<T>& m, MatrixType type = EigenDecomposer::Unknown );
    void setMatrix( const kvs::Matrix<T>& m, MatrixType type = EigenDecomposer::Unknown );
    void decompose();

private:
    bool calculate_by_qr();
    bool calculate_by_power();
};

} // end of namespace kvs
