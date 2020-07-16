#pragma once
#include <kvs/mpi/Communicator>
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

class ImageCompositor
{
private:
    int m_rank; ///< MPI rank (my rank)
    int m_size; ///< MPI size (number of nodes)
    MPI_Comm m_comm; ///< MPI communicator
    size_t m_width; ///< image width
    size_t m_height; ///< image height
    unsigned int m_pixel_type; ///< pixel type (RGBA 32-bit or RGBA-Z 64-bit)
    unsigned int m_merge_type; ///< merge type (depth-testing or alpha-blending)

public:
    ImageCompositor( const int rank, const int size, const MPI_Comm comm = MPI_COMM_WORLD );
    ImageCompositor( const kvs::mpi::Communicator& comm );
    ~ImageCompositor();

    bool initialize( const size_t width, const size_t height, const bool enable_depth_testing = false );
    bool destroy();
    bool run( kvs::ValueArray<kvs::UInt8>& color_buffer );
    bool run( kvs::ValueArray<kvs::UInt8>& color_buffer, kvs::ValueArray<kvs::Real32>& depth_buffer );
};

} // end of namespace kvs
