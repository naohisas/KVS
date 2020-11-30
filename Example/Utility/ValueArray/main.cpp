#include <kvs/ValueArray>
#include <kvs/Indent>
#include <kvs/Timer>
#include <vector>


void PerfTest( const size_t size, const size_t n )
{
    std::cout << "Performance Test (" << size << " elements, " << n << " times)" << std::endl;

    typedef kvs::ValueArray<float> Array;
    const kvs::Indent indent(4);

    // Random
    {
        Array a;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { a = Array::Random( size ); }
        timer.stop();
        std::cout << indent << "Random: " << timer.sec() << " [sec]" << std::endl;
    }

    // Linear
    {
        Array a;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { a = Array::Linear( size ); }
        timer.stop();
        std::cout << indent << "Linear: " << timer.sec() << " [sec]" << std::endl;
    }

    // Shuffle
    {
        Array a = Array::Random( size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { a.shuffle(); }
        timer.stop();
        std::cout << indent << "Shuffle: " << timer.sec() << " [sec]" << std::endl;
    }

    // Min
    {
        Array a = Array::Random( size );
        float value = 0.0f;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { value = a.min(); }
        timer.stop();
        std::cout << indent << "Min: " << timer.sec() << " [sec]" << std::endl;
    }

    // Sort
    {
        Array a = Array::Random( size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { a.sort(); }
        timer.stop();
        std::cout << indent << "Sort: " << timer.sec() << " [sec]" << std::endl;
    }

    // ArgMin
    {
        Array a = Array::Random( size );
        size_t index = 0;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { index = a.argmin(); }
        timer.stop();
        std::cout << indent << "ArgMin: " << timer.sec() << " [sec]" << std::endl;
    }

    // ArgSort
    {
        Array a = Array::Random( size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < n; ++i ) { a.argsort(); }
        timer.stop();
        std::cout << indent << "ArgSort: " << timer.sec() << " [sec]" << std::endl;
    }
}

int main()
{
    const kvs::Indent indent(4);

    std::cout << "Initialization" << std::endl;
    {
        kvs::ValueArray<float> v1( 3 );
        v1[0] = 1.0f;
        v1[1] = 2.0f;
        v1[2] = 3.0f;
        std::cout << indent << "kvs::ValueArray<float> v( 3 );" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        float temp2[3] = { 2.0f, 3.0f, 4.0f };
        kvs::ValueArray<float> v2( temp2, 3 );
        std::cout << indent << "float temp[3] = {...};" << std::endl;
        std::cout << indent << "kvs::ValueArray<float> v( temp, 3 );" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;

        std::vector<float> temp3 = { 3.0f, 4.0f, 5.0f };
        kvs::ValueArray<float> v3( temp3 );
        std::cout << indent << "std::vector<float> temp = {...};" << std::endl;
        std::cout << indent << "kvs::ValueArray<float> v( temp );" << std::endl;
        std::cout << indent.nextIndent() << v3.format() << std::endl;

        kvs::ValueArray<float> v4 = { 4, 5, 6 };
        std::cout << indent << "kvs::ValueArray<float> v = { 4, 5, 6 };" << std::endl;
        std::cout << indent.nextIndent() << v4.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Shallow Copy" << std::endl;
    {
        kvs::ValueArray<int> a = { 1, 2, 3 };
        std::cout << "a = " << a.format() << std::endl;

        auto b = a; // shallow copy
        std::cout << indent << "b = a;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;

        a[0] = 0;
        std::cout << indent << "a[0] = 0;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Deep Copy" << std::endl;
    {
        kvs::ValueArray<int> a = { 1, 2, 3 };
        std::cout << "a = " << a.format() << std::endl;

        auto b = a.clone(); // deep copy
        std::cout << indent << "b = a.clone();" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;

        a[0] = 0;
        std::cout << indent << "a[0] = 0;" << std::endl;
        std::cout << indent.nextIndent() << "a: " << a.format() << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Assignment" << std::endl;
    {
        kvs::ValueArray<int> a = { 1, 2, 3, 4, 5, 6 };
        std::cout << "a = " << a.format() << std::endl;

        kvs::ValueArray<int> b;
        b.assign( a.begin() + 2, a.end() - 1 );
        std::cout << indent << "b.assign( a.begin() + 2, a.end() - 1 );" << std::endl;
        std::cout << indent.nextIndent() << "b: " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Random" << std::endl;
    {
        const size_t n = 5;
        auto v = kvs::ValueArray<int>::Random( n );
        std::cout << indent << "kvs::ValueArray<int>::Random( n ); // n = 5" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        const kvs::UInt32 seed = 1;
        v = kvs::ValueArray<int>::Random( n, seed );
        std::cout << indent << "kvs::ValueArray<int>::Random( n, seed ); // seed = 1" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        const int min = 0, max = 9;
        v = kvs::ValueArray<int>::Random( n, min, max );
        std::cout << indent << "kvs::ValueArray<int>::Random( n, min, max ); // min = 0, max = 9" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;

        v = kvs::ValueArray<int>::Random( n, min, max, seed );
        std::cout << indent << "kvs::ValueArray<int>::Random( n, min, max, seed );" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Linear" << std::endl;
    {
        const size_t n = 5;
        auto v1 = kvs::ValueArray<int>::Linear( n );
        std::cout << indent << "kvs::ValueArray<int>::Linear( n ); // n = 5" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        v1 = kvs::ValueArray<int>::Linear( n, 10 );
        std::cout << indent << "kvs::ValueArray<int>::Linear( n, 10 ); // start = 10" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        v1 = kvs::ValueArray<int>::Linear( n, 5, 3 );
        std::cout << indent << "kvs::ValueArray<int>::Linear( n, 5, 3 ); // start = 5, step = 3" << std::endl;
        std::cout << indent.nextIndent() << v1.format() << std::endl;

        auto v2 = kvs::ValueArray<float>::Linear( n, 5.1f );
        std::cout << indent << "kvs::ValueArray<float>::Linear( n, 5.1 ); // start = 5.1" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;

        v2 = kvs::ValueArray<float>::Linear( n, 2.9f, 0.1f );
        std::cout << indent << "kvs::ValueArray<float>::Linear( n, 2.9, 0.1 ); // start = 2.9, step = 0.1" << std::endl;
        std::cout << indent.nextIndent() << v2.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Min/Max" << std::endl;
    {
        auto v = kvs::ValueArray<int>::Random( 10, 10, 99 );
        std::cout << "v = " << v.format() << std::endl;

        std::cout << indent << "min value: " << v.min() << std::endl;
        std::cout << indent << "max value: " << v.max() << std::endl;
        std::cout << indent << "index of min value: " << v.argmin() << std::endl;
        std::cout << indent << "index of max value: " << v.argmax() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Shuffle" << std::endl;
    {
        auto v = kvs::ValueArray<int>::Linear( 5 );
        std::cout << "v = " << v.format() << std::endl;

        v.shuffle();
        std::cout << indent << "v.shuffle();" << std::endl;
        std::cout << indent.nextIndent() << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Sort" << std::endl;
    {
        auto v = kvs::ValueArray<int>::Random( 4, 1, 9 );
        std::cout << "v = " << v.format() << std::endl;

        auto i = v.argsort();
        std::cout << indent << "i = v.argsort();" << std::endl;
        std::cout << indent.nextIndent() << "i: " << i.format() << std::endl;
        std::cout << indent << "i = v.argsort( false );" << std::endl;
        std::cout << indent.nextIndent() << "i: " << v.argsort( false ).format() << std::endl;

        v.sort();
        std::cout << indent << "v.sort();" << std::endl;
        std::cout << indent.nextIndent() << "v: " << v.format() << std::endl;

        v.sort( false );
        std::cout << indent << "v.sort( false );" << std::endl;
        std::cout << indent.nextIndent() << "v: " << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Slice" << std::endl;
    {
        auto v = kvs::ValueArray<int>::Linear( 5 );
        std::cout << "v = " << v.format() << std::endl;

        auto v1 = v.slice( {2} );
        std::cout << indent << "v1 = v.slice( {2} );" << std::endl;
        std::cout << indent.nextIndent() << "v1: " << v1.format() << std::endl;

        auto v2 = v.slice( {-2} );
        std::cout << indent << "v2 = v.slice( {-2} );" << std::endl;
        std::cout << indent.nextIndent() << "v2: " << v2.format() << std::endl;

        auto v3 = v.slice( {1,3} );
        std::cout << indent << "v3 = v.slice( {1,3} );" << std::endl;
        std::cout << indent.nextIndent() << "v3: " << v3.format() << std::endl;

        auto v4 = v.slice( {0,4,2} );
        std::cout << indent << "v4 = v.slice( {0,4,2} );" << std::endl;
        std::cout << indent.nextIndent() << "v4: " << v4.format() << std::endl;

        auto v5 = v[ {0,4,2} ];
        std::cout << indent << "v5 = v[ {0,4,2} ];" << std::endl;
        std::cout << indent.nextIndent() << "v5: " << v5.format() << std::endl;
        std::cout << std::endl;
    }

    PerfTest( 10000, 10000 );

    return 0;
}
