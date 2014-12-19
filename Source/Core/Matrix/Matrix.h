/****************************************************************************/
/**
 *  @file Matrix.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Matrix.h 1384 2012-12-04 01:26:40Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MATRIX_H_INCLUDE
#define KVS__MATRIX_H_INCLUDE

#include <iostream>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Vector>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  mxn matrix class.
 */
/*==========================================================================*/
template<typename T>
class Matrix
{
private:
    size_t          m_nrows;    ///< Number of rows.
    size_t          m_ncolumns; ///< Number of columns.
    kvs::Vector<T>* m_rows;     ///< Row vectors.

public:
    Matrix();
    Matrix( const size_t nrows, const size_t ncolumns );
    Matrix( const size_t nrows, const size_t ncolumns, const T* const elements );
    ~Matrix();

public:
    Matrix( const Matrix& other );
    Matrix& operator =( const Matrix& rhs );

public:
    void setSize( const size_t nrows, const size_t ncolumns );

    size_t rowSize() const;
    size_t columnSize() const;

    void zero();
    void identity();
    void swap( Matrix& other );
    void transpose();
    void invert();
    void print() const;

    T trace() const;
    T determinant() const;
    size_t pivot( const size_t row_index ) const;
    const Matrix transposed() const;
    const Matrix inverted() const;

public:
    const kvs::Vector<T>& operator []( const size_t index ) const;
    kvs::Vector<T>&       operator []( const size_t index );

    Matrix& operator +=( const Matrix& rhs );
    Matrix& operator -=( const Matrix& rhs );
    Matrix& operator *=( const Matrix& rhs );
    Matrix& operator *=( const T rhs );
    Matrix& operator /=( const T rhs );

    const Matrix operator -() const;

public:
    friend bool operator ==( const Matrix& lhs, const Matrix& rhs )
    {
        const size_t nrows = lhs.rowSize();
        if ( nrows != lhs.rowSize() )
            return false;

        for ( size_t r = 0; r < nrows; ++r )
        {
            if ( lhs[r] != rhs[r] )
                return false;
        }
        return true;
    }

