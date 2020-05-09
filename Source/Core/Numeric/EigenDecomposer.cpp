/*****************************************************************************/
/**
 *  @file   EigenDecomposer.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EigenDecomposer.cpp 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "EigenDecomposer.h"
#include <kvs/LUSolver>
#include <cmath>
#include <numeric>


namespace
{

template <typename T>
inline T HouseholderTransformation( kvs::Vector<T>& vec )
{
    T ret = static_cast<T>( vec.length() );
    if ( !kvs::Math::IsZero( ret ) )
    {
        if ( vec[0] < T(0) ) ret = -ret;
        vec[0] += ret;

        const T t = T(1) / static_cast<T>( std::sqrt( static_cast<double>( vec[0] * ret ) ) );
//        for ( size_t i = 0; i < vec.size(); i++ ) { vec[i] *= t; }
        vec *= t;
    }

    return -ret;
}

template <typename T>
inline void Tridiagonalization( kvs::Matrix<T>& m, kvs::Vector<T>* d, kvs::Vector<T>* e )
{
    KVS_ASSERT( m.isSymmetric() );
    KVS_ASSERT( m.rowSize() >= 3 );
    KVS_ASSERT( d->size() == m.rowSize() );
    KVS_ASSERT( e->size() == m.rowSize() );

    const int dim = static_cast<int>(m.rowSize());
    for ( int k = 0; k < dim - 2; k++ )
    {
        // Copy the k-th row vector of the matrix to 'row_vec' vector.
        kvs::Vector<T> row_vec( m[k] );

        // Copy the k-th element of 'row_vec' vector to the k-th element
        // of the d vector.
        (*d)[k] = row_vec[k];

        // Generate the vector from under the 'k+1'-th elements
        // of the 'row_vec' vector.
        // vec1 = { row_vec[k+1], row_vec[k+2], ... , row_vec[dim-k-1] }
       kvs::Vector<T> vec1( dim - k - 1 );
        for ( size_t i = 0; i < vec1.size(); i++ ) vec1[i] = row_vec[ ( k + 1 ) + i ];

        // Householder reduction of the 'vec1' vector.
        (*e)[k] = HouseholderTransformation<T>( vec1 );
        std::copy( vec1.begin(), vec1.end(), row_vec.begin() + ( k + 1 ) );
        std::copy( vec1.begin(), vec1.end(), m[k].begin() + ( k + 1 ) );
        if ( kvs::Math::IsZero( (*e)[k] ) ) { continue; }

        // Calculate the 'd' vector.
        for ( int i = k + 1; i < dim; i++ )
        {
            T s = T(0);
            for ( int j = k + 1; j < i;   j++ ) s += m[j][i] * row_vec[j];
            for ( int j = i;     j < dim; j++ ) s += m[i][j] * row_vec[j];
            (*d)[i] = s;
        }

        // Generate the vector from under the 'k+1'-th elements
        // of the 'd' vector.
        // vec2 = { d[k+1], d[k+2], ... , d[dim-k-1] }
        kvs::Vector<T> vec2( dim - k - 1 );
        for ( size_t i = 0; i < vec2.size(); i++ ) vec2[i] = (*d)[ ( k + 1 ) + i ];

        // Update the matrix.
        T t = vec1.dot( vec2 ) / T(2);
        for ( int i = dim - 1; i > k; i-- )
        {
            const T p = row_vec[i];
            const T q = (*d)[i] -= t * p;
            for ( int j = i; j < dim; j++ )
            {
                m[i][j] -= p * (*d)[j] + q * row_vec[j];
            }
        }
    }

    // Calculate the 'n-2'-th diagonal element.
    if ( dim >= 2 )
    {
        (*d)[ dim - 2 ] = m[ dim - 2 ][ dim - 2 ];
        (*e)[ dim - 2 ] = m[ dim - 2 ][ dim - 1 ];
    }

    // Calculate the last diagonal element.
    if ( dim >= 1 )
    {
        (*d)[ dim - 1 ] = m[ dim - 1 ][ dim - 1 ];
    }

    for ( int k = dim - 1; k >= 0; k-- )
    {
        // Copy the k-th row vector of the matrix to 'row_vec1' vector.
        kvs::Vector<T> row_vec1( m[k] );

        // Generate the vector from under the k-th elements
        // of the 'row_vec1' vector.
        // vec1 = { row_vec1[k+1], row_vec1[k+2], ... , row_vec1[dim-k-1] }
        kvs::Vector<T> vec1( dim - k - 1 );
        for ( size_t l = 0; l < vec1.size(); l++ ) vec1[l] = row_vec1[ ( k + 1 ) + l ];

        if ( k < dim - 2 )
        {
            for ( int i = k + 1; i < dim; i++ )
            {
                // Copy the i-th row vector of the matrix to 'row_vec2' vector.
                kvs::Vector<T> row_vec2( m[i] );

                // Generate the vector from under the k-th elements
                // of the 'row_vector2' vector.
                // vec2 = { row_vec2[k+1], row_vec2[k+2], ... , row_vec2[dim-k-1] }
                kvs::Vector<T> vec2( dim - k - 1 );
                for ( size_t l = 0; l < vec2.size(); l++ ) vec2[l] = row_vec2[ ( k + 1 ) + l ];

                const T t = vec1.dot( vec2 );
                for ( int j = k + 1; j < dim; j++ ) m[i][j] -= t * row_vec1[j];
            }
        }

        for ( int i = 0; i < dim; i++ ) m[k][i] = T(0);
        m[k][k] = T(1);
    }
}

template <typename T>
inline void HessenbergDecomposition( kvs::Matrix<T>& A )
{
    const size_t dim = A.rowSize();
    for ( size_t k = 1; k <= dim-2; ++k )
    {
        kvs::Vector<T> u( dim );
        for ( size_t i = k; i < dim; ++i ) u[i] = A[i][k-1];

        T ss = 0; for ( size_t i = k + 1; i < dim; ++i ) ss += u[i] * u[i];
        if ( abs(ss) <= 0.0 ) continue;

        T s = sqrt( ss + u[k]*u[k] );
        if ( u[k] > 0.0 ) s = -s;

        u[k] -= s;
        T uu = sqrt( ss + u[k]*u[k] );
        for ( size_t i = k; i < dim; ++i ) u[i] /= uu;

        kvs::Vector<T> f( dim );
        kvs::Vector<T> g( dim );
        for ( size_t i = 0; i < dim; ++i )
        {
            for ( size_t j = k; j < dim; ++j )
            {
                f[i] += A[i][j] * u[j];
                g[i] += A[j][i] * u[j];
            }
        }

        T gamma = u.dot( g );
        for ( size_t i = 0; i < dim; ++i)
        {
            f[i] -= gamma * u[i];
            g[i] -= gamma * u[i];
        }

        for ( size_t i = 0; i < dim; ++i )
        {
            for ( size_t j = 0; j < dim; ++j )
            {
                A[i][j] = A[i][j] - 2*u[i]*g[j] - 2*f[i]*u[j];
            }
        }
    }
}

template <typename T>
inline void InverseIteration( kvs::Matrix<T> A, const T eval, kvs::Vector<T>& evec )
{
    const double max_tolerance = kvs::EigenDecomposer<T>::MaxTolerance();

    const size_t dim = A.rowSize();
    kvs::Vector<T> y( dim ); y[0] = T(1);
    for ( size_t i = 0; i < dim; ++i ) { A[i][i] -= eval; }

    kvs::LUDecomposer<T> lu( A );
    kvs::LUSolver<T> solver( lu );

    T mu, v2, v2s;
    do
    {
        const kvs::Vector<T> v = solver.solve( y ); // A v = y
        mu = v.dot( y );
        v2 = v.dot( v );
        v2s = sqrt(v2);
        for ( size_t j = 0; j < dim; ++j ) y[j] = v[j] / v2s;
    }
    while ( std::abs( 1.0 - mu * mu / v2 ) > max_tolerance );

    evec = y;
}

template <typename T>
inline bool TridiagonalQRMethod( kvs::Matrix<T>& evecs, kvs::Vector<T>& evals, kvs::Vector<T>& e )
{
    const double max_tolerance = kvs::EigenDecomposer<T>::MaxTolerance();
    const size_t max_iterations = kvs::EigenDecomposer<T>::MaxIterations();

    const size_t dim = evecs.rowSize();
    for ( int h = static_cast<int>(dim) - 1; h > 0; h-- )
    {
        int j = h;
        while ( std::fabs( (double)(e[j]) ) >
                max_tolerance * ( std::fabs( (double)(evals[j-1])) + std::fabs( (double)(evals[j]) ) ) )
        { j--; if ( j == 0 ) break; }
        if ( j == h ) continue;

        size_t iter = 0;
        do
        {
            if ( ++iter > max_iterations ) return false;

            T w = ( evals[h-1] - evals[h] ) / T(2);
            T t = e[h] * e[h];
            T s = static_cast<T>( std::sqrt( (double)( w * w + t ) ) ); s = w < T(0) ? -s : s;
            T x = evals[j] - evals[h] + ( t / ( w + s ) );
            T y = e[j+1];
            for ( int k = j; k < h; k++ )
            {
                T c = T(0);
                if ( kvs::Math::Abs(x) >= kvs::Math::Abs(y) )
                {
                    t = -y / x;
                    c = T(1) / static_cast<T>( std::sqrt( (double)( t * t ) + 1.0 ) );
                    s = t * c;
                }
                else
                {
                    t = -x / y;
                    s = T(1) / static_cast<T>( std::sqrt( (double)( t * t ) + 1.0 ) );
                    c = t * s;
                }
                w = evals[k] - evals[k+1];
                t = ( w  *s + T(2) * c * e[k+1] ) * s;

                evals[k] -= t;
                evals[k+1] += t;

                e[k] = k > j ? c * e[k] - s * y : e[k];
                e[k+1] += s * ( c * w - T(2) * s * e[k+1] );

                // Following 7-lines is required for solving the eigen vectors.
                for ( size_t i = 0; i < dim; i++ )
                {
                    x = evecs[k][i];
                    y = evecs[k+1][i];
                    evecs[k][i] = c * x - s * y;
                    evecs[k+1][i] = s * x + c * y;
                }

                if ( k < h - 1 )
                {
                    x = e[k+1];
                    y = -s * e[k+2];
                    e[k+2] *= c;
                }
            }
        }
        while ( std::fabs( (double)(e[h]) ) >
                max_tolerance * ( std::fabs( (double)(evals[h-1]) ) + std::fabs( (double)(evals[h] ) ) ) );
    }

    return true;
}

template <typename T>
inline bool HessenbergQRMethod( kvs::Matrix<T>& evecs, kvs::Vector<T>& evals, kvs::Matrix<T> A )
{
    const double max_tolerance = kvs::EigenDecomposer<T>::MaxTolerance();
    const size_t max_iterations = kvs::EigenDecomposer<T>::MaxIterations();

    ::HessenbergDecomposition( A );

    const int dim = A.rowSize();
    kvs::Vector<T> s( dim );
    kvs::Vector<T> c( dim );
    size_t iter = 0;
    for ( int m = dim; m >= 2; )
    {
        if ( std::abs( A[m-1][m-2] ) < max_tolerance ) { --m; iter = 0; continue; }
        if ( ++iter > max_iterations ) { return false; }

        const T shift = A[m-1][m-1];
        for ( int i = 0; i < m; ++i ) { A[i][i] -= shift; }
        for ( int k = 0; k < m-1; ++k )
        {
            const T a = A[k][k], b = A[k+1][k], r = std::sqrt(a*a + b*b);
            s[k] = r == T(0) ? T(0) : b/r;
            c[k] = r == T(0) ? T(0) : a/r;
            for ( int j = k; j < m; ++j )
            {
                const T x = A[k][j], y = A[k+1][j];
                A[ k ][j] =  c[k] * x + s[k] * y;
                A[k+1][j] = -s[k] * x + c[k] * y;
            }
        }
        for ( int k = 0; k < m-1; ++k )
        {
            for ( int i = 0; i <= k+1; ++i)
            {
                const T x = A[i][k], y = A[i][k+1];
                A[i][ k ] =  c[k] * x + s[k] * y;
                A[i][k+1] = -s[k] * x + c[k] * y;
            }
        }
        for ( int i = 0; i < m; ++i) { A[i][i] += shift; }
    }

    for ( int i = 0; i < dim; ++i ) { evals[i] = A[i][i]; }

    return true;
}

template <typename T>
inline void EigSort( kvs::Matrix<T>& evecs, kvs::Vector<T>& evals )
{
    // Sorting eigen values and vectors.
    const size_t dim = evecs.rowSize();
    for ( size_t k = 0; k < dim - 1; k++ )
    {
        // Search maximum value and index.
        size_t max_index = k;
        T max_value = evals[ max_index ];
        for ( size_t i = k + 1; i < dim; i++ )
        {
            if ( evals[i] > max_value )
            {
                max_index = i;
                max_value = evals[i];
            }
        }

        // Swap the row-vector.
        if ( max_index != k )
        {
            evals[ max_index ] = evals[k];
            evals[k] = max_value;
            for ( size_t j = 0; j < dim; j++ )
            {
                T temp = evecs[max_index][j];
                evecs[max_index][j] = evecs[k][j];
                evecs[k][j] = temp;
            }
        }
    }
}

template <typename T>
inline bool PowerMethod( const kvs::Matrix<T>& m, T& eval, kvs::Vector<T>& evec )
{
    const double max_tolerance = kvs::EigenDecomposer<T>::MaxTolerance();
    const size_t max_iterations = kvs::EigenDecomposer<T>::MaxIterations();

    // Initial eigen value and vector.
    eval = T(1) + T(2) * static_cast<T>( max_tolerance );
    evec.setOnes();

    // Temporally eigen value and vector
    T eval0 = eval;
    kvs::Vector<T> evec0( evec.size() );

    // Estimate eigen value and vector.
    for ( size_t i = 0; i < max_iterations; i++ )
    {
        evec.normalize();
        evec0 = m * evec;
        eval0 = eval;
        eval = evec.dot( evec0 );
        evec = evec0.normalized();
        if ( kvs::Math::Abs( eval - eval0 ) < max_tolerance ) { return true; }
    }

    return false;
}

} // end of namespace


namespace kvs
{

// Initialize tolerance and iterations for eigen value calculation
template <typename T> double EigenDecomposer<T>::m_max_tolerance = 1.0e-10; ///< tolerance
template <typename T> size_t EigenDecomposer<T>::m_max_iterations = 1000; ///< maximum number of iterations

/*===========================================================================*/
/**
 *  @brief  Constructs and decomposes eigen-values and vectors.
 *  @param  m [in] MxM matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
EigenDecomposer<T>::EigenDecomposer( const kvs::Matrix<T>& m, MatrixType type )
{
    this->setMatrix( m, type );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Set a decomposed matrix.
 *  @param  m [in] MxM matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
void EigenDecomposer<T>::setMatrix( const kvs::Matrix<T>& m, MatrixType type )
{
    m_matrix_type = type;
    m_eigen_values.resize( m.rowSize() );
    m_eigen_vectors = m;
}

/*===========================================================================*/
/**
 *  @brief  Decompose eigne valuse and vectors.
 */
