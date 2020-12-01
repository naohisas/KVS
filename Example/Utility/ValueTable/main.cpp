#include <kvs/ValueTable>
#include <kvs/Indent>
#include <kvs/Timer>


void PerfTest( const size_t size, const size_t n )
{
    std::cout << "Performance Test" << std::endl;

    typedef kvs::ValueTable<float> Table;
    const kvs::Indent indent(4);

    // Random
    /*
    {
        Table t;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { t = Table::Random( size, size ); }
        timer.stop();
        std::cout << indent << "Random: " << timer.sec() << " [sec]" << std::endl;
    }
    */

    // Random
    /*
    {
        Table t;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { t = Table::Linear( size, size ); }
        timer.stop();
        std::cout << indent << "Linear: " << timer.sec() << " [sec]" << std::endl;
    }
    */

    // Iterators
    {
        Table t = Table::Random( size, size );
        {
            kvs::Timer timer( kvs::Timer::Start );
            for ( size_t j = 0; j < size; ++j )
            {
                for ( size_t i = 0; i < size; ++i )
                {
                    t.at( i, j ) *= 2;
                }
            }
            timer.stop();
            std::cout << indent << "For-loop (column-major): " << timer.sec() << " [sec]" << std::endl;
        }

        {
            kvs::Timer timer( kvs::Timer::Start );
            for ( size_t i = 0; i < size; ++i )
            {
                for ( size_t j = 0; j < size; ++j )
                {
                    t.at( i, j ) *= 2;
                }
            }
            timer.stop();
            std::cout << indent << "For-loop (row-major): " << timer.sec() << " [sec]" << std::endl;
        }

        {
            kvs::Timer timer( kvs::Timer::Start );
            for ( Table::iterator i = t.begin(); i != t.end(); ++i ) { *i *= 2; }
            timer.stop();
            std::cout << indent << "Iterator: " << timer.sec() << " [sec]" << std::endl;
        }

        {
            kvs::Timer timer( kvs::Timer::Start );
            for ( Table::reverse_iterator i = t.rbegin(); i != t.rend(); ++i ) { *i *= 2; }
            timer.stop();
            std::cout << indent << "Reverse iterator: " << timer.sec() << " [sec]" << std::endl;
        }

        {
            kvs::Timer timer( kvs::Timer::Start );
            Table::row_order_iterator i = t.beginInRowOrder();
            for ( ; i != t.endInRowOrder(); ++i ) { *i *= 1; }
            timer.stop();
            std::cout << indent << "Row order iterator: " << timer.sec() << " [sec]" << std::endl;
        }

        {
            kvs::Timer timer( kvs::Timer::Start );
            Table::row_order_reverse_iterator i = t.rbeginInRowOrder();
            for ( ; i != t.rendInRowOrder(); ++i ) { *i *= 2; }
            timer.stop();
            std::cout << indent << "Row order reverse iterator: " << timer.sec() << " [sec]" << std::endl;
        }
    }
}

