/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBasedRenderer.cpp 1569 2013-05-08 02:48:35Z naohisa.sakamoto@gmail.com $
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
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/TransferFunctionEditor>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"


namespace { bool Shown = false; }
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
    const bool noshading = arg.noShading();
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
    /* const */ kvs::glut::Screen& screen,
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
 *  @brief  Transfer function editor class.
 */
/*===========================================================================*/
class TransferFunctionEditor : public kvs::glut::TransferFunctionEditor
{
    kvsview::ParticleBasedRenderer::Argument* m_arg; ///< pointer to the argument
    kvs::ColorMapBar* m_colormap_bar; ///< pointer to the colormap bar
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object

public:

    TransferFunctionEditor( kvs::glut::Screen* screen, kvsview::ParticleBasedRenderer::Argument* arg ):
        kvs::glut::TransferFunctionEditor( screen ),
        m_arg( arg ),
        m_colormap_bar( NULL ),
        m_volume( NULL ) {}

    void apply()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );

        // Current transfer function.
        kvs::TransferFunction tfunc( transferFunction() );

        // Set mapper (CellByCellXXXSampling).
        kvs::PointObject* object = NULL;
        switch ( m_arg->sampling() )
        {
        case 1: // Metropolis sampling
        {
            kvs::CellByCellMetropolisSampling* mapper = new kvs::CellByCellMetropolisSampling();
            kvsview::ParticleBasedRenderer::SetupMapper( *m_arg, tfunc, *glut_screen, mapper );
            object = mapper->exec( m_volume );
            break;
        }
        case 2: // Rejection sampling
        {
            kvs::CellByCellRejectionSampling* mapper = new kvs::CellByCellRejectionSampling();
            kvsview::ParticleBasedRenderer::SetupMapper( *m_arg, tfunc, *glut_screen, mapper );
            object = mapper->exec( m_volume );
            break;
        }
        case 3: // Layered sampling
        {
            kvs::CellByCellLayeredSampling* mapper = new kvs::CellByCellLayeredSampling();
            kvsview::ParticleBasedRenderer::SetupMapper( *m_arg, tfunc, *glut_screen, mapper );
            object = mapper->exec( m_volume );
            break;
        }
        default: // Uniform sampling
        {
            kvs::CellByCellUniformSampling* mapper = new kvs::CellByCellUniformSampling();
            kvsview::ParticleBasedRenderer::SetupMapper( *m_arg, tfunc, *glut_screen, mapper );
            object = mapper->exec( m_volume );

            break;
        }
        }

        object->setName( ::ObjectName );
        glut_screen->scene()->replaceObject( ::ObjectName, object );

        // Create new particle volume renderer.
        if ( m_arg->noGPU() )
        {
            kvs::ParticleBasedRenderer* renderer = new kvs::ParticleBasedRenderer();
            kvsview::ParticleBasedRenderer::SetupRenderer( *m_arg, renderer );

            // Subpixel level.
            const size_t repetition_level = m_arg->repetitionLevel();
            const size_t subpixel_level = ::GetSubpixelLevel( repetition_level );
            renderer->setSubpixelLevel( subpixel_level );

            glut_screen->scene()->replaceRenderer( ::RendererName, renderer );
        }
        else
        {
            kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();
            kvsview::ParticleBasedRenderer::SetupRenderer( *m_arg, renderer );

            // repetition level.
            const size_t repetition_level = m_arg->repetitionLevel();
            renderer->setRepetitionLevel( repetition_level );

            if ( !m_arg->noLOD() ) renderer->enableLODControl();

            glut_screen->scene()->replaceRenderer( ::RendererName, renderer );
        }

        m_colormap_bar->setColorMap( transferFunction().colorMap() );

        glut_screen->redraw();
    }

    void attachVolume( const kvs::VolumeObjectBase* volume )
    {
        m_volume = volume;
    }

    void attachColorMapBar( kvs::ColorMapBar* colormap_bar )
    {
        m_colormap_bar = colormap_bar;
    }
};

/*===========================================================================*/
/**
 *  @brief  Key press event.
 */
/*===========================================================================*/
class KeyPressEvent : public kvs::KeyPressEventListener
{
    TransferFunctionEditor* m_editor; ///!< pointer to the transfer function

public:
    void update( kvs::KeyEvent* event )
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        switch ( event->key() )
        {
        case kvs::Key::o: glut_screen->scene()->controlTarget() = kvs::Scene::TargetObject; break;
        case kvs::Key::l: glut_screen->scene()->controlTarget() = kvs::Scene::TargetLight; break;
        case kvs::Key::c: glut_screen->scene()->controlTarget() = kvs::Scene::TargetCamera; break;
        case kvs::Key::t:
        {
            if ( ::Shown ) m_editor->hide();
            else m_editor->show();

            ::Shown = !::Shown;
            break;
        }
        default: break;
        }
    }

    void attachTransferFunctionEditor( TransferFunctionEditor* editor )
    {
        m_editor = editor;
    }
};

