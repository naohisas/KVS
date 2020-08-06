/*****************************************************************************/
/**
 *  @file   Isosurface.cpp
 *  @author Naohisa Sakamoto
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
#include <kvs/Screen>
#include <kvs/Application>
#include <kvs/Slider>
#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/OrientationAxis>
#include <kvs/TargetChangeEvent>
#include "CommandName.h"
#include "FileChecker.h"


namespace kvsview
{

namespace Isosurface
{

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
    addOption( Isosurface::CommandName, Isosurface::Description, 0 );
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
    return valueAs<kvs::Real64>( "l", default_value );
}

/*===========================================================================*/
/**
 *  @brief  Returns a polygon normal vector type.
 *  @return normal vector type
 */
/*===========================================================================*/
const kvs::PolygonObject::NormalType Argument::normalType()
{
    auto normal_type = kvs::PolygonObject::PolygonNormal;
    if ( this->hasOption("n") )
    {
        const auto option_value = this->optionValue<std::string>("n");
        if ( option_value == "poly" || option_value == "polygon" )
        {
            normal_type = kvs::PolygonObject::PolygonNormal;
        }
        else if ( option_value == "vert" || option_value == "vertex" )
        {
            normal_type = kvs::PolygonObject::VertexNormal;
        }
        else
        {
            kvsMessageError() << "Unknown normal type " << option_value << "." << std::endl;
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
    // Parse specified arguments.
    Isosurface::Argument arg( argc, argv );
    if ( !arg.parse() ) return ( false );

    // Viewer application.
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + Isosurface::CommandName );
    screen.show();

    // Check the input point data.
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

    // Create isosurface module.
    const auto* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
    const auto tfunc = arg.transferFunction( volume );
    kvs::PipelineModule mapper( new kvs::Isosurface );
    const kvs::Real64 level = arg.isolevel( volume, tfunc );
    const kvs::PolygonObject::NormalType normal = arg.normalType();
    mapper.get<kvs::Isosurface>()->setIsolevel( level );
    mapper.get<kvs::Isosurface>()->setNormalType( normal );
    mapper.get<kvs::Isosurface>()->setTransferFunction( tfunc );
    pipe.connect( mapper );

    // Execute the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError() << "Cannot execute the visulization pipeline." << std::endl;
        return ( false );
    }

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
            const auto* r = screen.scene()->renderer();
            const auto f = kvs::String::From( r->timer().fps(), 4 );
            label.setText( std::string( "FPS: " + f ).c_str() );
        } );
    label.show();

    const auto min_value = static_cast<kvs::Real32>( volume->minValue() );
    const auto max_value = static_cast<kvs::Real32>( volume->maxValue() );

    // Colormap bar.
    kvs::ColorMapBar colormap_bar( &screen );
    colormap_bar.setWidth( 150 );
    colormap_bar.setHeight( 60 );
    colormap_bar.setColorMap( tfunc.colorMap() );
    if ( !tfunc.hasRange() )
    {
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

    // Slider.
    kvs::Slider slider( &screen );
    slider.setMargin( 10 );
    slider.setCaption("Isolevel");
    slider.setValue( static_cast<float>( level ) );
    slider.setRange( min_value, max_value );
    slider.valueChanged(
        [&]()
        {
            const double level = slider.value();
            const bool d = true;
            auto* object = new kvs::Isosurface( volume, level, normal, d, tfunc );
            if ( object ) { screen.scene()->objectManager()->change( 1, object ); }
        } );
    slider.anchorToTopRight();
    slider.show();

    // Target change event
    kvs::TargetChangeEvent target_change_event;
    screen.addEvent( &target_change_event );

    return ( arg.clear(), app.run() );
}

} // end of namespace Isosurface

} // end of namespace kvsview
