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
#include <cmath>


namespace
{

template <typename T>
inline bool IsSymmetricMatrix( const kvs::Matrix<T>& m )
{
    const size_t nrows = m.rowSize();
    const size_t ncolumns = m.columnSize();
    if ( nrows != ncolumns ) return false;

    for ( size_t i = 0; i < nrows; i++ )
    {
        for ( size_t j = i + 1; j < ncolumns; j++ )
        {
            if ( !kvs::Math::Equal( m[i][j], m[j][i] ) ) return false;
        }
    }

    return true;
}

template <typename T>
inline const kvs::Vector<T> Normalize( const kvs::Vector<T>& vec )
{
    kvs::Vector<T> result( vec );
    result.normalize();

    return result;
}

template <typename T>
inline T HouseholderTransformation( kvs::Vector<T>& vec )
{
    T ret = static_cast<T>( vec.length() );
    if ( !kvs::Math::IsZero( ret ) )
    {
        if ( vec[0] < T(0) ) ret = -ret;
        vec[0] += ret;

        const T t = T(1) / static_cast<T>( std::sqrt( static_cast<double>( vec[0] * ret ) ) );
        for ( size_t i = 0; i < vec.size(); i++ ) { vec[i] *= t; }
    }

    return -ret;
}

template <typename T>
inline void Tridiagonalize( kvs::Matrix<T>& m, kvs::Vector<T>* d, kvs::Vector<T>* e )
{
    KVS_ASSERT( ::IsSymmetricMatrix<T>( m ) );
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
        for ( size_t i = 0; i < vec1.size(); i++ ) row_vec[ ( k + 1 ) + i ] = vec1[i];
        for ( size_t i = 0; i < vec1.size(); i++ ) m[k][ ( k + 1 ) + i ] = vec1[i];
        if ( kvs::Math::IsZero( (*e)[k] ) ) continue;

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
            T p = row_vec[i];
            T q = (*d)[i] -= t * p;
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
inline bool QRMethod( kvs::Matrix<T>& evecs, kvs::Vector<T>& evals, kvs::Vector<T>& e )
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
inline bool PowerMethod( const kvs::Matrix<T>& m, T& eval, kvs::Vector<T>& evec )
{
    const double max_tolerance = kvs::EigenDecomposer<T>::MaxTolerance();
    const size_t max_iterations = kvs::EigenDecomposer<T>::MaxIterations();

    // Initial eigen value and vector.
    eval = T(1);
    for ( size_t i = 0; i < evec.size(); i++ ) { evec[i] = T(1); }

    // Temporally eigen value and vector
    T eval0 = eval + T(2) * static_cast<T>( max_tolerance );
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
  *  @brief  Constructs a new eigen-value deompositor class.
  */
 /*===========================================================================*/
template <typename T>
EigenDecomposer<T>::EigenDecomposer()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs and decomposes eigen-values and vectors.
 *  @param  m [in] 3x3 matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
EigenDecomposer<T>::EigenDecomposer( const kvs::Matrix33<T>& m, MatrixType type )
{
    this->setMatrix( m, type );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs and decomposes eigen-values and vectors.
 *  @param  m [in] 4x4 matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
EigenDecomposer<T>::EigenDecomposer( const kvs::Matrix44<T>& m, MatrixType type )
{
    this->setMatrix( m, type );
    this->decompose();
}

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
 *  @brief  Returns the eigen vectors as row vectors of the matrix.
 *  @return eigen vectors
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& EigenDecomposer<T>::eigenVectors() const
{
    return m_eigen_vectors;
}

/*===========================================================================*/
/**
 *  @brief  Returns the eigen vector that is specified by the given index.
 *  @param  index [in] index
 *  @return eigen vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& EigenDecomposer<T>::eigenVector( const size_t index ) const
{
    return m_eigen_vectors[index];
}

/*===========================================================================*/
/**
 *  @brief  Returns the eigen values as elemnts of the vector.
 *  @return eigen valus
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& EigenDecomposer<T>::eigenValues() const
{
    return m_eigen_values;
}

/*===========================================================================*/
/**
 *  @brief  Returns the eigen value that is specified by the given index.
 *  @param  index [in] index
 *  @return eigen value
 */
/*===========================================================================*/
template <typename T>
T EigenDecomposer<T>::eigenValue( const size_t index ) const
{
    return m_eigen_values[index];
}

/*===========================================================================*/
/**
 *  @brief  Set a decomposed matrix.
 *  @param  m [in] 3x3 matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
void EigenDecomposer<T>::setMatrix( const kvs::Matrix33<T>& m, MatrixType type )
{
    m_matrix_type = type;
    m_eigen_values.setSize( 3 );
    m_eigen_vectors.setSize( 3, 3 );
    for ( size_t i = 0; i < 3; i++ )
    {
        for ( size_t j = 0; j < 3; j++ )
        {
            m_eigen_vectors[i][j] = m[i][j];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Set a decomposed matrix.
 *  @param  m [in] 4x4 matrix
 *  @param  type [in] matrix type (symmetric or unsymmetric)
 */
/*===========================================================================*/
template <typename T>
void EigenDecomposer<T>::setMatrix( const kvs::Matrix44<T>& m, MatrixType type )
{
    m_matrix_type = type;
    m_eigen_values.setSize( 4 );
    m_eigen_vectors.setSize( 4, 4 );
    for ( size_t i = 0; i < 4; i++ )
    {
        for ( size_t j = 0; j < 4; j++ )
        {
            m_eigen_vectors[i][j] = m[i][j];
        }
    }
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
    m_eigen_values.setSize( m.rowSize() );
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
        this->calculate_by_qr();
        break;
    case EigenDecomposer::Unsymmetric:
        this->calculate_by_power();
        break;
    default: // EigenDecomposer::Unknown
        if ( ::IsSymmetricMatrix<T>( m_eigen_vectors ) )
        {
            this->calculate_by_qr();
        }
        else
        {
            this->calculate_by_power();
        }
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculate eigen values and vectors by using Power method.
 */
/*===========================================================================*/
template <typename T>
bool EigenDecomposer<T>::calculate_by_power()
{
    KVS_ASSERT( m_eigen_vectors.rowSize() == m_eigen_vectors.columnSize() );

#if 0
    const size_t dim = m_eigen_vectors.rowSize();

    kvs::Matrix<T> m( m_eigen_vectors );
    if ( !::PowerMethod( m, m_eigen_values[0], m_eigen_vectors[0] ) ) { return false; }
//    m_eigen_vectors[0].normalize();

    const T R0 = m_eigen_values[0];
    const kvs::Vector<T> U0 = m_eigen_vectors[0];

    T r0 = m_eigen_values[0];
    kvs::Vector<T> u0 = m_eigen_vectors[0];
    kvs::Vector<T> v0 = u0;

    kvs::Vector<T> x = m[0] / ( r0 * u0[0] );
    for ( size_t i = 0; i < dim; i++ ) { m[i] -= r0 * u0[i] * x; }

    for ( size_t k = 1; k < dim; k++ )
    {
        // Power iteration.
        if ( !::PowerMethod( m, m_eigen_values[k], m_eigen_vectors[k] ) ) { return false; }
//        m_eigen_vectors[k].normalize();

        // Delfation.
        T r1 = m_eigen_values[k];
        kvs::Vector<T> u1 = m_eigen_vectors[k];
        kvs::Vector<T> v1 = ( r1 - r0 ) * u1 + r0 * x.dot( u1 ) * v0;
        v1.normalize();

        if ( k > 2 )
        {
            v1 = ( r1 - R0 ) * u1 + R0 * x.dot( u1 ) * U0;
            v1.normalize();
        }

        m_eigen_vectors[k] = v1;

//        x = m[k] / ( r1 * u1[k] );
        x = m[k] / ( r1 * u1[k] );
        for ( size_t i = 0; i < dim; i++ ) { m[i] -= r1 * u1[i] * x; }

        r0 = r1;
        u0 = u1;
        v0 = v1;
    }

    return true;
#else
    const size_t dim = m_eigen_vectors.rowSize();
    kvs::Matrix<T> eigen_vectors( dim, dim );
    kvs::Matrix<T> m( m_eigen_vectors );

    // temporally vectors
    kvs::Vector<T> temp_vec0( dim );
    kvs::Vector<T> temp_vec1( dim );
    kvs::Vector<T> temp_vec2( dim );
    for ( size_t i = 0; i < dim; i++ ) temp_vec0[i] = T(1);

    // temporally matrices
    kvs::Matrix<T> temp_mat0( dim, dim );
    kvs::Matrix<T> temp_mat1( dim, dim );
    for ( size_t i = 0; i < dim; i++ ) temp_mat0[i][i] = T(1);

    // Calculate the eigen values and the eigen matrices.
    //
    // NOTE: 'number_of_eigen_values' is a parameter which is number of eigen
    // values that we want to get. If we want to get a maximum eigen value and
    // a eigen vector only, we specify 'number_of_eigen_values = 1'. By doing
    // this, we can get the maximum eigen value and matrix fastly. In contrast,
    // 'number_of_eigen_values = dim' means that all eigen values and vectors
    // are calculated. Where 'dim' is a dimension of given matrix.
    size_t number_of_calculated_eigen_values = 0;
    const size_t number_of_eigen_values = dim;
    for ( size_t k = 0; k < number_of_eigen_values; k++ )
    {
        double length1 = temp_vec0.length();
        double length2 = 0.0;

        temp_vec1 = temp_vec0;

        size_t iter = 0;
        while ( iter < m_max_iterations )
        {
            // Modify the rounding error.
            if ( iter % 15 == 0 )
            {
                temp_vec2 = temp_mat0 * temp_vec1;
                temp_vec1 = temp_vec2;
                temp_vec1.normalize();
            }

            temp_vec2 = m_eigen_vectors * temp_vec1;
            length2 = temp_vec2.length();
            temp_vec2.normalize();

            // Convergence test
            if ( std::fabs( ( length2 - length1 ) / length1 ) < m_max_tolerance )
            {
                // converged
                int k1 = -1;
                for ( size_t i = 0; i < dim && k1 < 0; i++ )
                {
                    if ( std::fabs( static_cast<double>( temp_vec2[i] ) ) > 0.001 )
                    {
                        k1 = static_cast<int>(i);
                        if ( temp_vec2[i] * temp_vec1[i] < T(0) ) length2 = -length2;
                    }
                }

                m_eigen_values[k] = static_cast<T>( length2 ); // set calculated eigen value
                eigen_vectors[k] = temp_vec2; // set calculated eigen vector

                if ( k != dim - 1 )
                {
                    kvs::Matrix<T> diag( dim, dim );
                    for ( size_t i = 0; i < dim; i++ ) diag[i][i] = static_cast<T>( length2 );

                    kvs::Matrix<T> x( m_eigen_vectors - diag );
                    temp_mat1 = x * temp_mat0;
                    temp_mat0 = temp_mat1;
                    temp_vec1 = temp_mat0 * temp_vec0;
                    if ( !kvs::Math::IsZero( temp_vec1.length() ) ) temp_vec0 = temp_vec1;
                }

                break;
            }
            else
            {
                // not converged
                temp_vec1 = temp_vec2;
                length1 = length2;
                iter++;
            }
        } // end of 'while( iter < m_max_iterations )'
        if ( iter < m_max_iterations ) number_of_calculated_eigen_values++;
    } // end of 'for( int k = 0; k < number_of_eigen_values; k++ )'

    m_eigen_vectors = eigen_vectors;

    return true;
#endif
}

/*===========================================================================*/
/**
 *  @brief  Calculate eigen values and vectors by using QR method.
 */
/*===========================================================================*/
template <typename T>
bool EigenDecomposer<T>::calculate_by_qr()
{
    KVS_ASSERT( ::IsSymmetricMatrix<T>( m_eigen_vectors ) );
    KVS_ASSERT( m_eigen_vectors.rowSize() >= 3 );

    // Tridiagonalize the matrix.
    const size_t dim = m_eigen_vectors.rowSize();
    kvs::Vector<T> e( dim );
    ::Tridiagonalize<T>( m_eigen_vectors, &m_eigen_values, &e );
    for ( int i = static_cast<int>(dim) - 1; i > 0; i-- ) { e[i] = e[i-1]; } e[0] = T(0);

    // Calculate eigen values and vectors using QR method.
    if ( !::QRMethod( m_eigen_vectors, m_eigen_values, e ) ) { return false; }

    // Sorting eigen values and vectors.
    for ( size_t k = 0; k < dim - 1; k++ )
    {
        // Search maximum value and index.
        size_t max_index = k;
        T max_value = m_eigen_values[ max_index ];
        for ( size_t i = k + 1; i < dim; i++ )
        {
            if ( m_eigen_values[i] > max_value )
            {
                max_index = i;
                max_value = m_eigen_values[i];
            }
        }

        // Swap the row-vector.
        if ( max_index != k )
        {
            m_eigen_values[ max_index ] = m_eigen_values[k];
            m_eigen_values[k] = max_value;
            for ( size_t j = 0; j < dim; j++ )
            {
                T temp = m_eigen_vectors[max_index][j];
                m_eigen_vectors[max_index][j] = m_eigen_vectors[k][j];
                m_eigen_vectors[k][j] = temp;
            }
        }
    }

    return true;
}

// Template instantiation.
template class EigenDecomposer<float>;
template class EigenDecomposer<double>;

} // end of namespace kvs
