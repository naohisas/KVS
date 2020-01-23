#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TableObject>
#include <kvs/ParallelCoordinatesRenderer>
#include <kvs/ParallelAxis>


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    const size_t nsamples = 100;
    const kvs::ValueArray<float> random_x = kvs::ValueArray<float>::Random( nsamples );
    const kvs::ValueArray<float> random_y = kvs::ValueArray<float>::Random( nsamples );
    const kvs::ValueArray<float> random_z = kvs::ValueArray<float>::Random( nsamples );
    const kvs::ValueArray<float> random_w = kvs::ValueArray<float>::Random( nsamples );

    kvs::TableObject* object = new kvs::TableObject();
    object->addColumn( kvs::AnyValueArray( random_x ), "Ramdom X" );
    object->addColumn( kvs::AnyValueArray( random_y ), "Random Y" );
    object->addColumn( kvs::AnyValueArray( random_z ), "Random Z" );
    object->addColumn( kvs::AnyValueArray( random_w ), "Random W" );

    kvs::ParallelCoordinatesRenderer* renderer = new kvs::ParallelCoordinatesRenderer();
    renderer->enableAntiAliasing();
    renderer->setLineWidth( 2.0f );
    renderer->setLineOpacity( 128 );
    renderer->setTopMargin( 40 );
    renderer->setBottomMargin( 50 );
    renderer->setLeftMargin( 50 );
    renderer->setRightMargin( 50 );

    kvs::ParallelAxis* axis = new kvs::ParallelAxis();
    axis->setAxisColor( kvs::RGBColor::Black() );
    axis->setTopMargin( 40 );
    axis->setBottomMargin( 50 );
    axis->setLeftMargin( 50 );
    axis->setRightMargin( 50 );

    screen.registerObject( object, renderer );
    screen.registerObject( object, axis );
    screen.show();

    return app.run();
}
