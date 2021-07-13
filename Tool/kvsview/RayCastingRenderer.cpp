/*****************************************************************************/
/**
 *  @file   RayCastingRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "RayCastingRenderer.h"
#include <kvs/DebugNew>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/RayCastingRenderer>
#include <kvs/Bounds>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/RendererManager>
#include <kvs/LineObject>
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TransferFunctionEditor>
#include <kvs/TargetChangeEvent>
#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/OrientationAxis>
#include "CommandName.h"
#include "FileChecker.h"


namespace
{

const std::string RendererName( "RayCastingRenderer" );

/*===========================================================================*/
/**
 *  @brief  Create a ray casting renderer.
 *  @param  arg [in] command line argument
 *  @param  tfunc [in] transfer function
 */
/*===========================================================================*/
kvs::VolumeRendererBase* CreateRenderer(
    const kvsview::RayCastingRenderer::Argument& arg,
    const kvs::TransferFunction& tfunc )
{
    kvs::VolumeRendererBase* renderer = nullptr;
    if ( arg.nogpu() ) { renderer = new kvs::RayCastingRenderer(); }
    else { renderer = new kvs::glsl::RayCastingRenderer(); }

    renderer->setName( RendererName );
    renderer->setTransferFunction( tfunc );

    if ( arg.nogpu() )
    {
        auto* r = kvs::RayCastingRenderer::DownCast( renderer );
        r->setSamplingStep( arg.step() );
        if ( !arg.nolod() ) { r->enableLODControl(); }
    }
    else
    {
        auto* r = kvs::glsl::RayCastingRenderer::DownCast( renderer );
        r->setSamplingStep( arg.step() );
        if ( arg.jittering() ) { r->enableJittering(); }
    }

    if ( arg.noshading() ) { renderer->disableShading(); }
    else { renderer->enableShading(); }

    // Shader type.
    const float ka = arg.ambient();
    const float kd = arg.diffuse();
    const float ks = arg.specular();
    const float n = arg.shininess();
    switch ( arg.shader() )
    {
    case 0:
    {
        auto shader = kvs::Shader::Lambert( ka, kd );
        renderer->setShader( shader );
        break;
    }
    case 1:
    {
        auto shader = kvs::Shader::Phong( ka, kd, ks, n );
        renderer->setShader( shader );
        break;
    }
    case 2:
    {
        auto shader = kvs::Shader::BlinnPhong( ka, kd, ks, n );
        renderer->setShader( shader );
        break;
    }
    default: break;
    }

    return renderer;
}

} // end of namespace


