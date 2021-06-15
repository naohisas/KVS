#include <iostream>
#include <sstream>
#include <iomanip>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/HydrogenVolumeData>
#include <kvs/RayCastingRenderer>
#include <kvs/Timer>
#include <kvs/OffScreen>


int main( int argc, char** argv )
{
    std::cout << "OSMesa version: " << kvs::osmesa::Version() << std::endl;

    auto* object = new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    auto* renderer = new kvs::glsl::RayCastingRenderer();
    //auto* renderer = new kvs::RayCastingRenderer();

    kvs::OffScreen screen;
    screen.setSize( 512, 512 );
    screen.registerObject( object, renderer );

    kvs::Timer timer( kvs::Timer::Start );
    for ( size_t i = 0; i < 12; i++ )
    {
        std::stringstream num; num << std::setw(3) << std::setfill('0') << i;
        std::string filename = "output_" + num.str() + ".bmp";

        std::cout << "rendering to ... " << std::flush;
        auto R = kvs::Xform::Rotation( kvs::Mat3::RotationY( 30 ) );
        object->multiplyXform( R );
        screen.draw();
        screen.capture().write( filename );
        std::cout << filename << std::endl;
    }
    timer.stop();

    std::cout << "Total:   " << timer.sec() << " [sec]" << std::endl;
    std::cout << "Average: " << timer.sec() / 12.0f << " [sec]" << std::endl;

    return 0;
}