/*===========================================================================*/
/**
 *  @brief  Mouse double-click event.
 */
/*===========================================================================*/
class MouseDoubleClickEvent : public kvs::MouseDoubleClickEventListener
{
    TransferFunctionEditor* m_editor; ///!< pointer to the transfer function

public:
    void update( kvs::MouseEvent* event )
    {
        if ( ::Shown ) m_editor->hide();
        else m_editor->show();

        ::Shown = !::Shown;
    }

    void attachTransferFunctionEditor( TransferFunctionEditor* editor )
    {
        m_editor = editor;
    }
};

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
    addOption( kvsview::ParticleBasedRenderer::CommandName, kvsview::ParticleBasedRenderer::Description, 0 );
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
 *  @brief  Returns the sampling method number.
 *  @return sampling method number
 */
/*===========================================================================*/
const int Argument::sampling() const
{
    const int default_value = 0;
    if ( this->hasOption("sampling") ) return this->optionValue<int>("sampling");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the shader number.
 *  @return shader number
 */
/*===========================================================================*/
const int Argument::shader() const
{
    const int default_value = 0;
    if ( this->hasOption("shader") ) return this->optionValue<int>("shader");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Check whether the "noshading" option is specified or not.
 *  @return true, if the "noshading" option is specified
 */
/*===========================================================================*/
const bool Argument::noShading() const
{
    return this->hasOption("noshading");
}

/*===========================================================================*/
/**
 *  @brief  Check whether the "nolod" option is specified or not.
 *  @return true, if the "nolod" option is specified
 */
/*===========================================================================*/
const bool Argument::noLOD() const
{
    return this->hasOption("nolod");
}

/*===========================================================================*/
/**
 *  @brief  Check whether the "nogpu" option is specified or not.
 *  @return true, if the "nogpu" option is specified
 */
/*===========================================================================*/
const bool Argument::noGPU() const
{
    return this->hasOption("nogpu");
}

//const bool Argument::noZooming() const
//{
//    return this->hasOption("nozooming");
//}

/*===========================================================================*/
/**
 *  @brief  Returns the coefficient for ambient color.
 *  @return coefficient for ambient color
 */
/*===========================================================================*/
const float Argument::ambient() const
{
    const float default_value = this->shader() == 0 ? 0.4f : 0.3f;
    if ( this->hasOption("ka") ) return this->optionValue<float>("ka");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the coefficient for diffuse color.
 *  @return coefficient for diffuse color
 */
/*===========================================================================*/
const float Argument::diffuse() const
{
    const float default_value = this->shader() == 0 ? 0.6f : 0.5f;
    if ( this->hasOption("kd") ) return this->optionValue<float>("kd");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the coefficient for specular color.
 *  @return coefficient for specular color
 */
/*===========================================================================*/
const float Argument::specular() const
{
    const float default_value = 0.8f;
    if ( this->hasOption("ks") ) return this->optionValue<float>("ks");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the coefficient for shininess.
 *  @return coefficient for shininess
 */
/*===========================================================================*/
const float Argument::shininess() const
{
    const float default_value = 100.0f;
    if ( this->hasOption("n") ) return this->optionValue<float>("n");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the repetition level.
 *  @return repetition level
 */
/*===========================================================================*/
const size_t Argument::repetitionLevel() const
{
    const size_t default_value = 1;
    if ( this->hasOption("r") ) return this->optionValue<size_t>("r");
    else return default_value;
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
        const std::string filename = this->optionValue<std::string>("t");
        return kvs::TransferFunction( filename );
    }
    else if ( this->hasOption("T") )
    {
        const std::string filename = this->optionValue<std::string>("T");
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
    // GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Parse specified arguments.
    kvsview::ParticleBasedRenderer::Argument arg( argc, argv );
    if ( !arg.parse() ) return false;

    // Events.
    kvsview::ParticleBasedRenderer::KeyPressEvent key_press_event;
    kvsview::ParticleBasedRenderer::MouseDoubleClickEvent mouse_double_click_event;

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.addEvent( &key_press_event );
    screen.addEvent( &mouse_double_click_event );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::ParticleBasedRenderer::CommandName );
    screen.show();

    // Check the input point or volume data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportablePoint( m_input_name ) )
    {
        if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
                kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
        {
            kvsMessageError("%s is not volume data.", m_input_name.c_str());
            return false;
        }
    }

    // Visualization pipeline.
    kvs::VisualizationPipeline pipe( m_input_name );
    pipe.import();

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, kvs::Indent(4) );
    }

    // Pointer to the volume object data.
    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );

    // Transfer function.
    const kvs::TransferFunction tfunc = arg.transferFunction( volume );

    // Label (fps).
    kvsview::Widget::FPSLabel label( &screen, ::RendererName );
    label.show();

    // Colormap bar.
    kvsview::Widget::ColorMapBar colormap_bar( &screen );
    colormap_bar.setColorMap( tfunc.colorMap() );
    if ( !tfunc.hasRange() )
    {
        const kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        const kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        colormap_bar.setRange( min_value, max_value );
    }
    colormap_bar.show();

    // Orientation axis.
    kvsview::Widget::OrientationAxis orientation_axis( &screen );
    orientation_axis.show();

    // For bounding box.
    if ( arg.hasOption("bounds") )
    {
        kvs::Bounds* bounds = new kvs::Bounds();
        kvs::LineObject* line = bounds->outputLineObject( pipe.object() );
        delete bounds;

        line->setColor( kvs::RGBColor( 0, 0, 0 ) );
        if ( arg.hasOption("bounds_color") )
        {
            const kvs::UInt8 r( static_cast<kvs::UInt8>(arg.optionValue<int>("bounds_color",0)) );
            const kvs::UInt8 g( static_cast<kvs::UInt8>(arg.optionValue<int>("bounds_color",1)) );
            const kvs::UInt8 b( static_cast<kvs::UInt8>(arg.optionValue<int>("bounds_color",2)) );
            line->setColor( kvs::RGBColor( r, g, b ) );
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
        kvs::CellByCellMetropolisSampling* pmapper = mapper.get<kvs::CellByCellMetropolisSampling>();
        kvsview::ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    case 2: // Rejection sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellRejectionSampling );
        kvs::CellByCellRejectionSampling* pmapper = mapper.get<kvs::CellByCellRejectionSampling>();
        kvsview::ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    case 3: // Layered sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellLayeredSampling );
        kvs::CellByCellLayeredSampling* pmapper = mapper.get<kvs::CellByCellLayeredSampling>();
        kvsview::ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    default: // Uniform sampling
    {
        kvs::PipelineModule mapper( new kvs::CellByCellUniformSampling );
        kvs::CellByCellUniformSampling* pmapper = mapper.get<kvs::CellByCellUniformSampling>();
        kvsview::ParticleBasedRenderer::SetupMapper( arg, tfunc, screen, pmapper );
        pipe.connect( mapper );
        break;
    }
    }

    // Set a renderer (ParticleBasedRenderer).
    if ( arg.noGPU() )
    {
        kvs::PipelineModule renderer( new kvs::ParticleBasedRenderer );
        kvs::ParticleBasedRenderer* prenderer = renderer.get<kvs::ParticleBasedRenderer>();
        kvsview::ParticleBasedRenderer::SetupRenderer( arg, prenderer );

        // Subpixel level.
        const size_t repetition_level = arg.repetitionLevel();
        const size_t subpixel_level = ::GetSubpixelLevel( repetition_level );
        prenderer->setSubpixelLevel( subpixel_level );

        pipe.connect( renderer );
    }
    else
    {
        kvs::PipelineModule renderer( new kvs::glsl::ParticleBasedRenderer );
        kvs::glsl::ParticleBasedRenderer* prenderer = renderer.get<kvs::glsl::ParticleBasedRenderer>();
        kvsview::ParticleBasedRenderer::SetupRenderer( arg, prenderer );

        // Repetition level.
        const size_t repetition_level = arg.repetitionLevel();
        prenderer->setRepetitionLevel( repetition_level );

        if ( !arg.noLOD() ) prenderer->enableLODControl();
        else prenderer->disableLODControl();

//        if ( !arg.noZooming() ) prenderer->enableZooming();
//        else prenderer->disableZooming();

        pipe.connect( renderer );
    }

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return false;
    }

    // Register object.
    const_cast<kvs::ObjectBase*>( pipe.object() )->setName( ::ObjectName );
    screen.registerObject( &pipe );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, kvs::Indent(4) );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // Create transfer function editor.
    kvsview::ParticleBasedRenderer::TransferFunctionEditor editor( &screen, &arg );
    editor.setVolumeObject( volume );
    editor.setTransferFunction( arg.transferFunction( volume ) );
    editor.attachVolume( volume );
    editor.attachColorMapBar( &colormap_bar );
    editor.show();
    editor.hide();
    ::Shown = false;

    // Attach the transfer function editor to the key press event and the mouse double-click event.
    mouse_double_click_event.attachTransferFunctionEditor( &editor );
    key_press_event.attachTransferFunctionEditor( &editor );

    return ( arg.clear(), app.run() );
}

} // end of namespace ParticleBasedRenderer

} // end of namespace kvsview
