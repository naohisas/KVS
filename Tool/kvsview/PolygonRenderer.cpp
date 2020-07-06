/*****************************************************************************/
/**
 *  @file   PolygonRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PolygonRenderer.h"
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/KVSMLPolygonObject>
#include <kvs/Stl>
#include <kvs/PipelineModule>
#include <kvs/VisualizationPipeline>
#include <kvs/PolygonRenderer>
#include <kvs/Screen>
#include <kvs/Application>
#include "CommandName.h"
#include "FileChecker.h"


namespace kvsview
{

namespace PolygonRenderer
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, kvsview::PolygonRenderer::CommandName )
{
    // Parameters for the polygon renderer class.
    addOption( kvsview::PolygonRenderer::CommandName, PolygonRenderer::Description, 0 );
    addOption( "t", "two-side lighting flag (disable:0, enable:1). (default: 0)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
int Main::exec( int argc, char** argv )
{
    // Viewer application.
    kvs::Application app( argc, argv );

    // Parse specified arguments.
    kvsview::PolygonRenderer::Argument arg( argc, argv );
    if( !arg.parse() ) return false;

    // Create a global and screen class.
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + PolygonRenderer::CommandName );
    screen.show();

    // Check the input data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportablePolygon( m_input_name ) )
    {
        kvsMessageError() << m_input_name << " is not polygon data." << std::endl;
        return ( false );
    }

    // Visualization pipeline.
    kvs::VisualizationPipeline pipe( m_input_name );
    pipe.import();

    // Verbose information.
    const kvs::Indent indent(4);
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, indent );
    }

    // Set a polygon renderer.
    kvs::PipelineModule renderer( new kvs::PolygonRenderer );
    pipe.connect( renderer );
    if ( !pipe.exec() )
    {
        kvsMessageError() << "Cannot execute the visulization pipeline." << std::endl;
        return ( false );
    }
    if ( arg.twoSideLighting() != 0 )
    {
        static_cast<const kvs::PolygonRenderer*>( pipe.renderer() )->enableTwoSideLighting();
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

    return ( arg.clear(), app.run() );
}

} // end of namespace PolygonRenderer

} // end of namespace kvsview
