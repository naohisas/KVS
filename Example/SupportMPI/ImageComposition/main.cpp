#include <kvs/mpi/Environment>
#include <kvs/mpi/Communicator>
#include <kvs/mpi/ImageCompositor>
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

#if defined( KVS_SUPPORT_OSMESA )
#include <kvs/osmesa/Screen>
namespace { using OffScreen = kvs::osmesa::Screen; }
#elif defined( KVS_SUPPORT_EGL )
#include <kvs/egl/Screen>
namespace { using OffScreen = kvs::egl::Screen; }
#endif


//#define OUTPUT_ALPHA_IMAGES
//#define OUTPUT_DEPTH_IMAGES


namespace
{

inline kvs::StructuredVolumeObject* Div(
    const int id,
    const int ndiv,
    const kvs::StructuredVolumeObject* volume )
{
    const size_t slice_size = volume->resolution().x() * volume->resolution().y();
    const size_t nslices = volume->resolution().z() / ndiv;
    const size_t size = slice_size * ( ( id == ndiv - 1 ) ? volume->resolution().z() - nslices * ( id -1 ) : nslices );
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

#if defined( OUTPUT_ALPHA_IMAGES )
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
#endif

#if defined( OUTPUT_DEPTH_IMAGES )
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
#endif

} // end of namespace


int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();
    const int volume_size = argc > 1 ? atoi( argv[1] ) : 128;
    const int image_size = argc > 2 ? atoi( argv[2] ) : 512;

    // Input volume data.
    auto* volume = new kvs::HydrogenVolumeData( kvs::Vec3u::Constant( volume_size ) );
    auto* object = new kvs::Isosurface( Div( rank, size, volume ), 40 );
    auto* renderer = new kvs::glsl::PolygonRenderer();

    // Off-screen settings.
    ::OffScreen screen;
    screen.setSize( image_size, image_size );
    screen.registerObject( volume, new kvs::Bounds() );
    screen.registerObject( object, renderer );

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

    kvs::Timer timer;

    // Execute off-screen rendering.
    timer.start();
    screen.draw();
    timer.stop();
    {
        double min_sec = 0.0; world.reduce( root, timer.sec(), min_sec, MPI_MIN );
        double max_sec = 0.0; world.reduce( root, timer.sec(), max_sec, MPI_MAX );
        double sum_sec = 0.0; world.reduce( root, timer.sec(), sum_sec, MPI_SUM );
        if ( rank == root )
        {
            std::cout << "Rendering time:" << std::endl;
            std::cout << "    Min: " << min_sec << " [sec]" << std::endl;
            std::cout << "    Max: " << max_sec << " [sec]" << std::endl;
            std::cout << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
        }
    }

    // Read-back the rendering image of each sub-volume.
    timer.start();
    auto width = screen.width();
    auto height = screen.height();
    auto color_buffer = screen.readbackColorBuffer();
    auto depth_buffer = screen.readbackDepthBuffer();
    timer.stop();
    ::ColorImage( width, height, color_buffer ).write( "output_" + kvs::String::ToString( rank ) + ".bmp");
#if defined( OUTPUT_ALPHA_IMAGES )
    ::AlphaImage( width, height, color_buffer ).write( "output_alpha_" + kvs::String::ToString( rank ) + ".bmp");
#endif
#if defined( OUTPUT_DEPTH_IMAGES )
    ::DepthImage( width, height, depth_buffer ).write( "output_depth_" + kvs::String::ToString( rank ) + ".bmp");
#endif
    {
        double min_sec = 0.0; world.reduce( root, timer.sec(), min_sec, MPI_MIN );
        double max_sec = 0.0; world.reduce( root, timer.sec(), max_sec, MPI_MAX );
        double sum_sec = 0.0; world.reduce( root, timer.sec(), sum_sec, MPI_SUM );
        if ( rank == root )
        {
            std::cout << "Read-back time:" << std::endl;
            std::cout << "    Min: " << min_sec << " [sec]" << std::endl;
            std::cout << "    Max: " << max_sec << " [sec]" << std::endl;
            std::cout << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
        }
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
        if ( rank == root )
        {
            std::cout << "Composition time: " << timer.sec() << " [sec]" << std::endl;
            std::cout << "    Min: " << min_sec << " [sec]" << std::endl;
            std::cout << "    Max: " << max_sec << " [sec]" << std::endl;
            std::cout << "    Ave: " << sum_sec / size << " [sec]" << std::endl;
        }
    }

    // Write the merged image.
    if ( rank == root ) { ::ColorImage( width, height, color_buffer ).write( "output.bmp" ); }

    return 0;
}
