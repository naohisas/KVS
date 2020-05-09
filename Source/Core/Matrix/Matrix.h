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
#include <iomanip>
#include <cstring>
#include <initializer_list>
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
    class RowOrderIterator;
    class ColumnOrderIterator;

public:
    typedef Matrix<T> this_type;
    typedef T value_type;
    typedef kvs::Vector<value_type> row_type;

    // Iterators for row vectors.
    typedef row_type* rows_iterator;
    typedef const row_type* const_rows_iterator;

    // Row order iterators.
    typedef RowOrderIterator row_order_iterator;
    typedef const RowOrderIterator const_row_order_iterator;
    typedef std::reverse_iterator<row_order_iterator> row_order_reverse_iterator;
    typedef std::reverse_iterator<const_row_order_iterator> const_row_order_reverse_iterator;

    // Column order iterators.
    typedef ColumnOrderIterator column_order_iterator;
    typedef const ColumnOrderIterator const_column_order_iterator;
    typedef std::reverse_iterator<column_order_iterator> column_order_reverse_iterator;
    typedef std::reverse_iterator<const_column_order_iterator> const_column_order_reverse_iterator;

    // Iterators (row-order).
    typedef row_order_iterator iterator;
    typedef const_row_order_iterator const_iterator;
    typedef row_order_reverse_iterator reverse_iterator;
    typedef const_row_order_reverse_iterator const_reverse_iterator;

private:
    size_t m_nrows; ///< Number of rows.
    size_t m_ncols; ///< Number of columns.
    row_type* m_data; ///< Row vectors.

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

    Matrix( std::initializer_list<row_type> list );

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

    iterator begin() { return this->beginInRowOrder(); }
    iterator end() { return this->endInRowOrder(); }
    reverse_iterator rbegin() { return this->rbeginInRowOrder(); }
    reverse_iterator rend() { return this->rendInRowOrder(); }

    // Row order iterators.
    row_order_iterator beginInRowOrder()
    {
        typename row_type::iterator col = this->beginRows()->begin();
        rows_iterator row = this->beginRows();
        rows_iterator begin = this->beginRows();
        rows_iterator end = this->endRows();
        return row_order_iterator( col, row, begin, end );
    }

    row_order_iterator endInRowOrder()
    {
        typename row_type::iterator col = ( this->endRows() - 1 )->end();
        rows_iterator row = this->endRows();
        rows_iterator begin = this->beginRows();
        rows_iterator end = this->endRows();
        return row_order_iterator( col, row, begin, end );
    }

    row_order_reverse_iterator rbeginInRowOrder()
    {
        return row_order_reverse_iterator( this->endInRowOrder() );
    }

    row_order_reverse_iterator rendInRowOrder()
    {
        return row_order_reverse_iterator( this->beginInRowOrder() );
    }

    row_order_iterator beginRow( const size_t row_index )
    {
        KVS_ASSERT( row_index < this->rowSize() );
        return this->beginInRowOrder() + row_index * this->columnSize();
    }

    row_order_iterator endRow( const size_t row_index )
    {
        KVS_ASSERT( row_index < this->rowSize() );
        return this->beginInRowOrder() + ( row_index + 1 ) * this->columnSize();
    }

    // Column order iterations.
    column_order_iterator beginInColumnOrder()
    {
        typename row_type::iterator col = this->beginRows()->begin();
        rows_iterator row = this->beginRows();
        rows_iterator begin = this->beginRows();
        rows_iterator end = this->endRows();
        size_t offset = 0;
        return column_order_iterator( col, row, begin, end, offset );
    }

    column_order_iterator endInColumnOrder()
    {
        typename row_type::iterator col = ( this->endRows() - 1 )->end();
        rows_iterator row = this->endRows();
        rows_iterator begin = this->beginRows();
        rows_iterator end = this->endRows();
        size_t offset = ( end - 1 )->size();
        return column_order_iterator( col, row, begin, end, offset );
    }

    column_order_reverse_iterator rbeginInColumnOrder()
    {
        return column_order_reverse_iterator( this->endInColumnOrder() );
    }

    column_order_reverse_iterator rendInColumnOrder()
    {
        return column_order_reverse_iterator( this->beginInColumnOrder() );
    }

    column_order_iterator beginColumn( const size_t column_index )
    {
        KVS_ASSERT( column_index < this->columnSize() );
        return this->beginInColumnOrder() + column_index * this->rowSize();
    }

    column_order_iterator endColumn( const size_t column_index )
    {
        KVS_ASSERT( column_index < this->columnSize() );
        return this->beginInColumnOrder() + ( column_index + 1 ) * this->rowSize();
    }

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

    void resize( const size_t nrows, const size_t ncols );
    void swap( Matrix& other );
    void transpose();
    void invert();

    T trace() const;
    T determinant() const;
    size_t pivot( const size_t row_index ) const;
    const Matrix transposed() const;
    const Matrix inverted() const;

    bool isSquare() const;
    bool isSymmetric() const;
    bool isDiagonal() const;

    std::string format(
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( ", ", "[", "]", newline, indent );
    }

    std::string format(
        const std::string delim,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( delim, "", "", newline, indent );
    }

    std::string format(
        const std::string bracket_l,
        const std::string bracket_r,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( " ", bracket_l, bracket_r, newline, indent );
    }

    std::string format(
        const std::string delim,
        const std::string bracket_l,
        const std::string bracket_r,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const;

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
        return os << rhs.format( " ", "", "", true );
    }

