#include <kvs/OffScreen>
#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>
#include <kvs/mpi/ImageCompositor>
#include <kvs/mpi/Logger>
#include <kvs/StructuredVolumeObject>
#include <kvs/HydrogenVolumeData>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/RayCastingRenderer>
#include <kvs/PolygonRenderer>
#include <kvs/Bounds>
#include <kvs/String>
#include <kvs/Coordinate>
#include <kvs/Timer>

//#define VOLUME_RENDERING

namespace
{

inline kvs::StructuredVolumeObject* Div(
    const int id,
    const int ndiv,
    const kvs::StructuredVolumeObject* volume )
{
    const size_t slice_size = volume->resolution().x() * volume->resolution().y();
    const size_t nslices = volume->resolution().z() / ndiv;
    const size_t size = slice_size * nslices;
    const size_t offset = id * slice_size * ( nslices - 1 );
    kvs::ValueArray<kvs::UInt8> values( volume->values().asValueArray<kvs::UInt8>().data() + offset, size );

    const auto dimx = volume->resolution().x();
    const auto dimy = volume->resolution().y();
    const auto dimz = nslices;
    auto* object = new kvs::StructuredVolumeObject();
    object->setGridType( volume->gridType() );
    object->setVeclen( volume->veclen() );
    object->setResolution( kvs::Vec3ui( dimx, dimy, dimz ) );
    object->setValues( kvs::AnyValueArray( values ) );
    object->setMinMaxValues( 0, 255 );
    object->updateMinMaxCoords();
    return object;
}

inline kvs::ColorImage ColorImage(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& rgba )
{
    kvs::ValueArray<kvs::UInt8> rgb( width * height * 3 );
    for ( size_t i = 0; i < width * height; i++ )
    {
        rgb[ 3 * i + 0 ] = rgba[ 4 * i + 0 ];
        rgb[ 3 * i + 1 ] = rgba[ 4 * i + 1 ];
        rgb[ 3 * i + 2 ] = rgba[ 4 * i + 2 ];
    }
    return kvs::ColorImage( width, height, rgb );
}

inline kvs::ColorImage AlphaImage(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& rgba )
{
    kvs::ValueArray<kvs::UInt8> rgb( width * height * 3 );
    for ( size_t i = 0; i < width * height; i++ )
    {
        rgb[ 3 * i + 0 ] = rgba[ 4 * i + 3 ];
        rgb[ 3 * i + 1 ] = rgba[ 4 * i + 3 ];
        rgb[ 3 * i + 2 ] = rgba[ 4 * i + 3 ];
    }
    return kvs::ColorImage( width, height, rgb );
}

inline kvs::ColorImage DepthImage(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::Real32>& depth )
{
    kvs::ValueArray<kvs::UInt8> rgb( width * height * 3 );
    for ( size_t i = 0; i < width * height; i++ )
    {
        rgb[ 3 * i + 0 ] = int( depth[ i ] * 255 );
        rgb[ 3 * i + 1 ] = int( depth[ i ] * 255 );
        rgb[ 3 * i + 2 ] = int( depth[ i ] * 255 );
    }
    return kvs::ColorImage( width, height, rgb );
}

} // end of namespace


