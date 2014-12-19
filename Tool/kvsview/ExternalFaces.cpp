/*****************************************************************************/
/**
 *  @file   ExternalFaces.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExternalFaces.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "ExternalFaces.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/ExternalFaces>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/LegendBar>
#include <kvs/glut/OrientationAxis>
#include "CommandName.h"
#include "ObjectInformation.h"
#include "FileChecker.h"
#include "Widget.h"


namespace kvsview
{

namespace ExternalFaces
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, "ExternalFaces")
{
    // Parameters for the ExternalFaces class.
    addOption( kvsview::ExternalFaces::CommandName, kvsview::ExternalFaces::Description, 0 );
    addOption( "a", "Opacity (alpha value). (default: 255)", 1, false );
    addOption( "t", "Transfer function file. (optional: <filename>)", 1, false );
    addOption( "T", "Transfer function file with range adjustment. (optional: <filename>)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a opacity value.
 *  @return opacity value
 */
/*===========================================================================*/
const kvs::UInt8 Argument::opacity( void )
{
    const kvs::UInt8 default_value = 255;

    if ( this->hasOption("a") ) return( this->optionValue<kvs::UInt8>("a") );
    else return( default_value );
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
 *  @brief  Constructs a new Main class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Main::Main( int argc, char** argv )
{
    m_argc = argc;
    m_argv = argv;
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
const bool Main::exec( void )
{
    // GLUT viewer application.
    kvs::glut::Application app( m_argc, m_argv );

    // Parse specified arguments.
    kvsview::ExternalFaces::Argument arg( m_argc, m_argv );
    if( !arg.parse() ) return( false );

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::ExternalFaces::CommandName );

    // Check the input volume data.
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
        std::cout << "IMPORTED OBJECT" << std::endl;
        std::cout << kvsview::ObjectInformation( pipe.object() ) << std::endl;
        std::cout << std::endl;
    }

    // Pointer to the volume object data.
    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );

    // Transfer function.
    const kvs::TransferFunction tfunc = arg.transferFunction( volume );

    // Legend bar.
    kvsview::Widget::LegendBar legend_bar( &screen );
    legend_bar.setColorMap( tfunc.colorMap() );
    if ( !tfunc.hasRange() )
    {
        const kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        const kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        legend_bar.setRange( min_value, max_value );
    }
    legend_bar.show();

    // Orientation axis.
    kvsview::Widget::OrientationAxis orientation_axis( &screen );
    orientation_axis.show();

    // Setup ExternalFaces class.
    kvs::PipelineModule mapper( new kvs::ExternalFaces );
    const kvs::UInt8 opacity = arg.opacity();
    mapper.get<kvs::ExternalFaces>()->setOpacity( opacity );
    mapper.get<kvs::ExternalFaces>()->setTransferFunction( tfunc );
    pipe.connect( mapper );

    // Construct the visualization pipeline.
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return( false );
    }
    screen.registerObject( &pipe );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        std::cout << "RENDERERED OBJECT" << std::endl;
        std::cout << kvsview::ObjectInformation( pipe.object() ) << std::endl;
        std::cout << std::endl;
        std::cout << "VISUALIZATION PIPELINE" << std::endl;
        std::cout << pipe << std::endl;
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // Show the screen.
    screen.show();

    return( arg.clear(), app.run() );
}

} // end of namespace ExternalFaces

} // end of namespace kvsview
