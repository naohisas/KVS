/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::StochasticTetrahedraRenderer class.
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/TransferFunctionEditor>
#include <kvs/glut/Slider>
#include <kvs/glut/CheckBox>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/TransferFunction>
#include <kvs/StochasticTetrahedraRenderer>
#include <kvs/Scene>
#include <kvs/RendererManager>
#include <iostream>


/*===========================================================================*/
/**
 *  @brief  Transfer function editor.
 */
/*===========================================================================*/
class TransferFunctionEditor : public kvs::glut::TransferFunctionEditor
{
public:

    TransferFunctionEditor( kvs::glut::Screen* screen ):
        kvs::glut::TransferFunctionEditor( screen ){}

    void apply()
    {
        typedef kvs::StochasticTetrahedraRenderer Renderer;
        kvs::Scene* scene = static_cast<kvs::glut::Screen*>( screen() )->scene();
        Renderer* renderer = static_cast<Renderer*>( scene->rendererManager()->renderer( "Renderer" ) );
        renderer->setTransferFunction( transferFunction() );
        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  LOD check box.
 */
/*===========================================================================*/
class LODCheckBox : public kvs::glut::CheckBox
{
public:

    LODCheckBox( kvs::glut::Screen* screen ):
        kvs::glut::CheckBox( screen )
    {
        setMargin( 10 );
        setCaption( "Level-of-Detail" );
    }

    void stateChanged()
    {
        typedef kvs::StochasticTetrahedraRenderer Renderer;
        kvs::Scene* scene = static_cast<kvs::glut::Screen*>( screen() )->scene();
        Renderer* renderer = static_cast<Renderer*>( scene->rendererManager()->renderer( "Renderer" ) );
        renderer->setEnabledLODControl( state() );
        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  Repetition slider.
 */
/*===========================================================================*/
class RepetitionSlider : public kvs::glut::Slider
{
public:

    RepetitionSlider( kvs::glut::Screen* screen ):
        kvs::glut::Slider( screen )
    {
        setWidth( 150 );
        setMargin( 10 );
        setCaption( "Repetition" );
    }

    void valueChanged()
    {
        typedef kvs::StochasticTetrahedraRenderer Renderer;
        kvs::Scene* scene = static_cast<kvs::glut::Screen*>( screen() )->scene();
        Renderer* renderer = static_cast<Renderer*>( scene->rendererManager()->renderer( "Renderer" ) );
        renderer->setRepetitionLevel( int( value() + 0.5 ) );
        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    kvs::UnstructuredVolumeObject* object = new kvs::UnstructuredVolumeImporter( argv[1] );
    object->print( std::cout );

    kvs::TransferFunction tfunc( 256 );
    kvs::StochasticTetrahedraRenderer* renderer = new kvs::StochasticTetrahedraRenderer(); 
    renderer->setName( "Renderer" );
    renderer->setTransferFunction( tfunc );
    renderer->setRepetitionLevel( 50 );
    renderer->enableLODControl();

    kvs::glut::Screen screen( &app );
    screen.setTitle(" Example program for kvs::StochasticTetrahedraRenderer");
    screen.registerObject( object, renderer );
    screen.show();

    TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( object );
    editor.show();

    LODCheckBox checkbox( &screen );
    checkbox.setPosition( 0, 0 );
    checkbox.setState( true );
    checkbox.show();

    RepetitionSlider slider( &screen );
    slider.setPosition( checkbox.x(), checkbox.y() + checkbox.height() );
    slider.setValue( 50 );
    slider.setRange( 1, 100 );
    slider.show();

    return app.run();
}
