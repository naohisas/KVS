#include <iostream>
#include <kvs/Vector2>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Vector2<float> a( 1.0f, 2.0f );
    std::cout << "a = (" << a << ")" << std::endl;

    kvs::Vector2<float> b;
    b[0] = 3.0f;
    b[1] = 4.0f;
    std::cout << "b = (" << b[0] << " " << b[1] << ")" << std::endl;

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
    kvs::Vec2 c = a.normalized();
    std::cout << "c = a.normalized();" << std::endl;
    std::cout << "c = (" << c << ")" << std::endl; // c is normalized a
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
    std::cout << "kvs::Vec2::Zero() = (" << kvs::Vec2::Zero() << ")" << std::endl;
    std::cout << "kvs::Vec2::Ones() = (" << kvs::Vec2::Ones() << ")" << std::endl;
    std::cout << "kvs::Vec2::UnitX() = (" << kvs::Vec2::UnitX() << ")" << std::endl;
    std::cout << "kvs::Vec2::UnitY() = (" << kvs::Vec2::UnitY() << ")" << std::endl;
    std::cout << "kvs::Vec2::Identity() = (" << kvs::Vec2::Identity() << ")" << std::endl;
    std::cout << "kvs::Vec2::Constant(5) =  (" << kvs::Vec2::Constant( 5.0f ) << ")" << std::endl;
    std::cout << "kvs::Vec2::Random() = (" << kvs::Vec2::Random() << ")" << std::endl;

    return 0;
}
