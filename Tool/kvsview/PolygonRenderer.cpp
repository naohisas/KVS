/*****************************************************************************/
/**
 *  @file   PolygonRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderer.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
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
#include <kvs/glut/Screen>
#include <kvs/glut/Application>
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
    addOption( kvsview::PolygonRenderer::CommandName, kvsview::PolygonRenderer::Description, 0 );
    addOption( "t", "two-side lighting flag (disable:0, enable:1). (default: 0)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Two-side lighting flag.
 *  @return 1, if two-side lighting is enable
 */
/*===========================================================================*/
const int Argument::twoSideLighting() const
{
    int default_value = 0;

    if ( this->hasOption("t") )
    {
        return this->optionValue<int>("t");
    }

    return default_value;
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
    kvsview::PolygonRenderer::Argument arg( argc, argv );
    if( !arg.parse() ) return false;

    // Create a global and screen class.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::PolygonRenderer::CommandName );
    screen.show();

    // Check the input data.
    m_input_name = arg.value<std::string>();
    if ( !kvsview::FileChecker::ImportablePolygon( m_input_name ) )
    {
        kvsMessageError("%s is not polygon data.", m_input_name.c_str());
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

    // Set a polygon renderer.
    kvs::PipelineModule renderer( new kvs::PolygonRenderer );
    pipe.connect( renderer );
    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return false;
    }
    if ( arg.twoSideLighting() != 0 )
    {
        static_cast<const kvs::PolygonRenderer*>( pipe.renderer() )->enableTwoSideLighting();
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

} // end of namespace PolygonRenderer

} // end of namespace kvsview
