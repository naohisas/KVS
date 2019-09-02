#include <iostream>
#include <kvs/Vector4>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Vector4<float> a( 1.0f, 2.0f, 3.0f, 4.0f );
    std::cout << "a = (" << a << ")" << std::endl;

    kvs::Vector4<float> b;
    b[0] = 5.0f;
    b[1] = 6.0f;
    b[2] = 7.0f;
    b[3] = 8.0f;
    std::cout << "b = (" << b[0] << " " << b[1] << " " << b[2] << " " << b[3] << ")" << std::endl;

    kvs::Vector4<float> c( a.xy(), 1.0f, 2.0f );
    std::cout << "c = (" << c << ")" << std::endl;

    kvs::Vector4<float> d( b.xyz(), 1.0f );
    std::cout << "d = (" << d << ")" << std::endl;

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = (" << a + b << ")" << std::endl;
    std::cout << "a - b = (" << a - b << ")" << std::endl;
    std::cout << "a * b = (" << a * b << ")" << std::endl;
    std::cout << "a / b = (" << a / b << ")" <<  std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b; " << "a = (" << a << "), b = (" << b << ")" << std::endl;
    a -= b;
    std::cout << "a -= b; " << "a = (" << a << "), b = (" << b << ")" << std::endl;
    a *= b;
    std::cout << "a *= b; " << "a = (" << a << "), b = (" << b << ")" << std::endl;
    a /= b;
    std::cout << "a /= b; " << "a = (" << a << "), b = (" << b << ")" << std::endl;

    std::cout << std::endl;
    std::cout << "Length" << std::endl;
    std::cout << "a.length() = " << a.length() << std::endl;
    std::cout << "a.squaredLength() = " << a.squaredLength() << std::endl;

    std::cout << std::endl;
    std::cout << "Dot product" << std::endl;
    std::cout << "a.dot( b ) = " << a.dot( b ) << std::endl;

    std::cout << std::endl;
    std::cout << "Normalize" << std::endl;
    kvs::Vec4 e = a.normalized();
    std::cout << "kvs::Vec4 e = a.normalized();" << std::endl;
    std::cout << "e = (" << e << ")" << std::endl; // e is normalized a
    std::cout << "a = (" << a << ")" << std::endl; // a is not normalized

    a.normalize();
    std::cout << "a.normalize();" << std::endl;
    std::cout << "a = (" << a << ")" << std::endl; // a is normalized

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a = (" << a << ")" << std::endl;
    std::cout << "b = (" << b << ")" << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a = (" << a << ")" << std::endl;
    std::cout << "b = (" << b << ")" << std::endl;

    std::cout << std::endl;
    std::cout << "kvs::Vec4::Zero() = (" << kvs::Vec4::Zero() << ")" << std::endl;
    std::cout << "kvs::Vec4::Ones() = (" << kvs::Vec4::Ones() << ")" << std::endl;
    std::cout << "kvs::Vec4::UnitX() = (" << kvs::Vec4::UnitX() << ")" << std::endl;
    std::cout << "kvs::Vec4::UnitY() = (" << kvs::Vec4::UnitY() << ")" << std::endl;
    std::cout << "kvs::Vec4::UnitZ() = (" << kvs::Vec4::UnitZ() << ")" << std::endl;
    std::cout << "kvs::Vec4::UnitW() = (" << kvs::Vec4::UnitW() << ")" << std::endl;
    std::cout << "kvs::Vec4::Identity() = (" << kvs::Vec4::Identity() << ")" << std::endl;
    std::cout << "kvs::Vec4::Constant(5) =  (" << kvs::Vec4::Constant( 5.0f ) << ")" << std::endl;
    std::cout << "kvs::Vec4::Random() = (" << kvs::Vec4::Random() << ")" << std::endl;

    return 0;
}
