#include <iostream>
#include <kvs/ValueTable>
#include <kvs/PrincipalComponentAnalysis>


int main( int argc, char** argv )
{
    using PCA = kvs::PrincipalComponentAnalysis<float>;
    using Table = kvs::ValueTable<float>;

    Table data = {
        {-1, -1, 3},   // x components
        {-2, -1, 5},   // y components
        {-3, -2, 1} }; // z components

    PCA pca( data, 2 );
    auto result = pca.transform( data );
    std::cout << result << std::endl;

    return 0;
}
