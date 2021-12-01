/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::ParallelCoordinatesRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TableObject>
#include <kvs/ParallelCoordinatesRenderer>
#include <kvs/ParallelAxis>
#include <kvs/ParallelAxisInteractor>

#include <kvs/Label>


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
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setSize( 800, 300 );

    const size_t nsamples = 100;
    const auto random_x = kvs::ValueArray<float>::Random( nsamples );
    const auto random_y = kvs::ValueArray<float>::Random( nsamples );
    const auto random_z = kvs::ValueArray<float>::Random( nsamples );
    const auto random_w = kvs::ValueArray<float>::Random( nsamples );

    const int left = 50;
    const int top = 60;
    const int right = 50;
    const int bottom = 50;
    const kvs::Margins margins( left, top, right, bottom );

    auto* object = new kvs::TableObject();
    object->addColumn( kvs::AnyValueArray( random_x ), "Ramdom X" );
    object->addColumn( kvs::AnyValueArray( random_y ), "Random Y" );
    object->addColumn( kvs::AnyValueArray( random_z ), "Random Z" );
    object->addColumn( kvs::AnyValueArray( random_w ), "Random W" );

    auto* renderer = new kvs::ParallelCoordinatesRenderer();
    renderer->enableAntiAliasing();
    renderer->setLineWidth( 2.0f );
    renderer->setLineOpacity( 128 );
    renderer->setMargins( margins );

    auto* axis = new kvs::ParallelAxis();
    axis->setTitle( "Sample Data" );
    axis->setTitleOffset( 30 );
    axis->setMargins( margins );

    kvs::ParallelAxisInteractor axis_interactor( axis );
    screen.addEvent( &axis_interactor );

    screen.registerObject( object, axis );
    screen.registerObject( object, renderer );
    screen.create();

    kvs::Label fps( &screen );
    fps.setMargin( 10 );
    fps.screenUpdated( [&] ()
    {
        fps.setText( "%.3f fps", renderer->timer().fps() );
    } );
    fps.show();

    return app.run();
}