    friend bool operator !=( const Matrix& lhs, const Matrix& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Matrix operator +( const Matrix& lhs, const Matrix& rhs )
    {
        return Matrix( lhs ) += rhs;
    }

    friend const Matrix operator -( const Matrix& lhs, const Matrix& rhs )
    {
        return Matrix( lhs ) -= rhs;
    }

    friend const Matrix operator *( const Matrix& lhs, const Matrix& rhs )
    {
        KVS_ASSERT( lhs.columnSize() == rhs.rowSize() );

        const size_t L = lhs.rowSize();
        const size_t M = lhs.columnSize();
        const size_t N = rhs.columnSize();

        Matrix result( L, N );

        for ( size_t r = 0; r < L; ++r )
        {
            for ( size_t c = 0; c < N; ++c )
            {
                result[r][c] = T( 0 );
                for ( size_t k = 0; k < M; ++k )
                {
                    result[r][c] += lhs[r][k] * rhs[k][c];
                }
            }
        }

        return result;
    }

    friend const kvs::Vector<T> operator *( const Matrix& lhs, const kvs::Vector<T>& rhs )
    {
        KVS_ASSERT( lhs.columnSize() == rhs.size() );

        const size_t nrows    = lhs.rowSize();
        const size_t ncolumns = lhs.columnSize();

        kvs::Vector<T> result( nrows );

        for ( size_t r = 0; r < nrows; ++r )
        {
            result[r] = T( 0 );
            for ( size_t c = 0; c < ncolumns; ++c )
            {
                result[r] += lhs[r][c] * rhs[c];
            }
        }

        return result;
    }

    friend const kvs::Vector<T> operator *( const kvs::Vector<T>& lhs, const Matrix& rhs )
    {
        KVS_ASSERT( lhs.size() == rhs.rowSize() );

        const size_t nrows    = rhs.rowSize();
        const size_t ncolumns = rhs.columnSize();

        kvs::Vector<T> result( ncolumns );

        for ( size_t c = 0; c < ncolumns; ++c )
        {
            result[c] = T( 0 );
            for ( size_t r = 0; r < nrows; ++r )
            {
                result[c] += lhs[r] * rhs[r][c];
            }
        }

        return result;
    }

    friend const Matrix operator *( const Matrix& lhs, const T rhs )
    {
        return Matrix( lhs ) *= rhs;
    }

    friend const Matrix operator *( const T lhs, const Matrix& rhs )
    {
        return rhs * lhs;
    }

    friend const Matrix operator /( const Matrix& lhs, const T rhs )
    {
        return Matrix( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Matrix& rhs )
    {
        const size_t nrows = rhs.rowSize();
        if ( nrows != 0 )
        {
            for ( size_t r = 0; r < nrows - 1; ++r )
            {
                os <<  rhs[r] << std::endl;
            }
            os << rhs[ nrows - 1 ];
        }
        return os;
    }

public:
    KVS_DEPRECATED( size_t nrows() const ) { return this->rowSize(); }
    KVS_DEPRECATED( size_t ncolumns() const ) { return this->columnSize(); }
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new Matrix.
 */
/*===========================================================================*/
template <typename T>
inline Matrix<T>::Matrix():
    m_nrows( 0 ),
    m_ncolumns( 0 ),
    m_rows( 0 )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncolumns [in] Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline Matrix<T>::Matrix( const size_t nrows, const size_t ncolumns ):
    m_nrows( 0 ),
    m_ncolumns( 0 ),
    m_rows( 0 )
{
    this->setSize( nrows, ncolumns );
    this->zero();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncolumns [in] Number of columns of matrix.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Matrix<T>::Matrix( const size_t nrows, const size_t ncolumns, const T* const elements ):
    m_nrows( 0 ),
    m_ncolumns( 0 ),
    m_rows( 0 )
{
    this->setSize( nrows, ncolumns );

    Vector<T>* const m = m_rows;

    size_t index = 0;
    for ( size_t r = 0; r < nrows; ++r )
    {
        for ( size_t c = 0; c < ncolumns; ++c )
        {
            m[r][c]= elements[ index ];
            ++index;
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Constructs a copy of other.
 *  @param  other [in] Matrix.
 */
/*==========================================================================*/
template <typename T>
inline Matrix<T>::Matrix( const Matrix& other ):
    m_nrows( 0 ),
    m_ncolumns( 0 ),
    m_rows( 0 )
{
    this->setSize( other.rowSize(), other.columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] = other[r];
    }
}

/*==========================================================================*/
/**
 *  @brief  Substitution operator '='.
 *  @param  other [in] Matrix.
 */
/*==========================================================================*/
template <typename T>
inline Matrix<T>& Matrix<T>::operator =( const Matrix& rhs )
{
    this->setSize( rhs.rowSize(), rhs.columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] = rhs[r];
    }
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Matrix.
 */
/*===========================================================================*/
template<typename T>
inline Matrix<T>::~Matrix()
{
    delete [] m_rows;
}

/*==========================================================================*/
/**
 *  @brief  Sets the size of matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncolumns [in] Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::setSize( const size_t nrows, const size_t ncolumns )
{
    if ( this->rowSize() != nrows || this->columnSize() != ncolumns )
    {
        m_nrows    = nrows;
        m_ncolumns = ncolumns;

        delete [] m_rows;
        m_rows = NULL;

        if ( nrows != 0 && ncolumns != 0 )
        {
            m_rows = new kvs::Vector<T>[ nrows ];

            for ( size_t r = 0; r < nrows; ++r )
            {
                m_rows[r].setSize( ncolumns );
            }
        }
    }

    this->zero();
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of rows of matrix.
 *  @return Number of rows of matrix.
 */
/*==========================================================================*/
template<typename T>
inline size_t Matrix<T>::rowSize() const
{
    return m_nrows;
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of columns of matrix.
 *  @return Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline size_t Matrix<T>::columnSize() const
{
    return m_ncolumns;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements to zero.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::zero()
{
    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r].zero();
    }
}

/*==========================================================================*/
/**
 *  @brief  Sets this matrix to an identity matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::identity()
{
    KVS_ASSERT( this->rowSize() == this->columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;

    this->zero();

    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r][r] = T( 1 );
    }
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::swap( Matrix& other )
{
    std::swap( m_nrows, other.m_nrows );
    std::swap( m_ncolumns, other.m_ncolumns );
    std::swap( m_rows, other.m_rows );
}

/*==========================================================================*/
/**
 *  @brief  Transposes this matrix.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::transpose()
{
    const size_t          nrows    = this->rowSize();
    const size_t          ncolumns = this->columnSize();
    kvs::Vector<T>* const m        = m_rows;

    if ( nrows == ncolumns )
    {
        for ( size_t r = 0; r < nrows; ++r )
        {
            for ( size_t c = r + 1; c < ncolumns; ++c )
            {
                std::swap( m[r][c], m[c][r] );
            }
        }
    }
    else
    {
        Matrix result( ncolumns, nrows );

        for ( size_t r = 0; r < nrows; ++r )
        {
            for ( size_t c = 0; c < ncolumns; ++c )
            {
                result[c][r] = m[r][c];
            }
        }

        *this = result;
    }
}

/*==========================================================================*/
/**
 *  @brief  Inverts this matrix.
 *  @param  determinant [out] calculated determinant
 *  @return Inverse matrix.
 *  @todo   Implement an exception processing.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::invert()
{
    KVS_ASSERT( this->rowSize() == this->columnSize() );

    const size_t          size     = this->rowSize();
    const size_t          nrows    = this->rowSize();
    const size_t          ncolumns = this->columnSize();
    kvs::Vector<T>* const m     = m_rows;

    Matrix<T> result( nrows, nrows );
    result.identity();

    for ( size_t k = 0; k < size; k++ )
    {
        // Search a pivot row.
        const size_t pivot_row = this->pivot( k );

        // Swap the k-row and the pivot_row.
        if ( k != pivot_row )
        {
            m[k].swap( m[ pivot_row ] );
            result[k].swap( result[ pivot_row ] );
        }

        // Forward elimination
        const T diagonal_element = m[k][k];

        for ( size_t c = 0; c < ncolumns; ++c )
        {
            m[k][c]      /= diagonal_element;
            result[k][c] /= diagonal_element;
        }

        for ( size_t r = 0; r < nrows; ++r )
        {
            // Skip the pivot_row.
            if ( r != k )
            {
                const T value = m[r][k];
                for( size_t c = 0; c < ncolumns; ++c )
                {
                    m[r][c]      -= value * m[k][c];
                    result[r][c] -= value * result[k][c];
                }
            }
        }
    }
}

/*==========================================================================*/
/**
 *  @bri  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::print() const
{
    std::cout << *this << std::endl;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the trace of this matrix.
 *  @return Trace of this matrix.
 */
/*==========================================================================*/
template<typename T>
inline T Matrix<T>::trace() const
{
    KVS_ASSERT( this->rowSize() == this->columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;

    T result = T( 0 );
    for ( size_t r = 0; r < nrows; ++r )
    {
        result += m[r][r];
    }
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the determinant of this matrix.
 *  @return Determinant of this matrix.
 */
/*==========================================================================*/
template<typename T>
inline T Matrix<T>::determinant() const
{
    KVS_ASSERT( this->rowSize() == this->columnSize() );

    const size_t size     = this->rowSize();
    const size_t nrows    = this->rowSize();
    const size_t ncolumns = this->columnSize();

    Matrix<T> result( *this );
    T det = T( 1 );

    for ( size_t k = 0; k < size; ++k )
    {
        const size_t pivot_row = result.pivot( k );

        if ( k != pivot_row )
        {
            det *= T( -1 );
        }

        det *= result[k][k];

        for ( size_t r = k + 1; r < nrows; ++r )
        {
            const T value = result[r][k] / result[k][k];

            for ( size_t c = k + 1; c < ncolumns; ++c )
            {
                result[r][c] -= value * result[k][c];
            }
        }
    }

    return det;
}

/*===========================================================================*/
/**
 *  @brief  Returns the row index of pivot.
 *  @param  column [in] column index
 */
/*===========================================================================*/
template<typename T>
inline size_t Matrix<T>::pivot( const size_t column ) const
{
    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;

    // Search a max absolute value in the vector of a given row index.
    T      max = T( 0 );
    size_t k   = column;

    for ( size_t r = column; r < nrows; r++ )
    {
        const T abs = kvs::Math::Abs( m[r][column] );
        if( abs > max )
        {
            max = abs;
            k = r;
        }
    }

    return k;
}

/*==========================================================================*/
/**
 *  @brief  Copies this and transposes it.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline const Matrix<T> Matrix<T>::transposed() const
{
    Matrix result( *this );
    result.transpose();
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Copies this and inverts it.
 *  @param  determinant [out] calculated determinant
 *  @return Inverse matrix.
 */
/*==========================================================================*/
template<typename T>
inline const Matrix<T> Matrix<T>::inverted() const
{
    Matrix result( *this );
    result.invert();
    return result;
}

template<typename T>
inline const kvs::Vector<T>& Matrix<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->rowSize() );
    return m_rows[ index ];
}

template<typename T>
inline kvs::Vector<T>& Matrix<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < this->rowSize() );
    return m_rows[ index ];
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator +=( const Matrix& rhs )
{
    KVS_ASSERT( this->rowSize() == rhs.rowSize() );
    KVS_ASSERT( this->columnSize() == rhs.columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] += rhs[r];
    }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator -=( const Matrix& rhs )
{
    KVS_ASSERT( this->rowSize() == rhs.rowSize() );
    KVS_ASSERT( this->columnSize() == rhs.columnSize() );

    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] -= rhs[r];
    }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator *=( const Matrix& rhs )
{
    Matrix result( ( *this ) * rhs );
    return *this = result;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator *=( const T rhs )
{
    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] *= rhs;
    }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator /=( const T rhs )
{
    const size_t          nrows = this->rowSize();
    kvs::Vector<T>* const m     = m_rows;
    for ( size_t r = 0; r < nrows; ++r )
    {
        m[r] /= rhs;
    }
    return *this;
}

template<typename T>
inline const Matrix<T> Matrix<T>::operator -() const
{
    return Matrix( *this ) *= T( -1 );
}

} // end of namespace kvs

#endif // KVS__MATRIX_H_INCLUDE
