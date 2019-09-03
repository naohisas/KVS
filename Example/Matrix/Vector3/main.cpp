#include <iostream>
#include <kvs/Vector3>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Vector3<float> a( 1.0f, 2.0f, 3.0f );
    std::cout << "a = [" << a[0] << ", " << a[1] << ", " << a[2] << "]" << std::endl;

    kvs::Vector3<float> b;
    b[0] = 4.0f;
    b[1] = 5.0f;
    b[2] = 6.0f;
    std::cout << "b = " << b << std::endl;

    kvs::Vector3<float> c( a.xy(), 1.0f );
    std::cout << "c = "; c.print( std::cout );

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
    std::cout << "Cross product" << std::endl;
    std::cout << "a.cross( b ) = " << a.cross( b ) << std::endl;

    std::cout << std::endl;
    std::cout << "Normalize" << std::endl;
    kvs::Vec3 d = a.normalized();
    std::cout << "kvs::Vec3 d = a.normalized();" << std::endl;
    std::cout << "d = " << d << std::endl; // c is normalized a
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
    std::cout << "kvs::Vec3::Zero() = " << kvs::Vec3::Zero() << std::endl;
    std::cout << "kvs::Vec3::Ones() = " << kvs::Vec3::Ones() << std::endl;
    std::cout << "kvs::Vec3::UnitX() = " << kvs::Vec3::UnitX() << std::endl;
    std::cout << "kvs::Vec3::UnitY() = " << kvs::Vec3::UnitY() << std::endl;
    std::cout << "kvs::Vec3::UnitZ() = " << kvs::Vec3::UnitZ() << std::endl;
    std::cout << "kvs::Vec3::Identity() = " << kvs::Vec3::Identity() << std::endl;
    std::cout << "kvs::Vec3::Constant(5) =  " << kvs::Vec3::Constant( 5.0f ) << std::endl;
    std::cout << "kvs::Vec3::Random() = " << kvs::Vec3::Random() << std::endl;
    std::cout << "kvs::Vec3::Random(seed) = " << kvs::Vec3::Random( 1 ) << std::endl;
    std::cout << "kvs::Vec3::Random(min,max) = " << kvs::Vec3::Random( -1.0f, 1.0f ) << std::endl;
    std::cout << "kvs::Vec3::Random(min,max,seed) = " << kvs::Vec3::Random( -1.0f, 1.0f, 2 ) << std::endl;

    return 0;
}
