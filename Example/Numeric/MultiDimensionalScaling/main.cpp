#include <iostream>
#include <kvs/ValueTable>
#include <kvs/MultiDimensionalScaling>


int main( int argc, char** argv )
{
    using MDS = kvs::MultiDimensionalScaling<float>;
    using Table = kvs::ValueTable<float>;

    Table data = {
        {-1, -1, 3},   // x components
        {-2, -1, 5},   // y components
        {-3, -2, 1} }; // z components

    MDS mds( data, 2 );
    auto result = mds.transform( data );
    std::cout << result << std::endl;

    return 0;
}