namespace kvsview
{

namespace RayCastingRenderer
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a point renderer.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "RayCastingRenderer")
{
    // Parameters for the RayCastingRenderer class.
    addOption( RayCastingRenderer::CommandName, RayCastingRenderer::Description, 0 );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
    addOption( "noshading", "Disable shading. (optional)", 0, false );
    addOption( "nolod", "Disable Level-of-Detail control. (optional)", 0, false );
    addOption( "nogpu", "Disable GPU rendering. (optional)", 0, false );
    addOption( "jittering", "Enable jittering. (optional)", 0, false );
    addOption( "step", "Sampling step. (default: 0.5)", 1, false );
    addOption( "ka", "Coefficient of the ambient color. (default: lambert=0.4, phong=0.3)", 1, false );
    addOption( "kd", "Coefficient of the diffuse color. (default: lambert=0.6, phong=0.5)", 1, false );
    addOption( "ks", "Coefficient of the specular color. (default: 0.8)", 1, false );
    addOption( "n", "Shininess. (default: 100.0)", 1, false );
    addOption( "shader", "Shader type. (default: 0)\n"
               "\t      0 = Lambert shading\n"
               "\t      1 = Phong shading\n"
               "\t      2 = Blinn-Phong shading", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a transfer function.
 *  @param  volume [in] pointer to the volume object
 *  @return transfer function
 */
/*===========================================================================*/
kvs::TransferFunction Argument::transferFunction(
    const kvs::VolumeObjectBase* volume ) const
{
    if ( this->hasOption("t") )
    {
        const auto filename = this->optionValue<std::string>("t");
        return kvs::TransferFunction( filename );
    }
    else if ( this->hasOption("T") )
    {
        const auto filename = this->optionValue<std::string>("T");
        kvs::TransferFunction tfunc( filename );
        tfunc.adjustRange( volume );
        return tfunc;
    }
    else
    {
        const size_t resolution = 256;
        return kvs::TransferFunction( resolution );
    }
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
int Main::exec()
{
    // Parse specified arguments.
    RayCastingRenderer::Argument arg( m_argc, m_argv );
    if ( !arg.parse() ) return ( false );

    // Viewer application.
    kvs::Application app( m_argc, m_argv );
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + RayCastingRenderer::CommandName );
    screen.create();

    // Check the input data.
    m_input_name = arg.value<std::string>();
    if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
            kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
    {
        kvsMessageError() << m_input_name << " is not volume data." << std::endl;;
        return ( false );
    }

    // Visualization pipeline.
    kvs::VisualizationPipeline pipe( m_input_name );
    pipe.import();

    const kvs::Indent indent(4);
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, indent );
    }

    // Create ray casting renderer module.
    const auto* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
    const auto tfunc = arg.transferFunction( volume );
    kvs::PipelineModule renderer_module( ::CreateRenderer( arg, tfunc ) );
    pipe.connect( renderer_module );

    // Execute the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return ( false );
    }

    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, indent );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, indent );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // The ray casting renderer should be registered after the axis and bounds are registered.
    auto* object = const_cast<kvs::ObjectBase*>( pipe.object() );
    auto* renderer = const_cast<kvs::RendererBase*>( pipe.renderer() );
    screen.registerObject( object, renderer );

    // Label (fps).
    kvs::Label label( &screen );
    label.setMargin( 10 );
    label.anchorToTopLeft();
    label.screenUpdated( [&] ()
    {
        const auto* renderer = screen.scene()->renderer( ::RendererName );
        const auto fps = kvs::String::From( renderer->timer().fps(), 4 );
        label.setText( std::string( "FPS: " + fps ).c_str() );
    } );
    label.show();

    // Colormap bar.
    kvs::ColorMapBar colormap_bar( &screen );
    colormap_bar.setWidth( 150 );
    colormap_bar.setHeight( 60 );
    colormap_bar.setColorMap( tfunc.colorMap() );
    if ( !tfunc.hasRange() )
    {
        const auto min_value = static_cast<kvs::Real32>( volume->minValue() );
        const auto max_value = static_cast<kvs::Real32>( volume->maxValue() );
        colormap_bar.setRange( min_value, max_value );
    }
    colormap_bar.anchorToBottomRight();
    colormap_bar.show();

    // Orientation axis.
    kvs::OrientationAxis orientation_axis( &screen, screen.scene() );
    orientation_axis.setMargin( 0 );
    orientation_axis.setSize( 100 );
    orientation_axis.setAxisLength( 3.2f );
    orientation_axis.setBoxTypeToSolid();
    orientation_axis.enableAntiAliasing();
    orientation_axis.anchorToBottomLeft();
    orientation_axis.show();

    // Create transfer function editor.
    const auto nogpu = arg.nogpu();
    kvs::TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( kvs::VolumeObjectBase::DownCast( pipe.object() ) );
    editor.setTransferFunction( tfunc );
    editor.apply( [&] ( kvs::TransferFunction t )
    {
        auto* r = screen.scene()->renderer( ::RendererName );
        if ( nogpu )
        {
            using Renderer = kvs::RayCastingRenderer;
            Renderer::DownCast(r)->setTransferFunction(t);
        }
        else
        {
            using Renderer = kvs::glsl::RayCastingRenderer;
            Renderer::DownCast(r)->setTransferFunction(t);
        }
        colormap_bar.setColorMap( t.colorMap() );
        screen.redraw();
    } );
    editor.show();

    // Double click event
    kvs::MouseDoubleClickEventListener mouse_double_click_event;
    mouse_double_click_event.update(
        [&]( kvs::MouseEvent* )
        {
            if ( editor.isVisible() ) { editor.hide(); }
            else { editor.show(); }
        } );
    screen.addEvent( &mouse_double_click_event );

    // Key press event
    kvs::KeyPressEventListener key_press_event;
    key_press_event.update( [&] ( kvs::KeyEvent* e )
    {
        switch ( e->key() )
        {
        case kvs::Key::t:
        {
            if ( editor.isVisible() ) { editor.hide(); }
            else { editor.show(); }
            break;
        }
        default: break;
        }
    } );
    screen.addEvent( &key_press_event );
    editor.addEvent( &key_press_event );

    // Target change event
    kvs::TargetChangeEvent target_change_event;
    screen.addEvent( &target_change_event );

    return ( arg.clear(), app.run() );
}

} // end of namespace RayCastingRenderer

} // end of namespace kvsview
