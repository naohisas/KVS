#include <iostream>
#include <sstream>
#include <iomanip>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/HydrogenVolumeData>
#include <kvs/RayCastingRenderer>
#include <kvs/Timer>
#include <kvs/egl/Screen>


int main( int argc, char** argv )
{
    std::cout << "EGL version: " << kvs::egl::Version() << std::endl;

    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vec3u( 64, 64, 64 ) );

    kvs::StructuredVolumeObject* object = volume;
    // kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();
    kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();

    kvs::egl::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object, renderer );

    screen.displayInfo();
    kvs::Timer timer( kvs::Timer::Start );
    for ( size_t i = 0; i < 12; i++ )
    {
        std::stringstream num; num << std::setw(3) << std::setfill('0') << i;
        std::string filename = "output_" + num.str() + ".bmp";

        std::cout << "rendering to ... " << std::flush;
        object->multiplyXform( kvs::Xform::Rotation( kvs::Mat3::RotationY( 30 ) ) );
        screen.draw();
        screen.capture().write( filename );
        std::cout << filename << std::endl;
    }
    timer.stop();

    std::cout << "Total rendering time:   " << timer.sec() << " [sec]" << std::endl;
    std::cout << "Average rendering time: " << timer.sec() / 4.0f << " [sec]" << std::endl;

    return 0;
}
