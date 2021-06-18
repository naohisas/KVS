#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> values;
    if ( rank == root )
    {
        values = kvs::ValueArray<int>::Linear( 3 );
        std::cout << "Rank " << rank << ": Send " << values.format() << " to all" << std::endl;
    }

    world.broadcast( root, values );

    if ( rank != root )
    {
        std::cout << "Rank " << rank << ": Recv " << values.format() << " from Rank " << root << std::endl;
    }

    return 0;
}
