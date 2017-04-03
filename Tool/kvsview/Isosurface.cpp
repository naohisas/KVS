/*****************************************************************************/
/**
 *  @file   Isosurface.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Isosurface.cpp 1517 2013-04-09 11:55:49Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Isosurface.h"
#include <kvs/DebugNew>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/Isosurface>
#include <kvs/Value>
#include <kvs/KeyPressEventListener>
#include <kvs/Key>
#include <kvs/ObjectManager>
#include <kvs/glut/Screen>
#include <kvs/glut/Application>
#include <kvs/Slider>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"


namespace kvsview
{

namespace Isosurface
{

/*===========================================================================*/
/**
 *  @brief  Isolevel slider class.
 */
/*===========================================================================*/
class IsolevelSlider : public kvs::Slider
{
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object
    kvs::TransferFunction m_tfunc; ///< transfer function
    kvs::PolygonObject::NormalType m_normal; ///< normal vector type

public:

    IsolevelSlider( kvs::glut::Screen* screen ):
        kvs::Slider( screen ),
        m_volume( NULL ),
        m_normal( kvs::PolygonObject::PolygonNormal ) {}

    void setVolumeObject( const kvs::VolumeObjectBase* volume ) { m_volume = volume; }
    void setTransferFunction( const kvs::TransferFunction& tfunc ) { m_tfunc = tfunc; }
    void setNormal( const kvs::PolygonObject::NormalType normal ) { m_normal = normal; }

    void valueChanged()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        const double level = this->value();
        const bool d = true;
        kvs::PolygonObject* object = new kvs::Isosurface( m_volume, level, m_normal, d, m_tfunc );
        if ( object ) glut_screen->scene()->objectManager()->change( 1, object );
    }

    void screenResized()
    {
        setX( screen()->width() - width() - margin() );
    }
};

/*===========================================================================*/
/**
 *  @brief  Key press event.
 */
/*===========================================================================*/
class KeyPressEvent : public kvs::KeyPressEventListener
{
    void update( kvs::KeyEvent* event )
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        switch ( event->key() )
        {
        case kvs::Key::o: glut_screen->scene()->controlTarget() = kvs::Scene::TargetObject; break;
        case kvs::Key::l: glut_screen->scene()->controlTarget() = kvs::Scene::TargetLight; break;
        case kvs::Key::c: glut_screen->scene()->controlTarget() = kvs::Scene::TargetCamera; break;
        default: break;
        }
    }
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "Isosurface")
{
    // Parameters for the isosurface class.
    addOption( kvsview::Isosurface::CommandName, kvsview::Isosurface::Description, 0 );
    addOption( "l", "Isosurface level. (default: mean value)", 1, false );
    addOption( "n", "Normal vector type; 'poly[gon]' 'vert[ex]'. (default: poly)", 1, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a isolevel.
 *  @param  volume [in] pointer to the volume object
 *  @param  transfer_function [in] transfer function
 *  @return isolevel level
 */
/*===========================================================================*/
const kvs::Real64 Argument::isolevel(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function )
{
    if ( !volume->hasMinMaxValues() ) volume->updateMinMaxValues();
    double min_value = volume->minValue();
    double max_value = volume->maxValue();
    if ( transfer_function.hasRange() )
    {
        min_value = transfer_function.minValue();
        max_value = transfer_function.maxValue();
    }
    const kvs::Real64 default_value = ( max_value + min_value ) * 0.5;

    if ( this->hasOption("l") ) return this->optionValue<kvs::Real64>("l");
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns a polygon normal vector type.
 *  @return normal vector type
 */
/*===========================================================================*/
const kvs::PolygonObject::NormalType Argument::normalType( void )
{
    const kvs::PolygonObject::NormalType default_value = kvs::PolygonObject::PolygonNormal;

    kvs::PolygonObject::NormalType normal_type = default_value;
    if ( this->hasOption("n") )
    {
        const std::string option_value = this->optionValue<std::string>("n");
        if ( option_value == "poly" ||
             option_value == "polygon" )
        {
            normal_type = kvs::PolygonObject::PolygonNormal;
        }
        else if ( option_value == "vert" ||
                  option_value == "vertex" )
        {
            normal_type = kvs::PolygonObject::VertexNormal;
        }
        else
        {
            kvsMessageError("Unknown normal type '%s'.",option_value.c_str());
        }
    }

    return normal_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a transfer function.
 *  @param  volume [in] pointer to the volume object
 *  @return transfer function
 */
/*===========================================================================*/
const kvs::TransferFunction Argument::transferFunction( const kvs::VolumeObjectBase* volume )
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
    Isosurface::Argument arg( argc, argv );
    if ( !arg.parse() ) return false;

    // Event.
    kvsview::Isosurface::KeyPressEvent key_press_event;

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.addEvent( &key_press_event );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::Isosurface::CommandName );
    screen.show();

    // Check the input point data.
    m_input_name = arg.value<std::string>();
    if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
            kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
    {
        kvsMessageError("%s is not volume data.", m_input_name.c_str());
        return false;
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

    // Colormap bar.
    kvsview::Widget::ColorMapBar colormap_bar( &screen );
    colormap_bar.setColorMap( tfunc.colorMap() );
    if ( !tfunc.hasRange() )
    {
        const kvs::VolumeObjectBase* object = kvs::VolumeObjectBase::DownCast( pipe.object() );
        const kvs::Real32 min_value = static_cast<kvs::Real32>( object->minValue() );
        const kvs::Real32 max_value = static_cast<kvs::Real32>( object->maxValue() );
        colormap_bar.setRange( min_value, max_value );
    }
    colormap_bar.show();

    // Orientation axis.
    kvsview::Widget::OrientationAxis orientation_axis( &screen );
    orientation_axis.show();

    // Set up isosurface class.
    kvs::PipelineModule mapper( new kvs::Isosurface );
    const kvs::Real64 level = arg.isolevel( volume, tfunc );
    const kvs::PolygonObject::NormalType normal = arg.normalType();
    mapper.get<kvs::Isosurface>()->setIsolevel( level );
    mapper.get<kvs::Isosurface>()->setNormalType( normal );
    mapper.get<kvs::Isosurface>()->setTransferFunction( tfunc );
    pipe.connect( mapper );

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return false;
    }

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

    // Slider.
    kvsview::Isosurface::IsolevelSlider slider( &screen );
    slider.setMargin( 10 );
    slider.setCaption("Isolevel");
    slider.setVolumeObject( volume );
    slider.setTransferFunction( tfunc );
    slider.setNormal( normal );
    slider.setValue( static_cast<float>( level ) );
    slider.setRange( static_cast<float>( colormap_bar.minValue() ), static_cast<float>( colormap_bar.maxValue() ) );
    slider.show();

    return ( arg.clear(), app.run() );
}

} // end of namespace Isosurface

} // end of namespace kvsview
