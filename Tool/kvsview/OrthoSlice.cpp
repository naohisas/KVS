/*****************************************************************************/
/**
 *  @file   OrthoSlice.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrthoSlice.cpp 1517 2013-04-09 11:55:49Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "OrthoSlice.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/OrthoSlice>
#include <kvs/ObjectManager>
#include <kvs/glut/Screen>
#include <kvs/glut/Application>
#include <kvs/Slider>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"


namespace kvsview
{

namespace OrthoSlice
{

/*===========================================================================*/
/**
 *  @brief  Plane position slider class.
 */
/*===========================================================================*/
class PlaneSlider : public kvs::Slider
{
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object
    kvs::TransferFunction m_tfunc; ///< transfer function
    kvs::OrthoSlice::AlignedAxis m_axis; ///< axis

public:
    PlaneSlider( kvs::glut::Screen* screen ):
        kvs::Slider( screen ),
        m_volume( NULL ),
        m_axis( kvs::OrthoSlice::ZAxis ) {}

    void setVolumeObject( const kvs::VolumeObjectBase* volume ) { m_volume = volume; }
    void setTransferFunction( const kvs::TransferFunction& tfunc ) { m_tfunc = tfunc; }
    void setAxis( const kvs::OrthoSlice::AlignedAxis axis ) { m_axis = axis; }

    void valueChanged()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        const float position = static_cast<float>( this->value() );
        kvs::PolygonObject* object = new kvs::OrthoSlice( m_volume, position, m_axis, m_tfunc );
        if ( object ) glut_screen->scene()->objectManager()->change( 1, object );
    }

    void screenResized()
    {
        setX( screen()->width() - width() - margin() );
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
    kvsview::Argument::Common( argc, argv, "OrthoSlice")
{
    // Parameters for the isosurface class.
    addOption( kvsview::OrthoSlice::CommandName, kvsview::OrthoSlice::Description, 0 );
    addOption( "p", "Position of the point on the spefied axis. (default: 0)", 1, false );
    addOption( "a", "Axis (x:0, y:1, z:2). (default: 2)", 1, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a position of any point in the plane.
 *  @return position of the point
 */
/*===========================================================================*/
const float Argument::position()
{
    const float default_value( 0.0f );

    if ( this->hasOption("p") )
    {
        const float position( this->optionValue<float>("p") );
        return position;
    }
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns a normal vector.
 *  @return normal vector
 */
/*===========================================================================*/
const kvs::OrthoSlice::AlignedAxis Argument::axis()
{
    const kvs::OrthoSlice::AlignedAxis default_value( kvs::OrthoSlice::ZAxis );

    if ( this->hasOption("a") )
    {
        const int axis( this->optionValue<int>("a") );
        switch ( axis )
        {
        case 0: return kvs::OrthoSlice::XAxis;
        case 1: return kvs::OrthoSlice::YAxis;
        case 2: return kvs::OrthoSlice::ZAxis;
        default:
            kvsMessageError("Cannot specify 0, 1, or 2 for the option 'a'.");
            break;
        }
    }

    return default_value;
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
    kvsview::OrthoSlice::Argument arg( argc, argv );
    if( !arg.parse() ) return( false );

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::OrthoSlice::CommandName );
    screen.show();

    // Check the input point data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) &&
         !kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) )
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

    // Set up the slice plane class.
    kvs::PipelineModule mapper( new kvs::OrthoSlice );
    const float position = arg.hasOption("p") ? arg.position() : pipe.object()->objectCenter().z();
    const kvs::OrthoSlice::AlignedAxis axis( arg.axis() );
    mapper.get<kvs::OrthoSlice>()->setPlane( position, axis );
    mapper.get<kvs::OrthoSlice>()->setTransferFunction( tfunc );
    pipe.connect( mapper );

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return false;
    }

    screen.registerObject( &pipe );

    // Disable shading.
    pipe.renderer()->disableShading();

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, kvs::Indent(4) );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // Create a plane slider.
    kvsview::OrthoSlice::PlaneSlider slider( &screen );
    slider.setMargin( 10 );
    slider.setCaption("Slice");
    slider.setVolumeObject( volume );
    slider.setTransferFunction( tfunc );
    slider.setAxis( axis );
    slider.setValue( position );
    slider.setRange( volume->minObjectCoord()[axis], volume->maxObjectCoord()[axis] );
    slider.show();

    return ( arg.clear(), app.run() );
}

} // end of namespace OrthoSlice

} // end of namespace kvsview
