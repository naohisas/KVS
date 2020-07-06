/*****************************************************************************/
/**
 *  @file   SlicePlane.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SlicePlane.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "SlicePlane.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/SlicePlane>
#include <kvs/Screen>
#include <kvs/Application>
#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/OrientationAxis>
#include <kvs/TargetChangeEvent>
#include "CommandName.h"
#include "FileChecker.h"


namespace kvsview
{

namespace SlicePlane
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "SlicePlane")
{
    // Parameters for the isosurface class.
    addOption( kvsview::SlicePlane::CommandName, kvsview::SlicePlane::Description, 0 );
    addOption( "p", "Position of any point in the plane. (optional: <x y z>)", 3, false );
    addOption( "n", "Normal vector. (optional: <nx ny nz>)", 3, false );
    addOption( "c", "Coefficients of the plane c1 x + c2 y + c3 z + c4 = 0. (optional: <c1 c2 c3 c4>)", 4, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
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
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int Main::exec( int argc, char** argv )
{
    // Parse specified arguments.
    kvsview::SlicePlane::Argument arg( argc, argv );
    if ( !arg.parse() ) return false;

    // Viewer application.
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + SlicePlane::CommandName );
    screen.show();

    // Check the input point data.
    m_input_name = arg.value<std::string>();
    if ( !( kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
            kvsview::FileChecker::ImportableUnstructuredVolume( m_input_name ) ) )
    {
        kvsMessageError() << m_input_name << " is not volume data." << std::endl;
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

    // Pointer to the volume object data.
    const auto* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
    const auto tfunc = arg.transferFunction( volume );

    // Set up the slice plane class.
    kvs::PipelineModule mapper( new kvs::SlicePlane );
    if ( arg.hasOption("c") )
    {
        const kvs::Vector4f coefficients( arg.coefficients() );
        mapper.get<kvs::SlicePlane>()->setPlane( coefficients );
    }
    else if ( arg.hasOption("p") && arg.hasOption("n") )
    {
        const kvs::Vector3f point( arg.point() );
        const kvs::Vector3f normal( arg.normal() );
        mapper.get<kvs::SlicePlane>()->setPlane( point, normal );
    }
    else
    {
        const kvs::Vector3f point( pipe.object()->objectCenter() );
        const kvs::Vector3f normal( 0.0f, 0.0f, 1.0f );
        mapper.get<kvs::SlicePlane>()->setPlane( point, normal );
    }

    mapper.get<kvs::SlicePlane>()->setTransferFunction( tfunc );
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
            const auto f = kvs::String::ToString( r->timer().fps(), 4 );
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

    // Target change event
    kvs::TargetChangeEvent target_change_event;
    screen.addEvent( &target_change_event );

    return ( arg.clear(), app.run() );
}

} // end of namespace SlicePlane

} // end of namespace kvsview
