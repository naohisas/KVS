/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::StochasticTetrahedraRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TransferFunctionEditor>
#include <kvs/Slider>
#include <kvs/CheckBox>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/TransferFunction>
#include <kvs/StochasticTetrahedraRenderer>
#include <kvs/Scene>
#include <kvs/RendererManager>
#include <iostream>


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
    screen.setTitle(" Example program for kvs::StochasticTetrahedraRenderer");
    screen.create();

    auto* object = new kvs::UnstructuredVolumeImporter( argv[1] );
    object->print( std::cout );

    auto tfunc = kvs::TransferFunction( 256 );
    auto* renderer = new kvs::StochasticTetrahedraRenderer();
    renderer->setName( "Renderer" );
    renderer->setTransferFunction( tfunc );
    renderer->setRepetitionLevel( 50 );
    renderer->setLODControlEnabled( true );

    screen.registerObject( object, renderer );

    kvs::TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( object );
    editor.apply( [&]( kvs::TransferFunction tfunc )
    {
        renderer->setTransferFunction( tfunc );
        screen.redraw();
    } );
    editor.show();

    kvs::CheckBox checkbox( &screen );
    checkbox.setMargin( 10 );
    checkbox.setCaption( "Level-of-Detail" );
    checkbox.setState( true );
    checkbox.anchorToTopLeft();
    checkbox.stateChanged( [&]()
    {
        renderer->setLODControlEnabled( checkbox.state() );
        screen.redraw();
    } );
    checkbox.show();

    kvs::Slider slider( &screen );
    slider.setWidth( 150 );
    slider.setMargin( 10 );
    slider.setCaption( "Repetition" );
    slider.setValue( 50 );
    slider.setRange( 1, 100 );
    slider.anchorToBottom( &checkbox );
    slider.valueChanged( [&]()
    {
        renderer->setRepetitionLevel( int( slider.value() + 0.5 ) );
        screen.redraw();
    } );
    slider.show();

    return app.run();
}
