#include <iostream>
#include <kvs/Matrix44>


int main()
{
    std::cout << "Definition" << std::endl;
    kvs::Matrix44<float> a(
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 1.0f, 2.0f,
        2.0f, 4.0f, 6.0f, 1.0f,
        1.0f, 3.0f, 5.0f, 2.0f );
    std::cout << "a =" << std::endl << a << std::endl;

    kvs::Matrix44<float> b;
    b[0][0] = 1.0f; b[0][1] = 1.0f; b[0][2] = 2.0f; b[0][3] = 3.0f;
    b[1][0] = 2.0f; b[1][1] = 3.0f; b[1][2] = 1.0f; b[1][3] = 1.0f;
    b[2][0] = 1.0f; b[2][1] = 1.0f; b[2][2] = 3.0f; b[2][3] = 2.0f;
    b[3][0] = 3.0f; b[3][1] = 4.0f; b[3][2] = 5.0f; b[3][3] = 1.0f;
    std::cout << "b =" << std::endl << b << std::endl;

    kvs::Matrix44<float> c(
        kvs::Vec4( 1.0f, 2.0f, 3.0f, 4.0f ),
        kvs::Vec4( 2.0f, 3.0f, 4.0f, 5.0f ),
        kvs::Vec4( 3.0f, 4.0f, 5.0f, 6.0f ),
        kvs::Vec4( 4.0f, 5.0f, 6.0f, 7.0f ) );
    std::cout << "c =" << std::endl << c << std::endl;

    std::cout << std::endl;
    std::cout << "Calculation Operator" << std::endl;
    std::cout << "a + b =" << std::endl << a + b << std::endl;
    std::cout << "a - b =" << std::endl << a - b << std::endl;
    std::cout << "a * b =" << std::endl << a * b << std::endl;

    std::cout << std::endl;
    std::cout << "Combined Assignment" << std::endl;
    a += b;
    std::cout << "a += b;" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl << "b =" << std::endl << b << std::endl;
    a -= b;
    std::cout << "a -= b;" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl << "b =" << std::endl << b << std::endl;
    a *= b;
    std::cout << "a *= b;" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl << "b =" << std::endl << b << std::endl;

    std::cout << std::endl;
    std::cout << "Transpose" << std::endl;
    kvs::Mat4 d = a.transposed();
    std::cout << "kvs::Mat4 d = a.transposed();" << std::endl;
    std::cout << "d =" << std::endl << d << std::endl; // d is transposed a
    std::cout << "a =" << std::endl << a << std::endl; // a is not transposed

    a.transpose();
    std::cout << "a.transpose();" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl; // a is transposed

    std::cout << std::endl;
    std::cout << "Inverse" << std::endl;
    kvs::Mat4 e = a.inverted();
    std::cout << "kvs::Mat2 e = a.inverted();" << std::endl;
    std::cout << "e =" << std::endl << e << std::endl; // e is inverted a
    std::cout << "a =" << std::endl << a << std::endl; // a is not inverted

    a.invert();
    std::cout << "a.invert();" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl; // a is inverted

    std::cout << std::endl;
    std::cout << "Swap" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl;
    std::cout << "b =" << std::endl << b << std::endl;
    a.swap( b );
    std::cout << "a.swap( b );" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl;
    std::cout << "b =" << std::endl << b << std::endl;

    std::cout << std::endl;
    std::cout << "kvs::Mat4::Zero() =" << std::endl << kvs::Mat4::Zero() << std::endl;
    std::cout << "kvs::Mat4::Ones() =" << std::endl << kvs::Mat4::Ones() << std::endl;
    std::cout << "kvs::Mat4::Identity() =" << std::endl << kvs::Mat4::Identity() << std::endl;
    std::cout << "kvs::Mat4::Constant(5) =" << std::endl << kvs::Mat4::Constant( 5.0f ) << std::endl;
    std::cout << "kvs::Mat4::Diagonal(6) =" << std::endl << kvs::Mat4::Diagonal( 6.0f ) << std::endl;
    std::cout << "kvs::Mat4::Diagonal( kvs::Vec4(1,2,3,4) ) =" << std::endl << kvs::Mat4::Diagonal( kvs::Vec4(1.0f,2.0f,3.0f,4.0f) ) << std::endl;
    std::cout << "kvs::Mat4::Random() =" << std::endl << kvs::Mat4::Random() << std::endl;

    return 0;
}
