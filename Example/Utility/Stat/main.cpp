#include <iostream>
#include <kvs/Indent>
#include <kvs/Stat>
#include <kvs/ValueArray>


int main( int argc, char** argv )
{
    const size_t n = 10;
    kvs::ValueArray<float> X = kvs::ValueArray<float>::Random( n );
    kvs::ValueArray<float> Y = kvs::ValueArray<float>::Random( n );
    std::cout << "X = " << X << std::endl;
    std::cout << "Y = " << Y << std::endl;

    const kvs::Indent indent( 4 );

    std::cout << std::endl;
    std::cout << "Sum(X): " << kvs::Stat::Sum( X ) << std::endl;
    std::cout << "Mean(X): " << kvs::Stat::Mean( X ) << std::endl;
    std::cout << "Var(X): " << kvs::Stat::Var( X ) << std::endl;
    std::cout << indent << "ShiftedVar(X): " << kvs::Stat::ShiftedVar( X ) << std::endl;
    std::cout << indent << "TwoPassVar(X): " << kvs::Stat::TwoPassVar( X ) << std::endl;
    std::cout << indent << "CompensatedVar(X): " << kvs::Stat::CompensatedVar( X ) << std::endl;
    std::cout << indent << "OnlineVar(X): " << kvs::Stat::OnlineVar( X ) << std::endl;
    std::cout << "Cov(X,Y): " << kvs::Stat::Cov( X, Y ) << std::endl;
    std::cout << indent << "ShiftedCov(X,Y): " << kvs::Stat::ShiftedCov( X, Y ) << std::endl;
    std::cout << indent << "TwoPassCov(X,Y): " << kvs::Stat::TwoPassCov( X, Y ) << std::endl;
    std::cout << indent << "OnlineCov(X,Y): " << kvs::Stat::OnlineCov( X, Y ) << std::endl;
    std::cout << "StdDev(X): " << kvs::Stat::StdDev( X ) << std::endl;
    std::cout << indent << "StdDev(X,ShiftedVar): " << kvs::Stat::StdDev( X, kvs::Stat::ShiftedVar<float> ) << std::endl;
    std::cout << "Corr(X,Y): " << kvs::Stat::Corr( X, Y ) << std::endl;
    std::cout << "AutoCorr(X): " << kvs::Stat::AutoCorr( X ) << std::endl;
    std::cout << indent << "AutoCorr(X,0): " << kvs::Stat::AutoCorr( X, 0 ) << std::endl;
    std::cout << indent << "AutoCorr(X,1): " << kvs::Stat::AutoCorr( X, 1 ) << std::endl;
    std::cout << indent << "AutoCorr(X,2): " << kvs::Stat::AutoCorr( X, 2 ) << std::endl;
    std::cout << "CrossCorr(X,Y): " << kvs::Stat::CrossCorr( X, Y ) << std::endl;
    std::cout << indent << "CrossCorr(X,Y,0): " << kvs::Stat::CrossCorr( X, Y, 0 ) << std::endl;
    std::cout << indent << "CrossCorr(X,Y,1): " << kvs::Stat::CrossCorr( X, Y, 1 ) << std::endl;
    std::cout << indent << "CrossCorr(X,Y,2): " << kvs::Stat::CrossCorr( X, Y, 2 ) << std::endl;

    kvs::Stat::Standardize(X);
    std::cout << "Standardize(X): " << X << std::endl;
    std::cout << indent << "Mean(X): " << kvs::Stat::Mean(X) << std::endl;
    std::cout << indent << "Var(X): " << kvs::Stat::Var(X) << std::endl;
    return 0;
}