int main( int argc, char** argv )
{
    // MPI related parameters.
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world( MPI_COMM_WORLD );
    kvs::mpi::Logger logger( world );

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();

    // Input parameters.
    const int volume_size = argc > 1 ? atoi( argv[1] ) : 128;
    const int image_size = argc > 2 ? atoi( argv[2] ) : 512;
    const bool output_color_images = argc > 3 ? atoi( argv[3] ) == 1 ? true : false : false;
    const bool output_alpha_images = argc > 4 ? atoi( argv[4] ) == 1 ? true : false : false;
    const bool output_depth_images = argc > 5 ? atoi( argv[5] ) == 1 ? true : false : false;

    // Input volume data.
    auto* volume = new kvs::HydrogenVolumeData( kvs::Vec3u::Constant( volume_size ) );
#if defined( VOLUME_RENDERING )
    auto* object = Div( rank, size, volume );
    auto* renderer = new kvs::glsl::RayCastingRenderer();
#else // ISOSURFACE_RENDERING
    auto* object = new kvs::Isosurface( Div( rank, size, volume ), 40 );
    auto* renderer = new kvs::glsl::PolygonRenderer();
#endif

    // Off-screen settings.
    kvs::OffScreen screen;
    screen.setSize( image_size, image_size );
    screen.registerObject( volume, new kvs::Bounds() );
    screen.registerObject( object, renderer );
    {
        // Object rotation.
        auto R = kvs::Xform::Rotation( kvs::Mat3::RotationY( 70 ) );
        volume->multiplyXform( R );
        object->multiplyXform( R );

        // Registration for each sub-volume.
        const size_t nslices = volume->resolution().z() / size;
        const kvs::Vec3 offset( 0.0f, 0.0f, rank * ( nslices - 1 ) ); // in object coordinate
        const kvs::Vec3 origin( 0, 0, 0 ); // in object coordinate
        const kvs::Vec3 Ta = kvs::ObjectCoordinate( origin, object ).toWorldCoordinate().position();
        const kvs::Vec3 Tb = kvs::ObjectCoordinate( offset, object ).toWorldCoordinate().position();
        const kvs::Vec3 T = Tb - Ta; // in world coordinate
        object->multiplyXform( kvs::Xform::Translation( T ) );
    }

    // Timer.
    kvs::Timer timer;

    // Execute off-screen rendering.
    timer.start();
    screen.draw();
    timer.stop();
    {
        double min_sec = 0.0; world.reduce( root, timer.sec(), min_sec, MPI_MIN );
        double max_sec = 0.0; world.reduce( root, timer.sec(), max_sec, MPI_MAX );
        double sum_sec = 0.0; world.reduce( root, timer.sec(), sum_sec, MPI_SUM );
        logger( root ) << "Rendering time:" << std::endl;
        logger( root ) << "    Min: " << min_sec << " [sec]" << std::endl;
        logger( root ) << "    Max: " << max_sec << " [sec]" << std::endl;
        logger( root ) << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
    }

    // Read-back the rendering image of each sub-volume.
    timer.start();
    auto width = screen.width();
    auto height = screen.height();
    auto color_buffer = screen.readbackColorBuffer();
    auto depth_buffer = screen.readbackDepthBuffer();
    timer.stop();
    {
        double min_sec = 0.0; world.reduce( root, timer.sec(), min_sec, MPI_MIN );
        double max_sec = 0.0; world.reduce( root, timer.sec(), max_sec, MPI_MAX );
        double sum_sec = 0.0; world.reduce( root, timer.sec(), sum_sec, MPI_SUM );
        logger( root ) << "Read-back time:" << std::endl;
        logger( root ) << "    Min: " << min_sec << " [sec]" << std::endl;
        logger( root ) << "    Max: " << max_sec << " [sec]" << std::endl;
        logger( root ) << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
    }

    // Dump rendering images.
    {
        auto n = kvs::String::ToString( rank );
        if ( output_color_images ) { ::ColorImage( width, height, color_buffer ).write( "output_" + n + ".bmp"); }
        if ( output_alpha_images ) { ::AlphaImage( width, height, color_buffer ).write( "output_alpha_" + n + ".bmp"); }
        if ( output_depth_images ) { ::DepthImage( width, height, depth_buffer ).write( "output_depth_" + n + ".bmp"); }
    }

    // Image composition.
    timer.start();
    const bool depth_testing = true;
    kvs::ImageCompositor compositor( world );
    compositor.initialize( width, height, depth_testing );
    compositor.run( color_buffer, depth_buffer );
    compositor.destroy();
    timer.stop();
    {
        double min_sec = 0.0; world.reduce( root, timer.sec(), min_sec, MPI_MIN );
        double max_sec = 0.0; world.reduce( root, timer.sec(), max_sec, MPI_MAX );
        double sum_sec = 0.0; world.reduce( root, timer.sec(), sum_sec, MPI_SUM );
        logger( root ) << "Composition time: " << timer.sec() << " [sec]" << std::endl;
        logger( root ) << "    Min: " << min_sec << " [sec]" << std::endl;
        logger( root ) << "    Max: " << max_sec << " [sec]" << std::endl;
        logger( root ) << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
    }

    // Write the merged image.
    if ( rank == root )
    {
        ::ColorImage( width, height, color_buffer ).write( "output.bmp" );
    }

    return 0;
}
