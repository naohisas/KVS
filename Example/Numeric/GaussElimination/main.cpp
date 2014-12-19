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
#include <iomanip>
#include <kvs/GaussEliminationSolver>
#include <kvs/Vector>
#include <kvs/Matrix>

/*===========================================================================*/
/**
 *  @brief  Prints the equations and the solution.
 *  @param  A [in] coefficient matrix
 *  @param  b [in] constant term vector
 *  @param  x [in] solution vector
 */
/*===========================================================================*/
void Print( const kvs::Matrix<double>& A, const kvs::Vector<double>& b, const kvs::Vector<double>&x )
{
    std::cout << "> Linear simultaneous equation" << std::endl;
    std::cout.setf( std::ios::showpos );
    for ( size_t i = 0; i < 4; i++ )
    {
        for ( size_t j = 0; j < 3; j++ )
        {
            std::cout << "(" << A[i][j] << ") * X" << j << " + ";
        }
        std::cout << "(" << A[i][3] << ") * X3 = " << b[i] << std::endl;
    }
    std::cout.unsetf( std::ios::showpos );
    std::cout << "> Solution" << std::endl;
    std::cout << "(X0 X1 X2 X3) = (" << x << ")" << std::endl;
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

    // Solution vector x = (-7, 3, 2, 3)
    const kvs::Vector<double> x = kvs::GaussEliminationSolver<double>( A, b );

    // Print the equation and the solution.
    Print( A, b, x );
}
