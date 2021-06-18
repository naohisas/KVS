#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( 3 * world.size() );
    if ( rank == root )
    {
        for ( int i = 0; i < world.size(); i++ )
        {
            send_values[ 3 * i + 0 ] = i;
            send_values[ 3 * i + 1 ] = i;
            send_values[ 3 * i + 2 ] = i;
        }

        std::cout << "Rank " << rank << ": Send " << send_values.format() << " to all" << std::endl;
    }

    kvs::ValueArray<int> recv_values;
    world.scatter( root, send_values, recv_values );
    std::cout << "Rank " << rank << ": Recv " << recv_values.format() << " from Rank " << root << std::endl;

    return 0;
}
