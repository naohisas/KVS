#include <iostream>
#include <kvs/Timer>
#include <kvs/Vector>


namespace
{

void PerfTest( const size_t size, const size_t nloops )
{
    std::cout << "Performance Test (" << nloops << " times)" << std::endl;

    typedef kvs::Vector<float> Vec;
    const kvs::Indent indent(4);

    // Random
    {
        Vec a;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a = Vec::Random( size ); }
        timer.stop();
        std::cout << indent << "Random: " << timer.sec() << " [sec]" << std::endl;
    }

    // Swap
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a.swap( b ); }
        timer.stop();
        std::cout << indent << "Swap: " << timer.sec() << " [sec]" << std::endl;
    }

    // Normalize
    {
        Vec a = Vec::Random( size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a.normalized(); }
        timer.stop();
        std::cout << indent << "Normalize: " << timer.sec() << " [sec]" << std::endl;
    }

    // Length
    {
        Vec a = Vec::Random( size );
        double b = 0.0;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { b = a.length(); }
        timer.stop();
        std::cout << indent << "Length: " << timer.sec() << " [sec]" << std::endl;
    }

    // Dot product
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        float c = 0.0f;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a.dot( b ); }
        timer.stop();
        std::cout << indent << "Dot: " << timer.sec() << " [sec]" << std::endl;
    }

    // + operator
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        Vec c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a + b; }
        timer.stop();
        std::cout << indent << "+ operator: " << timer.sec() << " [sec]" << std::endl;
    }

    // - operator
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        Vec c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a - b; }
        timer.stop();
        std::cout << indent << "- operator: " << timer.sec() << " [sec]" << std::endl;
    }

    // * operator
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        Vec c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a * b; }
        timer.stop();
        std::cout << indent << "* operator: " << timer.sec() << " [sec]" << std::endl;
    }

    // / operator
    {
        Vec a = Vec::Random( size );
        Vec b = Vec::Random( size );
        Vec c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a / b; }
        timer.stop();
        std::cout << indent << "/ operator: " << timer.sec() << " [sec]" << std::endl;
    }
}

}

int main()
{
    std::cout << "Definition" << std::endl;
    const size_t n = 5;
    const float v1[n] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    kvs::Vector<float> a( n, v1 );
    std::cout << "a = [" << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] << "]" << std::endl;

    kvs::Vector<float> b( n );
    b[0] =  6.0f;
    b[1] =  7.0f;
    b[2] =  8.0f;
    b[3] =  9.0f;
    b[4] = 10.0f;
    std::cout << "b = [" << b << "]" << std::endl;

    kvs::Vec2 v2( 1.0f, 2.0f );
    kvs::Vector<float> c( v2 );
    std::cout << "c = " << c.format() << std::endl;

    kvs::Vec3 v3( 1.0f, 2.0f, 3.0f );
    kvs::Vector<float> d( v3 );
    std::cout << "d vector: " << std::endl << d << std::endl;

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = " << ( a + b ).format() << std::endl;
    std::cout << "a - b = " << ( a - b ).format() << std::endl;
    std::cout << "a * b = " << ( a * b ).format() << std::endl;
    std::cout << "a / b = " << ( a / b ).format() <<  std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b; " << "a = " << a.format() << ", b = " << b.format() << std::endl;
    a -= b;
    std::cout << "a -= b; " << "a = " << a.format() << ", b = " << b.format() << std::endl;
    a *= b;
    std::cout << "a *= b; " << "a = " << a.format() << ", b = " << b.format() << std::endl;
    a /= b;
    std::cout << "a /= b; " << "a = " << a.format() << ", b = " << b.format() << std::endl;

    std::cout << std::endl;
    std::cout << "Length" << std::endl;
    std::cout << "a.length() = " << a.length() << std::endl;
    std::cout << "a.squaredLength() = " << a.squaredLength() << std::endl;

    std::cout << std::endl;
    std::cout << "Dot product" << std::endl;
    std::cout << "a.dot( b ) = " << a.dot( b ) << std::endl;

    std::cout << std::endl;
    std::cout << "Normalize" << std::endl;
    kvs::Vec e = a.normalized();
    std::cout << "kvs::Vec4 e = a.normalized();" << std::endl;
    std::cout << "e = " << e.format() << std::endl; // e is normalized a
    std::cout << "a = " << a.format() << std::endl; // a is not normalized

    a.normalize();
    std::cout << "a.normalize();" << std::endl;
    std::cout << "a = " << a.format() << std::endl; // a is normalized

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a = " << a.format() << std::endl;
    std::cout << "b = " << b.format() << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a = " << a.format() << std::endl;
    std::cout << "b = " << b.format() << std::endl;

    std::cout << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "kvs::Vec::Zero(n) = " << kvs::Vec::Zero(n).format() << std::endl;
    std::cout << "kvs::Vec::Ones(n) = " << kvs::Vec::Ones(n).format() << std::endl;
    std::cout << "kvs::Vec::Unit(n) = " << kvs::Vec::Unit(n).format() << std::endl;
    std::cout << "kvs::Vec::Unit(n,1) = " << kvs::Vec::Unit(n,1).format() << std::endl;
    std::cout << "kvs::Vec::Unit(n,2) = " << kvs::Vec::Unit(n,2).format() << std::endl;
    std::cout << "kvs::Vec::Identity(n) = " << kvs::Vec::Identity(n).format() << std::endl;
    std::cout << "kvs::Vec::Constant(n,5) = " << kvs::Vec::Constant(n,5.0f).format() << std::endl;
    std::cout << "kvs::Vec::Random(n) = " << kvs::Vec::Random(n).format() << std::endl;
    std::cout << "kvs::Vec::Random(n,seed) = " << kvs::Vec::Random(n,1).format() << std::endl;
    std::cout << "kvs::Vec::Random(n,min,max) = " << kvs::Vec::Random(n,-1.0f,1.0f).format() << std::endl;
    std::cout << "kvs::Vec::Random(n,min,max,seed) = " << kvs::Vec::Random(n,-1.0f,1.0f,2).format() << std::endl;
    std::cout << std::endl;

    const size_t size = 10000;
    const size_t nloops = 10000;
    ::PerfTest( size, nloops );

    return 0;
}
