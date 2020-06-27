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
#include <kvs/PolygonObject>
#include <kvs/ExternalFaces>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/StochasticPolygonRenderer>
#include <kvs/StochasticTetrahedraRenderer>
#include <kvs/StochasticRenderingCompositor>
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
    screen.setTitle("Example program for kvs::StochasticRenderingCompositor");
    screen.create();

    auto* volume_object = new kvs::UnstructuredVolumeImporter( argv[1] );
    volume_object->print( std::cout );

    auto* volume_renderer = new kvs::StochasticTetrahedraRenderer();
    volume_renderer->setName("Renderer");

    auto* polygon_object = new kvs::ExternalFaces( volume_object );
    polygon_object->setName( "Polygon" );
    polygon_object->setColor( kvs::RGBColor::White() );
    polygon_object->setOpacity( 128 );
    polygon_object->print( std::cout << std::endl );

    auto* polygon_renderer = new kvs::StochasticPolygonRenderer();
    polygon_renderer->setPolygonOffset( 0.001f );

    screen.registerObject( polygon_object, polygon_renderer );
    screen.registerObject( volume_object, volume_renderer );

    kvs::StochasticRenderingCompositor compositor( screen.scene() );
    compositor.setRepetitionLevel( 50 );
    compositor.enableLODControl();
    screen.setEvent( &compositor );

    kvs::TransferFunctionEditor editor( &screen );
    editor.setPosition( screen.x() + screen.width(), screen.y() );
    editor.setVolumeObject( volume_object );
    editor.apply(
        [&]( kvs::TransferFunction tfunc ) {
            volume_renderer->setTransferFunction( tfunc );
            screen.redraw();
        } );
    editor.show();

    kvs::CheckBox checkbox( &screen );
    checkbox.setCaption( "Level-of-Detail" );
    checkbox.setMargin( 10 );
    checkbox.setState( true );
    checkbox.anchorToTopLeft();
    checkbox.stateChanged(
        [&]() {
            compositor.setEnabledLODControl( checkbox.state() );
            screen.redraw();
        } );
    checkbox.show();

    kvs::Slider opacity( &screen );
    opacity.setCaption( "Opacity" );
    opacity.setWidth( 150 );
    opacity.setMargin( 10 );
    opacity.setValue( 0.5 );
    opacity.setRange( 0, 1 );
    opacity.anchorToBottom( &checkbox );
    opacity.valueChanged(
        [&]() {
            auto* scene = screen.scene();
            auto* object1 = kvs::PolygonObject::DownCast( scene->object( "Polygon" ) );
            auto* object2 = new kvs::PolygonObject();
            object2->shallowCopy( *object1 );
            object2->setName( "Polygon" );
            object2->setOpacity( int( opacity.value() * 255 + 0.5 ) );
            scene->replaceObject( "Polygon", object2 );
        } );
    opacity.show();

    kvs::Slider repetition( &screen );
    repetition.setCaption( "Repetition" );
    repetition.setWidth( 150 );
    repetition.setMargin( 10 );
    repetition.setValue( 50 );
    repetition.setRange( 1, 100 );
    repetition.anchorToBottom( &opacity );
    repetition.valueChanged(
        [&]() {
            compositor.setRepetitionLevel( int( repetition.value() + 0.5 ) );
            screen.redraw();
        } );
    repetition.show();

    return app.run();
}
