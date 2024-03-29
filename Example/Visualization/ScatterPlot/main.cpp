/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::ScatterPlotRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TableObject>
#include <kvs/ScatterPlotRenderer>
#include <kvs/Axis2D>
#include <kvs/Axis2DInteractor>


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
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ScatterPlotRenderer" );

    const size_t nsamples = 100;
    const float min_value = 0.0f;
    const float max_value = 1.0f;
    const auto random_x = kvs::ValueArray<float>::Random( nsamples, min_value, max_value );
    const auto random_y = kvs::ValueArray<float>::Random( nsamples, min_value, max_value );

    auto* object = new kvs::TableObject();
    object->addColumn( random_x, min_value, max_value, "Random X" );
    object->addColumn( random_y, min_value, max_value, "Random Y" );

    const int left = 65;
    const int top = 35;
    const int right = 30;
    const int bottom = 50;
    const kvs::Margins margins( left, top, right, bottom );

    auto* renderer = new kvs::ScatterPlotRenderer();
    renderer->setMargins( margins );
    renderer->setPointSize( 5.0f );

    auto* axis = new kvs::Axis2D();
    axis->setTitle( "Title" );
    axis->setMargins( margins );
    axis->xAxis().setLabel( "X value" );
    axis->yAxis().setLabel( "Y value" );

    kvs::Axis2DInteractor axi_interactor( axis );
    screen.addEvent( &axi_interactor );

    screen.registerObject( object, axis );
    screen.registerObject( object, renderer );
    screen.create();

    return app.run();
}
