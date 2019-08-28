#include <kvs/MersenneTwister>
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/LinearRegression>
#include <iostream>
#include <fstream>
#include <iterator>


template <typename T>
std::ostream& operator << ( std::ostream& os, const kvs::ValueArray<T>& values )
{
    std::copy( values.begin(), values.end(), std::ostream_iterator<T>( os, ", " ) );
    return os;
}

template <typename T>
kvs::ValueArray<T> Random( const size_t n )
{
    kvs::MersenneTwister engine;
    kvs::ValueArray<T> values( n );
    for ( size_t i = 0; i < n; i++ )
    {
        values[i] = static_cast<T>( engine.randInteger(100) );
    }
    return values;
}

int main( int argc, char** argv )
{
    const size_t n = 10;
    kvs::ValueArray<float> Y = Random<float>( n );
    kvs::ValueArray<float> X0 = Random<float>( n );
    kvs::ValueArray<float> X1 = Random<float>( n );

    std::cout << "Y  = {" << Y << "}" << std::endl;
    std::cout << "X0 = {" << X0 << "}" << std::endl;
    std::cout << "X1 = {" << X1 << "}" << std::endl;

    kvs::ValueArray<float> dep( Y );
    kvs::ValueTable<float> indep( n, 2 );
    indep[0] = X0;
    indep[1] = X1;

    kvs::LinearRegression<float> regression( dep, indep );
    std::cout << "Coef: " << regression.coef() << std::endl;
    std::cout << "R2: " << regression.r2() << std::endl;
    std::cout << "Adjusted R2: " << regression.adjustedR2() << std::endl;

    // t-Test
    regression.test();
    std::cout << "t-values: " << regression.tValues() << std::endl;
    std::cout << "p-values: " << regression.pValues() << std::endl;

    return 0;
}
