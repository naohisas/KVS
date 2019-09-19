/****************************************************************************/
/**
 *  @file   Matrix.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <iostream>
#include <cstring>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Indent>
#include <kvs/Vector>
#include <kvs/Matrix22>
#include <kvs/Matrix33>
#include <kvs/Matrix44>
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
public:
    typedef Matrix<T> this_type;
    typedef T value_type;
    typedef kvs::Vector<value_type> row_type;
    typedef row_type* rows_iterator;
    typedef const row_type* const_rows_iterator;

private:
    size_t m_nrows; ///< Number of rows.
    size_t m_ncols; ///< Number of columns.
    kvs::Vector<T>* m_data; ///< Row vectors.

public:
    static const Matrix Zero( const size_t nrows, const size_t ncols );
    static const Matrix Ones( const size_t nrows, const size_t ncols );
    static const Matrix Identity( const size_t nrows, const size_t ncols );
    static const Matrix Constant( const size_t nrows, const size_t ncols, const T x );
    static const Matrix Diagonal( const size_t nrows, const size_t ncols, const T x );
    static const Matrix Diagonal( const kvs::Vector<T>& v );
    static const Matrix Random( const size_t nrows, const size_t ncols );
    static const Matrix Random( const size_t nrows, const size_t ncols, const kvs::UInt32 seed );
    static const Matrix Random( const size_t nrows, const size_t ncols, const T min, const T max );
    static const Matrix Random( const size_t nrows, const size_t ncols, const T min, const T max, const kvs::UInt32 seed );

public:
    Matrix(): m_nrows( 0 ), m_ncols( 0 ), m_data( nullptr ) {}
    ~Matrix() { delete [] m_data; }

    Matrix( const size_t nrows, const size_t ncols );
    Matrix( const size_t nrows, const size_t ncols, const T* const elements );
    Matrix( const kvs::Matrix22<T>& other );
    Matrix( const kvs::Matrix33<T>& other );
    Matrix( const kvs::Matrix44<T>& other );

    Matrix( const Matrix& other );
    Matrix& operator =( const Matrix& rhs );

    Matrix( Matrix&& other ) noexcept;
    Matrix& operator =( Matrix&& rhs ) noexcept;

public:
    size_t rowSize() const { return m_nrows; }
    size_t columnSize() const { return m_ncols; }
    size_t size() const { return m_nrows * m_ncols; }

    rows_iterator beginRows() { return m_data; }
    rows_iterator endRows() { return m_data + m_nrows; }
    const_rows_iterator beginRows() const { return m_data; }
    const_rows_iterator endRows() const { return m_data + m_nrows; }

    void setSize( const size_t nrows, const size_t ncols );
    void setZero();
    void setOnes();
    void setIdentity();
    void setConstant( const T x );
    void setDiagonal( const T x );
    void setDiagonal( const kvs::Vector<T>& v );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Matrix& other );
    void transpose();
    void invert();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    T trace() const;
    T determinant() const;
    size_t pivot( const size_t row_index ) const;
    const Matrix transposed() const;
    const Matrix inverted() const;
    bool isSquare() const;
    bool isSymmetric() const;
    bool isDiagonal() const;

public:
    const kvs::Vector<T>& operator []( const size_t index ) const;
    kvs::Vector<T>& operator []( const size_t index );
    Matrix& operator +=( const Matrix& rhs );
    Matrix& operator -=( const Matrix& rhs );
    Matrix& operator *=( const Matrix& rhs );
    Matrix& operator *=( const T rhs );
    Matrix& operator /=( const T rhs );
    const Matrix operator -() const;

    friend bool operator ==( const Matrix& lhs, const Matrix& rhs )
    {
        if ( lhs.size() != rhs.size() ) { return false; }
        return std::equal( lhs.beginRows(), lhs.endRows(), rhs.beginRows() );
    }

    friend bool operator !=( const Matrix& lhs, const Matrix& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Matrix operator +( const Matrix& lhs, const Matrix& rhs )
    {
        Matrix m( lhs ); m += rhs;
        return std::move( m );
    }

    friend const Matrix operator -( const Matrix& lhs, const Matrix& rhs )
    {
        Matrix m( lhs ); m -= rhs;
        return std::move( m );
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
                T sum(0);
                for ( size_t k = 0; k < M; ++k )
                {
                    sum += lhs[r][k] * rhs[k][c];
                }
                result[r][c] = sum;
            }
        }

        return std::move( result );
    }

    friend const kvs::Vector<T> operator *( const Matrix& lhs, const kvs::Vector<T>& rhs )
    {
        KVS_ASSERT( lhs.columnSize() == rhs.size() );

        const size_t nrows = lhs.rowSize();
        kvs::Vector<T> result( nrows );
        for ( size_t r = 0; r < nrows; ++r )
        {
            result[r] = lhs[r].dot( rhs );
        }

        return std::move( result );
    }

    friend const kvs::Vector<T> operator *( const kvs::Vector<T>& lhs, const Matrix& rhs )
    {
        KVS_ASSERT( lhs.size() == rhs.rowSize() );

        const size_t nrows = rhs.rowSize();
        const size_t ncols = rhs.columnSize();

        kvs::Vector<T> result( ncols );
        for ( size_t c = 0; c < ncols; ++c )
        {
            T sum(0);
            for ( size_t r = 0; r < nrows; ++r )
            {
                sum += lhs[r] * rhs[r][c];
            }
            result[c] = sum;
        }

        return std::move( result );
    }

    friend const Matrix operator *( const Matrix& lhs, const T rhs )
    {
        Matrix m( lhs ); m *= rhs;
        return std::move( m );
    }

    friend const Matrix operator *( const T lhs, const Matrix& rhs )
    {
        return std::move( rhs * lhs );
    }

    friend const Matrix operator /( const Matrix& lhs, const T rhs )
    {
        Matrix m( lhs ); m /= rhs;
        return std::move( m );
    }

    friend std::ostream& operator <<( std::ostream& os, const Matrix& rhs )
    {
        const size_t nrows = rhs.rowSize();
        if ( nrows == 0 ) { return os << "[[ ]]"; }
        os << "[" << rhs[0];
        for ( size_t i = 1; i < nrows; ++i ) { os << ", " << rhs[i]; }
        return os << "]";
    }

public:
    KVS_DEPRECATED( size_t nrows() const ) { return this->rowSize(); }
    KVS_DEPRECATED( size_t ncolumns() const ) { return this->columnSize(); }
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( void identity() ) { this->setIdentity(); }
    KVS_DEPRECATED( void print() const ) { this->print( std::cout ); }
};


/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Matrix<float> Mat;
typedef Matrix<double> Matd;


template <typename T>
inline const Matrix<T> Matrix<T>::Zero( const size_t nrows, const size_t ncols )
{
    Matrix<T> m( nrows, ncols );
    m.setZero();
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Ones( const size_t nrows, const size_t ncols )
{
    Matrix<T> m( nrows, ncols );
    m.setOnes();
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Identity( const size_t nrows, const size_t ncols )
{
    Matrix<T> m( nrows, ncols );
    m.setIdentity();
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Constant( const size_t nrows, const size_t ncols, const T x )
{
    Matrix<T> m( nrows, ncols );
    m.setConstant( x );
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Diagonal( const size_t nrows, const size_t ncols, const T x )
{
    Matrix<T> m( nrows, ncols );
    m.setDiagonal( x );
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Diagonal( const kvs::Vector<T>& v )
{
    Matrix<T> m( v.size(), v.size() );
    m.setDiagonal( v );
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Random( const size_t nrows, const size_t ncols )
{
    Matrix<T> m( nrows, ncols );
    m.setRandom();
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Random( const size_t nrows, const size_t ncols, const kvs::UInt32 seed )
{
    Matrix<T> m( nrows, ncols );
    m.setRandom( seed );
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Random( const size_t nrows, const size_t ncols, const T min, const T max )
{
    Matrix<T> m( nrows, ncols );
    m.setRandom( min, max );
    return std::move( m );
}

template <typename T>
inline const Matrix<T> Matrix<T>::Random( const size_t nrows, const size_t ncols, const T min, const T max, const kvs::UInt32 seed )
{
    Matrix<T> m( nrows, ncols );
    m.setRandom( min, max, seed );
    return std::move( m );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncols [in] Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline Matrix<T>::Matrix( const size_t nrows, const size_t ncols ):
    m_nrows( nrows ),
    m_ncols( ncols ),
    m_data( new row_type [ nrows ] )
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { v->setSize( ncols ); }
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncols [in] Number of columns of matrix.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Matrix<T>::Matrix( const size_t nrows, const size_t ncols, const T* const elements ):
    m_nrows( nrows ),
    m_ncols( ncols ),
    m_data( new row_type [ nrows ] )
{
    size_t offset = 0;
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v, offset += ncols )
    {
        v->setSize( ncols );
        std::memcpy( v->data(), elements + offset, sizeof(T) * v->size() );
    }
}

template <typename T>
inline Matrix<T>::Matrix( const kvs::Matrix22<T>& other ):
    m_nrows( 2 ),
    m_ncols( 2 ),
    m_data( new row_type [2] )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
}

template <typename T>
inline Matrix<T>::Matrix( const kvs::Matrix33<T>& other ):
    m_nrows( 3 ),
    m_ncols( 3 ),
    m_data( new row_type [3] )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
    m_data[2] = other[2];
}

template <typename T>
inline Matrix<T>::Matrix( const kvs::Matrix44<T>& other ):
    m_nrows( 4 ),
    m_ncols( 4 ),
    m_data( new row_type [4] )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
    m_data[2] = other[2];
    m_data[3] = other[3];
}

/*==========================================================================*/
/**
 *  @brief  Constructs a copy of other.
 *  @param  other [in] Matrix.
 */
