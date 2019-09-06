#include <kvs/ValueTable>
#include <kvs/Indent>
#include <kvs/Timer>


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
        std::cout << "kvs::ValueTable<float> v( 3, 4 );" << std::endl;
        std::cout << indent << "v = " << v1 << std::endl;

        const float c0[nrows] = { 1.0f, 1.0f, 1.0f };
        const float c1[nrows] = { 2.0f, 2.0f, 2.0f };
        const float c2[nrows] = { 3.0f, 3.0f, 3.0f };
        const float c3[nrows] = { 4.0f, 4.0f, 4.0f };
        kvs::ValueTable<float> v2( ncols );
        v2[0] = kvs::ValueArray<float>( c0, nrows );
        v2[1] = kvs::ValueArray<float>( c1, nrows );
        v2[2] = kvs::ValueArray<float>( c2, nrows );
        v2[3] = kvs::ValueArray<float>( c3, nrows );
        std::cout << "kvs::ValueTable<float> v( 4 );" << std::endl;
        std::cout << "v[0] = kvs::ValueArray<float>( c0, nrows );" << std::endl;
        std::cout << "v[1] = kvs::ValueArray<float>( c1, nrows );" << std::endl;
        std::cout << "..." << std::endl;
        std::cout << indent << "v = " << v2 << std::endl;
        std::cout << indent << "c0 = " << v2.column(0) << std::endl;
        std::cout << indent << "r0 = " << v2.row(0) << std::endl;

        kvs::ValueTable<int> v3;
        for ( size_t col = 0; col < ncols; ++col )
        {
            v3.pushBackColumn( kvs::ValueArray<int>::Random( nrows, 10, 99 ) );
        }
        std::cout << "kvs::ValueTable<int> v;" << std::endl;
        std::cout << "v.pushBackColumn( kvs::ValueArray<int>::Random( nrows, 1, 9 ) );" << std::endl;
        std::cout << "..." << std::endl;
        std::cout << indent << "v = " << v3 << std::endl;
        for ( size_t row = 0; row < nrows; ++row )
        {
            std::cout << indent << "r" << row << " = " << v3.row(row) << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Shallow Copy" << std::endl;
    {
        const size_t ncols = 2;
        const size_t nrows = 2;
        kvs::ValueTable<float> a( nrows, ncols );
        a[0][0] = 1.0f; a[1][0] = 3.0f;
        a[0][1] = 2.0f; a[1][1] = 4.0f;
        std::cout << indent << "a = " << a << std::endl;

        kvs::ValueTable<float> b = a; // shallow copy
        std::cout << "kvs::ValueTable<float> b = a;" << std::endl;
        std::cout << indent << "b = " << b << std::endl;

        a[0][0] = 0.0f;
        std::cout << "a[0][0] = 0.0f;" << std::endl;
        std::cout << indent << "a = " << a << std::endl;
        std::cout << indent << "b = " << b << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Deep Copy" << std::endl;
    {
        const size_t ncols = 2;
        const size_t nrows = 2;
        kvs::ValueTable<float> a( nrows, ncols );
        a[0][0] = 1.0f; a[1][0] = 3.0f;
        a[0][1] = 2.0f; a[1][1] = 4.0f;
        std::cout << indent << "a = " << a << std::endl;

        kvs::ValueTable<float> b = a.clone(); // deep copy
        std::cout << "kvs::ValueTable<float> b = a.clone();" << std::endl;
        std::cout << indent << "b = " << b << std::endl;

        a[0][0] = 0.0f;
        std::cout << "a[0][0] = 0.0f;" << std::endl;
        std::cout << indent << "a = " << a << std::endl;
        std::cout << indent << "b = " << b << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Random" << std::endl;
    {
        kvs::ValueTable<int> v = kvs::ValueTable<int>::Random( 3, 4 );
        std::cout << "kvs::ValueTable<int>::Random( 3, 4 );" << std::endl;
        std::cout << indent << "v = " << v << std::endl;

        const kvs::UInt32 seed = 1;
        v = kvs::ValueTable<int>::Random( 3, 4, seed );
        std::cout << "kvs::ValueTable<int>::Random( 3, 4, seed );" << std::endl;
        std::cout << indent << "v = " << v << std::endl;

        const int min = 0;
        const int max = 9;
        v = kvs::ValueTable<int>::Random( 3, 4, min, max );
        std::cout << "kvs::ValueTable<int>::Random( 3, 4, min, max );" << std::endl;
        std::cout << indent << "v = " << v << std::endl;

        v = kvs::ValueTable<int>::Random( 3, 4, min, max, seed );
        std::cout << "kvs::ValueTable<int>::Random( 3, 4, min, max, seed );" << std::endl;
        std::cout << indent << "v = " << v << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Linear" << std::endl;
    {
        kvs::ValueTable<int> v1 = kvs::ValueTable<int>::Linear( 3, 4 );
        std::cout << "kvs::ValueTable<int> v = kvs::ValueTable<int>::Linear( 3, 4 );" << std::endl;
        std::cout << indent << "v = " << v1 << std::endl;

        v1 = kvs::ValueTable<int>::Linear( 3, 4, 10 );
        std::cout << "kvs::ValueTable<int> v = kvs::ValueTable<int>::Linear( 3, 4, 10 );" << std::endl;
        std::cout << indent << "v = " << v1 << std::endl;

        v1 = kvs::ValueTable<int>::Linear( 3, 4, 5, 3 );
        std::cout << "kvs::ValueTable<int> v = kvs::ValueTable<int>::Linear( 3, 4, 5, 3 );" << std::endl;
        std::cout << indent << "v = " << v1 << std::endl;

        kvs::ValueTable<float> v2 = kvs::ValueTable<float>::Linear( 3, 4, 5.1f );
        std::cout << "kvs::ValueTable<float> v = kvs::ValueTable<float>::Linear( 3, 4, 5.1 );" << std::endl;
        std::cout << indent << "v = " << v2 << std::endl;

        v2 = kvs::ValueTable<float>::Linear( 3, 4, 4.9f, 0.1f );
        std::cout << "kvs::ValueTable<float> v = kvs::ValueTable<float>::Linear( 3, 4, 2.9, 0.1 );" << std::endl;
        std::cout << indent << "v = " << v2 << std::endl;

        v2 = kvs::ValueTable<float>::Linear( 3, 4, 0.1f, -4.9f );
        std::cout << "kvs::ValueTable<float> v = kvs::ValueTable<float>::Linear( 3, 4, 0.1, -2.3 );" << std::endl;
        std::cout << indent << "v = " << v2 << std::endl;
        std::cout << std::endl;
    }

    // Iterators.
    std::cout << "Iterator" << std::endl;
    {
        kvs::ValueTable<int> v = kvs::ValueTable<int>::Linear( 3, 4 );
        std::cout << indent << "v = " << v << std::endl;

        {
            std::cout << "kvs::ValueTable<int>::iterator ";
            std::cout << "(column-major order)" << std::endl;
            kvs::ValueTable<int>::iterator c = v.begin();
            std::cout << indent << "{" << *c;
            while ( ++c != v.end() ) { std::cout << ", " << *c; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << "kvs::ValueTable<int>::reverse_iterator ";
            std::cout << "(column-major order - reverse)" << std::endl;
            kvs::ValueTable<int>::reverse_iterator c = v.rbegin();
            std::cout << indent << "{" << *c;
            while ( ++c != v.rend() ) { std::cout << ", " << *c; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << "kvs::ValueTable<int>::row_order_iterator ";
            std::cout << "(row-major order)" << std::endl;
            kvs::ValueTable<int>::row_order_iterator r = v.beginInRowOrder();
            std::cout << indent << "{" << *r;
            while ( ++r != v.endInRowOrder() ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }

        {
            std::cout << "kvs::ValueTable<int>::row_order_reverse_iterator ";
            std::cout << "(row-major order - reverse)" << std::endl;
            kvs::ValueTable<int>::row_order_reverse_iterator r = v.rbeginInRowOrder();
            std::cout << indent << "{" << *r;
            while ( ++r != v.rendInRowOrder() ) { std::cout << ", " << *r; }
            std::cout << "}" << std::endl;
        }
    }

    return 0;
}
