/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for OrientationAxis widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/OrientationAxis>
#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Color map.
    kvs::ColorMap cmap( 256 );
    cmap.addPoint( 0, kvs::RGBColor( 0, 0, 0 ) );
    cmap.addPoint( 128, kvs::RGBColor( 255, 0, 0 ) );
    cmap.addPoint( 255, kvs::RGBColor( 255, 255, 255 ) );
    cmap.create();

    // Object.
    kvs::StructuredVolumeObject* volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, 100 );
    delete volume;

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "OrientationAxis" );
    screen.registerObject( object );
    screen.show();

    // Orientation axis.
    kvs::OrientationAxis orientation_axis( &screen, screen.scene() );
    orientation_axis.setBoxType( kvs::OrientationAxis::SolidBox );
    orientation_axis.show();

    return( app.run() );
}
