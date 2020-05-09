/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of the Gauss Elimination method.
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
#include <kvs/GaussEliminationSolver>
#include <kvs/Vector>
#include <kvs/Matrix>
#include <kvs/Indent>
#include <kvs/Timer>


/*===========================================================================*/
/**
 *  @brief  Prints the equations and the solution.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 *  @param  x [in] solution vector
 */
/*===========================================================================*/
void PrintResult(
    const kvs::Matrix<double>& A,
    const kvs::Vector<double>& b,
    const kvs::Vector<double>&x )
{
    const kvs::Indent indent(4);
    std::cout << "Linear equation: A * x = b" << std::endl;
    std::cout << indent << "A = " << A << std::endl;
    std::cout << indent << "b = " << b << std::endl;
    std::cout << "Solution" << std::endl;
    std::cout << indent << "x = " << x << std::endl;
    std::cout << "Check" << std::endl;
    std::cout << indent << "b     = " << b << std::endl;
    std::cout << indent << "A * x = " << A * x << std::endl;
}

void PerfTest( const kvs::Matrix<double>& A, const kvs::Vector<double>& b, const size_t nloops )
{
    const kvs::Indent indent(4);

    std::cout << "Performance Test" << std::endl;
    kvs::Vector<double> x;
    kvs::Timer timer( kvs::Timer::Start );
    for ( size_t i = 0; i < nloops; i++ ) { x = kvs::GaussEliminationSolver<double>( A, b ); }
    timer.stop();
    std::cout << indent << "Calculation (" << nloops << " times): " << timer.sec() << " [sec]" << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    /* The following linear simultaneous equation can be solved by using
     * Gauss Elimination method.
     *
     *     1 * X0 - 1 * X1 + 2 * X2 - 1 * X3 =  -8
     *     2 * X0 - 2 * X1 + 3 * X2 - 3 * X3 = -20
     *     1 * X0 + 1 * X1 + 1 * X2 + 0 * X3 =  -2
     *     1 * X0 - 1 * X1 + 4 * X2 + 3 * X3 =   4
     *
     * The coefficient matrix (A) and the constant term vector (b) are
     * represented as follows:
     */
    kvs::Matrix<double> A( 4, 4 );
    A[0][0] = 1.0; A[0][1] = -1.0; A[0][2] = 2.0; A[0][3] = -1.0;
    A[1][0] = 2.0; A[1][1] = -2.0; A[1][2] = 3.0; A[1][3] = -3.0;
    A[2][0] = 1.0; A[2][1] =  1.0; A[2][2] = 1.0; A[2][3] =  0.0;
    A[3][0] = 1.0; A[3][1] = -1.0; A[3][2] = 4.0; A[3][3] =  3.0;

    kvs::Vector<double> b( 4 );
    b[0] =  -8.0;
    b[1] = -20.0;
    b[2] =  -2.0;
    b[3] =   4.0;

    // Solution vector x = (-7, 3, 2, 2)
    const kvs::Vector<double> x = kvs::GaussEliminationSolver<double>( A, b );
    PrintResult( A, b, x );

    const size_t nloops = 100000;
    PerfTest( A, b, nloops );
}
