/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for transfer function editor.
 *  @author Naohisa Sakamoto
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

    void apply()
    {
        auto* scene = kvs::glut::Screen::DownCast( screen() )->scene();
        auto* renderer = kvs::glsl::RayCastingRenderer::DownCast( scene->rendererManager()->renderer() );
        renderer->setTransferFunction( transferFunction() );
        screen()->redraw();
    }
};

int main( int argc, char** argv )
{
    // Set an application and a screen.
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    // Read volume data.
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            object = new kvs::HydrogenVolumeData( kvs::Vec3u( 32, 32, 32 ) );

    // Set ray-casting renderer.
    auto* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setShader( kvs::Shader::BlinnPhong() );

    // Register the volume object and renderer.
    screen.registerObject( object, renderer );
    screen.show();

    // Transfer function editor.
    TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( object );
    editor.show();

    return( app.run() );
}