public:
    KVS_DEPRECATED( size_t nrows() const ) { return this->rowSize(); }
    KVS_DEPRECATED( size_t ncolumns() const ) { return this->columnSize(); }
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( void identity() ) { this->setIdentity(); }
    KVS_DEPRECATED( void print() const ) { std::cout << *this << std::endl; }
    KVS_DEPRECATED( void setSize( const size_t nrows, const size_t ncols ) ) { this->resize( nrows, ncols ); }
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
    for ( rows_iterator v = m_data; v != last; ++v ) { v->resize( ncols ); }
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
        v->resize( ncols );
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

template <typename T>
inline Matrix<T>::Matrix( std::initializer_list<row_type> list ):
    m_nrows( std::distance( list.begin(), list.end() ) ),
    m_ncols( list.begin()->size() ),
    m_data( new row_type [ m_nrows ] )
{
    std::copy( list.begin(), list.end(), this->beginRows() );
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
 *  @brief  Sets the size of matrix.
 *  @param  nrows [in] Number of rows of matrix.
 *  @param  ncols [in] Number of columns of matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix<T>::resize( const size_t nrows, const size_t ncols )
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
        for ( rows_iterator v = m_data; v != last; ++v ) { v->resize( ncols ); }
        m_ncols = ncols;
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
 *  @brief  Prints the elements as a formatted string.
 *  @param  delim [in] delimiter
 *  @param  bracket_l [in] left bracket
 *  @param  bracket_r [in] right bracket
 *  @param  newline [in] flag for newline for each row
 *  @param  indent [in] indent for each row
 */
/*==========================================================================*/
template<typename T>
inline std::string Matrix<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r,
    const bool newline,
    const kvs::Indent& indent ) const
{
    std::ostringstream os;
    os << indent << bracket_l;
    {
        const std::string offset( bracket_l.size(), ' ' );
        if ( m_nrows == 0 ) { os << indent << bracket_l << " " << bracket_r; }
        else
        {
            os << m_data[0].format( delim, bracket_l, bracket_r );
            for ( size_t i = 1; i < m_nrows; ++i )
            {
                os << delim; if ( newline ) { os << std::endl << indent << offset; }
                os << m_data[i].format( delim, bracket_l, bracket_r );
            }
        }
    }
    os << bracket_r;
    return os.str();
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

template <typename T>
class Matrix<T>::RowOrderIterator
{
private:
    typedef RowOrderIterator this_type;
    typedef typename Matrix<T>::row_type::reference column_reference;
    typedef typename Matrix<T>::row_type::iterator column_iterator;
    typedef typename Matrix<T>::row_type::const_reference const_column_reference;
    typedef typename Matrix<T>::row_type::const_iterator const_column_iterator;

public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

private:
    column_iterator m_col_iterator;
    rows_iterator m_row_iterator;
    rows_iterator m_row_begin;
    rows_iterator m_row_end;

public:
    RowOrderIterator( const RowOrderIterator& other ):
        m_col_iterator( other.m_col_iterator ),
        m_row_iterator( other.m_row_iterator ),
        m_row_begin( other.m_row_begin ),
        m_row_end( other.m_row_end ) {}

    RowOrderIterator(
        column_iterator col_iterator,
        rows_iterator row_iterator,
        rows_iterator row_begin,
        rows_iterator row_end ):
        m_col_iterator( col_iterator ),
        m_row_iterator( row_iterator ),
        m_row_begin( row_begin ),
        m_row_end( row_end ) {}

    this_type& operator =( const this_type& rhs )
    {
        if ( this != &rhs )
        {
            m_col_iterator = rhs.m_col_iterator;
            m_row_iterator = rhs.m_row_iterator;
            m_row_begin = rhs.m_row_begin;
            m_row_end = rhs.m_row_end;
        }
        return *this;
    }

    this_type& operator ++()
    {
        this->increment();
        return *this;
    }

    this_type& operator --()
    {
        this->decrement();
        return *this;
    }

    this_type operator ++(int)
    {
        this_type result = *this;
        this->increment();
        return result;
    }

    this_type operator --(int)
    {
        this_type result = *this;
        this->decrement();
        return result;
    }

    this_type& operator +=( unsigned int n )
    {
        while ( n > 0 ) { ++(*this); --n; }
        return *this;
    }

    this_type& operator -=( unsigned int n )
    {
        while ( n > 0 ) { --(*this); --n; }
        return *this;
    }

    this_type operator +( unsigned int n ) const
    {
        this_type result = *this;
        result += n;
        return result;
    }

    this_type operator -( unsigned int n ) const
    {
        this_type result = *this;
        result -= n;
        return result;
    }

    column_reference operator *()
    {
        return *m_col_iterator;
    }

    const_column_reference operator *() const
    {
        return *m_col_iterator;
    }

    bool operator ==( const this_type& other ) const
    {
        return ( m_col_iterator == other.m_col_iterator &&
                 m_row_iterator == other.m_row_iterator );
    }

    bool operator !=( const this_type& other ) const
    {
        return !( *this == other );
    }

    friend difference_type operator -( const this_type& lhs, const this_type& rhs )
    {
        return difference_type( lhs.index() ) - difference_type( rhs.index() );
    }

private:
    size_t index() const
    {
        const size_t ncols = std::distance( m_row_begin->begin(), m_row_begin->end() );
        const size_t j = std::distance( m_row_begin, m_row_iterator );
        const size_t i =
            ( m_row_iterator == m_row_end ) ? 0 :
            ( m_col_iterator == m_row_iterator->end() ) ? 0 :
            std::distance( m_row_iterator->begin(), m_col_iterator );
        return j * ncols + i;
    }

    void increment()
    {
        if ( m_row_iterator == m_row_end ) { return; }
        if ( m_col_iterator == ( m_row_end - 1 )->end() ) { return; }

        ++m_col_iterator;
        if ( m_col_iterator == m_row_iterator->end() )
        {
            ++m_row_iterator;
            if ( m_row_iterator != m_row_end )
            {
                m_col_iterator = m_row_iterator->begin();
            }
            else
            {
                m_col_iterator = ( m_row_end - 1 )->end();
            }
        }
    }

    void decrement()
    {
        if ( m_col_iterator == m_row_begin->begin() ) { return; }
        if ( m_row_iterator == m_row_end || m_col_iterator == m_row_iterator->begin() )
        {
            --m_row_iterator;
            m_col_iterator = m_row_iterator->end();
        }
        --m_col_iterator;
    }
};


template <typename T>
class Matrix<T>::ColumnOrderIterator
{
private:
    typedef ColumnOrderIterator this_type;
    typedef typename Matrix<T>::row_type::reference column_reference;
    typedef typename Matrix<T>::row_type::iterator column_iterator;
    typedef typename Matrix<T>::row_type::const_reference const_column_reference;
    typedef typename Matrix<T>::row_type::const_iterator const_column_iterator;

public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

private:
    column_iterator m_col_iterator;
    rows_iterator m_row_iterator;
    rows_iterator m_row_begin;
    rows_iterator m_row_end;
    size_t m_col_offset;

public:
    ColumnOrderIterator( const ColumnOrderIterator& other ):
        m_col_iterator( other.m_col_iterator ),
        m_row_iterator( other.m_row_iterator ),
        m_row_begin( other.m_row_begin ),
        m_row_end( other.m_row_end ),
        m_col_offset( other.m_col_offset ) {}

    ColumnOrderIterator(
        column_iterator col_iterator,
        rows_iterator row_iterator,
        rows_iterator row_begin,
        rows_iterator row_end,
        size_t col_offset ):
        m_col_iterator( col_iterator ),
        m_row_iterator( row_iterator ),
        m_row_begin( row_begin ),
        m_row_end( row_end ),
        m_col_offset( col_offset ) {}

    this_type& operator =( const this_type& rhs )
    {
        if ( this != &rhs )
        {
            m_col_iterator = rhs.m_col_iterator;
            m_row_iterator = rhs.m_row_iterator;
            m_row_begin = rhs.m_row_begin;
            m_row_end = rhs.m_row_end;
            m_col_offset = rhs.m_col_offset;
        }
        return *this;
    }

    this_type& operator ++()
    {
        this->increment();
        return *this;
    }

    this_type& operator --()
    {
        this->decrement();
        return *this;
    }

    this_type operator ++(int)
    {
        this_type result = *this;
        this->increment();
        return result;
    }

    this_type operator --(int)
    {
        this_type result = *this;
        this->decrement();
        return result;
    }

    this_type& operator +=( unsigned int n )
    {
        while ( n > 0 ) { ++(*this); --n; }
        return *this;
    }

    this_type& operator -=( unsigned int n )
    {
        while ( n > 0 ) { --(*this); --n; }
        return *this;
    }

    this_type operator +( unsigned int n ) const
    {
        this_type result = *this;
        result += n;
        return result;
    }

    this_type operator -( unsigned int n ) const
    {
        this_type result = *this;
        result -= n;
        return result;
    }

    column_reference operator *()
    {
        return *m_col_iterator;
    }

    const_column_reference operator *() const
    {
        return *m_col_iterator;
    }

    bool operator ==( const this_type& other ) const
    {
        return ( m_col_iterator == other.m_col_iterator &&
                 m_row_iterator == other.m_row_iterator );
    }

    bool operator !=( const this_type& other ) const
    {
        return !( *this == other );
    }

    friend difference_type operator -( const this_type& lhs, const this_type& rhs )
    {
        return difference_type( lhs.index() ) - difference_type( rhs.index() );
    }

private:
    size_t index() const
    {
        const size_t nrows = std::distance( m_row_begin, m_row_end );
        const size_t ncols = std::distance( m_row_begin->begin(), m_row_begin->end() );
        const size_t j =
            ( m_row_iterator == m_row_end ) ? ncols :
            ( m_col_iterator == m_row_iterator->end() ) ? ncols :
            std::distance( m_row_iterator->begin(), m_col_iterator );
        const size_t i = ( m_row_iterator == m_row_end ) ? 0 : std::distance( m_row_begin, m_row_iterator );
        return j * nrows + i;
    }

    void increment()
    {
        if ( m_row_iterator == m_row_end ) { return; }
        if ( m_col_iterator == ( m_row_end - 1 )->end() ) { return; }

        ++m_row_iterator;
        if ( m_row_iterator == m_row_end )
        {
            m_row_iterator = m_row_begin;
            m_col_offset++;
        }

        m_col_iterator = m_row_iterator->begin() + m_col_offset;
        if ( m_col_iterator == m_row_iterator->end() )
        {
            m_col_iterator = ( m_row_end - 1 )->end();
            m_row_iterator = m_row_end;
        }
    }

    void decrement()
    {
        if ( m_col_iterator == m_row_begin->begin() ) { return; }

        if ( m_row_iterator == m_row_begin || m_col_iterator == ( m_row_end - 1 )->end() )
        {
            m_row_iterator = m_row_end;
            --m_col_offset;
        }
        --m_row_iterator;

        m_col_iterator = m_row_iterator->begin() + m_col_offset;
    }
};

} // end of namespace kvs

namespace std
{

template <typename T>
inline void swap( kvs::Matrix<T>& lhs, kvs::Matrix<T>& rhs )
{
    lhs.swap( rhs );
}

} // end of namespace std