/*==========================================================================*/
template <typename T>
inline Matrix<T>::Matrix( const Matrix& other ):
    m_nrows( other.rowSize() ),
    m_ncols( other.columnSize() ),
    m_data( new row_type [ other.rowSize() ] )
{
    const_rows_iterator last = m_data + m_nrows;
    const_rows_iterator r = other.m_data;
    for ( rows_iterator v = m_data; v != last; ++v, ++r ) { *v = *r; }
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
    if ( this != &rhs )
    {
        const size_t nrows = rhs.rowSize();
        if ( m_nrows != nrows )
        {
            delete [] m_data;
            m_data = new row_type [ nrows ];
        }

        const_rows_iterator last = m_data + nrows;
        const_rows_iterator r = rhs.m_data;
        for ( rows_iterator v = m_data; v != last; ++v, ++r ) { *v = *r; }

        m_nrows = nrows;
        m_ncols = rhs.columnSize();
    }
    return *this;
}

template <typename T>
inline Matrix<T>::Matrix( Matrix&& other ) noexcept:
    m_nrows( other.m_nrows ),
    m_ncols( other.m_ncols ),
    m_data( other.m_data  )
{
    other.m_nrows = 0;
    other.m_ncols = 0;
    other.m_data = nullptr;
}

template <typename T>
inline Matrix<T>& Matrix<T>::operator =( Matrix&& rhs ) noexcept
{
    if ( this != &rhs )
    {
        delete [] m_data;
        m_nrows = rhs.m_nrows;
        m_ncols = rhs.m_ncols;
        m_data = rhs.m_data;

        rhs.m_nrows = 0;
        rhs.m_ncols = 0;
        rhs.m_data = nullptr;
    }
    return *this;
}

