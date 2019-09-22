/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of the eigen decomposition.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/EigenDecomposer>
#include <kvs/Vector>
#include <kvs/Matrix>
#include <kvs/Timer>
#include <kvs/Indent>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Print the matrix and the solved eigenvalues and eigenvectors.
 *  @param  M [in] input matrix
 *  @param  eigen [in] eigen decomposer
 */
/*===========================================================================*/
void PrintResult( const kvs::Matrix<double>& M, const kvs::EigenDecomposer<double>& eigen )
{
    const kvs::Indent indent(4);
    const kvs::Vector<double>& L = eigen.eigenValues();
    const kvs::Matrix<double>& E = eigen.eigenVectors();

    std::cout.precision(3);
    std::cout.setf( std::ios::right );
    std::cout.setf( std::ios::fixed, std::ios::floatfield );
    std::cout << "Input matrix" << std::endl;
    std::cout << indent << "M = " << M << std::endl;
    std::cout << "Eigen values" << std::endl;
    std::cout << indent << "L0 = " << L[0] << std::endl;
    std::cout << indent << "L1 = " << L[1] << std::endl;
    std::cout << indent << "L2 = " << L[2] << std::endl;
    std::cout << "Eigen vectors" << std::endl;
    std::cout << indent << "E0 = " << E[0] << std::endl;
    std::cout << indent << "E1 = " << E[1] << std::endl;
    std::cout << indent << "E2 = " << E[2] << std::endl;
    std::cout << "Check" << std::endl;
    std::cout << indent << "M  * E0 = " << M * E[0] << std::endl;
    std::cout << indent << "L0 * E0 = " << L[0] * E[0] << std::endl;
    std::cout << indent << "M  * E1 = " << M * E[1] << std::endl;
    std::cout << indent << "L1 * E1 = " << L[1] * E[1] << std::endl;
    std::cout << indent << "M  * E2 = " << M * E[2] << std::endl;
    std::cout << indent << "L2 * E2 = " << L[2] * E[2] << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Perfomance test of the eigen decompsition..
 *  @param  M [in] input matrix
 *  @param  n [in] number of trials
 */
/*===========================================================================*/
void PerfTest( const kvs::Matrix<double>& M, const size_t nloops )
{
    const kvs::Indent indent(4);

    std::cout << "Performance Test" << std::endl;
    kvs::Timer timer( kvs::Timer::Start );
    for ( size_t i = 0; i < nloops; i++ ) { kvs::EigenDecomposer<double> e( M ); }
    timer.stop();
    std::cout << indent << "Calculation (" << nloops << " times): " << timer.sec() << " [sec]" << std::endl;
}

}

/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main()
{
    /* The maximum number of interations and the tolerance can be set for the
     * eigen value decompostion (QR method or Power method) by using the
     * following static functions. The folowing set values are default.
     *
     *   kvs::EigenDecomposer<double>::SetMaxIterations( 1000 );
     *   kvs::EigenDecomposer<double>::SetMaxTolerance( 1.0e-10 );
     */

    // Case 1: Symmetric matrix
    {
        /* For symmetric matrix, the tridiagonal QR method is used for
         * the calculation of the eigen values and vectors. The matrix
         * M has three eigenvalues (Li) and three eigenvectors (Ei for Li).
         *
         *   L0 = 1.944, E0 = (0.519,  0.637,  0.570)
         *   L1 = 0.707, E1 = (0.787, -0.096, -0.609)
         *   L2 = 0.349, E2 = (0.333, -0.765,  0.551)
         */
        kvs::Matrix<double> M( 3, 3 );
        M[0][0] = 1.0; M[0][1] = 0.5; M[0][2] = 0.3;
        M[1][0] = 0.5; M[1][1] = 1.0; M[1][2] = 0.6;
        M[2][0] = 0.3; M[2][1] = 0.6; M[2][2] = 1.0;

        kvs::EigenDecomposer<double> eigen( M );
        ::PrintResult( M, eigen );

        const size_t nloops = 100000;
        ::PerfTest( M, nloops );
    }

    std::cout << std::endl;

    // Case 2: Asymmetric matrix
    {
        /* For asymmetric matrix, the hessenberg QR method is used for
         * the calculation of the eigen values and vectors. The matrix
         * M has three  eigenvalues (Li) and three eigenvectors (Ei for Li).
         *
         *   L0 = 11.464, E0 = (0.146, -0.509,  0.848)
         *   L1 =  4.536, E1 = (0.658, -0.681, -0.322)
         *   L2 = -4.000, E2 = (0.577, -0.577, -0.577)
         */
        kvs::Matrix<double> M( 3, 3 );
        M[0][0] = 1.0; M[0][1] =  2.0; M[0][2] =  3.0;
        M[1][0] = 3.0; M[1][1] =  4.0; M[1][2] = -5.0;
        M[2][0] = 5.0; M[2][1] = -6.0; M[2][2] =  7.0;

        kvs::EigenDecomposer<double> eigen( M );
        ::PrintResult( M, eigen );

        const size_t nloops = 100000;
        ::PerfTest( M, nloops );
    }
}
