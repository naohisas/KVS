/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::StochasticPolygonRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/CheckBox>
#include <kvs/Slider>
#include <kvs/PolygonObject>
#include <kvs/PolygonImporter>
#include <kvs/StructuredVolumeObject>
#include <kvs/HydrogenVolumeData>
#include <kvs/Isosurface>
#include <kvs/StochasticPolygonRenderer>
#include <kvs/Scene>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <iostream>


/*===========================================================================*/
/**
 *  @brief  LOD check box.
 */
/*===========================================================================*/
class LODCheckBox : public kvs::CheckBox
{
public:

    LODCheckBox( kvs::Screen* screen ):
        kvs::CheckBox( screen )
    {
        setMargin( 10 );
        setCaption( "Level-of-Detail" );
    }

    void stateChanged()
    {
        typedef kvs::StochasticPolygonRenderer Renderer;
        kvs::Scene* scene = kvs::Screen::DownCast( screen() )->scene();
        Renderer* renderer = static_cast<Renderer*>( scene->rendererManager()->renderer( "Renderer" ) );
        renderer->setEnabledLODControl( state() );
        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  Opacity slider.
 */
/*===========================================================================*/
class OpacitySlider : public kvs::Slider
{
public:

    OpacitySlider( kvs::Screen* screen ):
        kvs::Slider( screen )
    {
        setWidth( 150 );
        setMargin( 10 );
        setCaption( "Opacity" );
    }

    void valueChanged()
    {
        typedef kvs::PolygonObject Object;
        kvs::Scene* scene = kvs::Screen::DownCast( screen() )->scene();
        Object* object1 = Object::DownCast( scene->objectManager()->object( "Polygon" ) );
        Object* object2 = new Object();
        object2->shallowCopy( *object1 );
        object2->setName( "Polygon" );
        object2->setOpacity( int( value() * 255 + 0.5 ) );
        scene->objectManager()->change( "Polygon", object2 );
    }
};

/*===========================================================================*/
/**
 *  @brief  Repetition slider.
 */
/*===========================================================================*/
class RepetitionSlider : public kvs::Slider
{
public:

    RepetitionSlider( kvs::Screen* screen ):
        kvs::Slider( screen )
    {
        setWidth( 150 );
        setMargin( 10 );
        setCaption( "Repetition" );
    }

    void valueChanged()
    {
        typedef kvs::StochasticPolygonRenderer Renderer;
        kvs::Scene* scene = kvs::Screen::DownCast( screen() )->scene();
        Renderer* renderer = static_cast<Renderer*>( scene->rendererManager()->renderer( "Renderer" ) );
        renderer->setRepetitionLevel( int( value() + 0.5 ) );
        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  Import polygon object
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return pointer to the imported polygon object
 */
/*===========================================================================*/
kvs::PolygonObject* Import( int argc, char** argv )
{
    kvs::PolygonObject* object = NULL;
    if ( argc > 1 )
    {
        object = new kvs::PolygonImporter( argv[1] );
    }
    else
    {
        /* If the data file isn't specified, a hydrogen volume data is generated by
         * using kvs::HydrogenVolumeData class and then an isosurface polygon object
         * is extracted from the volume data.
         */
        kvs::Vector3ui resolution( 64, 64, 64 );
        kvs::StructuredVolumeObject* volume = new kvs::HydrogenVolumeData( resolution );
        volume->updateMinMaxValues();

        const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
        const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
        const bool d = false;
        const kvs::TransferFunction t( 256 );
        object = new kvs::Isosurface( volume, i, n, d, t );

        delete volume;
    }

    return object;
}

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

    kvs::PolygonObject* object = Import( argc, argv );
    object->setName( "Polygon" );
    object->setOpacity( 128 );
    object->print( std::cout );

    kvs::StochasticPolygonRenderer* renderer = new kvs::StochasticPolygonRenderer();
    renderer->setName( "Renderer" );
    renderer->setRepetitionLevel( 50 );
    renderer->enableLODControl();

    kvs::Screen screen( &app );
    screen.setTitle("Example program for kvs::StochasticPolygonRenderer");
    screen.registerObject( object, renderer );
    screen.create();

    LODCheckBox checkbox( &screen );
    checkbox.setPosition( 0, 0 );
    checkbox.setState( true );
    checkbox.show();

    OpacitySlider opacity( &screen );
    opacity.setPosition( checkbox.x(), checkbox.y() + checkbox.height() );
    opacity.setValue( 0.5 );
    opacity.setRange( 0, 1 );
    opacity.show();

    RepetitionSlider repetition( &screen );
    repetition.setPosition( opacity.x(), opacity.y() + opacity.height() );
    repetition.setValue( 50 );
    repetition.setRange( 1, 100 );
    repetition.show();

    return app.run();
}
