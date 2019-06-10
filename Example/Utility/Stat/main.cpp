#include <kvs/Stat>
#include <kvs/ValueArray>
#include <kvs/MersenneTwister>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>


template <typename T>
std::ostream& operator << ( std::ostream& os, const kvs::ValueArray<T>& values )
{
    std::copy( values.begin(), values.end(), std::ostream_iterator<T>( os, ", " ) );
    return os;
}

template <typename T>
kvs::ValueArray<T> Random( const size_t n )
{
    kvs::ValueArray<T> values( n );
    kvs::MersenneTwister engine;
    std::generate( values.begin(), values.end(), engine );
    return values;
}

int main( int argc, char** argv )
{
    const size_t n = 10;
    kvs::ValueArray<float> values1 = Random<float>( n );
    kvs::ValueArray<float> values2 = Random<float>( n );
    std::cout << "X = {" << values1 << "}" << std::endl;
    std::cout << "Y = {" << values2 << "}" << std::endl;

    std::cout << std::endl;
    std::cout << "Sum(X): " << kvs::Stat::Sum( values1 ) << std::endl;
    std::cout << "Mean(X): " << kvs::Stat::Mean( values1 ) << std::endl;
    std::cout << "Var(X): " << kvs::Stat::Var( values1 ) << std::endl;
    std::cout << "ShiftedVar(X): " << kvs::Stat::ShiftedVar( values1 ) << std::endl;
    std::cout << "TwoPassVar(X): " << kvs::Stat::TwoPassVar( values1 ) << std::endl;
    std::cout << "CompensatedVar(X): " << kvs::Stat::CompensatedVar( values1 ) << std::endl;
    std::cout << "OnlineVar(X): " << kvs::Stat::OnlineVar( values1 ) << std::endl;
    std::cout << "Cov(X,Y): " << kvs::Stat::Cov( values1, values2 ) << std::endl;
    std::cout << "ShiftedCov(X,Y): " << kvs::Stat::ShiftedCov( values1, values2 ) << std::endl;
    std::cout << "TwoPassCov(X,Y): " << kvs::Stat::TwoPassCov( values1, values2 ) << std::endl;
    std::cout << "OnlineCov(X,Y): " << kvs::Stat::OnlineCov( values1, values2 ) << std::endl;
    std::cout << "StdDev(X): " << kvs::Stat::StdDev( values1 ) << std::endl;
    std::cout << "StdDev(X,ShiftedVar): " << kvs::Stat::StdDev( values1, kvs::Stat::ShiftedVar<float> ) << std::endl;
    std::cout << "Corr(X,Y): " << kvs::Stat::Corr( values1, values2 ) << std::endl;
    std::cout << "AutoCorr(X,1): " << kvs::Stat::AutoCorr( values1, 1 ) << std::endl;
    std::cout << "CrossCorr(X,Y,1): " << kvs::Stat::CrossCorr( values1, values2, 1 ) << std::endl;

    return 0;
}
