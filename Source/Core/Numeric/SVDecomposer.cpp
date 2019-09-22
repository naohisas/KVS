/*****************************************************************************/
/**
 *  @file   SVDecomposer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SVDecomposer.cpp 1385 2012-12-04 03:25:29Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "SVDecomposer.h"
#include <cmath>
#include <kvs/Macro>
#include <kvs/Math>


namespace kvs
{

template <typename T> size_t SVDecomposer<T>::m_max_iterations = 30;

/*===========================================================================*/
/**
 *  @brief  Sets a maximum number of iterations.
 *  @param  max_iterations [in] maximum number of iterations
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::SetMaxIterations( const size_t max_iterations )
{
    m_max_iterations = max_iterations;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVDecomposer class.
 */
/*===========================================================================*/
template <typename T>
SVDecomposer<T>::SVDecomposer()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVDecomposer class.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
SVDecomposer<T>::SVDecomposer( const kvs::Matrix33<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVDecomposer class.
 *  @param  m [in] 4x4 matrix
 */
/*===========================================================================*/
template <typename T>
SVDecomposer<T>::SVDecomposer( const kvs::Matrix44<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SVDecomposer class.
 *  @param  m [in] MxM matrix
 */
/*===========================================================================*/
template <typename T>
SVDecomposer<T>::SVDecomposer( const Matrix<T>& m )
{
    this->setMatrix( m );
    this->decompose();
}

/*===========================================================================*/
/**
 *  @brief  '=' operator for the SVDecomposer class.
 *  @param  s [in] SVDecomposer
 */
/*===========================================================================*/
template <typename T>
SVDecomposer<T>& SVDecomposer<T>::operator = ( const SVDecomposer<T>& s )
{
    m_u = s.m_u;
    m_w = s.m_w;
    m_v = s.m_v;

    return( *this );
}

/*===========================================================================*/
/**
 *  @brief  Returns the U matrix.
 *  @return U matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& SVDecomposer<T>::U() const
{
    return( m_u );
}

/*===========================================================================*/
/**
 *  @brief  Returns the W vector.
 *  @return W vector
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& SVDecomposer<T>::W() const
{
    return( m_w );
}

/*===========================================================================*/
/**
 *  @brief  Returns the V matrix.
 *  @return V matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& SVDecomposer<T>::V() const
{
    return( m_v );
}

/*===========================================================================*/
/**
 *  @brief  Returns the left singular matrix.
 *  @return left singular matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& SVDecomposer<T>::leftSingularMatrix() const
{
    return( m_u );
}

/*===========================================================================*/
/**
 *  @brief  Returns the singular values.
 *  @return singular values
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& SVDecomposer<T>::singularValues() const
{
    return( m_w );
}

/*===========================================================================*/
/**
 *  @brief  Returns the right singular matrix.
 *  @return right singular matrix
 */
/*===========================================================================*/
template <typename T>
const kvs::Matrix<T>& SVDecomposer<T>::rightSingularMatrix() const
{
    return( m_v );
}

/*===========================================================================*/
/**
 *  @brief  Sets a 3x3 matrix.
 *  @param  m [in] 3x3 matrix
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::setMatrix( const kvs::Matrix33<T>& m )
{
    m_w.resize( 3 );
    m_v.resize( 3, 3 );
    m_u.resize( 3, 3 );
    for ( size_t i = 0; i < 3; i++ )
    {
        for ( size_t j = 0; j < 3; j++ )
        {
            m_u[i][j] = m[i][j];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a 4x4 matrix.
 *  @param  m [in] 4x4 matrix
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::setMatrix( const kvs::Matrix44<T>& m )
{
    m_w.resize( 4 );
    m_v.resize( 4, 4 );
    m_u.resize( 4, 4 );
    for ( size_t i = 0; i < 4; i++ )
    {
        for ( size_t j = 0; j < 4; j++ )
        {
            m_u[i][j] = m[i][j];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a MxM matrix.
 *  @param  m [in] MxM matrix
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::setMatrix( const Matrix<T>& m )
{
    m_u = m;
    m_w.resize( m.columnSize() );
    m_v.resize( m.columnSize(), m.columnSize() );
}

/*===========================================================================*/
/**
 *  @brief  Decompose.
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::decompose()
{
    int row    = m_u.rowSize();
    int column = m_u.columnSize();

    kvs::Vector<T> rv1( column );

    int l  = 0;
    int nm = 0;
    int its;
    int flag;

    T c, f, h, s, x, y, z;
    T g     = T(0);
    T scale = T(0);
    T anorm = T(0);

    // Householder reduction to bidiagonal form
    for( int i = 0; i < column; i++ )
    {
        rv1[i] = scale * g;

        l     = i+1;
        g     = T(0);
        scale = T(0);
        s     = T(0);

        if( i < row )
        {
            for( int k = i; k < row; k++ ) scale += kvs::Math::Abs( m_u[k][i] );

            if( !kvs::Math::IsZero( scale ) )
            {
                for( int k = i; k < row; k++ )
                {
                    m_u[k][i] /= scale;
                    s += m_u[k][i] * m_u[k][i];
                }// end of for-loop 'k'

                f = m_u[i][i];
                g = -kvs::Math::Sgn( static_cast<T>(std::sqrt((double)s)), f );
                h = f * g - s ;
                m_u[i][i] = f - g ;

                for( int j = l; j < column; j++ )
                {
                    s = T(0);
                    for( int k = i; k < row; k++ ) s += m_u[k][i] * m_u[k][j];
                    f = s / h ;
                    for( int k = i; k < row; k++ ) m_u[k][j] += f * m_u[k][i];
                } // end of for-loop 'j'

                for( int k = i ; k < row; k++ ) m_u[k][i] *= scale ;
            } // if scale
        } // if i <= m

        m_w[i] = scale * g ;
        g = s = scale = T(0);

        if( i < row && i != ( column - 1 ) )
        {
            for( int k = l ; k < column; k++ ) scale += kvs::Math::Abs( m_u[i][k] ) ;

            if( !kvs::Math::IsZero( scale ) )
            {
                for( int k = l; k < column; k++ )
                {
                    m_u[i][k] /= scale ;
                    s += m_u[i][k] * m_u[i][k];
                } // for k

                f = m_u[i][l];
                g = -kvs::Math::Sgn( static_cast<T>(std::sqrt((double)s)), f );
                h = f * g - s ;
                m_u[i][l] = f - g ;

                for( int k = l; k < column; k++ ) rv1[k] = m_u[i][k] / h ;
                for( int j = l; j < row;    j++ )
                {
                    s = T(0);
                    for( int k = l; k < column; k++ ) s += m_u[j][k] * m_u[i][k];
                    for( int k = l; k < column; k++ ) m_u[j][k] += s * rv1[k];
                } // for j

                for( int k = l; k < column; k++ ) m_u[i][k] *= scale;
            } // if scale
        } // if i != m && i != n

        anorm = kvs::Math::Max( anorm, kvs::Math::Abs(m_w[i]) + kvs::Math::Abs(rv1[i]) );
    } // for i

    // Accumulation of right-hand transformations.
    for( int i = column - 1; i >= 0; i-- )
    {
        if( i < column )
        {
            if( !kvs::Math::IsZero( g ) )
            {
                for( int j = l; j < column; j++ )
                    m_v[j][i] = ( m_u[i][j] / m_u[i][l] ) / g ;
                // double division to reduce underflow
                for( int j = l ; j < column; j++ )
                {
                    s = T(0);
                    for( int k = l; k < column; k++ ) s += m_u[i][k] * m_v[k][j];
                    for( int k = l; k < column; k++ ) m_v[k][j] += s * m_v[k][i];
                } // for j
            } // if g
            for( int j = l; j < column; j++ ) m_v[i][j] = m_v[j][i] = T(0);
        } // if i < n

        m_v[i][i] = T(1);
        g = rv1[i];
        l = i;

    } // for i

    for( int i = kvs::Math::Min( row, column ) - 1; i >= 0; i-- )
    {
        l = i + 1 ;
        g = m_w[i] ;
        for( int j = l; j < column; j++ ) m_u[i][j] = T(0);

        if( !kvs::Math::IsZero( g ) )
        {
            g = T(1) / g ;
            for( int j = l; j < column; j++ )
            {
                s = T(0);
                for( int k = l; k < row; k++ ) s += m_u[k][i] * m_u[k][j];
                f = ( s / m_u[i][i] ) * g;
                for( int k = i; k < row; k++ ) m_u[k][j] += f * m_u[k][i];
            } // for j

            for( int j = i; j < row; j++ ) m_u[j][i] *= g ;
        }
        else
        {
            for( int j = i; j < row; j++ ) m_u[j][i] = T(0) ;
        }
        ++ m_u[i][i];
    } // for i

    //Diagonalization of the bidiagonal form; Loop over
    for( int k = column - 1; k >= 0; k-- )
    {
        // singular values, and over allowed iterations.
        const int max_iterations = static_cast<int>( m_max_iterations );
        for( its = 0 ; its < max_iterations; its++ )
        {
            // Test for splitting.
            flag = 1;
            for( l = k; l >= 0; l-- )
            {
                nm = l - 1 ; // Note that rv1[1] is always zero.
                if( kvs::Math::Abs( rv1[l] ) + anorm == anorm )
                {
                    flag = 0 ;
                    break;
                }
                if( kvs::Math::Abs( m_w[nm] ) + anorm == anorm ) break;
            } // for l

            if( flag )
            {
                c = T(0);
                s = T(1);
                for( int i = l; i <= k; i++ )
                {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];

                    if( kvs::Math::Abs( f ) + anorm == anorm ) break;

                    g      = m_w[i];
                    h      = kvs::Math::Pythag( f, g );
                    m_w[i] = h;
                    h      = T(1) / h ;
                    c      = g * h ;
                    s      = -f * h;

                    for( int j = 0 ; j < row; j++ )
                    {
                        y = m_u[j][nm];
                        z = m_u[j][i];
                        m_u[j][nm] = y * c + z * s;
                        m_u[j][i]  = z * c - y * s;
                    } // for j
                } // for i
            } // if flag

            // Convergence
            z = m_w[k];
            if( l == k )
            {
                // singular value is made non negative.
                if( z < T(0) )
                {
                    m_w[k] = -z;
                    for ( int j = 0; j < column; j++ ) m_v[j][k] = -m_v[j][k];
                } // if z < 0
                break;
            } // if l == k

            // Not converged.
            KVS_ASSERT( its != static_cast<int>( m_max_iterations - 1 ) );

            x  = m_w[l];
            nm = k - 1 ;
            y  = m_w[nm] ;
            g  = rv1[nm] ;
            h  = rv1[k] ;
            f  = ( (y-z)*(y+z) + (g-h)*(g+h) ) / ( T(2) * h * y ) ;
            g  = kvs::Math::Pythag( f, T(1) );
            f  = ( (x-z)*(x+z) + h * ( ( y / ( f + kvs::Math::Sgn(g,f) ) ) - h ) ) / x ;
            c  = T(1);
            s  = T(1);

            // Next QR transformation;
            for( int j = l; j <= nm; j++ )
            {
                int i = j + 1 ;

                g      = rv1[i];
                y      = m_w[i];
                h      = s * g;
                g      = c * g;
                z      = kvs::Math::Pythag( f, h );
                rv1[j] = z ;
                c      = f / z ;
                s      = h / z ;
                f      = x * c + g * s ;
                g      = g * c - x * s ;
                h      = y * s ;
                y      *= c;

                for( int jj = 0; jj < column; jj++ )
                {
                    x = m_v[jj][j];
                    z = m_v[jj][i];
                    m_v[jj][j] = x * c + z * s;
                    m_v[jj][i] = z * c - x * s;
                } // for jj

                z = kvs::Math::Pythag( f, h );
                m_w[j] = z;

                // Rotation can be arbitrary if z =0;
                if( z )
                {
                    z = T(1) / z;
                    c = f * z;
                    s = h * z;
                } // if

                f = ( c * g ) + ( s * y );
                x = ( c * y ) - ( s * g );
                for( int jj = 0; jj < row; jj++ )
                {
                    y = m_u[jj][j];
                    z = m_u[jj][i];
                    m_u[jj][j] = y * c + z * s;
                    m_u[jj][i] = z * c - y * s;
                } // for jj
            } // for j

            rv1[l] = T(0);
            rv1[k] = f;
            m_w[k] = x;

        } // for its
    } // for k

    SVDecomposer<T>::sort( &m_u, &m_v, &m_w );
}

/*===========================================================================*/
/**
 *  @brief  Sort the left singular matrix, right singular matrix and the singular values. (descending order)
 *  @param  umat [out] pointer to the left singular matrix
 *  @param  vmat [out] pointer to right singular matrix
 *  @param  wvec [out] pointer to singular values
 */
/*===========================================================================*/
template <typename T>
void SVDecomposer<T>::sort( kvs::Matrix<T>* umat, kvs::Matrix<T>* vmat, kvs::Vector<T>* wvec )
{
    int dim = umat->rowSize();

    for( int k = 0; k < dim - 1; k++ )
    {
        // Search maximum value and index.
        int max_index = k;
        T   max_value = (*wvec)[ max_index ];
        for( int i = k + 1; i < dim; i++ )
        {
            if( (*wvec)[i] > max_value )
            {
                max_index = i;
                max_value = (*wvec)[i];
            }
        }

        // Swap the row-vector.
        if( max_index != k )
        {
            (*wvec)[ max_index ] = (*wvec)[k];
            (*wvec)[k]           = max_value;
            for( int j = 0; j < dim; j++ )
            {
                T temp_u              = (*umat)[j][max_index];
                (*umat)[j][max_index] = (*umat)[j][k];
                (*umat)[j][k]         = temp_u;

                T temp_v              = (*vmat)[j][max_index];
                (*vmat)[j][max_index] = (*vmat)[j][k];
                (*vmat)[j][k]         = temp_v;
            }
        }
    }
}

template <typename T>
void SVDecomposer<T>::correctSingularValues()
{
    int column = m_u.columnSize();

    // Editing of the singular values.
    T w_max = m_w[0];
    T w_min = w_max * T( KVS__MATH_TINY_VALUE );
    for( int i = 0; i < column; i++ )
    {
        if( m_w[i] < w_min ) m_w[i] = T(0);
    }
}

// template instantiation
template class SVDecomposer<float>;
template class SVDecomposer<double>;

} // end of namespace kvs
