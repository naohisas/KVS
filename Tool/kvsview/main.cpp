/*****************************************************************************/
/**
 *  @file   main.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1497 2013-04-04 07:17:54Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/MemoryDebugger>
#include "main.h"
#include "Argument.h"
#include "Default.h"
#include "PointRenderer.h"
#include "LineRenderer.h"
#include "PolygonRenderer.h"
#include "Isosurface.h"
#include "SlicePlane.h"
#include "OrthoSlice.h"
#include "TransferFunction.h"
#include "ExtractEdges.h"
#include "ExtractVertices.h"
#include "ExternalFaces.h"
#include "RayCastingRenderer.h"
#include "ParticleBasedRenderer.h"
#include "Histogram.h"
#include <kvs/Message>

KVS_MEMORY_DEBUGGER;


#define KVSVIEW_HELP( method )                                          \
    if ( help == #method ) return( method ::Argument( m_argc, m_argv ).parse() )

#define KVSVIEW_EXEC( method )                                          \
    if ( arg.hasOption( #method ) ) { return( arg.clear(), method ::Main( m_argc, m_argv ).exec() ); }

namespace kvsview
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Main class.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
Main::Main( int argc, char** argv )
{
    m_argc = argc;
    m_argv = argv;
}

/*===========================================================================*/
/**
 *  @brief  Execute main process.
 */
/*===========================================================================*/
bool Main::exec( void )
{
    Argument arg( m_argc, m_argv );
    if( !arg.read() ) return( false );

    // Output help messsage for the specified visualization method.
    if ( arg.hasOption("help") )
    {
        const std::string help = arg.optionValue<std::string>("help");
        KVSVIEW_HELP( Default );
        KVSVIEW_HELP( PointRenderer );
        KVSVIEW_HELP( LineRenderer );
        KVSVIEW_HELP( PolygonRenderer );
        KVSVIEW_HELP( Isosurface );
        KVSVIEW_HELP( SlicePlane );
        KVSVIEW_HELP( OrthoSlice );
        KVSVIEW_HELP( TransferFunction );
        KVSVIEW_HELP( ExtractEdges );
        KVSVIEW_HELP( ExtractVertices );
        KVSVIEW_HELP( ExternalFaces );
        KVSVIEW_HELP( RayCastingRenderer );
        KVSVIEW_HELP( ParticleBasedRenderer );
        KVSVIEW_HELP( Histogram );

        kvsMessageError( "Unknown visualization method '%s'.", help.c_str() );
        return( false );
    }

    // Execute the specified visualization method.
    {
        KVSVIEW_EXEC( Default );
        KVSVIEW_EXEC( PointRenderer );
        KVSVIEW_EXEC( LineRenderer );
        KVSVIEW_EXEC( PolygonRenderer );
        KVSVIEW_EXEC( Isosurface );
        KVSVIEW_EXEC( SlicePlane );
        KVSVIEW_EXEC( OrthoSlice );
        KVSVIEW_EXEC( TransferFunction );
        KVSVIEW_EXEC( ExtractEdges );
        KVSVIEW_EXEC( ExtractVertices );
        KVSVIEW_EXEC( ExternalFaces );
        KVSVIEW_EXEC( RayCastingRenderer );
        KVSVIEW_EXEC( ParticleBasedRenderer );
        KVSVIEW_EXEC( Histogram );
    }

    return( arg.clear(), Default::Main( m_argc, m_argv ).exec() );
}

} // end of namespace kvsview


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    KVS_MEMORY_DEBUGGER__SET_ARGUMENT( argc, argv );

    kvsview::Main m( argc, argv );
    return( m.exec() );
}