int main()
{
    const kvs::Indent indent(4);

    std::cout << "Initialization" << std::endl;
    {
        const size_t ncols = 4;
        const size_t nrows = 3;
        kvs::ValueTable<float> v1( nrows, ncols );
        v1[0][0] = 1.0f; v1[1][0] = 2.0f; v1[2][0] = 9.0f; v1[3][0] = 2.0f;
        v1[0][1] = 2.0f; v1[1][1] = 4.0f; v1[2][1] = 7.0f; v1[3][1] = 9.0f;
        v1[0][2] = 3.0f; v1[1][2] = 6.0f; v1[2][2] = 5.0f; v1[3][2] = 3.0f;
        std::cout << indent << "kvs::ValueTable<float> v( nrows, ncols ); // nrows = 3, ncols = 4" << std::endl;
        std::cout << indent << "v[0][0] = 1; v[1][0] = 2; v[2][0] = 9; v[3][0] = 2; // row 0" << std::endl;
        std::cout << indent << "..." << std::endl;
        std::cout << indent.nextIndent() << "v: " << v1.format() << std::endl;

        kvs::ValueTable<float> v2( ncols );
        v2[0] = kvs::ValueArray<float>( { 1.0f, 1.0f, 1.0f } );
        v2[1] = kvs::ValueArray<float>( { 2.0f, 2.0f, 2.0f } );
        v2[2] = kvs::ValueArray<float>( { 3.0f, 3.0f, 3.0f } );
        v2[3] = kvs::ValueArray<float>( { 4.0f, 4.0f, 4.0f } );
        std::cout << indent << "kvs::ValueTable<float> v( ncols );" << std::endl;
        std::cout << indent << "v[0] = kvs::ValueArray<float>( { 1.0f, 1.0f, 1.0f } ); // col 0" << std::endl;
        std::cout << indent << "..." << std::endl;
        std::cout << indent.nextIndent() << "v: " << v2.format() << std::endl;
        std::cout << indent.nextIndent() << "col 0: " << v2.column(0).format() << std::endl;
        std::cout << indent.nextIndent() << "row 0: " << v2.row(0).format() << std::endl;

        kvs::ValueTable<int> v3;
        for ( size_t col = 0; col < ncols; ++col )
        {
            v3.pushBackColumn( kvs::ValueArray<int>::Random( nrows, 10, 99 ) );
        }
        std::cout << indent << "kvs::ValueTable<int> v;" << std::endl;
        std::cout << indent << "v.pushBackColumn( kvs::ValueArray<int>::Random( nrows, 1, 9 ) );" << std::endl;
        std::cout << indent << "..." << std::endl;
        std::cout << indent.nextIndent() << "v: " << v3.format() << std::endl;
        for ( size_t row = 0; row < nrows; ++row )
        {
            std::cout << indent.nextIndent() << "row " << row << ": " << v3.row(row).format() << std::endl;
        }

        kvs::ValueTable<float> v4 = {
            {1, 2, 3},   // column 0
            {4, 5, 6},   // column 1
            {7, 8, 9} }; // column 2
        std::cout << indent << "kvs::ValueTable<float> v = { {1, 2, 3}, {...}, {...} };" << std::endl;
        std::cout << indent.nextIndent() << "v: " << v4.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Shallow Copy" << std::endl;
    {
        kvs::ValueTable<int> a{ {1, 2}, {3, 4} };
        std::cout << "a = " << a.format() << std::endl;

        auto b = a; // shallow copy
        std::cout << indent << "b = a;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;

        a[0][0] = 0;
        std::cout << indent << "a[0][0] = 0;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Deep Copy" << std::endl;
    {
        kvs::ValueTable<int> a{ {1, 2}, {3, 4} };
        std::cout << "a = " << a.format() << std::endl;

        auto b = a.clone(); // deep copy
        std::cout << indent << "b = a.clone();" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;

        a[0][0] = 0;
        std::cout << indent << "a[0][0] = 0;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Random" << std::endl;
    {
        const size_t nrows = 3;
        const size_t ncols = 4;
        auto v = kvs::ValueTable<int>::Random( nrows, ncols );
        std::cout << indent << "kvs::ValueTable<int>::Random( nrows, ncols ); // nrows = 3, ncols = 4" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        const kvs::UInt32 seed = 1;
        v = kvs::ValueTable<int>::Random( nrows, ncols, seed );
        std::cout << indent << "kvs::ValueTable<int>::Random( nrows, ncols, seed ); // seed = 1" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        const int min = 0;
        const int max = 9;
        v = kvs::ValueTable<int>::Random( nrows, ncols, min, max );
        std::cout << indent << "kvs::ValueTable<int>::Random( nrows, ncols, min, max ); // min = 0, max = 9" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        v = kvs::ValueTable<int>::Random( nrows, ncols, min, max, seed );
        std::cout << indent << "kvs::ValueTable<int>::Random( nrows, ncols, min, max, seed );" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Linear" << std::endl;
    {
        const size_t nrows = 3;
        const size_t ncols = 4;
        auto v1 = kvs::ValueTable<int>::Linear( nrows, ncols );
        std::cout << indent << "kvs::ValueTable<int>::Linear( nrows, ncols );" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        v1 = kvs::ValueTable<int>::Linear( nrows, ncols, 10 );
        std::cout << indent << "kvs::ValueTable<int>::Linear( nrows, ncols, 10 ); // start = 10" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        v1 = kvs::ValueTable<int>::Linear( nrows, ncols, 5, 3 );
        std::cout << indent << "kvs::ValueTable<int>::Linear( nrows, ncols, 5, 3 ); // start = 5, step = 3" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        auto v2 = kvs::ValueTable<float>::Linear( nrows, ncols, 5.1f );
        std::cout << indent << "kvs::ValueTable<float>::Linear( nrows, ncols, 5.1 ); // start = 5.1" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;

        v2 = kvs::ValueTable<float>::Linear( nrows, ncols, 4.9f, 0.1f );
        std::cout << indent << "kvs::ValueTable<float>::Linear( nrows, ncols, 2.9, 0.1 ); // start = 2.9, step = 0.1" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;

        v2 = kvs::ValueTable<float>::Linear( nrows, ncols, 0.1f, -4.9f );
        std::cout << indent << "kvs::ValueTable<float>::Linear( nrows, ncols, 0.1, -2.3 ); // start = 0.1, step = -2.3" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;
        std::cout << std::endl;
    }

    // Iterators.
    std::cout << "Iterator" << std::endl;
    {
        kvs::ValueTable<int> v = kvs::ValueTable<int>::Linear( 3, 4 );
        std::cout << v.format() << std::endl;

        {
            std::cout << indent << "kvs::ValueTable<int>::iterator (column-major order)" << std::endl;
            kvs::ValueTable<int>::iterator c = v.begin();
            std::cout << indent.nextIndent() << "{" << *c;
            while ( ++c != v.end() ) { std::cout << ", " << *c; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::reverse_iterator (column-major order: reverse)" << std::endl;
            kvs::ValueTable<int>::reverse_iterator c = v.rbegin();
            std::cout << indent.nextIndent() << "{" << *c;
            while ( ++c != v.rend() ) { std::cout << ", " << *c; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::iterator (column-major order: colum(1))" << std::endl;
            kvs::ValueTable<int>::iterator r = v.beginColumn(1);
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.endColumn(1) ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::iterator (column-major order: colum(1) - column(2))" << std::endl;
            kvs::ValueTable<int>::iterator r = v.beginColumn(1);
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.endColumn(2) ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::row_order_iterator (row-major order)" << std::endl;
            kvs::ValueTable<int>::row_order_iterator r = v.beginInRowOrder();
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.endInRowOrder() ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::row_order_reverse_iterator (row-major order: reverse)" << std::endl;
            kvs::ValueTable<int>::row_order_reverse_iterator r = v.rbeginInRowOrder();
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.rendInRowOrder() ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::row_order_iterator (row-major order: row(1))" << std::endl;
            kvs::ValueTable<int>::row_order_iterator r = v.beginRow(1);
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.endRow(1) ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::row_order_iterator (row-major order: row(1) - row(2))" << std::endl;
            kvs::ValueTable<int>::row_order_iterator r = v.beginRow(1);
            std::cout << indent.nextIndent() << "{" << *r;
            while ( ++r != v.endRow(2) ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << indent << "kvs::ValueTable<int>::iterator ( *c *= 2 )" << std::endl;
            kvs::ValueTable<int>::iterator c = v.begin();
            std::cout << indent.nextIndent() << "{" << ( *c * 2 );
            while ( ++c != v.end() ) { std::cout << ", " << ( *c * 2 ); }
            std::cout << "}" << std::endl;
        }

        std::cout << std::endl;
    }

    // Faltten.
    std::cout << "Faltten" << std::endl;
    {
        const size_t nrows = 3;
        const size_t ncols = 3;
        auto v = kvs::ValueTable<int>::Random( nrows, ncols, 0, 9 );
        std::cout << "v = " << v.format() << std::endl;

        std::cout << indent << "Column-major order array" << std::endl;
        std::cout << indent.nextIndent() << kvs::ValueArray<int>( v.beginColumn(0), v.endColumn(ncols-1) ).format() << std::endl;

        std::cout << indent << "Row-major order array" << std::endl;
        std::cout << indent.nextIndent() << kvs::ValueArray<int>( v.beginRow(0), v.endRow(nrows-1) ).format() << std::endl;
        std::cout << std::endl;
    }

    // Slicing.
    std::cout << "Slicing" << std::endl;
    {
        const size_t nrows = 3;
        const size_t ncols = 4;
        auto v = kvs::ValueTable<int>::Linear( nrows, ncols );
        std::cout << "v = " << v.format() << std::endl;

        std::cout << indent << "Column-major order slicing" << std::endl;
        for ( size_t i = 0; i < ncols; ++i )
        {
            std::cout << indent.nextIndent() << "col " << i << ": "
                      << kvs::ValueArray<int>( v.beginColumn(i), v.endColumn(i) ).format() << std::endl;
        }
        std::cout << indent.nextIndent() << "col 1-2: "
                  << kvs::ValueArray<int>( v.beginColumn(1), v.endColumn(2) ).format() << std::endl;

        std::cout << indent << "Row-major order slicing" << std::endl;
        for ( size_t i = 0; i < nrows; ++i )
        {
            std::cout << indent.nextIndent() << "row " << i << ": "
                      << kvs::ValueArray<int>( v.beginRow(i), v.endRow(i) ).format() << std::endl;
        }
        std::cout << indent.nextIndent() << "row 1-2: "
                  << kvs::ValueArray<int>( v.beginRow(1), v.endRow(2) ).format() << std::endl;

        std::cout << indent << "v.slice( {1} );" << std::endl;
        std::cout << indent.nextIndent() << v.slice( {1} ).format() << std::endl;

        std::cout << indent << "v.slice( {1,3} );" << std::endl;
        std::cout << indent.nextIndent() << v.slice( {1,3} ).format() << std::endl;

        std::cout << indent << "v.slice( {0,4,2} );" << std::endl;
        std::cout << indent.nextIndent() << v.slice( {0,4,2} ).format() << std::endl;

        std::cout << indent << "v.slice( {1,3}, {0,3} );" << std::endl;
        std::cout << indent.nextIndent() << v.slice( {1,3}, {0,3} ).format() << std::endl;

        std::cout << indent << "v.sliceRow( {0,2} );" << std::endl;
        std::cout << indent.nextIndent() << v.sliceRow( {0,2} ).format() << std::endl;

        std::cout << indent << "v.sliceColumn( {0,2} );" << std::endl;
        std::cout << indent.nextIndent() << v.sliceColumn( {0,2} ).format() << std::endl;

        std::cout << indent << "v[{0,2}];" << std::endl;
        std::cout << indent.nextIndent() << v[{0,2}].format() << std::endl;
        std::cout << std::endl;
    }

    PerfTest( 10000, 10000 );

    return 0;
}
