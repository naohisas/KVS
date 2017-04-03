/*****************************************************************************/
/**
 *  @file   PointRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointRenderer.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "PointRenderer.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/PointRenderer>
#include <kvs/glut/Screen>
#include <kvs/glut/Application>
#include "CommandName.h"
#include "FileChecker.h"


namespace kvsview
{

namespace PointRenderer
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, kvsview::PointRenderer::CommandName )
{
    // Parameters for the point renderer class.
    addOption( kvsview::PointRenderer::CommandName, kvsview::PointRenderer::Description, 0 );
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
    kvsview::PointRenderer::Argument arg( argc, argv );
    if( !arg.parse() ) return false;

    // Create screen.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::PointRenderer::CommandName );
    screen.show();

    // Check the input data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportablePoint( m_input_name ) )
    {
        kvsMessageError("%s is not point data.", m_input_name.c_str());
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

    // Set a point renderer.
    kvs::PipelineModule renderer( new kvs::PointRenderer );
    pipe.connect( renderer );
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

    return ( arg.clear(), app.run() );
}

} // end of namespace PointRenderer

} // end of namespace kvsview
