#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int rank = world.rank();
    auto send_values = kvs::ValueArray<int>::Linear( world.size() );
    std::cout << "Rank " << rank << ": Send " << send_values.format() << " to all" << std::endl;

    kvs::ValueArray<int> recv_values;
    world.allToAll( send_values, recv_values );
    std::cout << "Rank " << rank << ": Recv " << recv_values.format() << " from all" << std::endl;

    return 0;
}
