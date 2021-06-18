#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();
    if ( rank != root )
    {
        const int to = root;
        kvs::ValueArray<int> values( { rank, rank, rank } );
        std::cout << "Rank " << rank << ": Send " << values.format() << " to Rank " << to << std::endl;
        world.send( to, 0, values );
    }
    else
    {
        for ( int i = 0; i < size; i++ )
        {
            if ( i == root ) { continue; }
            const int from = i;
            kvs::ValueArray<int> values;
            world.receive( from, 0, values );
            std::cout << "Rank " << rank << ": Recv " << values.format() << " from Rank " << from << std::endl;
        }
    }

    return 0;
}
