#include <iostream>
#include <kvs/Timer>
#include <kvs/Matrix>
#include <kvs/Vector>


namespace
{

void PerfTest( const size_t size, const size_t nloops )
{
    std::cout << "Performance Test (" << nloops << " times)" << std::endl;

    typedef kvs::Matrix<float> Mat;
    const kvs::Indent indent(4);

    // Random
    {
        Mat a;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a = Mat::Random( size, size ); }
        timer.stop();
        std::cout << indent << "Random: " << timer.sec() << " [sec]" << std::endl;
    }

    // Swap
    {
        Mat a = Mat::Random( size, size );
        Mat b = Mat::Random( size, size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a.swap( b ); }
        timer.stop();
        std::cout << indent << "Swap: " << timer.sec() << " [sec]" << std::endl;
    }

    // Transpose
    {
        Mat a = Mat::Random( size, size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a.transposed(); }
        timer.stop();
        std::cout << indent << "Transpose: " << timer.sec() << " [sec]" << std::endl;
    }

    // Inverse
    {
        Mat a = Mat::Random( size, size );
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { a.inverted(); }
        timer.stop();
        std::cout << indent << "Inverse: " << timer.sec() << " [sec]" << std::endl;
    }

    // + operator
    {
        Mat a = Mat::Random( size, size );
        Mat b = Mat::Random( size, size );
        Mat c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a + b; }
        timer.stop();
        std::cout << indent << "+ operator: " << timer.sec() << " [sec]" << std::endl;
    }

    // - operator
    {
        Mat a = Mat::Random( size, size );
        Mat b = Mat::Random( size, size );
        Mat c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a - b; }
        timer.stop();
        std::cout << indent << "- operator: " << timer.sec() << " [sec]" << std::endl;
    }

    // * operator
    {
        Mat a = Mat::Random( size, size );
        Mat b = Mat::Random( size, size );
        Mat c;
        kvs::Timer timer( kvs::Timer::Start );
        for ( size_t i = 0; i < nloops; ++i ) { c = a * b; }
        timer.stop();
        std::cout << indent << "* operator: " << timer.sec() << " [sec]" << std::endl;
    }
}

}


