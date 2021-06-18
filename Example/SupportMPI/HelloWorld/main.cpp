#include <iostream>
#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int size = world.size();
    const int rank = world.rank();
    std::cout << "Size = " << size << ", Rank = " << rank << std::endl;

    return 0;
}
