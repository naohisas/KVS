#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TableObject>
#include <kvs/ScatterPlotRenderer>
#include <kvs/Axis2D>


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    const size_t nsamples = 100;
    const kvs::ValueArray<float> random_x = kvs::ValueArray<float>::Random( nsamples );
    const kvs::ValueArray<float> random_y = kvs::ValueArray<float>::Random( nsamples );

    kvs::TableObject* object = new kvs::TableObject();
    object->addColumn( kvs::AnyValueArray( random_x ), "Ramdom X" );
    object->addColumn( kvs::AnyValueArray( random_y ), "Random Y" );

    kvs::ScatterPlotRenderer* renderer = new kvs::ScatterPlotRenderer();
    renderer->setPointSize( 5.0f );
    renderer->setPointColor( kvs::RGBColor( 200, 200, 255 ) );
    renderer->setEdgeWidth( 1.0f );
    renderer->setLeftMargin( 110 );
    renderer->setBottomMargin( 50 );

    kvs::Axis2D* axis = new kvs::Axis2D();
    axis->setLeftMargin( 110 );
    axis->setBottomMargin( 50 );

    screen.registerObject( object, axis );
    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
