/*****************************************************************************/
/**
 *  @file   ExtractVertices.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractVertices.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "ExtractVertices.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/ExtractVertices>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include "CommandName.h"
#include "FileChecker.h"
#include "Widget.h"


namespace kvsview
{

namespace ExtractVertices
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "ExtractVertices")
{
    // Parameters for the isosurface class.
    addOption( kvsview::ExtractVertices::CommandName, kvsview::ExtractVertices::Description, 0 );
    addOption( "s", "Point size. (default: 1)", 1, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a point size.
 *  @return point size
 */
/*===========================================================================*/
const kvs::Real32 Argument::size()
{
    const kvs::Real32 default_value = 0.0;
    if ( this->hasOption("s") ) return( this->optionValue<kvs::Real32>("s") );
    else return( default_value );
}

/*===========================================================================*/
/**
 *  @brief  Returns a transfer function.
 *  @param  volume [in] pointer to the volume data
 *  @return transfer function
 */
/*===========================================================================*/
const kvs::TransferFunction Argument::transferFunction( const kvs::VolumeObjectBase* volume )
{
    if ( this->hasOption("t") )
    {
        const std::string filename = this->optionValue<std::string>("t");
        return( kvs::TransferFunction( filename ) );
    }
    else if ( this->hasOption("T") )
    {
        const std::string filename = this->optionValue<std::string>("T");
        kvs::TransferFunction tfunc( filename );
        tfunc.adjustRange( volume );
        return( tfunc );
    }
    else
    {
        const size_t resolution = 256;
        return( kvs::TransferFunction( resolution ) );
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
    kvsview::ExtractVertices::Argument arg( argc, argv );
    if( !arg.parse() ) return false;

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::ExtractVertices::CommandName );
    screen.show();

    // Check the input volume data.
    m_input_name = arg.value<std::string>();
    if ( !(kvsview::FileChecker::ImportableStructuredVolume( m_input_name ) ||
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
        const kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        const kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        colormap_bar.setRange( min_value, max_value );
    }
    colormap_bar.show();

    // Orientation axis.
    kvsview::Widget::OrientationAxis orientation_axis( &screen );
    orientation_axis.show();

    // Set up the isosurface class.
    kvs::PipelineModule mapper( new kvs::ExtractVertices );
    const kvs::Real32 size = arg.size();
    mapper.get<kvs::ExtractVertices>()->setSize( size );
    mapper.get<kvs::ExtractVertices>()->setTransferFunction( tfunc );
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
        pipe.print(  std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    return ( arg.clear(), app.run() );
}

} // end of namespace ExtractVertices

} // end of namespace kvsview