int main()
{
    std::cout << "Definition" << std::endl;
    const size_t nrows = 5;
    const size_t ncols = 5;
    const float m1[ nrows * ncols ] = {
        1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
        5.0f, 2.0f, 1.0f, 2.0f, 1.0f,
        2.0f, 4.0f, 3.0f, 2.0f, 2.0f,
        1.0f, 3.0f, 1.0f, 2.0f, 1.0f,
        3.0f, 4.0f, 1.0f, 2.0f, 3.0f
    };
    kvs::Matrix<float> a( nrows, ncols, m1 );
    std::cout << "a = [" << a[0];
    for ( size_t i = 1; i < a.rowSize(); ++i ) { std::cout << ", " << a[i]; }
    std::cout << "]" << std::endl;

    kvs::Matrix<float> b( nrows, ncols );
    b[0][0] = 1.0f; b[0][1] = 1.0f; b[0][2] = 2.0f; b[0][3] = 3.0f; b[0][4] = 1.0f;
    b[1][0] = 2.0f; b[1][1] = 3.0f; b[1][2] = 1.0f; b[1][3] = 1.0f; b[1][4] = 8.0f;
    b[2][0] = 5.0f; b[2][1] = 1.0f; b[2][2] = 3.0f; b[2][3] = 2.0f; b[2][4] = 2.0f;
    b[3][0] = 3.0f; b[3][1] = 1.0f; b[3][2] = 2.0f; b[3][3] = 1.0f; b[3][4] = 3.0f;
    b[4][0] = 2.0f; b[4][1] = 5.0f; b[4][2] = 1.0f; b[4][3] = 2.0f; b[4][4] = 1.0f;
    std::cout << "b = " << b.format() << std::endl;

    kvs::Matrix33<float> m2(
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f );
    kvs::Matrix<float> c( m2 );
    std::cout << "c matrix:" << std::endl << c << std::endl;

    std::cout << std::endl;
    std::cout << "Iterator" << std::endl;
    std::cout << c.format( true, kvs::Indent(0) ) << std::endl;
    std::cout << "row-major vector: " << kvs::Vector<float>( c.begin(), c.end() ).format() << std::endl;
    std::cout << "column-major vector: " << kvs::Vector<float>( c.beginInColumnOrder(), c.endInColumnOrder() ).format() << std::endl;
    std::cout << "0-th row vector: " << kvs::Vector<float>( c.beginRow(0), c.endRow(0) ).format() << std::endl;
    std::cout << "0-th column vector: " << kvs::Vector<float>( c.beginColumn(0), c.endColumn(0) ).format() << std::endl;

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = " << ( a + b ).format() << std::endl;
    std::cout << "a - b = " << ( a - b ).format() << std::endl;
    std::cout << "a * b = " << ( a * b ).format() << std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b;" << std::endl;
    std::cout << "a = " << a.format() << std::endl
              << "b = " << b.format() << std::endl;
    a -= b;
    std::cout << "a -= b;" << std::endl;
    std::cout << "a = " << a.format() << std::endl
              << "b = " << b.format() << std::endl;
    a *= b;
    std::cout << "a *= b;" << std::endl;
    std::cout << "a = " << a.format() << std::endl
              << "b = " << b.format() << std::endl;

    std::cout << std::endl;
    std::cout << "Transpose" << std::endl;
    kvs::Mat d = a.transposed();
    std::cout << "kvs::Mat d = a.transposed();" << std::endl;
    std::cout << "d = " << d.format() << std::endl; // d is transposed a
    std::cout << "a = " << a.format() << std::endl; // a is not transposed

    a.transpose();
    std::cout << "a.transpose();" << std::endl;
    std::cout << "a = " << a.format() << std::endl; // a is transposed

    std::cout << std::endl;
    std::cout << "Inverse" << std::endl;
    kvs::Mat e = a.inverted();
    std::cout << "kvs::Mat2 e = a.inverted();" << std::endl;
    std::cout << "e = " << e.format() << std::endl; // e is inverted a
    std::cout << "a = " << a.format() << std::endl; // a is not inverted

    a.invert();
    std::cout << "a.invert();" << std::endl;
    std::cout << "a = " << a.format() << std::endl; // a is inverted

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a = " << a.format() << std::endl;
    std::cout << "b = " << b.format() << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a = " << a.format() << std::endl;
    std::cout << "b = " << b.format() << std::endl;

    std::cout << std::endl;
    kvs::Indent indent(4);
    std::cout << "kvs::Mat::Zero(5,5)" << std::endl
              << kvs::Mat::Zero( nrows, ncols ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Ones(5,5)" << std::endl
              << kvs::Mat::Ones( nrows, ncols ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Identity(5,5)" << std::endl
              << kvs::Mat::Identity( nrows, ncols ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Constant(5,5,3)" << std::endl
              << kvs::Mat::Constant( nrows, ncols, 3.0f ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Diagonal(5,5,6)" << std::endl
              << kvs::Mat::Diagonal( nrows, ncols, 6.0f ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Random(5,5)" << std::endl
              << kvs::Mat::Random( nrows, ncols ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Random(5,5,seed)" << std::endl
              << kvs::Mat::Random( nrows, ncols, 1 ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Random(5,5,min,max)" << std::endl
              << kvs::Mat::Random( nrows, ncols, -1.0f, 1.0f ).format( true, indent ) << std::endl;
    std::cout << "kvs::Mat::Random(5,5,min,max,seed)" << std::endl
              << kvs::Mat::Random( nrows, ncols, -1.0f, 1.0f, 2 ).format( true, indent ) << std::endl;
    std::cout << std::endl;

    const size_t size = 100;
    const size_t nloops = 10000;
    ::PerfTest( size, nloops );

    return 0;
}
