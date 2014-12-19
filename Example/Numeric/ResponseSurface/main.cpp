/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of Response Surface Method.
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
#include <kvs/ResponseSurface>
#include <iostream>
#include <kvs/Vector>
#include <kvs/Matrix>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    const size_t npoints = 12; // number of data points
    const size_t nvariables = 2; // number of independent variables

    kvs::Matrix<double> X( npoints, nvariables ); // independent variables
    kvs::Vector<double> Y( npoints ); // dependent variables (responses)
    X[ 0][0] = -1.0; X[ 0][1] = -1.0;    Y[ 0] = 16.2;
    X[ 1][0] = -1.0; X[ 1][1] =  1.0;    Y[ 1] =  7.5;
    X[ 2][0] = -1.0; X[ 2][1] =  0.0;    Y[ 2] = 10.2;
    X[ 3][0] = -1.0; X[ 3][1] =  0.5;    Y[ 3] = 13.2;
    X[ 4][0] =  1.0; X[ 4][1] = -1.0;    Y[ 4] = -4.2;
    X[ 5][0] =  1.0; X[ 5][1] =  1.0;    Y[ 5] = 15.6;
    X[ 6][0] = -1.0; X[ 6][1] =  0.0;    Y[ 6] = 10.2;
    X[ 7][0] =  1.0; X[ 7][1] =  0.0;    Y[ 7] =  5.2;
    X[ 8][0] =  0.0; X[ 8][1] =  0.0;    Y[ 8] =  4.8;
    X[ 9][0] =  0.0; X[ 9][1] =  1.0;    Y[ 9] =  6.0;
    X[10][0] =  0.0; X[10][1] =  0.5;    Y[10] =  5.0;
    X[11][0] =  0.5; X[11][1] =  0.5;    Y[11] =  9.5;

    // Independent variables.
    std::cout << "> Independent variables" << std::endl << X << std::endl;

    // Dependent variables (response values).
    std::cout << "> Dependent variables" << std::endl << Y << std::endl;

    // Response Surface Method (RSM).
    kvs::ResponseSurface<double> rsm( X, Y );
    std::cout << "> Response Surface Method" << std::endl;
    std::cout << "Second-order model: Y = "
              << rsm[0] << " + "
              << rsm[1] << " X0 + "
              << rsm[2] << " X1 + "
              << rsm[3] << " X0*X0 + "
              << rsm[4] << " X1*X1 + "
              << rsm[5] << " X0*X1" << std::endl;
    std::cout << "T-test values: " << rsm.Tvalues() << std::endl;
    std::cout << "R-square: " << rsm.Rsquare() << std::endl;

    // Improve the RSM.
    const double threshold = 2.45; // threshold for the t-test.
    rsm.improve( threshold );
    std::cout << "> Improved Response Surface Method" << std::endl;
    std::cout << "Second-order model: Y = "
              << rsm[0] << " + "
              << rsm[1] << " X0 + "
              << rsm[2] << " X1 + "
              << rsm[3] << " X0*X0 + "
              << rsm[4] << " X1*X1 + "
              << rsm[5] << " X0*X1" << std::endl;
    std::cout << "T-test values: " << rsm.Tvalues() << std::endl;
    std::cout << "R-square: " << rsm.Rsquare() << std::endl;

    return( 0 );
}
