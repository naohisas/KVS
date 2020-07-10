#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>
#include <kvs/mpi/Window>


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();
    kvs::mpi::Window<int> window;
    if ( rank != root )
    {
        const int to = root;
        const int offset = rank;

        kvs::ValueArray<int> values( { rank } );

        window.create( world, values );
        window.fence();
        window.put( values, to, offset );
        window.fence();
    }
    else
    {
        window.allocate( world, size );
    }

    if ( rank == root )
    {
        std::cout << "Rank " << root << ": Recv " << window.buffer().format() << " from all" << std::endl;
    }
    window.free();

    return 0;
}
