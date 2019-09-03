#include <iostream>
#include <kvs/Vector>


int main()
{
    std::cout << "Definition" << std::endl;
    const size_t n = 5;
    const float v1[n] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    kvs::Vector<float> a( n, v1 );
    std::cout << "a = [" << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << "]" << std::endl;

    kvs::Vector<float> b( n );
    b[0] =  6.0f;
    b[1] =  7.0f;
    b[2] =  8.0f;
    b[3] =  9.0f;
    b[4] = 10.0f;
    std::cout << "b = " << b << std::endl;

    kvs::Vec2 v2( 1.0f, 2.0f );
    kvs::Vector<float> c( v2 );
    std::cout << "c = "; c.print( std::cout );

    kvs::Vec3 v3( 1.0f, 2.0f, 3.0f );
    kvs::Vector<float> d( v3 );
    std::cout << "d vector" << std::endl;
    d.print( std::cout, kvs::Indent(4) );

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / b <<  std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b; " << "a = " << a << ", b = " << b << std::endl;
    a -= b;
    std::cout << "a -= b; " << "a = " << a << ", b = " << b << std::endl;
    a *= b;
    std::cout << "a *= b; " << "a = " << a << ", b = " << b << std::endl;
    a /= b;
    std::cout << "a /= b; " << "a = " << a << ", b = " << b << std::endl;

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
    std::cout << "e = " << e << std::endl; // e is normalized a
    std::cout << "a = " << a << std::endl; // a is not normalized

    a.normalize();
    std::cout << "a.normalize();" << std::endl;
    std::cout << "a = " << a << std::endl; // a is normalized

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    std::cout << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "kvs::Vec::Zero(n) = " << kvs::Vec::Zero(n) << std::endl;
    std::cout << "kvs::Vec::Ones(n) = " << kvs::Vec::Ones(n) << std::endl;
    std::cout << "kvs::Vec::Unit(n) = " << kvs::Vec::Unit(n) << std::endl;
    std::cout << "kvs::Vec::Unit(n,1) = " << kvs::Vec::Unit(n,1) << std::endl;
    std::cout << "kvs::Vec::Unit(n,2) = " << kvs::Vec::Unit(n,2) << std::endl;
    std::cout << "kvs::Vec::Identity(n) = " << kvs::Vec::Identity(n) << std::endl;
    std::cout << "kvs::Vec::Constant(n,5) = " << kvs::Vec::Constant(n,5.0f) << std::endl;
    std::cout << "kvs::Vec::Random(n) = " << kvs::Vec::Random(n) << std::endl;
    std::cout << "kvs::Vec::Random(n,seed) = " << kvs::Vec::Random(n,1) << std::endl;
    std::cout << "kvs::Vec::Random(n,min,max) = " << kvs::Vec::Random(n,-1.0f,1.0f) << std::endl;
    std::cout << "kvs::Vec::Random(n,min,max,seed) = " << kvs::Vec::Random(n,-1.0f,1.0f,2) << std::endl;

    return 0;
}
