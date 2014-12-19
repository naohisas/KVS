/*****************************************************************************/
/**
 *  @file   ValueTable.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__VALUE_TABLE_H_INCLUDE
#define KVS__VALUE_TABLE_H_INCLUDE

#include <vector>
#include <iterator>
#include <algorithm>
#include <kvs/ValueArray>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ValueTable class.
 */
/*===========================================================================*/
template <typename T>
class ValueTable
{
public:

    typedef kvs::ValueArray<T> Column;
    typedef std::vector<Column> Columns;
    typedef typename Columns::iterator column_iterator;
    typedef typename Columns::const_iterator const_column_iterator;
    typedef Column& column_reference;
    typedef const Column& const_column_reference;
    typedef std::reverse_iterator<column_iterator> column_reverse_iterator;
    typedef const std::reverse_iterator<const_column_iterator> const_column_reverse_iterator;

private:

    Columns m_columns;

public:

    ValueTable(){}

    explicit ValueTable( const size_t ncolumns )
    {
        m_columns.resize( ncolumns );
    }

    ValueTable( const size_t nrows, const size_t ncolumns )
    {
        m_columns.resize( ncolumns );
        for ( size_t i = 0; i < ncolumns; i++ )
        {
            m_columns[i] = kvs::ValueArray<T>( nrows );
        }
    }

    column_reference operator []( const size_t column_index )
    {
        KVS_ASSERT( column_index < m_columns.size() );
        return m_columns[ column_index ];
    }

    const_column_reference operator []( const size_t column_index ) const
    {
        KVS_ASSERT( column_index < m_columns.size() );
        return m_columns[ column_index ];
    }

    ValueTable& operator =( const ValueTable& rhs )
    {
        ValueTable temp( rhs );
        temp.swap( *this );
        return *this;
    }

    friend bool operator ==( const ValueTable& lhs, const ValueTable& rhs )
    {
        return lhs.columnSize() == rhs.columnSize() &&
               std::equal( lhs.beginColumn(), lhs.endColumn(), rhs.beginColumn() );
    }

    friend bool operator <( const ValueTable& lhs, const ValueTable& rhs )
    {
        return std::lexicographical_compare( lhs.beginColumn(), lhs.endColumn(),
                                             rhs.beginColumn(), rhs.endColumn() );
    }

    friend bool operator !=( const ValueTable& lhs, const ValueTable& rhs )
    {
        return !( lhs == rhs );
    }

    friend bool operator >( const ValueTable& lhs, const ValueTable& rhs )
    {
        return rhs < lhs;
    }

    friend bool operator <=( const ValueTable& lhs, const ValueTable& rhs )
    {
        return !( rhs < lhs );
    }

    friend bool operator >=( const ValueTable& lhs, const ValueTable& rhs )
    {
        return !( lhs < rhs );
    }

    column_iterator beginColumn()
    {
        return m_columns.begin();
    }

    const_column_iterator beginColumn() const
    {
        return m_columns.begin();
    }

    column_iterator endColumn()
    {
        return this->beginColumn() + this->columnSize();
    }

    const_column_iterator endColumn() const
    {
        return this->beginColumn() + this->columnSize();
    }

    column_reverse_iterator rbeginColumn()
    {
        return column_reverse_iterator( this->endColumn() );
    }

    const_column_reverse_iterator rbeginColumn() const
    {
        return const_column_reverse_iterator( this->endColumn() );
    }

    column_reverse_iterator rendColumn()
    {
        return column_reverse_iterator( this->beginColumn() );
    }

    const_column_reverse_iterator rendColumn() const
    {
        return const_column_reverse_iterator( this->beginColumn() );
    }

    column_reference frontColumn()
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[0];
    }

    const_column_reference frontColumn() const
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[0];
    }

    column_reference backColumn()
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[ this->columnSize() - 1 ];
    }

    const_column_reference backColumn() const
    {
        KVS_ASSERT( !this->empty() );
        return ( *this )[ this->columnSize() - 1 ];
    }

    void pushBackColumn( const Column& column )
    {
        m_columns.push_back( column );
    }

    void popBackColumn()
    {
        m_columns.pop_back();
    }

    size_t columnSize() const
    {
        return m_columns.size();
    }

    column_reference column( const size_t index )
    {
        return ( *this )[ index ];
    }

    const_column_reference column( const size_t index ) const
    {
        return ( *this )[ index ];
    }

    const kvs::ValueArray<T> row( const size_t index ) const
    {
        size_t ncolumns = this->columnSize();
        kvs::ValueArray<T> row( ncolumns );
        for ( size_t i = 0; i < ncolumns; i++ )
        {
            row[i] = this->column(i).at( index );
        }

        return row;
    }

    T& at( const size_t row_index, const size_t column_index )
    {
        return ( *this )[ column_index ].at( row_index );
    }

    const T& at( const size_t row_index, const size_t column_index ) const
    {
        return ( *this )[ column_index ].at( row_index );
    }

    const Columns& columns() const
    {
        return m_columns;
    }

    size_t size() const
    {
        size_t ncells = 0;
        for ( size_t i = 0; i < m_columns.size(); i++ )
        {
            ncells += m_columns[i].size();
        }

        return ncells;
    }

    bool empty() const
    {
        return this->columnSize() == 0;
    }

    void clear()
    {
        m_columns.clear();
    }

    void swap( ValueTable& other )
    {
        std::swap( m_columns, other.m_columns );
    }
};

} // end of namespace kvs

#endif // KVS__VALUE_TABLE_H_INCLUDE
