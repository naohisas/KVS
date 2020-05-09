/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::ScatterPlotRenderer class.
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TableObject>
#include <kvs/ScatterPlotRenderer>
#include <kvs/Axis2D>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    const size_t nsamples = 100;
    const auto random_x = kvs::ValueArray<float>::Random( nsamples );
    const auto random_y = kvs::ValueArray<float>::Random( nsamples );

    auto* object = new kvs::TableObject();
    object->addColumn( kvs::AnyValueArray( random_x ), "Ramdom X" );
    object->addColumn( kvs::AnyValueArray( random_y ), "Random Y" );

    auto* renderer = new kvs::ScatterPlotRenderer();
    renderer->setPointSize( 5.0f );
    renderer->setPointColor( kvs::RGBColor( 200, 200, 255 ) );
    renderer->setEdgeWidth( 1.0f );
    renderer->setLeftMargin( 110 );
    renderer->setBottomMargin( 50 );

    auto* axis = new kvs::Axis2D();
    axis->setLeftMargin( 110 );
    axis->setBottomMargin( 50 );

    screen.registerObject( object, axis );
    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
