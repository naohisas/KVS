#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( { rank, rank, rank } );
    std::cout << "Rank " << rank << ": Send " << send_values.format() << " to Rank " << root << std::endl;

    kvs::ValueArray<int> recv_values;
    world.gather( root, send_values, recv_values );
    if ( rank == root )
    {
        std::cout << "Rank " << rank << ": Recv " << recv_values.format() << " from all" << std::endl;
    }

    return 0;
}
