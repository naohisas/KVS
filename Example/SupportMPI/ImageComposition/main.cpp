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

#if defined( KVS_SUPPORT_OSMESA )
#include <kvs/osmesa/Screen>
namespace local { using Screen = kvs::osmesa::Screen; }
#elif defined( KVS_SUPPORT_EGL )
#include <kvs/egl/Screen>
namespace local { using Screen = kvs::egl::Screen; }
#endif

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
        //if ( rgba[ 4 * i + 3 ] != 255 ) std::cout << "a = " << (int)rgba[ 4 * i + 3 ] << std::endl;
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

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();

    // Input volume data.
    auto* volume = new kvs::HydrogenVolumeData( kvs::Vec3u::Constant( 128 ) );
    auto* object = new kvs::Isosurface( Div( rank, size, volume ), 40 );
    auto* renderer = new kvs::glsl::PolygonRenderer();

    // Off-screen settings.
    local::Screen screen;
    screen.setSize( 512, 512 );
//    screen.setBackgroundColor( kvs::RGBColor( 0, 0, 0 ) );
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

    // Execute off-screen rendering.
    screen.draw();

    // Read-back the rendering image of each sub-volume.
    auto width = screen.width();
    auto height = screen.height();
    auto color_buffer = screen.readbackColorBuffer();
    auto depth_buffer = screen.readbackDepthBuffer();

#if defined( DEBUG )
    ColorImage( width, height, color_buffer ).write( "output_" + kvs::String::ToString( rank ) + ".bmp");
    AlphaImage( width, height, color_buffer ).write( "output_alpha_" + kvs::String::ToString( rank ) + ".bmp");
    DepthImage( width, height, depth_buffer ).write( "output_depth_" + kvs::String::ToString( rank ) + ".bmp");
#endif

    const bool depth_testing = true;
    kvs::ImageCompositor compositor( world );
    compositor.initialize( width, height, depth_testing );
    compositor.run( color_buffer, depth_buffer );
    compositor.destroy();

    // Write the merged image.
    if ( rank == root ) ColorImage( width, height, color_buffer ).write( "output.bmp" );

    return 0;
}
