/*****************************************************************************/
/**
 *  @file   ValueTable.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <kvs/Value>
#include <kvs/ValueArray>
#include <kvs/SliceRange>
#include <kvs/Assert>
#include <kvs/Deprecated>


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
    class ColumnOrderIterator;
    class RowOrderIterator;

public:
    using Column = kvs::ValueArray<T>;
    using Columns = std::vector<Column>;

    // Iterators.
    using column_iterator = typename Columns::iterator;
    using const_column_iterator = typename Columns::const_iterator;
    using column_reverse_iterator = std::reverse_iterator<column_iterator>;
    using const_column_reverse_iterator = const std::reverse_iterator<const_column_iterator>;

    using column_reference = Column&;
    using const_column_reference = const Column&;

    using this_type = ValueTable<T>;
    using value_type = T;

    // Column order iterators.
    using column_order_iterator = ColumnOrderIterator;
    using const_column_order_iterator = const ColumnOrderIterator;
    using column_order_reverse_iterator = std::reverse_iterator<column_order_iterator>;
    using const_column_order_reverse_iterator = std::reverse_iterator<const_column_order_iterator>;

    // Row order iterators.
    using row_order_iterator = RowOrderIterator;
    using const_row_order_iterator = const RowOrderIterator;
    using row_order_reverse_iterator = std::reverse_iterator<row_order_iterator>;
    using const_row_order_reverse_iterator = std::reverse_iterator<const_row_order_iterator>;

    // Standard iterators (column-order).
    using iterator = column_order_iterator;
    using const_iterator = const_column_order_iterator;
    using reverse_iterator = column_order_reverse_iterator;
    using const_reverse_iterator = const_column_order_reverse_iterator;

private:
    Columns m_columns; ///< column vector

public:
    static ValueTable Random( const size_t nrows, const size_t ncols, const unsigned int seed = 0 )
    {
        this_type m( nrows, ncols );
        if ( seed == 0 ) { static bool flag = true; if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; } }
        else { kvs::Value<T>::SetSeed( seed ); }
        for ( auto& c : m.columns() ) { for ( auto& r : c ) { r = kvs::Value<T>::Random(); } }
        return m;
    }

    static ValueTable Random( const size_t nrows, const size_t ncols, const T min, const T max, const unsigned int seed = 0 )
    {
        this_type m( nrows, ncols );
        if ( seed == 0 ) { static bool flag = true; if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; } }
        else { kvs::Value<T>::SetSeed( seed ); }
        for ( auto& c : m.columns() ) { for ( auto& r : c ) { r = kvs::Value<T>::Random( min, max ); } }
        return m;
    }

    static ValueTable Linear( const size_t nrows, const size_t ncols, const T start = T(0), const T step = T(1) )
    {
        this_type m( ncols );
        const_column_iterator last = m.endColumn();
        T value = start;
        for ( column_iterator c = m.beginColumn(); c != last; ++c, value += step * nrows )
        {
            *c = Column::Linear( nrows, value, step );
        }
        return m;
    }

public:
    ValueTable(){}

    explicit ValueTable( const size_t ncolumns )
    {
        m_columns.resize( ncolumns );
    }

    ValueTable( const size_t nrows, const size_t ncolumns )
    {
        m_columns.resize( ncolumns );
        for ( size_t i = 0; i < ncolumns; ++i )
        {
            m_columns[i] = kvs::ValueArray<T>( nrows );
        }
    }

    ValueTable( std::initializer_list<Column> list )
    {
        m_columns.resize( std::distance( list.begin(), list.end() ) );
        std::copy( list.begin(), list.end(), m_columns.begin() );
    }

    std::string format(
        const std::string delim = ", " ) const
    {
        return this->format( delim, "{", "}" );
    }

    std::string format(
        const std::string bracket_l,
        const std::string bracket_r ) const
    {
        return this->format( ", ", bracket_l, bracket_r );
    }

    std::string format(
        const std::string delim,
        const std::string bracket_l,
        const std::string bracket_r ) const
    {
        const std::string tab = ( bracket_l.empty() && bracket_r.empty() ) ? "\t" : "";
        std::ostringstream os;
        os << bracket_l;
        {
            const size_t ncols = this->columnSize();
            if ( ncols == 0 ) { os << bracket_l << " " << bracket_r; }
            else
            {
                os  << m_columns[0].format( delim, bracket_l, bracket_r );
                for ( size_t i = 1; i < ncols; ++i )
                {
                    os << delim << tab << m_columns[i].format( delim, bracket_l, bracket_r );
                }
            }
        }
        os << bracket_r;
        return os.str();
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

    ValueTable operator []( const kvs::SliceRange& column_range ) const
    {
        return this->slice( column_range );
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
        return std::lexicographical_compare(
            lhs.beginColumn(), lhs.endColumn(),
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

    friend std::ostream& operator <<( std::ostream& os, const this_type& rhs )
    {
//        const size_t ncols = rhs.columnSize();
//        if ( ncols == 0 ) { return os << "{{ }}"; }
//        os << "{" << rhs[0];
//        for ( size_t i = 1; i < ncols; ++i ) { os << ", " << rhs[i]; }
//        return os << "}";
        return os << rhs.format( " ", "", "" );
    }

    iterator begin()
    {
        return this->beginInColumnOrder();
    }

    iterator end()
    {
        return this->endInColumnOrder();
    }

    reverse_iterator rbegin()
    {
        return this->rbeginInColumnOrder();
    }

    reverse_iterator rend()
    {
        return this->rendInColumnOrder();
    }

    // Column order iteratirons.
    column_order_iterator beginInColumnOrder()
    {
        typename Column::iterator row = m_columns.begin()->begin();
        column_iterator column = m_columns.begin();
        column_iterator begin = m_columns.begin();
        column_iterator end = m_columns.end();
        return column_order_iterator( row, column, begin, end );
    }

    column_order_iterator endInColumnOrder()
    {
        typename Column::iterator row = m_columns.end()->end();
        column_iterator column = m_columns.end();
        column_iterator begin = m_columns.begin();
        column_iterator end = m_columns.end();
        return column_order_iterator( row, column, begin, end );
    }

    column_order_reverse_iterator rbeginInColumnOrder()
    {
        return column_order_reverse_iterator( this->endInColumnOrder() );
    }

    column_order_reverse_iterator rendInColumnOrder()
    {
        return column_order_reverse_iterator( this->beginInColumnOrder() );
    }

    // Row order iterations.
    row_order_iterator beginInRowOrder()
    {
        typename Column::iterator row = m_columns.begin()->begin();
        column_iterator column = m_columns.begin();
        column_iterator begin = m_columns.begin();
        column_iterator end = m_columns.end();
        size_t offset = 0;
        return row_order_iterator( row, column, begin, end, offset );
    }

    row_order_iterator endInRowOrder()
    {
        typename Column::iterator row = m_columns.end()->end();
        column_iterator column = m_columns.end();
        column_iterator begin = m_columns.begin();
        column_iterator end = m_columns.end();
        size_t offset = ( end - 1 )->size();
        return row_order_iterator( row, column, begin, end, offset );
    }

    row_order_reverse_iterator rbeginInRowOrder()
    {
        return row_order_reverse_iterator( this->endInRowOrder() );
    }

    row_order_reverse_iterator rendInRowOrder()
    {
        return row_order_reverse_iterator( this->beginInRowOrder() );
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

    // Deprecated ?
    // {
    column_iterator beginColumn() { return m_columns.begin(); }
    const_column_iterator beginColumn() const { return m_columns.begin(); }
    column_iterator endColumn() { return this->beginColumn() + this->columnSize(); }
    const_column_iterator endColumn() const { return this->beginColumn() + this->columnSize(); }
    column_reverse_iterator rbeginColumn() { return column_reverse_iterator( this->endColumn() ); }
    const_column_reverse_iterator rbeginColumn() const { return const_column_reverse_iterator( this->endColumn() ); }
    column_reverse_iterator rendColumn() { return column_reverse_iterator( this->beginColumn() ); }
    const_column_reverse_iterator rendColumn() const { return const_column_reverse_iterator( this->beginColumn() ); }
    column_reference frontColumn() { KVS_ASSERT( !this->empty() ); return ( *this )[0]; }
    const_column_reference frontColumn() const { KVS_ASSERT( !this->empty() ); return ( *this )[0]; }
    column_reference backColumn() { KVS_ASSERT( !this->empty() ); return ( *this )[ this->columnSize() - 1 ]; }
    const_column_reference backColumn() const { KVS_ASSERT( !this->empty() ); return ( *this )[ this->columnSize() - 1 ]; }
    // }

    size_t columnSize() const
    {
        return m_columns.size();
    }

    void pushBackColumn( const Column& column )
    {
        m_columns.push_back( column );
    }

    void popBackColumn()
    {
        m_columns.pop_back();
    }

    size_t rowSize( const size_t column_index = 0 ) const
    {
        return ( *this )[ column_index ].size();
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

    Columns& columns()
    {
        return m_columns;
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

    void swap( ValueTable& other )
    {
        std::swap( m_columns, other.m_columns );
    }

    ValueTable clone() const
    {
        ValueTable result;
        const_column_iterator last = this->endColumn();
        for ( const_column_iterator c = this->beginColumn(); c != last; ++c )
        {
            result.pushBackColumn( c->clone() );
        }
        return result;
    }

    ValueTable slice( const kvs::SliceRange& column_range ) const
    {
        return this->sliceColumn( column_range );
    }

    ValueTable slice( const kvs::SliceRange& row_range, const kvs::SliceRange& column_range ) const
    {
        return this->sliceColumn( column_range ).sliceRow( row_range );
    }

    ValueTable sliceColumn( const kvs::SliceRange& column_range ) const
    {
        const auto& range = column_range;
        range.adjust( this->columnSize() );

        ValueTable ret( range.size() );
        for ( long i = range.start, j = 0; i < range.stop; i += range.step )
        {
            ret[j++] = this->column(i);
        }
        return ret;
    }

    ValueTable sliceRow( const kvs::SliceRange& row_range ) const
    {
        ValueTable ret;
        for ( size_t i = 0; i < this->columnSize(); ++i )
        {
            ret.pushBackColumn( this->column(i).slice( row_range ) );
        }
        return ret;
    }

    void fill( const T& value )
    {
        const_column_iterator last = this->endColumn();
        for ( column_iterator c = this->beginColumn(); c != last; ++c )
        {
            c->fill( value );
        }
    }

    void release()
    {
        Columns().swap( m_columns );
    }

public:
    KVS_DEPRECATED( void clear() ) { m_columns.clear(); }
};

template <typename T>
class ValueTable<T>::ColumnOrderIterator
{
private:
    using this_type = ColumnOrderIterator;
    using row_reference = typename ValueTable<T>::Column::reference;
    using row_iterator = typename ValueTable<T>::Column::iterator;
    using const_row_reference = typename ValueTable<T>::Column::const_reference;
    using const_row_iterator = typename ValueTable<T>::Column::const_iterator;

public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

private:
    row_iterator m_row_iterator;
    column_iterator m_col_iterator;
    column_iterator m_col_begin;
    column_iterator m_col_end;

public:
    ColumnOrderIterator( const ColumnOrderIterator& other ):
        m_row_iterator( other.m_row_iterator ),
        m_col_iterator( other.m_col_iterator ),
        m_col_begin( other.m_col_begin ),
        m_col_end( other.m_col_end ) {}

    ColumnOrderIterator(
        row_iterator row_iterator,
        column_iterator col_iterator,
        column_iterator& col_begin,
        column_iterator& col_end ):
        m_row_iterator( row_iterator ),
        m_col_iterator( col_iterator ),
        m_col_begin( col_begin ),
        m_col_end( col_end ) {}

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

    this_type operator +( unsigned int n )
    {
        this_type result = *this;
        result += n;
        return result;
    }

    this_type operator -( unsigned int n )
    {
        this_type result = *this;
        result -= n;
        return result;
    }

    row_reference operator *()
    {
        return *m_row_iterator;
    }

    const_row_reference operator *() const
    {
        return *m_row_iterator;
    }

    bool operator ==( const this_type& other )
    {
        return ( m_row_iterator == other.m_row_iterator &&
                 m_col_iterator == other.m_col_iterator );
    }

    bool operator !=( const this_type& other )
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
        const size_t nrows = std::distance( m_col_begin->begin(), m_col_begin->end() );
        const size_t j = std::distance( m_col_begin, m_col_iterator );
        const size_t i = ( m_col_iterator == m_col_end ) ? 0 : std::distance( m_col_iterator->begin(), m_row_iterator );
        return j * nrows + i;
    }

    void increment()
    {
        if ( m_row_iterator == m_col_end->end() ) { return; }
        ++m_row_iterator;
        if ( m_row_iterator == m_col_iterator->end() )
        {
            ++m_col_iterator;
            if ( m_col_iterator != m_col_end )
            {
                m_row_iterator = m_col_iterator->begin();
            }
            else
            {
                m_row_iterator = m_col_iterator->end();
            }
        }
    }

    void decrement()
    {
        if ( m_row_iterator == m_col_begin->begin() ) { return; }
        if ( m_col_iterator == m_col_end || m_row_iterator == m_col_iterator->begin() )
        {
            --m_col_iterator;
            m_row_iterator = m_col_iterator->end();
        }
        --m_row_iterator;
    }
};

template <typename T>
class ValueTable<T>::RowOrderIterator
{
private:
    using this_type = RowOrderIterator;
    using row_reference = typename ValueTable<T>::Column::reference;
    using row_iterator = typename ValueTable<T>::Column::iterator;
    using const_row_reference = typename ValueTable<T>::Column::const_reference;
    using const_row_iterator = typename ValueTable<T>::Column::const_iterator;

public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

private:
    row_iterator m_row_iterator;
    column_iterator m_col_iterator;
    column_iterator m_col_begin;
    column_iterator m_col_end;
    size_t m_row_offset;

public:
    RowOrderIterator( const RowOrderIterator& other ):
        m_row_iterator( other.m_row_iterator ),
        m_col_iterator( other.m_col_iterator ),
        m_col_begin( other.m_col_begin ),
        m_col_end( other.m_col_end ),
        m_row_offset( other.m_row_offset ) {}

    RowOrderIterator(
        row_iterator row_iterator,
        column_iterator col_iterator,
        column_iterator& col_begin,
        column_iterator& col_end,
        size_t row_offset ):
        m_row_iterator( row_iterator ),
        m_col_iterator( col_iterator ),
        m_col_begin( col_begin ),
        m_col_end( col_end ),
        m_row_offset( row_offset ) {}

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

    this_type operator +( unsigned int n )
    {
        this_type result = *this;
        result += n;
        return result;
    }

    this_type operator -( unsigned int n )
    {
        this_type result = *this;
        result -= n;
        return result;
    }

    row_reference operator *()
    {
        return *m_row_iterator;
    }

    const_row_reference operator *() const
    {
        return *m_row_iterator;
    }

    bool operator ==( const this_type& other )
    {
        return ( m_col_iterator == other.m_col_iterator &&
                 m_row_iterator == other.m_row_iterator );
    }

    bool operator !=( const this_type& other )
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
        const size_t nrows = std::distance( m_col_begin->begin(), m_col_begin->end() );
        const size_t ncols = std::distance( m_col_begin, m_col_end );
        const size_t j = ( m_row_iterator == m_col_end->end() ) ? nrows : std::distance( m_col_iterator->begin(), m_row_iterator );
        const size_t i = ( m_col_iterator == m_col_end ) ? 0 : std::distance( m_col_begin, m_col_iterator );
        return j * ncols + i;
    }

    void increment()
    {
        if ( m_row_iterator == m_col_end->end() ) { return; }

        ++m_col_iterator;
        if ( m_col_iterator == m_col_end )
        {
            m_col_iterator = m_col_begin;
            m_row_offset++;
        }

        m_row_iterator = m_col_iterator->begin() + m_row_offset;
        if ( m_row_iterator == m_col_iterator->end() )
        {
            m_row_iterator = m_col_end->end();
            m_col_iterator = m_col_end;
        }
    }

    void decrement()
    {
        if ( m_row_iterator == m_col_begin->begin() ) { return; }

        if ( m_col_iterator == m_col_begin || m_row_iterator == m_col_end->end() )
        {
            m_col_iterator = m_col_end;
            --m_row_offset;
        }
        --m_col_iterator;

        m_row_iterator = m_col_iterator->begin() + m_row_offset;
    }
};

} // end of namespace kvs

namespace std
{

template <typename T>
inline void swap( kvs::ValueTable<T>& lhs, kvs::ValueTable<T>& rhs )
{
    lhs.swap( rhs );
}

} // end of namespace std
