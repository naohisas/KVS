#include <iostream>
#include <kvs/Matrix>


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
    std::cout << "a =" << std::endl << a << std::endl;

    kvs::Matrix<float> b( nrows, ncols );
    b[0][0] = 1.0f; b[0][1] = 1.0f; b[0][2] = 2.0f; b[0][3] = 3.0f; b[0][4] = 1.0f;
    b[1][0] = 2.0f; b[1][1] = 3.0f; b[1][2] = 1.0f; b[1][3] = 1.0f; b[1][4] = 8.0f;
    b[2][0] = 5.0f; b[2][1] = 1.0f; b[2][2] = 3.0f; b[2][3] = 2.0f; b[2][4] = 2.0f;
    b[3][0] = 3.0f; b[3][1] = 1.0f; b[3][2] = 2.0f; b[3][3] = 1.0f; b[3][4] = 3.0f;
    b[4][0] = 2.0f; b[4][1] = 5.0f; b[4][2] = 1.0f; b[4][3] = 2.0f; b[4][4] = 1.0f;
    std::cout << "b =" << std::endl << b << std::endl;

    kvs::Matrix33<float> m2(
        1.0f, 2.0f, 3.0f,
        2.0f, 3.0f, 4.0f,
        3.0f, 4.0f, 5.0f );
    kvs::Matrix<float> c( m2 );
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
    kvs::Mat d = a.transposed();
    std::cout << "kvs::Mat d = a.transposed();" << std::endl;
    std::cout << "d =" << std::endl << d << std::endl; // d is transposed a
    std::cout << "a =" << std::endl << a << std::endl; // a is not transposed

    a.transpose();
    std::cout << "a.transpose();" << std::endl;
    std::cout << "a =" << std::endl << a << std::endl; // a is transposed

    std::cout << std::endl;
    std::cout << "Inverse" << std::endl;
    kvs::Mat e = a.inverted();
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
    std::cout << "kvs::Mat::Zero(5,5) =" << std::endl << kvs::Mat::Zero( nrows, ncols ) << std::endl;
    std::cout << "kvs::Mat::Ones(5,5) =" << std::endl << kvs::Mat::Ones( nrows, ncols ) << std::endl;
    std::cout << "kvs::Mat::Identity(5,5) =" << std::endl << kvs::Mat::Identity( nrows, ncols ) << std::endl;
    std::cout << "kvs::Mat::Constant(5,5,3) =" << std::endl << kvs::Mat::Constant( nrows, ncols, 3.0f ) << std::endl;
    std::cout << "kvs::Mat::Diagonal(5,5,6) =" << std::endl << kvs::Mat::Diagonal( nrows, ncols, 6.0f ) << std::endl;
    std::cout << "kvs::Mat::Random() =" << std::endl << kvs::Mat::Random( nrows, ncols ) << std::endl;

    return 0;
}