/*==========================================================================*/
/**
 *  @brief  Sets the size of matrix.
 *  @param  nrows    [in] Number of rows of matrix.
 *  @param  ncols [in] Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::setSize( const size_t nrows, const size_t ncols )
{
    if ( nrows == 0 )
    {
        delete [] m_data;
        m_nrows = 0;
        m_ncols = 0;
        m_data = nullptr;
        return;
    }

    if ( m_nrows != nrows )
    {
        delete [] m_data;
        m_data = new row_type [ nrows ];
        m_nrows = nrows;
    }

    if ( ncols == 0 )
    {
        m_ncols = 0;
        return;
    }

    if ( m_ncols != ncols )
    {
        const_rows_iterator last = m_data + nrows;
        for ( rows_iterator v = m_data; v != last; ++v ) { v->setSize( ncols ); }
        m_ncols = ncols;
    }
}

template<typename T>
inline void Matrix<T>::setZero()
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { v->setZero(); }
}

template<typename T>
inline void Matrix<T>::setOnes()
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { v->setOnes(); }
}

template<typename T>
inline void Matrix<T>::setIdentity()
{
    size_t i = 0;
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v, ++i ) { v->setUnit(i); }
}

template<typename T>
inline void Matrix<T>::setConstant( const T x )
{
   const_rows_iterator last = m_data + m_nrows;
   for ( rows_iterator v = m_data; v != last; ++v ) { v->setConstant(x); }
}

template<typename T>
inline void Matrix<T>::setDiagonal( const T x )
{
    for ( size_t i = 0; i < m_nrows; ++i )
    {
        m_data[i].setZero();
        m_data[i][i] = x;
    }
}

template<typename T>
inline void Matrix<T>::setDiagonal( const kvs::Vector<T>& v )
{
    for ( size_t i = 0; i < m_nrows; ++i )
    {
        m_data[i].setZero();
        m_data[i][i] = v[i];
    }
}

template<typename T>
inline void Matrix<T>::setRandom()
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { v->setRandom(); }
}

template<typename T>
inline void Matrix<T>::setRandom( const kvs::UInt32 seed )
{
    if ( m_nrows > 0 )
    {
        m_data[0].setRandom( seed );
        const_rows_iterator last = m_data + m_nrows;
        for ( rows_iterator v = m_data + 1; v != last; ++v ) { v->setRandom(); }
    }
}

template<typename T>
inline void Matrix<T>::setRandom( const T min, const T max )
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { v->setRandom( min, max ); }
}

template<typename T>
inline void Matrix<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    if ( m_nrows > 0 )
    {
        m_data[0].setRandom( min, max, seed );
        const_rows_iterator last = m_data + m_nrows;
        for ( rows_iterator v = m_data + 1; v != last; ++v ) { v->setRandom( min, max ); }
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
    std::swap( m_ncols, other.m_ncols );
    std::swap( m_data, other.m_data );
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
    const size_t nrows = this->rowSize();
    const size_t ncols = this->columnSize();
    kvs::Vector<T>* const m = m_data;

    if ( nrows == ncols )
    {
        for ( size_t r = 0; r < nrows; ++r )
        {
            for ( size_t c = r + 1; c < ncols; ++c )
            {
                std::swap( m[r][c], m[c][r] );
            }
        }
    }
    else
    {
        Matrix result( ncols, nrows );
        for ( size_t r = 0; r < nrows; ++r )
        {
            for ( size_t c = 0; c < ncols; ++c )
            {
                result[c][r] = m[r][c];
            }
        }
        *this = std::move( result );
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

    const size_t size = this->rowSize();
    const size_t nrows = this->rowSize();
    const size_t ncols = this->columnSize();
    kvs::Vector<T>* const m = m_data;

    Matrix<T> result( nrows, nrows );
    result.setIdentity();
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
        for ( size_t c = 0; c < ncols; ++c )
        {
            m[k][c] /= diagonal_element;
            result[k][c] /= diagonal_element;
        }

        for ( size_t r = 0; r < nrows; ++r )
        {
            // Skip the pivot_row.
            if ( r != k )
            {
                const T value = m[r][k];
                for( size_t c = 0; c < ncols; ++c )
                {
                    m[r][c] -= value * m[k][c];
                    result[r][c] -= value * result[k][c];
                }
            }
        }
    }

    *this = std::move( result );
}

/*==========================================================================*/
/**
 *  @bri  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::print( std::ostream& os, const kvs::Indent& indent ) const
{
    if ( m_nrows == 0 ) { os << indent << "[[ ]]" << std::endl; }
    else
    {
        os << indent << "[" << m_data[0];
        for ( size_t i = 1; i < m_nrows; ++i )
        {
            os << "," << std::endl << indent << " " << m_data[i];
        }
        os << "]" << std::endl;
    }
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

    const size_t nrows = this->rowSize();
    kvs::Vector<T>* const m = m_data;

    T result = T(0);
    for ( size_t r = 0; r < nrows; ++r ) { result += m[r][r]; }
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

    const size_t size = this->rowSize();
    const size_t nrows = this->rowSize();
    const size_t ncols = this->columnSize();

    Matrix<T> result( *this );
    T det = T(1);
    for ( size_t k = 0; k < size; ++k )
    {
        const size_t pivot_row = result.pivot( k );
        if ( k != pivot_row ) { det *= T(-1); }

        det *= result[k][k];
        for ( size_t r = k + 1; r < nrows; ++r )
        {
            const T value = result[r][k] / result[k][k];
            for ( size_t c = k + 1; c < ncols; ++c )
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
    const size_t nrows = this->rowSize();
    kvs::Vector<T>* const m = m_data;

    // Search a max absolute value in the vector of a given row index.
    T max = T( 0 );
    size_t k = column;
    for ( size_t r = column; r < nrows; r++ )
    {
        const T abs = kvs::Math::Abs( m[r][column] );
        if ( abs > max ) { max = abs; k = r; }
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
    Matrix result( *this ); result.transpose();
    return std::move( result );
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
    Matrix result( *this ); result.invert();
    return std::move( result );
}

template<typename T>
inline bool Matrix<T>::isSquare() const
{
    return this->rowSize() == this->columnSize();
}

template<typename T>
inline bool Matrix<T>::isSymmetric() const
{
    if ( !this->isSquare() ) { return false; }

    for ( size_t i = 0; i < m_nrows; i++ )
    {
        for ( size_t j = i + 1; j < m_ncols; j++ )
        {
            if ( !kvs::Math::Equal( m_data[i][j], m_data[j][i] ) ) { return false; }
        }
    }

    return true;
}

template<typename T>
inline bool Matrix<T>::isDiagonal() const
{
    if ( !this->isSquare() ) { return false; }

    for ( size_t i = 0; i < m_nrows; i++ )
    {
        for ( size_t j = i + 1; j < m_ncols; j++ )
        {
            if ( !kvs::Math::IsZero( m_data[i][j] ) ||
                 !kvs::Math::IsZero( m_data[j][i] ) ) { return false; }
        }
    }

    return true;
}

template<typename T>
inline const kvs::Vector<T>& Matrix<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->rowSize() );
    return *( m_data + index );
}

template<typename T>
inline kvs::Vector<T>& Matrix<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < this->rowSize() );
    return *( m_data + index );
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator +=( const Matrix& rhs )
{
    KVS_ASSERT( this->rowSize() == rhs.rowSize() );
    KVS_ASSERT( this->columnSize() == rhs.columnSize() );

    const_rows_iterator last = m_data + m_nrows;
    const_rows_iterator r = rhs.m_data;
    for ( rows_iterator v = m_data; v != last; ++v, ++r ) { *v += *r; }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator -=( const Matrix& rhs )
{
    KVS_ASSERT( this->rowSize() == rhs.rowSize() );
    KVS_ASSERT( this->columnSize() == rhs.columnSize() );

    const_rows_iterator last = m_data + m_nrows;
    const_rows_iterator r = rhs.m_data;
    for ( rows_iterator v = m_data; v != last; ++v, ++r ) { *v -= *r; }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator *=( const Matrix& rhs )
{
    *this = *this * rhs;
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator *=( const T rhs )
{
    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { *v *= rhs; }
    return *this;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator /=( const T rhs )
{
    KVS_ASSERT( !kvs::Math::IsZero( rhs ) );

    const_rows_iterator last = m_data + m_nrows;
    for ( rows_iterator v = m_data; v != last; ++v ) { *v /= rhs; }
    return *this;
}

template<typename T>
inline const Matrix<T> Matrix<T>::operator -() const
{
    Matrix m( *this ); m *= T(-1);
    return std::move( m );
}

} // end of namespace kvs
