/*****************************************************************************/
/**
 *  @file   EigenDecomposer.h
 *  @author Naohisa Sakamoto
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
#include <kvs/Deprecated>


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
    static double m_max_tolerance; ///< tolerance
    static size_t m_max_iterations; ///< maximum number of iterations

public:
    static void SetMaxTolerance( const double tolerance ) { m_max_tolerance = tolerance; }
    static void SetMaxIterations( const size_t iterations ) { m_max_iterations = iterations; }
    static double MaxTolerance() { return m_max_tolerance; }
    static size_t MaxIterations() { return m_max_iterations; }

private:
    MatrixType m_matrix_type; ///< matrix type (symmetric or unsymmetric)
    kvs::Matrix<T> m_eigen_vectors; ///< eigen vectors as row vectors
    kvs::Vector<T> m_eigen_values; ///< eigen values as vector

public:
    EigenDecomposer() {}
    EigenDecomposer( const kvs::Matrix<T>& m, MatrixType type = EigenDecomposer::Unknown );

    const kvs::Matrix<T>& eigenVectors() const { return m_eigen_vectors; }
    const kvs::Vector<T>& eigenVector( const size_t index ) const { return m_eigen_vectors[index]; }
    const kvs::Vector<T>& eigenValues() const { return m_eigen_values; }
    T eigenValue( const size_t index ) const { return m_eigen_values[index]; }

    void setMatrix( const kvs::Matrix<T>& m, MatrixType type = EigenDecomposer::Unknown );
    void decompose();

private:
    bool decompose_symmetric_matrix();
    bool decompose_unsymmetric_matrix();

public:
    KVS_DEPRECATED( EigenDecomposer( const kvs::Matrix33<T>& m, MatrixType type = EigenDecomposer::Unknown ) )
    {
        this->setMatrix( kvs::Matrix<T>(m), type );
        this->decompose();
    }

    KVS_DEPRECATED( EigenDecomposer( const kvs::Matrix44<T>& m, MatrixType type = EigenDecomposer::Unknown ) )
    {
        this->setMatrix( kvs::Matrix<T>(m), type );
        this->decompose();
    }

    KVS_DEPRECATED( void setMatrix( const kvs::Matrix33<T>& m, MatrixType type = EigenDecomposer::Unknown ) )
    {
        m_matrix_type = type;
        m_eigen_values.resize(3);
        m_eigen_vectors = kvs::Matrix<T>(m);
    }

    KVS_DEPRECATED( void setMatrix( const kvs::Matrix44<T>& m, MatrixType type = EigenDecomposer::Unknown ) )
    {
        m_matrix_type = type;
        m_eigen_values.resize(4);
        m_eigen_vectors = kvs::Matrix<T>(m);
    }
};

} // end of namespace kvs
