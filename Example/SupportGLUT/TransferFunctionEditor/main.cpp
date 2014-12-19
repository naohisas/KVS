/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for transfer function editor.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1518 2013-04-10 01:13:11Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/RendererManager>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/TransferFunctionEditor>


class TransferFunctionEditor : public kvs::glut::TransferFunctionEditor
{
public:

    TransferFunctionEditor( kvs::glut::Screen* screen ):
        kvs::glut::TransferFunctionEditor( screen ){}

    void apply( void )
    {
        const kvs::RendererBase* base = static_cast<kvs::glut::Screen*>(screen())->scene()->rendererManager()->renderer();
        kvs::RayCastingRenderer* renderer = (kvs::RayCastingRenderer*)base;
        renderer->setTransferFunction( transferFunction() );
        screen()->redraw();
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            object = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );

    kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();
    renderer->setShader( kvs::Shader::BlinnPhong() );
    renderer->enableLODControl();

    kvs::glut::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.show();

    TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( object );
    editor.show();

    return( app.run() );
}