/*===========================================================================*/
template <typename T>
void EigenDecomposer<T>::decompose()
{
    switch( m_matrix_type )
    {
    case EigenDecomposer::Symmetric:
        this->decompose_symmetric_matrix();
        break;
    case EigenDecomposer::Unsymmetric:
        this->decompose_unsymmetric_matrix();
        break;
    default:
        // EigenDecomposer::Unknown
        if ( m_eigen_vectors.isSymmetric() )
        {
            this->decompose_symmetric_matrix();
        }
        else
        {
            this->decompose_unsymmetric_matrix();
        }
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates all eigen values and its eigen vectors with tridiagonal QR iteration.
 */
/*===========================================================================*/
template <typename T>
bool EigenDecomposer<T>::decompose_symmetric_matrix()
{
    KVS_ASSERT( m_eigen_vectors.isSymmetric() );
    KVS_ASSERT( m_eigen_vectors.rowSize() >= 3 );

    // Tridiagonalization.
    const size_t dim = m_eigen_vectors.rowSize();
    kvs::Vector<T> e( dim );
    ::Tridiagonalization<T>( m_eigen_vectors, &m_eigen_values, &e );
    for ( int i = static_cast<int>(dim) - 1; i > 0; i-- ) { e[i] = e[i-1]; } e[0] = T(0);

    // QR factorization for the tridiagonal matrix.
    if ( !::TridiagonalQRMethod( m_eigen_vectors, m_eigen_values, e ) ) { return false; }

    // Sorting eigen pairs.
    ::EigSort( m_eigen_vectors, m_eigen_values );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Calculates all eigen values and its eigen vectors with hessenberg QR iteration.
 */
/*===========================================================================*/
template <typename T>
bool EigenDecomposer<T>::decompose_unsymmetric_matrix()
{
    KVS_ASSERT( m_eigen_vectors.rowSize() == m_eigen_vectors.columnSize() );

    // QR factorization.
    const size_t dim = m_eigen_vectors.rowSize();
    kvs::Matrix<T> m( m_eigen_vectors );
    if ( !::HessenbergQRMethod( m_eigen_vectors, m_eigen_values, m ) ) { return false; }

    // Inverse iteration for calculating eigen vectors with the eigen values.
    for ( size_t i = 0; i < dim; i++ )
    {
        ::InverseIteration( m, m_eigen_values[i], m_eigen_vectors[i] );
    }

    // Sorting eigen pairs.
    ::EigSort( m_eigen_vectors, m_eigen_values );

    return true;
}

// Template instantiation.
template class EigenDecomposer<float>;
template class EigenDecomposer<double>;

} // end of namespace kvs
