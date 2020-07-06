/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ParticleBasedRenderer.h"
#include <kvs/DebugNew>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/CellByCellMetropolisSampling>
#include <kvs/CellByCellUniformSampling>
#include <kvs/CellByCellRejectionSampling>
#include <kvs/CellByCellLayeredSampling>
#include <kvs/ParticleBasedRenderer>
#include <kvs/Bounds>
#include <kvs/PaintEventListener>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Key>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/LineObject>
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TransferFunctionEditor>
#include <kvs/TargetChangeEvent>
#include <kvs/Label>
#include <kvs/OrientationAxis>
#include <kvs/ColorMapBar>
#include "CommandName.h"
#include "FileChecker.h"


namespace { const std::string ObjectName( "ParticleObject" ); }
namespace { const std::string RendererName( "ParticleRenderer" ); }


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns subpixel level calculated from repetition level.
 *  @param  repetition_level [in] repetition level
 *  @return subpixel level
 */
/*===========================================================================*/
inline const size_t GetSubpixelLevel( const size_t repetition_level )
{
    return( static_cast<size_t>( sqrtf( static_cast<float>( repetition_level ) ) + 0.5f ) );
}

} // end of namespace


namespace kvsview
{

namespace ParticleBasedRenderer
{

/*===========================================================================*/
/**
 *  @brief  Set up particle volume renderer.
 *  @param  arg [in] command line argument
 *  @param  tfunc [in] transfer function
 *  @param  renderer [in] renderer module
 */
/*===========================================================================*/
template <typename Renderer>
const void SetupRenderer(
    const kvsview::ParticleBasedRenderer::Argument& arg,
    Renderer* renderer )
{
    // Renderer name.
    renderer->setName( ::RendererName );

    // Shading on/off.
    const bool noshading = arg.noshading();
    if ( noshading ) renderer->disableShading();
    else renderer->enableShading();

    // Shader type.
    const float ka = arg.ambient();
    const float kd = arg.diffuse();
    const float ks = arg.specular();
    const float n = arg.shininess();
    const int shader = arg.shader();
    switch ( shader )
    {
    case 0: // Lambert shading
    {
        renderer->setShader( kvs::Shader::Lambert( ka, kd ) );
        break;
    }
    case 1: // Phong shading
    {
        renderer->setShader( kvs::Shader::Phong( ka, kd, ks, n ) );
        break;
    }
    case 2: // Blinn-phong shading
    {
        renderer->setShader( kvs::Shader::BlinnPhong( ka, kd, ks, n ) );
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Set up cell-by-cell particle sampler.
 *  @param  arg [in] command line argument
 *  @param  tfunc [in] transfer function
 *  @param  screen [in] screen
 *  @param  mapper [in] mapper module
 */
/*===========================================================================*/
template <typename Mapper>
const void SetupMapper(
    const kvsview::ParticleBasedRenderer::Argument& arg,
    const kvs::TransferFunction& tfunc,
    /* const */ kvs::Screen& screen,
    Mapper* mapper )
{
    const size_t repetition_level = arg.repetitionLevel();
    const float step = 0.5f;
    const float depth = 0.0f;
    mapper->attachCamera( screen.scene()->camera() );
    mapper->setTransferFunction( tfunc );
    mapper->setRepetitionLevel( repetition_level );
    mapper->setSamplingStep( step );
    mapper->setObjectDepth( depth );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "ParticleBasedRenderer")
{
    // Parameters for the ParticleBasedRenderer class.
    addOption( ParticleBasedRenderer::CommandName, ParticleBasedRenderer::Description, 0 );
    addOption( "r", "Repetition level. (default: 1)", 1, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
    addOption( "noshading", "Disable shading. (optional)", 0, false );
    addOption( "nolod", "Disable Level-of-Detail control. (optional)", 0, false );
    addOption( "nogpu", "Disable GPU rendering. (optional)", 0, false );
    addOption( "nozooming", "Disable particle resizing. (optional)", 0, false );
    addOption( "sampling", "Sampling type. (default: 0)\n"
               "\t      0 = Cell-by-cell uniform sampling\n"
               "\t      1 = Cell-by-cell metropolis sampling\n"
               "\t      2 = Cell-by-cell rejection sampling\n"
               "\t      3 = Cell-by-cell layered sampling", 1, false );
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
const kvs::TransferFunction Argument::transferFunction( const kvs::VolumeObjectBase* volume ) const
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
int Main::exec( int argc, char** argv )
{
    // Parse specified arguments.
    kvsview::ParticleBasedRenderer::Argument arg( argc, argv );
    if ( !arg.parse() ) return false;

    // Viewer application.
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + ParticleBasedRenderer::CommandName );
    screen.create();

    // Check the input point or volume data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportablePoint( m_input_name ) )
    {
        if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
                kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
        {
            kvsMessageError() << m_input_name << " is not volume data." << std::endl;
            return ( false );
        }
    }

    // Visualization pipeline.
    kvs::VisualizationPipeline pipe( m_input_name );

    // Import data.
    pipe.import();

    const kvs::Indent indent(4);
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, indent );
    }

    // Pointer to the volume object data.
    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
    const kvs::TransferFunction tfunc = arg.transferFunction( volume );

    // For bounding box.
    if ( arg.hasOption("bounds") )
    {
        auto* bounds = new kvs::Bounds();
        auto* line = bounds->outputLineObject( pipe.object() );
        delete bounds;

        if ( arg.hasOption("bounds_color") )
        {
            line->setColor( arg.valueAsRGBColor( "bounds_color" ) );
        }

        kvs::VisualizationPipeline p( line );
        p.exec();
        p.renderer()->disableShading();

        screen.registerObject( &p );
    }

    // Set a mapper (cell-by-cell sampling)
    switch ( arg.sampling() )
    {
    case 1: // Metropolis sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellMetropolisSampling );
        auto* pmapper = mapper.get<kvs::CellByCellMetropolisSampling>();
        ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    case 2: // Rejection sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellRejectionSampling );
        auto* pmapper = mapper.get<kvs::CellByCellRejectionSampling>();
        ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    case 3: // Layered sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellLayeredSampling );
        auto* pmapper = mapper.get<kvs::CellByCellLayeredSampling>();
        ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    default: // Uniform sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellUniformSampling );
        auto* pmapper = mapper.get<kvs::CellByCellUniformSampling>();
        ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    }

    // Set a renderer (ParticleBasedRenderer).
    if ( arg.nogpu() )
    {
        kvs::PipelineModule renderer( new kvs::ParticleBasedRenderer );
        auto* prenderer = renderer.get<kvs::ParticleBasedRenderer>();
        ParticleBasedRenderer::SetupRenderer( arg, prenderer );

        // Subpixel level.
        const size_t repetition_level = arg.repetitionLevel();
        const size_t subpixel_level = ::GetSubpixelLevel( repetition_level );
        prenderer->setSubpixelLevel( subpixel_level );

        pipe.connect( renderer );
    }
    else
    {
        kvs::PipelineModule renderer( new kvs::glsl::ParticleBasedRenderer );
        auto* prenderer = renderer.get<kvs::glsl::ParticleBasedRenderer>();
        ParticleBasedRenderer::SetupRenderer( arg, prenderer );

        // Repetition level.
        const size_t repetition_level = arg.repetitionLevel();
        prenderer->setRepetitionLevel( repetition_level );

        if ( !arg.nolod() ) prenderer->enableLODControl();
        else prenderer->disableLODControl();

        pipe.connect( renderer );
    }

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError() << "Cannot execute the visulization pipeline." << std::endl;
        return false;
    }

    // Register object.
    const_cast<kvs::ObjectBase*>( pipe.object() )->setName( ::ObjectName );
    screen.registerObject( &pipe );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, indent );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, indent );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // Label (fps).
    kvs::Label label( &screen );
    label.setMargin( 10 );
    label.anchorToTopLeft();
    label.screenUpdated(
        [&]()
        {
            const auto* renderer = screen.scene()->renderer( ::RendererName );
            const auto fps = kvs::String::ToString( renderer->timer().fps(), 4 );
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
    kvs::TransferFunctionEditor editor( &screen );
    editor.setTransferFunction( tfunc );
    editor.apply(
        [&]( kvs::TransferFunction t )
        {
            // Set mapper (CellByCellXXXSampling).
            kvs::PointObject* object = nullptr;
            switch ( arg.sampling() )
            {
            case 1: // Metropolis sampling
            {
                auto* mapper = new kvs::CellByCellMetropolisSampling();
                ParticleBasedRenderer::SetupMapper( arg, t, screen, mapper );
                object = mapper->exec( volume );
                break;
            }
            case 2: // Rejection sampling
            {
                auto* mapper = new kvs::CellByCellRejectionSampling();
                ParticleBasedRenderer::SetupMapper( arg, t, screen, mapper );
                object = mapper->exec( volume );
                break;
            }
            case 3: // Layered sampling
            {
                auto* mapper = new kvs::CellByCellLayeredSampling();
                ParticleBasedRenderer::SetupMapper( arg, t, screen, mapper );
                object = mapper->exec( volume );
                break;
            }
            default: // Uniform sampling
            {
                auto* mapper = new kvs::CellByCellUniformSampling();
                ParticleBasedRenderer::SetupMapper( arg, t, screen, mapper );
                object = mapper->exec( volume );
                break;
            }
            }

            object->setName( ::ObjectName );
            screen.scene()->replaceObject( ::ObjectName, object );

            // Create new particle volume renderer.
            if ( arg.nogpu() )
            {
                auto* renderer = new kvs::ParticleBasedRenderer();
                ParticleBasedRenderer::SetupRenderer( arg, renderer );

                // Subpixel level.
                const size_t repetition_level = arg.repetitionLevel();
                const size_t subpixel_level = ::GetSubpixelLevel( repetition_level );
                renderer->setSubpixelLevel( subpixel_level );

                screen.scene()->replaceRenderer( ::RendererName, renderer );
            }
            else
            {
                auto* renderer = new kvs::glsl::ParticleBasedRenderer();
                ParticleBasedRenderer::SetupRenderer( arg, renderer );

                // repetition level.
                const size_t repetition_level = arg.repetitionLevel();
                renderer->setRepetitionLevel( repetition_level );
                if ( !arg.nolod() ) { renderer->enableLODControl(); }

                screen.scene()->replaceRenderer( ::RendererName, renderer );
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
    key_press_event.update(
        [&]( kvs::KeyEvent* e )
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

} // end of namespace ParticleBasedRenderer

} // end of namespace kvsview
