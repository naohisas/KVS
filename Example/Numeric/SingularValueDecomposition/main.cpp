/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of SVD
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/SVDecomposition>
#include <kvs/Vector>
#include <kvs/Matrix>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main()
{
    // Input matrix
    kvs::Matrix<double> M =
        { { 2.0, 4.0, 1.0, 3.0 },
          { 1.0, 5.0, 3.0, 2.0 },
          { 5.0, 7.0, 0.0, 7.0 } };

    const kvs::Indent indent(4);
    std::cout << "Matrix M: " << std::endl << M.format( true, indent ) << std::endl;
    std::cout << "Rank(M): " << std::endl << indent << M.rank() << std::endl;

    // Singular Value Decomposition
    kvs::SVDecomposition<double> svd( M );
    auto U = svd.leftSingularMatrix();
    auto V = svd.rightSingularMatrix();
    auto w = svd.singularValues();
    auto W = kvs::Matrix<double>::Diagonal( w );
    auto S = U * W * V.transposed();

    std::cout << "Sigular Values (w): " << std::endl << indent << w.format() << std::endl;
    std::cout << "Left Sigular Matrix (U): " << std::endl << U.format( true, indent ) << std::endl;
    std::cout << "Right Sigular Matrix (V): " << std::endl << V.format( true, indent ) << std::endl;
    std::cout << "U * diag(w) * V^t: " << std::endl << S.format( true ) << std::endl;

    return 0;
}
