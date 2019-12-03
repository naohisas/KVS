/*****************************************************************************/
/**
 *  @file   RayCastingRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RayCastingRenderer.cpp 1517 2013-04-09 11:55:49Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "RayCastingRenderer.h"
#include <kvs/DebugNew>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/AVSField>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/RayCastingRenderer>
#include <kvs/Bounds>
#include <kvs/PaintEventListener>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Key>
#include <kvs/RendererManager>
#include <kvs/LineObject>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/TransferFunctionEditor>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"

namespace { bool Shown = false; }
namespace { const std::string RendererName( "RayCastingRenderer" ); }


namespace kvsview
{

namespace RayCastingRenderer
{

/*===========================================================================*/
/**
 *  @brief  Set shader and transfer function to ray casting renderer.
 *  @param  arg [in] command line argument
 *  @param  tfunc [in] transfer function
 *  @param  renderer [in] renderer module
 */
/*===========================================================================*/
template <typename Renderer>
const void SetupRenderer(
    const kvsview::RayCastingRenderer::Argument& arg,
    const kvs::TransferFunction& tfunc,
    kvs::PipelineModule& renderer )
{
    // Renderer name.
    renderer.template get<Renderer>()->setName( ::RendererName );

    // Sampling step.
    const float step = arg.step();
    renderer.template get<Renderer>()->setSamplingStep( step );

    // Transfer function.
    renderer.template get<Renderer>()->setTransferFunction( tfunc );

    // Shading on/off.
    const bool noshading = arg.noShading();
    if ( noshading ) renderer.template get<Renderer>()->disableShading();
    else renderer.template get<Renderer>()->enableShading();

    // Shader type.
    const float ka = arg.ambient();
    const float kd = arg.diffuse();
    const float ks = arg.specular();
    const float n = arg.shininess();
    const int shader = arg.shader();
    switch ( shader )
    {
    case 0: // Lamber shading
    {
        renderer.template get<Renderer>()->setShader( kvs::Shader::Lambert( ka, kd ) );
        break;
    }
    case 1: // Phong shading
    {
        renderer.template get<Renderer>()->setShader( kvs::Shader::Phong( ka, kd, ks, n ) );
        break;
    }
    case 2: // Blinn-phong shading
    {
        renderer.template get<Renderer>()->setShader( kvs::Shader::BlinnPhong( ka, kd, ks, n ) );
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Transfer function editor class.
 */
/*===========================================================================*/
class TransferFunctionEditor : public kvs::glut::TransferFunctionEditor
{
    bool m_no_gpu; ///!< flag to check if the GPU shader is enabled
    kvs::ColorMapBar* m_colormap_bar; ///!< pointer to the colormap bar

public:

    TransferFunctionEditor( kvs::glut::Screen* screen, const bool no_gpu ):
        kvs::glut::TransferFunctionEditor( screen ),
        m_no_gpu( no_gpu ) {}

    void apply()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        const kvs::RendererBase* base = glut_screen->scene()->rendererManager()->renderer( ::RendererName );
        if ( m_no_gpu )
        {
            kvs::RayCastingRenderer* renderer = (kvs::RayCastingRenderer*)base;
            renderer->setTransferFunction( transferFunction() );
        }
        else
        {
            kvs::glsl::RayCastingRenderer* renderer = (kvs::glsl::RayCastingRenderer*)base;
            renderer->setTransferFunction( transferFunction() );
        }

        m_colormap_bar->setColorMap( transferFunction().colorMap() );

        screen()->redraw();
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
 *  @brief  Constructs a new Argument class for a point renderer.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "RayCastingRenderer")
{
    // Parameters for the RayCastingRenderer class.
    addOption( kvsview::RayCastingRenderer::CommandName, kvsview::RayCastingRenderer::Description, 0 );
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

const bool Argument::jittering() const
{
    return this->hasOption("jittering");
}

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
 *  @brief  Returns sampling step.
 *  @return sampling step
 */
/*===========================================================================*/
const float Argument::step() const
{
    const float default_value = 0.5f;
    if ( this->hasOption("step") ) return this->optionValue<float>("step");
    else return default_value;
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
    kvsview::RayCastingRenderer::Argument arg( argc, argv );
    if( !arg.parse() ) return( false );

    // Events.
    kvsview::RayCastingRenderer::KeyPressEvent key_press_event;
    kvsview::RayCastingRenderer::MouseDoubleClickEvent mouse_double_click_event;

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.addEvent( &key_press_event );
    screen.addEvent( &mouse_double_click_event );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::RayCastingRenderer::CommandName );
    screen.show();

    // Check the input point data.
    m_input_name = arg.value<std::string>();
    if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
            kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
    {
        kvsMessageError("%s is not volume data.", m_input_name.c_str());
        return( false );
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

    // Bounding box.
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

    // Set up a RayCastingRenderer class.
    if ( arg.noGPU() )
    {
        kvs::PipelineModule renderer( new kvs::RayCastingRenderer );
        kvsview::RayCastingRenderer::SetupRenderer<kvs::RayCastingRenderer>( arg, tfunc, renderer );

        if ( !arg.noLOD() )
        {
            renderer.get<kvs::RayCastingRenderer>()->enableLODControl();
        }

        pipe.connect( renderer );
    }
    else
    {
        kvs::PipelineModule renderer( new kvs::glsl::RayCastingRenderer );
        kvsview::RayCastingRenderer::SetupRenderer<kvs::glsl::RayCastingRenderer>( arg, tfunc, renderer );

        if ( arg.jittering() )
        {
            renderer.get<kvs::glsl::RayCastingRenderer>()->enableJittering();
        }
        pipe.connect( renderer );
    }

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return( false );
    }

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, kvs::Indent(4) );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // The ray casting renderer should be registered after the axis and bounds are registered.
    screen.registerObject( const_cast<kvs::ObjectBase*>(pipe.object()), const_cast<kvs::RendererBase*>(pipe.renderer()) );

    // Create transfer function editor.
    kvsview::RayCastingRenderer::TransferFunctionEditor editor( &screen, arg.noGPU() );
    editor.setVolumeObject( kvs::VolumeObjectBase::DownCast( pipe.object() ) );
    editor.setTransferFunction( tfunc );
    editor.attachColorMapBar( &colormap_bar );
    editor.show();
    editor.hide();
    ::Shown = false;

    // Attach the transfer function editor to the key press event and the mouse double-click event.
    mouse_double_click_event.attachTransferFunctionEditor( &editor );
    key_press_event.attachTransferFunctionEditor( &editor );

    return ( arg.clear(), app.run() );
}

} // end of namespace RayCastingRenderer

} // end of namespace kvsview
