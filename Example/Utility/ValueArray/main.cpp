#include <kvs/ValueArray>
#include <kvs/Indent>
#include <kvs/Timer>
#include <vector>


void PerfTest( const size_t size, const size_t n )
{
    std::cout << "Performance Test" << std::endl;

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
        std::cout << "kvs::ValueArray<float> v( 3 );" << std::endl;
        std::cout << indent << "v = " << v1.format() << std::endl;

        float temp2[3] = { 2.0f, 3.0f, 4.0f };
        kvs::ValueArray<float> v2( temp2, 3 );
        std::cout << "float temp[3] = ..." << std::endl;
        std::cout << "kvs::ValueArray<float> v( temp, 3 );" << std::endl;
        std::cout << indent << "v2 = " << v2.format() << std::endl;

        std::vector<float> temp3;
        temp3.push_back( 3.0f );
        temp3.push_back( 4.0f );
        temp3.push_back( 5.0f );
        kvs::ValueArray<float> v3( temp3 );
        std::cout << "std::vector<float> temp; ..." << std::endl;
        std::cout << "kvs::ValueArray<float> v( temp );" << std::endl;
        std::cout << indent << "v = " << v3.format() << std::endl;

        kvs::ValueArray<float> v4 = { 4, 5, 6 };
        std::cout << "kvs::ValueArray<float> v4 = { 4, 5, 6 };" << std::endl;
        std::cout << indent << "v = " << v4.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Shallow Copy" << std::endl;
    {
        kvs::ValueArray<float> a( 3 );
        a[0] = 1.0f;
        a[1] = 2.0f;
        a[2] = 3.0f;
        std::cout << "a = " << a.format() << std::endl;

        kvs::ValueArray<float> b = a; // shallow copy
        std::cout << "kvs::ValueArray<float> b = a;" << std::endl;
        std::cout << indent << "b = " << b << std::endl;

        a[0] = 0.0f;
        std::cout << "a[0] = 0.0f;" << std::endl;
        std::cout << indent << "a = " << a.format() << std::endl;
        std::cout << indent << "b = " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Deep Copy" << std::endl;
    {
        kvs::ValueArray<float> a( 3 );
        a[0] = 1.0f;
        a[1] = 2.0f;
        a[2] = 3.0f;
        std::cout << "a = " << a.format() << std::endl;

        kvs::ValueArray<float> b = a.clone(); // deep copy
        std::cout << "kvs::ValueArray<float> b = a.clone();" << std::endl;
        std::cout << indent << "b = " << b.format() << std::endl;

        a[0] = 0.0f;
        std::cout << "a[0] = 0.0f;" << std::endl;
        std::cout << indent << "a = " << a.format() << std::endl;
        std::cout << indent << "b = " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Assignment" << std::endl;
    {
        int temp[6] = { 1, 2, 3, 4, 5, 6 };
        kvs::ValueArray<int> a( temp, 6 );
        std::cout << "a = " << a.format() << std::endl;

        kvs::ValueArray<int> b;
        b.assign( a.begin() + 2, a.end() - 1 );
        std::cout << "kvs::ValueArray<int> b;" << std::endl;
        std::cout << "b.assign( a.begin() + 2, a.end() - 1 );" << std::endl;
        std::cout << indent << "b = " << b.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Random" << std::endl;
    {
        kvs::ValueArray<int> v = kvs::ValueArray<int>::Random( 5 );
        std::cout << "kvs::ValueArray<int>::Random(n);" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;

        const kvs::UInt32 seed = 1;
        v = kvs::ValueArray<int>::Random( 5, seed );
        std::cout << "kvs::ValueArray<int>::Random(n,seed);" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;

        const int min = 0;
        const int max = 9;
        v = kvs::ValueArray<int>::Random( 5, min, max );
        std::cout << "kvs::ValueArray<int>::Random(n,min,max);" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;

        v = kvs::ValueArray<int>::Random( 5, min, max, seed );
        std::cout << "kvs::ValueArray<int>::Random(n,min,max,seed);" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Linear" << std::endl;
    {
        kvs::ValueArray<int> v1 = kvs::ValueArray<int>::Linear( 5 );
        std::cout << "kvs::ValueArray<int> v = kvs::ValueArray<int>::Linear( 5 );" << std::endl;
        std::cout << indent << "v = " << v1.format() << std::endl;

        v1 = kvs::ValueArray<int>::Linear( 5, 10 );
        std::cout << "kvs::ValueArray<int> v = kvs::ValueArray<int>::Linear( 5, 10 );" << std::endl;
        std::cout << indent << "v = " << v1.format() << std::endl;

        v1 = kvs::ValueArray<int>::Linear( 5, 5, 3 );
        std::cout << "kvs::ValueArray<int> v = kvs::ValueArray<int>::Linear( 5, 5, 3 );" << std::endl;
        std::cout << indent << "v = " << v1.format() << std::endl;

        kvs::ValueArray<float> v2 = kvs::ValueArray<float>::Linear( 5, 5.1f );
        std::cout << "kvs::ValueArray<float> v = kvs::ValueArray<float>::Linear( 5, 5.1f );" << std::endl;
        std::cout << indent << "v = " << v2.format() << std::endl;

        v2 = kvs::ValueArray<float>::Linear( 5, 4.9f, 0.1f );
        std::cout << "kvs::ValueArray<float> v = kvs::ValueArray<float>::Linear( 5, 2.9f, 0.1f );" << std::endl;
        std::cout << indent << "v = " << v2.format() << std::endl;

        v2 = kvs::ValueArray<float>::Linear( 5, 0.1f, -4.9f );
        std::cout << "kvs::ValueArray<float> v = kvs::ValueArray<float>::Linear( 5, 0.1f, -2.3f );" << std::endl;
        std::cout << indent << "v = " << v2.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Min/Max" << std::endl;
    {
        kvs::ValueArray<int> v = kvs::ValueArray<int>::Random( 10, 10, 99 );
        std::cout << "v = " << v.format() << std::endl;
        std::cout << indent << "min value: " << v.min() << std::endl;
        std::cout << indent << "max value: " << v.max() << std::endl;
        std::cout << indent << "index of min value: " << v.argmin() << std::endl;
        std::cout << indent << "index of max value: " << v.argmax() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Shuffle" << std::endl;
    {
        int temp[6] = { 1, 2, 3, 4, 5, 6 };
        kvs::ValueArray<int> v( temp, 6 );
        std::cout << "v = " << v.format() << std::endl;

        v.shuffle();
        std::cout << "v.shuffle();" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Sort" << std::endl;
    {
        kvs::ValueArray<int> v = kvs::ValueArray<int>::Random( 10, 10, 99 );
        std::cout << "v = " << v.format() << std::endl;

        kvs::ValueArray<size_t> i = v.argsort();
        std::cout << "kvs::ValueArray<size_t> i = v.argsort();" << std::endl;
        std::cout << indent << "i = " << i.format() << std::endl;
        std::cout << "kvs::ValueArray<size_t> i = v.argsort( false );" << std::endl;
        std::cout << indent << "i = " << v.argsort( false ).format() << std::endl;

        v.sort();
        std::cout << "v.sort();" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;

        v.sort( false );
        std::cout << "v.sort( false );" << std::endl;
        std::cout << indent << "v = " << v.format() << std::endl;
        std::cout << std::endl;
    }

    PerfTest( 10000, 10000 );

    return 0;
}
