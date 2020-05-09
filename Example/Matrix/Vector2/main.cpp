#include <iostream>
#include <kvs/Vector2>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Vector2<float> a( 1.0f, 2.0f );
    std::cout << "a = [" << a[0] << " " << a[1] << "]" << std::endl;

    kvs::Vector2<float> b;
    b[0] = 3.0f;
    b[1] = 4.0f;
    std::cout << "b = [" << b << "]" << std::endl;

    kvs::Vector2<float> c = a;
    std::cout << "c = " << c.format() << std::endl;

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b = " << ( a + b ).format() << std::endl;
    std::cout << "a - b = " << ( a - b ).format() << std::endl;
    std::cout << "a * b = " << ( a * b ).format() << std::endl;
    std::cout << "a / b = " << ( a / b ).format() << std::endl;

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
    kvs::Vec2 d = a.normalized();
    std::cout << "d = a.normalized();" << std::endl;
    std::cout << "d = " << d.format() << std::endl; // d is normalized a
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
    std::cout << "kvs::Vec2::Zero() = " << kvs::Vec2::Zero().format() << std::endl;
    std::cout << "kvs::Vec2::Ones() = " << kvs::Vec2::Ones().format() << std::endl;
    std::cout << "kvs::Vec2::UnitX() = " << kvs::Vec2::UnitX().format() << std::endl;
    std::cout << "kvs::Vec2::UnitY() = " << kvs::Vec2::UnitY().format() << std::endl;
    std::cout << "kvs::Vec2::Identity() = " << kvs::Vec2::Identity().format() << std::endl;
    std::cout << "kvs::Vec2::Constant(5) = " << kvs::Vec2::Constant( 5.0f ).format() << std::endl;
    std::cout << "kvs::Vec2::Random() = " << kvs::Vec2::Random().format() << std::endl;
    std::cout << "kvs::Vec2::Random(seed) = " << kvs::Vec2::Random( 1 ).format() << std::endl;
    std::cout << "kvs::Vec2::Random(min,max) = " << kvs::Vec2::Random( -1.0f, 1.0f ).format() << std::endl;
    std::cout << "kvs::Vec2::Random(min,max,seed) = " << kvs::Vec2::Random( -1.0f, 1.0f, 2 ).format() << std::endl;

    return 0;
}
