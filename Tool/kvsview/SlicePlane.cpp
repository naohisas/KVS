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
#include <kvs/glut/Screen>
#include <kvs/glut/Application>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"


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
 *  @brief  Returns coefficients of the plane.
 *  @return coefficients
 */
/*===========================================================================*/
const kvs::Vec4 Argument::coefficients()
{
    const kvs::Vec4 default_value( 0.0f, 0.0f, 0.0f, 0.0f );

    if ( this->hasOption("c") )
    {
        const kvs::Vec4 coefficients(
            this->optionValue<float>("c",0),
            this->optionValue<float>("c",1),
            this->optionValue<float>("c",2),
            this->optionValue<float>("c",3) );

        return coefficients;
    }
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns a position of any point in the plane.
 *  @return position of the point
 */
/*===========================================================================*/
const kvs::Vec3 Argument::point()
{
    const kvs::Vec3 default_value( 0.0f, 0.0f, 0.0f );

    if ( this->hasOption("p") )
    {
        const kvs::Vec3 point(
            this->optionValue<float>("p",0),
            this->optionValue<float>("p",1),
            this->optionValue<float>("p",2) );

        return point;
    }
    else return default_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns a normal vector.
 *  @return normal vector
 */
/*===========================================================================*/
const kvs::Vec3 Argument::normal()
{
    const kvs::Vec3 default_value( 0.0f, 0.0f, 0.0f );

    if ( this->hasOption("n") )
    {
        const kvs::Vec3 point(
            this->optionValue<float>("n",0),
            this->optionValue<float>("n",1),
            this->optionValue<float>("n",2) );

        return point;
    }
    else return default_value;
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
    // GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Parse specified arguments.
    kvsview::SlicePlane::Argument arg( argc, argv );
    if ( !arg.parse() ) return false;

    // Create a global and screen class.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::SlicePlane::CommandName );
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
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, kvs::Indent(4) );
        pipe.print( std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    return ( arg.clear(), app.run() );
}

} // end of namespace SlicePlane

} // end of namespace kvsview
