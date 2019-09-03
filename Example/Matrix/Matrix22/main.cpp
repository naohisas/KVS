#include <iostream>
#include <kvs/Matrix22>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Matrix22<float> a(
        1.0f, 2.0f,
        3.0f, 4.0f );
    std::cout << "a = [" << a[0] << ", " << a[1] << "]" << std::endl;

    kvs::Matrix22<float> b;
    b[0][0] = 5.0f; b[0][1] = 6.0f;
    b[1][0] = 7.0f; b[1][1] = 8.0f;
    std::cout << "b = " << b << std::endl;

    kvs::Matrix22<float> c(
        kvs::Vec2( 1.0f, 2.0f ),
        kvs::Vec2( 2.0f, 3.0f ) );
    std::cout << "c matrix" << std::endl; c.print( std::cout, kvs::Indent(4) );

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b;" << std::endl;
    std::cout << "a = " << a << std::endl << "b = " << b << std::endl;
    a -= b;
    std::cout << "a -= b;" << std::endl;
    std::cout << "a = " << a << std::endl << "b = " << b << std::endl;
    a *= b;
    std::cout << "a *= b;" << std::endl;
    std::cout << "a = " << a << std::endl << "b = " << b << std::endl;

    std::cout << std::endl;
    std::cout << "Transpose" << std::endl;
    kvs::Mat2 d = a.transposed();
    std::cout << "kvs::Mat2 d = a.transposed();" << std::endl;
    std::cout << "d = " << d << std::endl; // d is transposed a
    std::cout << "a = " << a << std::endl; // a is not transposed

    a.transpose();
    std::cout << "a.transpose();" << std::endl;
    std::cout << "a = " << a << std::endl; // a is transposed

    std::cout << std::endl;
    std::cout << "Inverse" << std::endl;
    kvs::Mat2 e = a.inverted();
    std::cout << "kvs::Mat2 e = a.inverted();" << std::endl;
    std::cout << "e = " << e << std::endl; // e is inverted a
    std::cout << "a = " << a << std::endl; // a is not inverted

    a.invert();
    std::cout << "a.invert();" << std::endl;
    std::cout << "a = " << a << std::endl; // a is inverted

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    std::cout << std::endl;
    kvs::Indent indent(4);
    std::cout << "kvs::Mat2::Zero()" << std::endl; kvs::Mat2::Zero().print( std::cout, indent );
    std::cout << "kvs::Mat2::Ones()" << std::endl; kvs::Mat2::Ones().print( std::cout, indent );
    std::cout << "kvs::Mat2::Identity()" << std::endl; kvs::Mat2::Identity().print( std::cout, indent );
    std::cout << "kvs::Mat2::Constant(5)" << std::endl; kvs::Mat2::Constant( 5.0f ).print( std::cout, indent );
    std::cout << "kvs::Mat2::Diagonal(6)" << std::endl; kvs::Mat2::Diagonal( 6.0f ).print( std::cout, indent );
    std::cout << "kvs::Mat2::Diagonal( kvs::Vec2(1,2) )" << std::endl; kvs::Mat2::Diagonal( kvs::Vec2(1.0f,2.0f) ).print( std::cout, indent );
    std::cout << "kvs::Mat2::Random()" << std::endl; kvs::Mat2::Random().print( std::cout, indent );
    std::cout << "kvs::Mat2::Random(seed)" << std::endl; kvs::Mat2::Random( 1 ).print( std::cout, indent );
    std::cout << "kvs::Mat2::Random(min,max)" << std::endl; kvs::Mat2::Random( -1.0f, 1.0f ).print( std::cout, indent );
    std::cout << "kvs::Mat2::Random(min,max,seed)" << std::endl; kvs::Mat2::Random( -1.0f, 1.0f, 2 ).print( std::cout, indent );

    return 0;
}
