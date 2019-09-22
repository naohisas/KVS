#include <kvs/GaussDistribution>
#include <kvs/ExponentialDistribution>
#include <kvs/StudentTDistribution>
#include <kvs/ChiSquaredDistribution>
#include <kvs/FisherFDistribution>
#include <kvs/ValueArray>
#include <iostream>


int main()
{
    const size_t size = 50;
    const float start = 0.0f;
    const float step = 0.1f;
    kvs::ValueArray<float> xvalues = kvs::ValueArray<float>::Linear( size, start, step );
    std::cout << "X = " << xvalues << std::endl << std::endl;

    const double mean = 0.0;
    const double sdev = 1.0;
    kvs::GaussDistribution gauss( mean, sdev );
    std::cout << "Gauss Distribution:" << std::endl;
    std::cout << "    PDF: " << gauss.pdf( xvalues ) << std::endl;
    std::cout << "    CDF: " << gauss.cdf( xvalues ) << std::endl;
    std::cout << std::endl;

    const double lambda = 1.0;
    kvs::ExponentialDistribution exp( lambda );
    std::cout << "Exponential Distribution:" << std::endl;
    std::cout << "    PDF: " << exp.pdf( xvalues ) << std::endl;
    std::cout << "    CDF: " << exp.cdf( xvalues ) << std::endl;
    std::cout << std::endl;

    const size_t t_dof = 1;
    kvs::StudentTDistribution t( t_dof );
    std::cout << "Student T Distribution:" << std::endl;
    std::cout << "    PDF: " << t.pdf( xvalues ) << std::endl;
    std::cout << "    CDF: " << t.cdf( xvalues ) << std::endl;
    std::cout << std::endl;

    const size_t chi2_dof = 3;
    kvs::ChiSquaredDistribution chi2( chi2_dof );
    std::cout << "Chi-Squared Distribution:" << std::endl;
    std::cout << "    PDF: " << chi2.pdf( xvalues ) << std::endl;
    std::cout << "    CDF: " << chi2.cdf( xvalues ) << std::endl;
    std::cout << std::endl;

    const size_t f_dof1 = 100;
    const size_t f_dof2 = 100;
    kvs::FisherFDistribution f( f_dof1, f_dof2 );
    std::cout << "Fisher's F Distribution: " << std::endl;
    std::cout << "    PDF: " << f.pdf( xvalues ) << std::endl;
    std::cout << "    CDF: " << f.cdf( xvalues ) << std::endl;
    std::cout << std::endl;

    return 0;
}
